#include "Packet.h"

Packet::Packet(const QString& source, const QString& destination, int size, PacketType type)
    : source(source), destination(destination), size(size), totalCycles(0), bufferCycles(0), type(type), payload(), pathLen(0) {}

QByteArray Packet::getPayload() const {
    return payload;
}

void Packet::setPayload(const QByteArray& newPayload) {
    payload = newPayload;
}

LSA Packet::getLSA(){
    return lsaContent;
}

void Packet::setLSA(const LSA& lsa){
    lsaContent = lsa;
}

QString Packet::getSource() const {
    return source;
}

QString Packet::getDestination() const {
    return destination;
}

int Packet::getSize() const {
    return size;
}

void Packet::setSource(const QString& source) {
    this->source = source;
}

void Packet::setDestination(const QString& destination) {
    this->destination = destination;
}

void Packet::setSize(int size) {
    this->size = size;
}

void Packet::addNodeToPath(const QString& nodeIP) {
    path.append(nodeIP);
}

QList<QString> Packet::getPath() const {
    return path;
}

void Packet::incrementTotalCycles() {
    ++totalCycles;
}

void Packet::incrementBufferCycles() {
    ++bufferCycles;
}

int Packet::getTotalCycles() const {
    return totalCycles;
}

int Packet::getBufferCycles() const {
    return bufferCycles;
}

Packet::PacketType Packet::getType() const {
    return type;
}

QString Packet::packetTypeToString(PacketType type) {
    switch (type) {
    case PacketType::Data:
        return "Data";
    case PacketType::Control:
        return "Control";
    case PacketType::DHCPRequest:
        return "DHCP Request";
    case PacketType::DHCPResponse:
        return "DHCP Response";
    case PacketType::RIP:
        return "RIP";
    case PacketType::OSPF:
        return "OSPF";
    default:
        return "Unknown Type";
    }
}

