#include "Router.h"
#include <QDebug>
#include "../Packet/Packet.h"


Router::Router(int nodeID, const QSharedPointer<HostAddress>& ipAddress, int portCount,
               UT::GatewayType gatewayType, UT::IPVersion ipVersion, int bufferSize, QObject* parent)
    : Node(nodeID, parent),
    ipAddress(ipAddress),
    ipVersion(ipVersion),
    bufferSize(bufferSize),
    gatewayType(gatewayType),
    ripProtocol(new RIP(this)),
    ospfProtocol(new OSPF(this, this)),
    isDHCPEnabled(false) {
    createPorts(portCount);
    bindingManager = new PortBindingManager();

    dhcpServer = new DHCP(1, this);
    dhcpServer->setEnabled(false);
    isDHCPEnabled = false;
    pcPort = portCount - 1;
    freePort = 0;
    cntArrival = 0;
    packetLoss = 0;
    connect(dhcpServer, &DHCP::dhcpOfferSent, this,
            [this](const DHCPRequest &req, const QSharedPointer<HostAddress> &ip) {
                Q_EMIT dhcpOfferSignal(req, ip);
            });

    connect(dhcpServer, &DHCP::dhcpAckSent, this,
            [this](const DHCPRequest &req, const QSharedPointer<HostAddress> &ip) {
                Q_EMIT dhcpAckSignal(req, ip);
            });



    connect(dhcpServer, &DHCP::dhcpAckSent, this, &Router::onDhcpAckReceived);

}

Router::~Router() {
    delete ripProtocol;
    ports.clear();
    delete dhcpServer;
    delete bindingManager;
}

void Router::initializeOSPF() {
    ospfProtocol->initialize(ipAddress, ports);
}

void Router::setMACAddress(const QSharedPointer<MACAddress>& mac){
    this->macAddress = mac;
}

QSharedPointer<Port> Router::getFreePort() {
    for (auto &port : ports) {
        if (!port->isConnectedToRouter()) {
            return port;
        }
    }
    return QSharedPointer<Port>();
}

QSharedPointer<HostAddress> Router::getIPAddress() const {
    return ipAddress;
}

void Router::processOSPFUpdate(const QSharedPointer<HostAddress>& neighbor,
                               const LSA& lsa)
{
    ospfProtocol->processLSA(neighbor, lsa);
    ospfProtocol->runDijkstra();
}

void Router::initializeRIP() {
    ripProtocol->initialize(ipAddress, ports);
    ripProtocol->sendRoutingUpdates();
}

void Router::processRIPUpdate(const QSharedPointer<HostAddress>& neighbor,
                              const QVector<RIPEntry>& receivedTable) {
    ripProtocol->processRoutingUpdate(neighbor, receivedTable);
    ripProtocol->updateRoutingTable();
}

OSPF* Router::getOspfProtocol(){
    return ospfProtocol;
}

void Router::createPorts(int portCount) {
    for (int i = 1; i <= portCount; ++i) {
        QString ipAddress = QString("192.168.%1.%2").arg(id).arg(i);

        auto port = QSharedPointer<Port>::create(i, this, id, ipAddress, nullptr);

        connect(port.data(), &Port::packetReceived, this, &Router::packetReceived);

        ports.append(port);
    }
}

void Router::printLSDB(){
    ospfProtocol->printLSDB();
}

void Router::packetReceived(const PacketPtr_t packet) {
    if (packet->getType() == Packet::PacketType::OSPF) {
        qDebug() << "Here in Router " << id << " I received LSA From Router " << packet->getSource();

        LSA lsa = ospfProtocol->convertPacketToLSA(packet);

        ospfProtocol->processLSA(ipAddress, lsa);
    } else {
        cntArrival ++;
        qDebug() << "Here in Router " << id << "I received packet to" << packet->getDestination();
        if (buffer.size() < bufferSize){
            buffer.append(packet);
            packet->pathLen++;
        }else{
            packetLoss++;
        }
    }
}



void Router::enableDHCPServer() {

    if (dhcpServer) {
        if (!isDHCPEnabled) {
            dhcpServer->setEnabled(true);
            isDHCPEnabled = true;
            qDebug() << "DHCP Server enabled on Router" << this->id;
        } else {
            qDebug() << "DHCP Server is already enabled.";
        }
    }

}

