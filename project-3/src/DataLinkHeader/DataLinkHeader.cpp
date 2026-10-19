#include "DataLinkHeader.h"

DataLinkHeader::DataLinkHeader(const QString& sourceMAC, const QString& destMAC, quint16 type)
    : sourceMAC(sourceMAC), destMAC(destMAC), type(type) {}

QString DataLinkHeader::getSourceMAC() const {
    return sourceMAC;
}

QString DataLinkHeader::getDestMAC() const {
    return destMAC;
}

quint16 DataLinkHeader::getType() const {
    return type;
}

void DataLinkHeader::setSourceMAC(const QString& mac) {
    sourceMAC = mac;
}

void DataLinkHeader::setDestMAC(const QString& mac) {
    destMAC = mac;
}

void DataLinkHeader::setType(quint16 type) {
    this->type = type;
}
