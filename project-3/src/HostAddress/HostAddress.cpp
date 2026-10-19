#include "HostAddress.h"
#include "IPV4.h"
#include "IPV6.h"
#include <QHostAddress>
#include <cstring>


HostAddress::HostAddress(QObject *parent)
    : QObject(parent)
{
    IPv4Ptr_t ipv4 = IPv4Ptr_t::create();
    m_address = ipv4;
    initConnections();
}

HostAddress::HostAddress(SpecialAddress address, QObject *parent)
    : QObject(parent)
{
    if (address == SpecialAddress::LocalHostIPv6 || address == SpecialAddress::AnyIPv6) {
        IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
        m_address = ipv6;
    } else {
        IPv4Ptr_t ipv4 = IPv4Ptr_t::create();
        m_address = ipv4;
    }
    initConnections();
    m_address->fromSpecialAddress(address);
}

HostAddress::HostAddress(const QString &address, QObject *parent)
    : QObject(parent)
{
    {
        IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
        ipv6->fromString(address);
        if (!ipv6->isNull() && ipv6->isValid()) {
            m_address = ipv6;
            initConnections();
            return;
        }
    }

    {
        IPv4Ptr_t ipv4 = IPv4Ptr_t::create();
        ipv4->fromString(address);
        m_address = ipv4;
        initConnections();
    }
}

HostAddress::HostAddress(const Q_IPV6ADDR &ip6Addr, QObject *parent)
    : QObject(parent)
{
    IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
    std::array<uint8_t,16> arr;
    for (int i = 0; i < 16; ++i) arr[i] = ip6Addr[i];
    ipv6->fromBytes(arr);
    m_address = ipv6;
    initConnections();
}

HostAddress::HostAddress(const quint8 *ip6Addr, QObject *parent)
    : QObject(parent)
{
    IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
    std::array<uint8_t,16> arr;
    for (int i = 0; i < 16; ++i) arr[i] = ip6Addr[i];
    ipv6->fromBytes(arr);
    m_address = ipv6;
    initConnections();
}

HostAddress::HostAddress(const sockaddr *sockaddr, QObject *parent)
    : QObject(parent)
{
    IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
    if (ipv6->fromSockAddr(sockaddr, sizeof(struct sockaddr_in6))) {
        m_address = ipv6;
        initConnections();
        return;
    }
    IPv4Ptr_t ipv4 = IPv4Ptr_t::create();
    ipv4->fromSockAddr(sockaddr, sizeof(struct sockaddr_in));
    m_address = ipv4;
    initConnections();
}

HostAddress::HostAddress(quint32 ip4Addr, QObject *parent)
    : QObject(parent)
{
    IPv4Ptr_t ipv4 = IPv4Ptr_t::create(ip4Addr);
    m_address = ipv4;
    initConnections();
}

HostAddress::HostAddress(QSharedPointer<AbstractIP> ip, QObject* parent)
    : QObject(parent), m_address(ip) {
}

HostAddress::~HostAddress() {
}

void HostAddress::initConnections() {
    if (m_address) {
        connect(m_address.data(), &AbstractIP::valueChanged, this, &HostAddress::addressChanged);
        connect(m_address.data(), &AbstractIP::errorOccurred, this, &HostAddress::errorOccurred);
    }
}

void HostAddress::clear() {
    if (m_address) m_address->clear();
}

bool HostAddress::isBroadcast() const {
    if (!m_address) return false;
    IPv4_t *ipv4 = dynamic_cast<IPv4_t*>(m_address.data());
    return ipv4 && ipv4->isBroadcast();
}

bool HostAddress::isEqual(const HostAddress &other, ConversionMode mode) const {
    Q_UNUSED(mode)
    return *this == other;
}

bool HostAddress::isGlobal() const {
    if (!m_address || m_address->isNull() || m_address->isLoopback() || isLinkLocal() || isPrivateUse())
        return false;
    return true;
}

bool HostAddress::isInSubnet(const HostAddress &subnet, int netmask) const {
    if (!m_address || !subnet.m_address) return false;
    return m_address->matchesSubnet(*subnet.m_address, netmask);
}

bool HostAddress::isInSubnet(const QPair<HostAddress, int> &subnet) const {
    return isInSubnet(subnet.first, subnet.second);
}

bool HostAddress::isLinkLocal() const {
    return m_address && m_address->isLinkLocal();
}

bool HostAddress::isLoopback() const {
    return m_address && m_address->isLoopback();
}

bool HostAddress::isMulticast() const {
    return m_address && m_address->isMulticast();
}

bool HostAddress::isNull() const {
    return !m_address || m_address->isNull();
}

bool HostAddress::isPrivateUse() const {
    return m_address && m_address->isPrivateUse();
}

bool HostAddress::isSiteLocal() const {
    // For IPv6: fec0::/10 (deprecated)
    IPv6_t *ipv6 = dynamic_cast<IPv6_t*>(m_address.data());
    if (ipv6) {
        auto b = ipv6->toBytes();
        return (b[0] == 0xFE && (b[1] & 0xC0) == 0xC0);
    }
    return false;
}

bool HostAddress::isUniqueLocalUnicast() const {
    return m_address && m_address->isUniqueLocalUnicast();
}

int HostAddress::protocol() const {
    if (!m_address) return (int)NetworkLayerProtocol::UnknownNetworkLayerProtocol;
    switch (m_address->version()) {
    case UT::IPVersion::IPv4:
        return (int)NetworkLayerProtocol::IPv4Protocol;
    case UT::IPVersion::IPv6:
        return (int)NetworkLayerProtocol::IPv6Protocol;
    }
    return (int)NetworkLayerProtocol::UnknownNetworkLayerProtocol;
}