void Router::requestIPViaDHCP(int asNumber, bool wantsIPv6) {
    DHCPRequest request;
    request.nodeID = QString::number(id);
    request.macAddress = this->macAddress;
    request.asNumber = asNumber;
    request.wantsIPv6 = wantsIPv6;

    Q_EMIT dhcpDiscoverSignal(request);
}

void Router::disableDHCPServer() {

    if (dhcpServer) {
        if (isDHCPEnabled) {
            dhcpServer->setEnabled(false);
            isDHCPEnabled = false;
            qDebug() << "DHCP Server disabled on Router" << this->id;
        } else {
            qDebug() << "DHCP Server is already disabled.";
        }
    }

}

QSharedPointer<HostAddress> Router::assignIPAddressToNode(int nodeID,
                                                          const QString &macString,
                                                          int asNumber)
{
    if (!isDHCPEnabled || !dhcpServer) {
        qDebug() << "[Router]" << this->getId() << "DHCP Server is not enabled.";
        return QSharedPointer<HostAddress>();
    }

    DHCPRequest request;
    request.nodeID    = QString::number(nodeID);
    request.macAddress = macAddress;
    request.asNumber  = asNumber;
    request.wantsIPv6 = (this->ipVersion == UT::IPVersion::IPv6);

    QSharedPointer<HostAddress> assignedIP = dhcpServer->assignIPAddress(request);

    return assignedIP;
}

void Router::releaseIPAddressFromNode(const QString &macString)
{
    if (!isDHCPEnabled || !dhcpServer) {
        qDebug() << "[Router]" << this->getId() << "DHCP Server is not enabled.";
        return;
    }

    dhcpServer->releaseIP(macAddress);
}

void Router::saveDHCPRecords(const QString &filePath)
{
    if (!isDHCPEnabled || !dhcpServer) {
        qDebug() << "[Router]" << this->getId() << "DHCP Server is not enabled.";
        return;
    }

    dhcpServer->saveRecordsToFile(filePath);
}

void Router::loadDHCPRecords(const QString &filePath)
{
    if (!isDHCPEnabled || !dhcpServer) {
        qDebug() << "[Router]" << this->getId() << "DHCP Server is not enabled.";
        return;
    }

    dhcpServer->loadRecordsFromFile(filePath);
}

void Router::addRoute(const QSharedPointer<HostAddress>& network, const QSharedPointer<HostAddress>& mask, const QSharedPointer<HostAddress>& gateway, int metric, const QString& protocol) {
    RouteEntry entry(network, mask, gateway, metric, protocol);
    routingTable[network->toString()] = entry;
}

void Router::updateRoute(const QSharedPointer<HostAddress>& network, const QSharedPointer<HostAddress>& mask, const QSharedPointer<HostAddress>& gateway, int metric, const QString& protocol) {

}

void Router::deleteRoute(const QSharedPointer<HostAddress>& network) {
}

void Router::updateRoutingTable(const QSharedPointer<HostAddress>& destination) {
    auto it = routingTable.find(destination->toString());
    if (it != routingTable.end()) {
        RouteEntry& entry = it.value();
        entry.gateway = destination;
        entry.metric = 10;
    } else {
        RouteEntry newEntry(destination, destination, destination, 1, "Static");
        routingTable.insert(destination->toString(), newEntry);
    }
    qDebug() << "Routing table updated for destination" << destination->toString();
}




void Router::printRoutingTable() {
    qDebug() << "Routing Table:";
    for (const auto& entry : routingTable) {
        QString destination = entry.destinationNetwork ? entry.destinationNetwork->toString() : "NULL";
        QString mask = entry.subnetMask ? entry.subnetMask->toString() : "NULL";
        QString gateway = entry.gateway ? entry.gateway->toString() : "NULL";

        qDebug() << "Network: " << destination << ";"
                 << " Mask: " << mask << ";"
                 << " Gateway: " << gateway << ";"
                 << " Metric: " << entry.metric << ";"
                 << " Protocol: " << entry.protocol << ";";
    }
}


QVector<QSharedPointer<Port>> Router::getPorts(){
    return ports;
}


void Router::connectToRouter(Router* otherRouter) {
    if (!otherRouter || !bindingManager) return;
    auto& myPort = ports[freePort];
    auto& theirPort = otherRouter->getPorts()[otherRouter->freePort];
    qDebug() << freePort << ' ' << otherRouter->freePort;
    freePort ++;
    otherRouter->freePort ++;
    qDebug() << "Here Router " << id << ' ' << "connect to Router " << otherRouter->id;
    qDebug() << "Bind Port " << myPort->getPortNumber() << ' ' << theirPort->getPortNumber();
    bindingManager->bindPorts(myPort, theirPort);
    connectedRouters[otherRouter] = QPair<QSharedPointer<Port>, QSharedPointer<Port>>(myPort, theirPort);
}

