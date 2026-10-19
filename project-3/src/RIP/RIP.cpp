#include "RIP.h"
#include <QDebug>

RIP::RIP(QObject* parent) : QObject(parent) {}

RIP::~RIP() {}

void RIP::initialize(const QSharedPointer<HostAddress>& routerIP, QVector<QSharedPointer<Port>>& ports) {
    this->routerIP = routerIP;
    this->ports = ports;

    discoverNeighbors();
   
}

void RIP::discoverNeighbors() {
    for (const auto& port : ports) {
        QSharedPointer<HostAddress> connectedHost = port->getConnectedHost();
        if (!connectedHost.isNull()) {
            neighborCosts[connectedHost] = 1; 
        }
    }
}

void RIP::sendRoutingUpdates() {
    QVector<RIPEntry> tableEntries;
    for (const auto& entry : routingTable) {
        tableEntries.append(entry);
    }

    sendPacketToNeighbors(tableEntries);
}

void RIP::sendPacketToNeighbors(const QVector<RIPEntry>& routingTable) {
    for (const auto& port : ports) {
        QSharedPointer<HostAddress> neighbor = port->getConnectedHost();
        if (!neighbor.isNull()) {
            qDebug() << "Sending routing update to" << neighbor->toString();
        }
    }
}

void RIP::processRoutingUpdate(const QSharedPointer<HostAddress>& neighbor,
                               const QVector<RIPEntry>& receivedTable) {
   
}

void RIP::updateRoutingTable() {
   
}

QVector<RIPEntry> RIP::getRoutingTable() const {
    QVector<RIPEntry> entries;
    for (const auto& entry : routingTable) {
        entries.append(entry);
    }
    return entries;
}