QString HostAddress::scopeId() const {
    return m_address ? m_address->scopeId() : QString();
}

void HostAddress::setAddress(quint32 ip4Addr) {
    m_address = IPv4Ptr_t::create(ip4Addr);
    initConnections();
}

void HostAddress::setAddress(SpecialAddress address) {
    if (address == SpecialAddress::LocalHostIPv6 || address == SpecialAddress::AnyIPv6) {
        m_address = IPv6Ptr_t::create();
    } else {
        m_address = IPv4Ptr_t::create();
    }
    initConnections();
    m_address->fromSpecialAddress(address);
}

bool HostAddress::setAddress(const QString &address) {
    {
        IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
        ipv6->fromString(address);
        if (!ipv6->isNull() && ipv6->isValid()) {
            m_address = ipv6;
            initConnections();
            return true;
        }
    }
    {
        IPv4Ptr_t ipv4 = IPv4Ptr_t::create();
        ipv4->fromString(address);
        if (!ipv4->isNull() && ipv4->isValid()) {
            m_address = ipv4;
            initConnections();
            return true;
        }
    }
    return false;
}

void HostAddress::setAddress(const Q_IPV6ADDR &ip6Addr) {
    IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
    std::array<uint8_t,16> arr;
    for (int i = 0; i < 16; ++i) arr[i] = ip6Addr[i];
    ipv6->fromBytes(arr);
    m_address = ipv6;
    initConnections();
}

void HostAddress::setAddress(const quint8 *ip6Addr) {
    IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
    std::array<uint8_t,16> arr;
    for (int i = 0; i < 16; ++i) arr[i] = ip6Addr[i];
    ipv6->fromBytes(arr);
    m_address = ipv6;
    initConnections();
}

void HostAddress::setAddress(const sockaddr *sockaddr) {
    IPv6Ptr_t ipv6 = IPv6Ptr_t::create();
    if (ipv6->fromSockAddr(sockaddr, sizeof(struct sockaddr_in6))) {
        m_address = ipv6;
        initConnections();
        return;
    }
    IPv4Ptr_t ipv4 = IPv4Ptr_t::create();
    ipv4->fromSockAddr(sockaddr, sizeof(struct sockaddr_in));
    m_address = ipv4;
    initConnections();
}

void HostAddress::setScopeId(const QString &id) {
    if (m_address) m_address->setScopeId(id);
}

void HostAddress::swap(HostAddress &other) {
    std::swap(m_address, other.m_address);
}

quint32 HostAddress::toIPv4Address(bool *ok) const {
    IPv4_t *ipv4 = dynamic_cast<IPv4_t*>(m_address.data());
    if (ipv4) {
        if (ok) *ok = true;
        return (quint32)ipv4->toValue();
    }
    if (ok) *ok = false;
    return 0;
}

Q_IPV6ADDR HostAddress::toIPv6Address() const {
    Q_IPV6ADDR addr;
    std::memset(&addr, 0, sizeof(addr));
    IPv6_t *ipv6 = dynamic_cast<IPv6_t*>(m_address.data());
    if (ipv6) {
        auto bytes = ipv6->toBytes();
        for (int i = 0; i < 16; ++i) {
            addr[i] = bytes[i];
        }
    }
    return addr;
}

QString HostAddress::toString() const {
    return m_address ? m_address->toString() : QString();
}

QByteArray HostAddress::toByteArray() const {
    return m_address ? m_address->toByteArray() : QByteArray();
}

bool HostAddress::fromByteArray(const QByteArray &data) {
    if (!m_address) return false;
    m_address->fromByteArray(data);
    return !m_address->isNull();
}

bool HostAddress::toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const {
    return m_address && m_address->toSockAddr(addr, addrlen);
}

bool HostAddress::fromSockAddr(const struct sockaddr *addr, socklen_t addrlen) {
    return m_address && m_address->fromSockAddr(addr, addrlen);
}

bool HostAddress::matchesSubnet(const HostAddress &network, int prefixLength) const {
    return m_address && network.m_address && m_address->matchesSubnet(*network.m_address, prefixLength);
}

bool HostAddress::operator!=(SpecialAddress other) const {
    HostAddress tmp(other);
    return !(*this == tmp);
}

bool HostAddress::operator!=(const HostAddress &other) const {
    return !(*this == other);
}

HostAddress &HostAddress::operator=(SpecialAddress address) {
    setAddress(address);
    return *this;
}

bool HostAddress::operator==(SpecialAddress other) const {
    HostAddress tmp(other);
    return *this == tmp;
}

QSharedPointer<HostAddress> HostAddress::nextAddress() const {
    if (m_address.isNull()) {
        return nullptr;
    }

    QSharedPointer<AbstractIP> nextIP = m_address->nextAddress();
    if (nextIP.isNull()) {
        return nullptr;
    }

    return QSharedPointer<HostAddress>(new HostAddress(nextIP));
}


bool HostAddress::operator==(const HostAddress &other) const {
    if (!m_address && !other.m_address) return true;
    if (!m_address || !other.m_address) return false;

    if (m_address->version() != other.m_address->version()) {
        return false;
    }

    if (m_address->version() == UT::IPVersion::IPv4) {
        return *(dynamic_cast<IPv4_t*>(m_address.data())) ==
               *(dynamic_cast<IPv4_t*>(other.m_address.data()));
    } else {
        return *(dynamic_cast<IPv6_t*>(m_address.data())) ==
               *(dynamic_cast<IPv6_t*>(other.m_address.data()));
    }
}


