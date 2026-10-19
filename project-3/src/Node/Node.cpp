#include "Node.h"

Node::Node(int id, const QSharedPointer<HostAddress>& ip, const QSharedPointer<MACAddress>& mac, QObject *parent)
    : QThread(parent), id(id), ip(ip), macAddress(mac) {}

Node::Node(int id, QObject* parent)
    : QThread(parent), id(id), ip(nullptr), macAddress(nullptr) {
}

Node::~Node() {}

int Node::getId() const {
    return id;
}

QSharedPointer<HostAddress> Node::getIp() const {
    return ip;
}

QSharedPointer<MACAddress> Node::getMacAddress() const {
    return macAddress;
}
