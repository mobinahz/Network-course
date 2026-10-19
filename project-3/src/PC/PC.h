#ifndef PC_H
#define PC_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QSharedPointer>

#include "../Node/Node.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"
#include "../DHCP/DHCP.h"        
#include "MACAddress.h"
#include "MACAddressGenerator.h"
#include "HostAddress.h"


class PC : public Node {
    Q_OBJECT

public:
   
    explicit PC(int id,
                const QSharedPointer<HostAddress>& ip = QSharedPointer<HostAddress>(),
                const QSharedPointer<MACAddress>& mac = QSharedPointer<MACAddress>(),
                QObject *parent = nullptr);

    virtual ~PC();

  
    void requestIPViaDHCP(int asNumber, bool wantsIPv6) override;
    QSharedPointer<Port> getPort();
    void sendPackets(const QSharedPointer<HostAddress>& destAddress);
    void packetReceived(const PacketPtr_t packet);
    int cntArrival;
    int sumArrival;
    int totPacket;

public Q_SLOTS:
   
    void generateAndSendPackets(int numberOfPackets);

  
    void onDhcpOfferReceived(const DHCPRequest &request, const QSharedPointer<HostAddress> &offeredIP);

    
    void onDhcpAckReceived(const DHCPRequest &request, const QSharedPointer<HostAddress> &assignedIP);

Q_SIGNALS:
  
    void sendPacket(const QSharedPointer<Packet>& packet);

protected:

    QSharedPointer<Port> port;

 
    QMap<QSharedPointer<HostAddress>, QSharedPointer<MACAddress>> knownHosts;

private:
    QSharedPointer<HostAddress> ipAddress;
    QSharedPointer<MACAddress>  macAddress;

    void initializeNetworkInfo();

 
    QSharedPointer<Packet> createPacket(const QSharedPointer<HostAddress>& destAddress);

 
};

#endif // PC_H

