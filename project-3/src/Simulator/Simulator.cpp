#include "Simulator.h"
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

Simulator::Simulator(QObject* parent)
    : QThread(parent),
    coordinator(EventsCoordinator::instance()),
    network(nullptr) {}

Simulator::~Simulator() {
    if (isRunning()) {
        coordinator->stopCoordinator();
        quit();
        wait();
    }
    delete network;
}

void Simulator::post_process(){
    qDebug() << "\n\n\n\n\n\n\n\nREPORT:";
    qDebug() << "Packet loss Percentage:";
    int total = 0;
    int loss = 0;
    for (auto& router:network->getAS1()->getRouters() + network->getAS2()->getRouters()){
        loss += router->packetLoss;
    }
    for (auto& pc:network->getAS1()->getPCs() + network->getAS2()->getPCs()){
        total += pc->totPacket;
    }
    qDebug() << loss * 1.0 / total * 1.0;
    qDebug() << "Hop Average:";
    int sum = 0;
    int cnt = 0;
    for (auto& pc:network->getAS1()->getPCs() + network->getAS2()->getPCs()){
        sum += pc->sumArrival;
        cnt += pc->cntArrival;
    }
    float ans = sum * 1.0 / cnt*1.0;
    qDebug() << ans + 2;
    qDebug() << "User Routers:";
    for (auto& router:network->getAS1()->getRouters() + network->getAS2()->getRouters()){
        if (router->cntArrival > 0) qDebug() << router->getId();
    }
    QVector<QPair<int, int>> usage;
    for (auto& router:network->getAS1()->getRouters() + network->getAS2()->getRouters()){
        usage.append(qMakePair(router->cntArrival, router->getId()));
    }
    qDebug() << "Least to Most Usage:";
    std::sort(usage.begin(), usage.end());
    for (auto u:usage){
        qDebug() << u.second;
    }
    qDebug() << "Example Routing Table:";
    network->getAS1()->getRouters()[6]->printRoutingTable();
}

void Simulator::run() {
    qDebug() << "Simulator started.";

    // Initialize Network
    network = new Network();

    QVector<QSharedPointer<PC>> allPcs = network->getAS1()->getPCs() + network->getAS2()->getPCs();
    qDebug() << "Printing Pcs From Simulator ";
    for (auto& pc:allPcs){
        qDebug() << pc->getIp()->toString();
    }
    coordinator = new EventsCoordinator(nullptr, allPcs);
    // Initialize OSPF on all routers
    for (auto router : network->getAS1()->getRouters()) {
        router->initializeOSPF();
        QObject::connect(coordinator, &EventsCoordinator::eventTriggered,
                         router, &Router::onClockTick);
    }

    for (auto router : network->getAS2()->getRouters()) {
        router->initializeOSPF();
        QObject::connect(coordinator, &EventsCoordinator::eventTriggered,
                         router, &Router::onClockTick);
    }

    QThread::msleep(100);

    // Run Dijkstra on all routers
    for (auto router : network->getAS1()->getRouters()) {
        router->getOspfProtocol()->runDijkstra();
    }

    for (auto router : network->getAS2()->getRouters()) {
        router->getOspfProtocol()->runDijkstra();
    }

    // Send a packet from AS1 PC to AS2 PC
    QSharedPointer<HostAddress> destinationIP = QSharedPointer<HostAddress>::create("192.168.200.38");
    coordinator->startCoordinator();

    network->getAS1()->getPCs()[0]->sendPackets(destinationIP);

    QTimer::singleShot(coordinator->duration, [this]() {
        coordinator->stopCoordinator();
        post_process();
        QCoreApplication::quit();
    });

    exec();
}
