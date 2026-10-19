#include "PC.h"
#include <QDebug>

PC::PC(int id,
       const QSharedPointer<HostAddress>& ip,
       const QSharedPointer<MACAddress>& mac,
       QObject *parent)
    : Node(id, parent),
    ipAddress(ip),
    macAddress(mac)
{
    cntArrival = 0;
    sumArrival = 0;
    totPacket = 0;
    this->ip = ip;
    if (!macAddress || macAddress->toString().isEmpty()) {
        MACAddressGenerator generator(new MACAddress("00:00:00:00:00:00"));
        macAddress = QSharedPointer<MACAddress>::create(generator.nextAddress()->toString());
    }

    qDebug() << "[PC]" << this->getId()
             << "created with IP =" << (ipAddress ? ipAddress->toString() : "None")
             << "and MAC =" << (macAddress ? macAddress->toString() : "None");

    QString ipAddress = QString("192.168.%1.%2").arg(id).arg(1);
    port = QSharedPointer<Port>::create(1, nullptr, id, ipAddress, nullptr);
    connect(port.data(), &Port::packetReceived, this, &PC::packetReceived);
    initializeNetworkInfo();
}

PC::~PC()
{
    qDebug() << "[PC]" << this->getId() << "destroyed.";
}

QSharedPointer<Port> PC::getPort(){
    return port;
}

void PC::packetReceived(const PacketPtr_t packet){
    qDebug() << "In PC  " << id << " One Packet Received";
    cntArrival++;
    sumArrival += packet->pathLen;
}

void PC::initializeNetworkInfo()
{
    
}

void PC::generateAndSendPackets(int numberOfPackets)
{
    QList<QSharedPointer<Packet>> packets;
    for (int i = 0; i < numberOfPackets; ++i) {
        auto destIP = QSharedPointer<HostAddress>::create("192.168.1.200");
        QSharedPointer<Packet> p = createPacket(destIP);
        packets.append(p);
    }
}

QSharedPointer<Packet> PC::createPacket(const QSharedPointer<HostAddress>& destAddress)
{
    QString sourceStr = (ipAddress ? ipAddress->toString() : "0.0.0.0");
    QString destStr   = (destAddress ? destAddress->toString() : "255.255.255.255");
    int packetSize    = 128; 
    Packet::PacketType type = Packet::PacketType::Data;

    QSharedPointer<Packet> packet =
        QSharedPointer<Packet>::create(sourceStr, destStr, packetSize, type);
    return packet;
}

void PC::sendPackets(const QSharedPointer<HostAddress>& destAddress)
{
    totPacket++;
    port->sendPacket(createPacket(destAddress));
}

void PC::requestIPViaDHCP(int asNumber, bool wantsIPv6)
{
    DHCPRequest req;
    req.nodeID     = QString::number(this->getId());
    req.macAddress = macAddress; 
    req.asNumber   = asNumber;
    req.wantsIPv6  = wantsIPv6;

    Q_EMIT dhcpDiscoverSignal(req);

    qDebug() << "[PC]" << this->getId()
             << "emitting DHCP DISCOVER for AS" << asNumber
             << (wantsIPv6 ? "(IPv6)" : "(IPv4)");
}


void PC::onDhcpOfferReceived(const DHCPRequest &request, const QSharedPointer<HostAddress> &offeredIP)
{
    if (request.nodeID == QString::number(this->getId()) &&
        macAddress &&
        (request.macAddress->toString() == macAddress->toString()))
    {
        qDebug() << "[PC]" << this->getId()
        << "received DHCP OFFER for IP:" << offeredIP->toString();

        Q_EMIT dhcpRequestSignal(request);
    }
}

void PC::onDhcpAckReceived(const DHCPRequest &request, const QSharedPointer<HostAddress> &assignedIP)
{
    if (request.nodeID == QString::number(this->getId()) &&
        macAddress &&
        (request.macAddress->toString() == macAddress->toString()))
    {
        ipAddress = QSharedPointer<HostAddress>::create(assignedIP->toString());

        qDebug() << "[PC]" << this->getId()
                 << "received DHCP ACK. Assigned IP: "
                 << ipAddress->toString();
    }
}
