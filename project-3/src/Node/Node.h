#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QString>
#include <QThread>
#include "MACAddress.h"
#include "HostAddress.h"
#include "Globals.h"
#include "./../DHCP/DHCP.h"


class Node : public QThread {
    Q_OBJECT

public:
    explicit Node(int id, const QSharedPointer<HostAddress>& ip, const QSharedPointer<MACAddress>& mac, QObject *parent = nullptr);
    Node(int id, QObject *parent = nullptr);
    virtual ~Node();

    int getId() const;
    QSharedPointer<HostAddress> getIp() const;
    QSharedPointer<MACAddress> getMacAddress() const;

    virtual void requestIPViaDHCP(int asNumber, bool wantsIPv6) = 0;  


Q_SIGNALS:
    void dhcpDiscoverSignal(const DHCPRequest &request);
    void dhcpRequestSignal(const DHCPRequest &request);

protected:
    int id;           
    QSharedPointer<HostAddress> ip;          
    QSharedPointer<MACAddress> macAddress; 


private:
        
};

#endif // NODE_H

