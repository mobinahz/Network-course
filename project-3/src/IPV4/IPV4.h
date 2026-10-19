#ifndef IPV4_H
#define IPV4_H

#include "AbstractIP.h"
#include <array>

template <UT::IPVersion version>
class IP;

typedef IP<UT::IPVersion::IPv4> IPv4_t;
typedef IP<UT::IPVersion::IPv6> IPv6_t;
typedef QSharedPointer<IPv4_t>  IPv4Ptr_t;
typedef QSharedPointer<IPv6_t>  IPv6Ptr_t;

template<>
class IP<UT::IPVersion::IPv4> : public AbstractIP {
public:
    explicit IP(QObject *parent = nullptr);
    explicit IP(const QString &ipString, QObject *parent = nullptr);
    explicit IP(uint32_t ipValue, QObject *parent = nullptr);
    ~IP() override;

    QString toString() const override;
    uint64_t toValue() const override;
    void fromString(const QString &ipString) override;
    bool isLoopback() const override;
    bool isMulticast() const override;

    UT::IPVersion version() const override;
    bool isNull() const override;
    bool isValid() const override;
    void clear() override;
    void fromSpecialAddress(SpecialAddress addr) override;

    void fromValue(uint32_t val);
    void fromBytes(const std::array<uint8_t,4> &bytes);
    std::array<uint8_t,4> toBytes() const;
    bool isBroadcast() const;

    void setScopeId(const QString &scopeId) override { Q_UNUSED(scopeId); }
    QString scopeId() const override { return QString(); }

    IPv6_t toIPv6Mapped() const;
    IPv6_t toIPv6Compatible() const;

    QByteArray toByteArray() const override;
    void fromByteArray(const QByteArray &data) override;

    bool toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const override;
    bool fromSockAddr(const struct sockaddr *addr, socklen_t addrlen) override;

    bool matchesSubnet(const AbstractIP &network, int prefixLength) const override;
    bool isLinkLocal() const override;
    bool isPrivateUse() const override;
    bool isUniqueLocalUnicast() const override;

    QSharedPointer<IPv4_t> networkAddress(int prefixLength) const;
    QSharedPointer<IPv4_t> broadcastAddress(int prefixLength) const;
    QSharedPointer<AbstractIP> nextAddress() const override;



    QSharedPointer<IPv4_t> previousAddress() const;


    bool operator==(const IP<UT::IPVersion::IPv4> &other) const;
    bool operator<(const IP<UT::IPVersion::IPv4> &other) const;
    bool operator>(const IP<UT::IPVersion::IPv4> &other) const;
    IP<UT::IPVersion::IPv4>& operator=(uint32_t val);

private:
    uint32_t ipValue;
};

#endif // IPV4_H
