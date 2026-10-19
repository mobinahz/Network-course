#ifndef IPV6_H
#define IPV6_H

#include "AbstractIP.h"
#include <array>

template <UT::IPVersion version>
class IP;

typedef IP<UT::IPVersion::IPv4> IPv4_t;
typedef IP<UT::IPVersion::IPv6> IPv6_t;
typedef QSharedPointer<IPv4_t>  IPv4Ptr_t;
typedef QSharedPointer<IPv6_t>  IPv6Ptr_t;

template<>
class IP<UT::IPVersion::IPv6> : public AbstractIP {
public:
    explicit IP(QObject *parent = nullptr);
    explicit IP(const QString &ipString, QObject *parent = nullptr);
    explicit IP(const std::array<uint8_t, 16>& value, QObject *parent = nullptr);
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

    void fromBytes(const std::array<uint8_t,16> &bytes);
    std::array<uint8_t,16> toBytes() const;

    bool isIPv4Mapped() const;
    bool isIPv4Compatible() const;
    IPv4_t toIPv4() const;

    QByteArray toByteArray() const override;
    void fromByteArray(const QByteArray &data) override;

    bool toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const override;
    bool fromSockAddr(const struct sockaddr *addr, socklen_t addrlen) override;

    void setScopeId(const QString &scopeId) override;
    QString scopeId() const override;

    bool matchesSubnet(const AbstractIP &network, int prefixLength) const override;
    bool isLinkLocal() const override;
    bool isPrivateUse() const override;
    bool isUniqueLocalUnicast() const override;

    QSharedPointer<IPv6_t> applyMask(int prefixLength) const;
    QSharedPointer<AbstractIP> nextAddress() const override;

    QSharedPointer<IPv6_t> previousAddress() const;

    bool operator==(const IP<UT::IPVersion::IPv6> &other) const;
    bool operator<(const IP<UT::IPVersion::IPv6> &other) const;
    bool operator>(const IP<UT::IPVersion::IPv6> &other) const;

private:
    std::array<uint8_t,16> ipValue;
    QString m_scopeId;
};

#endif // IPV6_H
