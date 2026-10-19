#ifndef MACADDRESSGENERATOR_H
#define MACADDRESSGENERATOR_H

#include "MACAddress.h"
#include "QObject"

class MACAddressGenerator:public QObject
{
public:
    MACAddressGenerator(MACAddress* start = new MACAddress("00:00:00:00:00:00"));

    MACAddress* nextAddress();

private:
    MACAddress* m_currentAddress;
    void incrementMAC(MACAddress* mac);
};

#endif // MACADDRESSGENERATOR_H
