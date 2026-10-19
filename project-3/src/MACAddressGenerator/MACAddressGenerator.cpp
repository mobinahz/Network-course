#include "MACAddressGenerator.h"
#include <QByteArray>
#include <QString>
#include <QStringList>


MACAddressGenerator::MACAddressGenerator(MACAddress* start)
    : m_currentAddress(start)
{}

MACAddress* MACAddressGenerator::nextAddress()
{
    MACAddress* result = m_currentAddress;
    incrementMAC(m_currentAddress);
    return result;
}


void MACAddressGenerator::incrementMAC(MACAddress* mac)
{
    QByteArray macBytes = mac->getBytes();

    for (int i = 5; i >= 0; --i) {
        macBytes[i]++;
        if (macBytes[i] != 0) {
            break;
        }
    }

    mac->setBytes(macBytes);
}
