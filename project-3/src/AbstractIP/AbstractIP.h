#ifndef ABSTRACTIP_H
#define ABSTRACTIP_H

#include "Globals.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QByteArray>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#else
// Unix-like 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

template <UT::IPVersion version>
class IP;

typedef IP<UT::IPVersion::IPv4> IPv4_t;
typedef IP<UT::IPVersion::IPv6> IPv6_t;
typedef QSharedPointer<IPv4_t>  IPv4Ptr_t;
typedef QSharedPointer<IPv6_t>  IPv6Ptr_t;

enum class ConversionModeFlag {
    StrictConversion = 0x1,
    ConvertV4MappedToIPv4 = 0x2,
    ConvertV4CompatToIPv4 = 0x4,
    ConvertLocalHost = 0x8,
    ConvertUnspecifiedAddress = 0x10,
    TolerantConversion = 0x20
};
Q_DECLARE_FLAGS(ConversionMode, ConversionModeFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ConversionMode)

enum class SpecialAddress {
    Null,
    LocalHost,
    LocalHostIPv6,
    Broadcast,
    AnyIPv4,
    AnyIPv6,
    Any
};

class AbstractIP : public QObject {
    Q_OBJECT

public:
    explicit AbstractIP(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~AbstractIP() {}

    virtual QString toString() const = 0;
    virtual uint64_t toValue() const = 0;
    virtual void fromString(const QString &ipString) = 0;
    virtual bool isLoopback() const = 0;
    virtual bool isMulticast() const = 0;

    virtual UT::IPVersion version() const = 0;
    virtual bool isNull() const = 0;
    virtual bool isValid() const = 0;
    virtual void clear() = 0;
    virtual void fromSpecialAddress(SpecialAddress addr) = 0;

    virtual QByteArray toByteArray() const = 0;
    virtual void fromByteArray(const QByteArray &data) = 0;

    virtual bool toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const = 0;
    virtual bool fromSockAddr(const struct sockaddr *addr, socklen_t addrlen) = 0;

    virtual void setScopeId(const QString &scopeId) { Q_UNUSED(scopeId); }
    virtual QString scopeId() const { return QString(); }

    virtual bool matchesSubnet(const AbstractIP &network, int prefixLength) const = 0;
    virtual QSharedPointer<AbstractIP> nextAddress() const = 0;



    virtual bool isLinkLocal() const = 0;
    virtual bool isPrivateUse() const = 0;
    virtual bool isUniqueLocalUnicast() const = 0;

Q_SIGNALS:
    void valueChanged();
    void addressChanged();
    void errorOccurred(QString error);
};

#endif // ABSTRACTIP_H
