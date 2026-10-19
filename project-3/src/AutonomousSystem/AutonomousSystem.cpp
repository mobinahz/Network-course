#include "AutonomousSystem.h"
#include "ConfigurationManager.h"


AutonomousSystem::AutonomousSystem(int id, QObject* parent)
    : QObject(parent), id(id) {
    initializeFromConfig();
}

AutonomousSystem::~AutonomousSystem() {}

void AutonomousSystem::initializeFromConfig() {
    auto& configManager = ConfigurationManager::getInstance();
    configManager.loadConfiguration("C:/Users/Asus/Documents/Arshia/University/term7/Shabake/Clone Mojadad/CN_CHomeworks_3/assets/config.json");
    int portCount = configManager.get<int>("router_port_count");
    int bufferSize = configManager.get<int>("router_buffer_size");
    UT::IPVersion ipVersion = UT::IPVersion::IPv4;
    portCount = 8;
    routers.clear();

    QString routerBaseIP = (id == 1) ? "192.168.100" : "192.168.200";
    int routerStart = (id == 1) ? 1 : 17;
    int pcStart = (id == 1) ? 24 : 32;
    int routerCount = (id == 1) ? 16 : 7;
    int pcCount = (id == 1) ? 8 : 7;
    portCount = 8;

    for (int i = 0; i < routerCount; ++i) {
        QString routerIP = QString("%1.%2").arg(routerBaseIP).arg(routerStart + i);
        auto ipAddress = QSharedPointer<HostAddress>(new HostAddress(routerIP));


        UT::GatewayType gatewayType = (i >= 4) ?
                                          UT::GatewayType::as_gateway : UT::GatewayType::user_gateway;

        routers.append(new Router(i + routerStart, ipAddress, portCount, gatewayType, ipVersion, bufferSize, this));
    }

    for (int i = 0; i < pcCount; ++i) {
        QString pcIP = QString("%1.%2").arg(routerBaseIP).arg(pcStart + i);
        auto pcIPAddress = QSharedPointer<HostAddress>(new HostAddress(pcIP));
        auto pcMACAddress = QSharedPointer<MACAddress>::create();
        QSharedPointer<PC> pc = QSharedPointer<PC>::create(pcStart + i, pcIPAddress, pcMACAddress, this);
        pcs.append(pc);
    }
}

void AutonomousSystem::connectToAS(QSharedPointer<AutonomousSystem> otherAS){
    auto& otherRouters = otherAS->getRouters();
    for (int i = 13; i < 16; i++){
        routers[i]->connectToRouter(otherRouters[i - 13]);
    }
}

const QVector<Router*>& AutonomousSystem::getRouters() const {
    return routers;
}

const QVector<QSharedPointer<PC>>& AutonomousSystem::getPCs() const {
    return pcs;
}


void AutonomousSystem::connectPCtoGateway(const QSharedPointer<PC>& pc, const QSharedPointer<Router>& gateway)
{
    
    connect(pc.data(), &PC::dhcpDiscoverSignal,
            gateway.data(), &Router::onDhcpDiscoverFromPC);
    connect(pc.data(), &PC::dhcpRequestSignal,
            gateway.data(), &Router::onDhcpRequestFromPC);

    
    connect(gateway.data(), &Router::dhcpOfferSignal,
            pc.data(), &PC::onDhcpOfferReceived);
    connect(gateway.data(), &Router::dhcpAckSignal,
            pc.data(), &PC::onDhcpAckReceived);
}


void AutonomousSystem::enableDHCPServer(int dhcpServerIndex)
{
    
    for (int i = 0; i < routers.size(); i++) {
        routers[i]->disableDHCPServer();
    }

    
    if (dhcpServerIndex >= 0 && dhcpServerIndex < routers.size()) {
        routers[dhcpServerIndex]->enableDHCPServer();
    }
}


QSharedPointer<Router> AutonomousSystem::getAsGateway() const {
    return asGateway;
}

QSharedPointer<Router> AutonomousSystem::getUserGateway() const {
    return userGateway;
}

