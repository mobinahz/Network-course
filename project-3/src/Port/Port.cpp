#include "Port.h"
#include "Router.h"

Port::Port(int portNumber, Router* parentRouter, int id, QString ip, QObject* parent)
    : QObject(parent), portNumber(portNumber), parentRouter(parentRouter), id(id), ip(ip) {
    if (parentRouter == nullptr){
        QSharedPointer<HostAddress> ipHost = QSharedPointer<HostAddress>::create(ip);
        parentRouter = new Router(
            id,
            ipHost,
            8,
            UT::GatewayType::as_gateway,
            UT::IPVersion::IPv4,
            1024,
            nullptr
            );
    }
}

Port::~Port() {
    disconnectAll();
}

int Port::getPortNumber() {
    return portNumber;
}

void Port::connectToRouter(QSharedPointer<Port> otherPort) {
    if (!connectedRouterPort && otherPort && otherPort.get() != this && !otherPort->isConnectedToRouter()) {
        connectedRouterPort = otherPort;
        qDebug() << "Inja " << this->parentRouter->getIPAddress()->toString() << ' ' << this->parentRouter->getIPAddress()->toString();
        otherPort->connectedRouterPort = sharedFromThis(); // Assumes Port inherits QEnableSharedFromThis
        Q_EMIT routerConnected(portNumber, otherPort->getPortNumber());
        // qDebug() << "Lets Get Out";
    }
}

void Port::connectToPC(QSharedPointer<HostAddress> pcAddress) {
    if (!connectedPCs.contains(pcAddress)) {
        connectedPCs.append(pcAddress);
        Q_EMIT pcConnected(portNumber, pcAddress);
    }
}

void Port::disconnectFromRouter() {
    if (connectedRouterPort) {
        auto tempPort = connectedRouterPort;
        connectedRouterPort.clear();
        tempPort->connectedRouterPort.clear();
        Q_EMIT routerDisconnected(portNumber);
    }
}

void Port::disconnectFromPC(QSharedPointer<HostAddress> pcAddress) {
    if (connectedPCs.removeOne(pcAddress)) {
        Q_EMIT pcDisconnected(portNumber, pcAddress);
    }
}

void Port::disconnectAll() {
    disconnectFromRouter();
    while (!connectedPCs.isEmpty()) {
        disconnectFromPC(connectedPCs.first());
    }
}

bool Port::isConnectedToRouter() const {
    return !connectedRouterPort.isNull();
}

QVector<QSharedPointer<HostAddress>> Port::getConnectedPCs() const {
    return connectedPCs;
}

QSharedPointer<HostAddress> Port::getConnectedHost() const
{
    qDebug() << id << ' ' << isPCConnected;
    if (isPCConnected){
        return connectedPC;
    }
    if (!connectedPCs.isEmpty()) {
        return connectedPCs.first();
    }

    if (!connectedRouterPort.isNull()) {
        Router* otherRouter = connectedRouterPort->parentRouter;
        if (otherRouter) {
            return otherRouter->getIPAddress();
        }
    }

    return QSharedPointer<HostAddress>();
}

void Port::sendPacket(const QSharedPointer<Packet>& packet) {
    qDebug() << "Here Sending Packet to" << packet->getDestination() << "in Port " << portNumber;
    Q_EMIT packetSent(packet);
}

void Port::receivePacket(const QSharedPointer<Packet>& packet) {
    qDebug() << "Here packet received to" << packet->getDestination();
    Q_EMIT packetReceived(packet);
}





