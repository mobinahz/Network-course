#include "./../Network/Network.h"


Network::Network(QObject* parent): QObject(parent) {
    AS1 = QSharedPointer<AutonomousSystem>(new AutonomousSystem(1));      
    AS2 = QSharedPointer<AutonomousSystem>(new AutonomousSystem(2));
    topologyController.setupAS(AS1, UT::TopologyType::Mesh);
    topologyController.setupAS(AS2, UT::TopologyType::RingStar);
    connectAS1toAS2();
}


Network::~Network() {}



QSharedPointer<AutonomousSystem> Network::getAS1() {
    return AS1;
}

QSharedPointer<AutonomousSystem> Network::getAS2() {
    return AS2;
}

void Network::connectAS1toAS2() {

    if (!AS1 || !AS2) return;

    QSharedPointer<Router> as1Gateway = AS1->getAsGateway();
    QSharedPointer<Router> as2Gateway = AS2->getAsGateway();
    for (int i = 13; i < 16; i++){
        AS1->getRouters()[i]->connectToRouter(AS2->getRouters()[i - 13]);
    }

}
