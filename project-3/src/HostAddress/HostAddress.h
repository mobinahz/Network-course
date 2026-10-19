#ifndef HOSTADDRESS_H
#define HOSTADDRESS_H

#include "AbstractIP.h"
#include "Globals.h"
#include "IPV4.h"
#include "IPV6.h"

#include <QObject>
#include <QSharedPointer>
#include <QPair>
#include <QString>
#include <QByteArray>
#include <QHostAddress>

enum class NetworkLayerProtocol {
    UnknownNetworkLayerProtocol,
    IPv4Protocol,
    IPv6Protocol
};

class HostAddress : public QObject {
    Q_OBJECT
public:
    explicit HostAddress(QObject *parent = nullptr);
    explicit HostAddress(SpecialAddress address, QObject *parent = nullptr);
    explicit HostAddress(const QString &address, QObject *parent = nullptr);
    explicit HostAddress(const Q_IPV6ADDR &ip6Addr, QObject *parent = nullptr);
    explicit HostAddress(const quint8 *ip6Addr, QObject *parent = nullptr);
    explicit HostAddress(const sockaddr *sockaddr, QObject *parent = nullptr);
    explicit HostAddress(quint32 ip4Addr, QObject *parent = nullptr);
    explicit HostAddress(QSharedPointer<AbstractIP> address, QObject *parent = nullptr);

    HostAddress(const HostAddress &address) = delete;
    HostAddress &operator=(const HostAddress &address) = delete;

    ~HostAddress() override;

    void clear();

    bool isBroadcast() const;
    bool isEqual(const HostAddress &other, ConversionMode mode = ConversionModeFlag::TolerantConversion) const;
    bool isGlobal() const;
    bool isInSubnet(const HostAddress &subnet, int netmask) const;
    bool isInSubnet(const QPair<HostAddress, int> &subnet) const;
    bool isLinkLocal() const;
    bool isLoopback() const;
    bool isMulticast() const;
    bool isNull() const;
    bool isPrivateUse() const;
    bool isSiteLocal() const;
    bool isUniqueLocalUnicast() const;
    int protocol() const;
    QString scopeId() const;

    QSharedPointer<HostAddress> nextAddress() const;


    void setAddress(quint32 ip4Addr);
    void setAddress(SpecialAddress address);
    bool setAddress(const QString &address);
    void setAddress(const Q_IPV6ADDR &ip6Addr);
    void setAddress(const quint8 *ip6Addr);
    void setAddress(const sockaddr *sockaddr);
    void setScopeId(const QString &id);
    void swap(HostAddress &other);

    quint32 toIPv4Address(bool *ok = nullptr) const;
    Q_IPV6ADDR toIPv6Address() const;
    QString toString() const;

    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray &data);
    bool toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const;
    bool fromSockAddr(const struct sockaddr *addr, socklen_t addrlen);
    bool matchesSubnet(const HostAddress &network, int prefixLength) const;

    bool operator!=(SpecialAddress other) const;
    bool operator!=(const HostAddress &other) const;
    HostAddress &operator=(SpecialAddress address);
    bool operator==(SpecialAddress other) const;
    bool operator==(const HostAddress &other) const;

Q_SIGNALS:
    void addressChanged();
    void errorOccurred(QString error);

private:
    void initConnections();

private:
    QSharedPointer<AbstractIP> m_address;
};


#endif // HOSTADDRESS_H