void Router::connectToPC(QSharedPointer<PC> pc){
    auto& myPort = ports[freePort];
    QSharedPointer<Port> theirPort = pc->getPort();
    qDebug() << "Connecting Router " << myPort->id << " To PC " << pc->getId() << "On Port " << myPort->getPortNumber();
    freePort++;
    bindingManager->bindPorts(myPort, theirPort);
    myPort->connectedPC = pc->getIp();
    myPort->isPCConnected = true;
    connectedPCs[pc] = QPair<QSharedPointer<Port>, QSharedPointer<Port>>(myPort, theirPort);
    // qDebug() << "Get Out From connectToPC";
}

void Router::disconnectRouter(Router* otherRouter) {
    if (!otherRouter || !bindingManager) return;

    for (auto& myPort : ports) {
        for (auto& theirPort : otherRouter->getPorts()) {
            if (bindingManager->arePortsBound(myPort, theirPort)) {
                bindingManager->unbindPorts(myPort, theirPort);
            }
        }
    }
}


QSharedPointer<Port> Router::findNextHopPort(const PacketPtr_t& packet)
{
    if (!packet) {
        qDebug() << "[Router" << id << "] findNextHopPort: null packet!";
        return nullptr;
    }

    QString destStr = packet->getDestination();
    QSharedPointer<HostAddress> destAddr(new HostAddress(destStr));

    auto it = routingTable.find(destAddr->toString());
    if (it == routingTable.end()) {
        qDebug() << "[Router" << id << "] No route found for destination"
                 << destStr;
        return nullptr;
    }

    const RouteEntry& route = it.value();
    QSharedPointer<HostAddress> gateway = route.gateway;
    if (!gateway) {
        qDebug() << "[Router" << id << "] Route entry for" << destStr
                 << "has a null gateway!";
        return nullptr;
    }

    for (auto& p : ports) {
        QSharedPointer<HostAddress> connectedHost = p->getConnectedHost();
        if (connectedHost &&
            connectedHost->toString() == gateway->toString()) {
            return p;
        }
    }

    qDebug() << "[Router" << id << "] Could not find port to gateway"
             << gateway->toString();
    return nullptr;
}


void Router::onClockTick(){
    qDebug() << "[Router" << id << "] Clock tick event. Buffer size:" << buffer.size();

    for (auto& p : ports) {
        p->isBusyThisTick = false;
    }


    while (!buffer.isEmpty()) {
        PacketPtr_t packet = buffer.first();

        QSharedPointer<Port> outPort = findNextHopPort(packet);
        qDebug() << "Here in Router id want to send packet to " << outPort->getPortNumber();

        if (!outPort) {
            qDebug() << "[Router" << id << "] No valid outPort for packet. Dropping.";
            buffer.removeFirst();
            continue;
        }

        if (outPort->isBusyThisTick) {
            qDebug() << "[Router" << id << "] Port" << outPort->getPortNumber()
                     << "is busy. Leaving the remaining packets for next tick.";
            break;
        }

        outPort->isBusyThisTick = true;
        outPort->sendPacket(packet);

        buffer.removeFirst();
        qDebug() << "[Router" << id << "] Sent packet to" << packet->getDestination()
                 << "using port" << outPort->getPortNumber();
    }
}

void Router::onDhcpDiscoverFromPC(const DHCPRequest &request)
{

    if (!isDHCPEnabled || !dhcpServer) {
        qDebug() << "[Router]" << id << "DHCP is disabled, ignoring DISCOVER.";
        return;
    }


    dhcpServer->onDhcpDiscover(request);
}



void Router::onDhcpRequestFromPC(const DHCPRequest &request)
{
    if (!isDHCPEnabled || !dhcpServer) {
        qDebug() << "[Router]" << id << "DHCP is disabled, ignoring REQUEST.";
        return;
    }

    dhcpServer->onDhcpRequest(request);
}

void Router::onDhcpAckReceived(const DHCPRequest &request, const QSharedPointer<HostAddress> &assignedIP) {
    if (request.nodeID == QString::number(id)) {
        ipAddress = assignedIP;
        qDebug() << "Router" << id << "has been assigned IP Address:" << ipAddress->toString();
    }
}
