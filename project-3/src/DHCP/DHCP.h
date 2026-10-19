#ifndef DHCP_H
#define DHCP_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QSharedPointer>

#include "HostAddress.h"
#include "MACAddress.h"

struct DHCPRequest {
    QString    nodeID;
    QSharedPointer<MACAddress>       macAddress;
    int        asNumber;
    bool       wantsIPv6;
};


class DHCP : public QObject
{
    Q_OBJECT

public:
    explicit DHCP(int asNumber, QObject *parent = nullptr);
    ~DHCP();


    void setEnabled(bool enabled);
    bool isEnabled() const { return m_enabled; }

    void releaseIP(const QSharedPointer<MACAddress>& macAdress);

 
    void saveRecordsToFile(const QString &filePath);

    void loadRecordsFromFile(const QString &filePath);

    QSharedPointer<HostAddress> assignIPAddress(const DHCPRequest &request);

Q_SIGNALS:
    void dhcpOfferSent(const DHCPRequest &request, const QSharedPointer<HostAddress> &offeredIP);
    void dhcpAckSent(const DHCPRequest &request, const QSharedPointer<HostAddress> &assignedIP);

public Q_SLOTS:
    void onDhcpDiscover(const DHCPRequest &request);
    void onDhcpRequest(const DHCPRequest &request);

private:

    void logAssignment(const QString &nodeID,
                       const HostAddress &ip,
                       const QSharedPointer<MACAddress> &mac,
                       int asNumber);


    QSharedPointer<HostAddress> getNextAvailableIPv4();


    QSharedPointer<HostAddress> getNextAvailableIPv6();

private:
    bool m_enabled;
    int  m_asNumber;

    QMap<QString, QSharedPointer<HostAddress>> m_assignedIPs;

    QSharedPointer<HostAddress> m_nextIPv4;
    QSharedPointer<HostAddress> m_nextIPv6;

    QFile m_logFile;
};


#endif

