#include "PortBindingManager.h"

PortBindingManager::PortBindingManager(QObject* parent) : QObject(parent) {}


PortBindingManager::~PortBindingManager() {
    auto it = bindings.begin();
    while (it != bindings.end()) {
        QSharedPointer<Port> port1 = it.key();
        QSharedPointer<Port> port2 = it.value();
        terminateConnection(port1, port2);
        it = bindings.erase(it);
    }
}


bool PortBindingManager::bindPorts(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2) {
    if (!port1 || !port2 || port1 == port2 || bindings.contains(port1) || bindings.contains(port2) || port1->isConnectedToRouter() || port2->isConnectedToRouter()) {
        return false; 
    }

    establishConnection(port1, port2);
    return true;
}

void PortBindingManager::unbindPorts(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2) {
    if (arePortsBound(port1, port2)) {
        terminateConnection(port1, port2);
    }
}

bool PortBindingManager::arePortsBound(const QSharedPointer<Port>& port1, const QSharedPointer<Port>& port2) const {
    return (bindings.value(port1) == port2 && bindings.value(port2) == port1);
}

void PortBindingManager::establishConnection(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2) {
    bindings[port1] = port2;
    bindings[port2] = port1;
    qDebug() << "Here in PortBindin";
    port1->connectToRouter(port2);
    port2->connectToRouter(port1);
    QObject::connect(port1.data(), &Port::packetSent, port2.data(), &Port::receivePacket);
    QObject::connect(port2.data(), &Port::packetSent, port1.data(), &Port::receivePacket);
    // qDebug() << "Here in Out PortBin";
}

void PortBindingManager::terminateConnection(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2) {
    if (port1) {
        port1->disconnectFromRouter();
    }
    if (port2) {
        port2->disconnectFromRouter();
    }
    bindings.remove(port1);
    bindings.remove(port2);
}
