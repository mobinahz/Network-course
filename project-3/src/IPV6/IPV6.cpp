#include "IPV6.h"
#include "IPV4.h"

#include <QHostAddress>
#include <QAbstractSocket>
#include <QByteArray>
#include <cstring>
#ifdef Q_OS_UNIX
#include <netinet/in.h>
#include <sys/socket.h>
#endif

IP<UT::IPVersion::IPv6>::IP(QObject *parent)
    : AbstractIP(parent)
{
    ipValue.fill(0);
}

IP<UT::IPVersion::IPv6>::IP(const QString &ipString, QObject *parent)
    : AbstractIP(parent)
{
    ipValue.fill(0);
    fromString(ipString);
}

IP<UT::IPVersion::IPv6>::IP(const std::array<uint8_t, 16>& value, QObject *parent)
    : AbstractIP(parent), ipValue(value)
{
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

IP<UT::IPVersion::IPv6>::~IP() {}

QString IP<UT::IPVersion::IPv6>::toString() const {
    QHostAddress addr;
    Q_IPV6ADDR qaddr;
    for (int i = 0; i < 16; ++i) qaddr[i] = ipValue[i];
    addr.setAddress(qaddr);
    if (!m_scopeId.isEmpty() && isLinkLocal()) {
        return addr.toString() + "%" + m_scopeId;
    }
    return addr.toString();
}

uint64_t IP<UT::IPVersion::IPv6>::toValue() const {
    // top 64 bits
    uint64_t val = 0;
    for (int i = 0; i < 8; ++i) {
        val = (val << 8) | ipValue[i];
    }
    return val;
}

void IP<UT::IPVersion::IPv6>::fromString(const QString &ipString) {
    ipValue.fill(0);
    QString addrStr = ipString;
    int percentIndex = addrStr.indexOf('%');
    if (percentIndex != -1) {
        m_scopeId = addrStr.mid(percentIndex+1);
        addrStr = addrStr.left(percentIndex);
    } else {
        m_scopeId.clear();
    }

    QHostAddress tmp(addrStr);
    if (tmp.protocol() == QAbstractSocket::IPv6Protocol) {
        Q_IPV6ADDR addr = tmp.toIPv6Address();
        for (int i = 0; i < 16; ++i) {
            ipValue[i] = addr[i];
        }
        Q_EMIT valueChanged();
        Q_EMIT addressChanged();
    } else {
        ipValue.fill(0);
        Q_EMIT valueChanged();
        Q_EMIT errorOccurred("Invalid IPv6 address: " + ipString);
    }
}

bool IP<UT::IPVersion::IPv6>::isLoopback() const {
    static const std::array<uint8_t,16> loopback = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    return ipValue == loopback;
}

bool IP<UT::IPVersion::IPv6>::isMulticast() const {
    return (ipValue[0] == 0xFF);
}

UT::IPVersion IP<UT::IPVersion::IPv6>::version() const {
    return UT::IPVersion::IPv6;
}

bool IP<UT::IPVersion::IPv6>::isNull() const {
    for (auto b : ipValue) {
        if (b != 0) return false;
    }
    return true;
}

bool IP<UT::IPVersion::IPv6>::isValid() const {
    return true;
}

void IP<UT::IPVersion::IPv6>::clear() {
    ipValue.fill(0);
    m_scopeId.clear();
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

void IP<UT::IPVersion::IPv6>::fromSpecialAddress(SpecialAddress addr) {
    switch (addr) {
    case SpecialAddress::Null:
    case SpecialAddress::Any:
    case SpecialAddress::AnyIPv6:
        fromString("::");
        break;
    case SpecialAddress::LocalHost:
    case SpecialAddress::LocalHostIPv6:
        fromString("::1");
        break;
    case SpecialAddress::Broadcast:
        fromString("ff02::1");
        break;
    case SpecialAddress::AnyIPv4:
        fromString("::ffff:0.0.0.0");
        break;
    }
}

void IP<UT::IPVersion::IPv6>::fromBytes(const std::array<uint8_t,16> &bytes) {
    ipValue = bytes;
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

std::array<uint8_t,16> IP<UT::IPVersion::IPv6>::toBytes() const {
    return ipValue;
}

QSharedPointer<AbstractIP> IP<UT::IPVersion::IPv6>::nextAddress() const {
    std::array<uint8_t, 16> nextValue = ipValue;

    for (int i = 15; i >= 0; --i) {
        if (++nextValue[i] != 0) {
            break; 
        }
    }

    if (std::all_of(nextValue.begin(), nextValue.end(), [](uint8_t byte) { return byte == 0; })) {
        return nullptr;
    }

    return QSharedPointer<AbstractIP>(new IP<UT::IPVersion::IPv6>(nextValue));
}


bool IP<UT::IPVersion::IPv6>::isIPv4Mapped() const {
    static const uint8_t prefix[12] = {0,0,0,0,0,0,0,0,0,0,0xFF,0xFF};
    return std::equal(prefix, prefix+12, ipValue.begin());
}

bool IP<UT::IPVersion::IPv6>::isIPv4Compatible() const {
    static const uint8_t prefix[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    return std::equal(prefix, prefix+12, ipValue.begin()) && !isIPv4Mapped();
}

IPv4_t IP<UT::IPVersion::IPv6>::toIPv4() const {
    if (isIPv4Mapped() || isIPv4Compatible()) {
        uint32_t val = 0;
        for (int i = 12; i < 16; ++i) {
            val = (val << 8) | ipValue[i];
        }
        return IPv4_t(val);
    }
    return IPv4_t();
}

QByteArray IP<UT::IPVersion::IPv6>::toByteArray() const {
    QByteArray arr(16, 0);
    for (int i = 0; i < 16; ++i) {
        arr[i] = static_cast<char>(ipValue[i]);
    }
    return arr;
}

void IP<UT::IPVersion::IPv6>::fromByteArray(const QByteArray &data) {
    if (data.size() != 16) {
        Q_EMIT errorOccurred("Invalid IPv6 byte array size");
        return;
    }
    for (int i = 0; i < 16; ++i) {
        ipValue[i] = static_cast<uint8_t>(data[i]);
    }
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

bool IP<UT::IPVersion::IPv6>::toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const {
#ifdef Q_OS_UNIX
    if (!addr || !addrlen || *addrlen < (socklen_t)sizeof(struct sockaddr_in6))
        return false;
    struct sockaddr_in6 *sa = reinterpret_cast<struct sockaddr_in6*>(addr);
    memset(sa, 0, sizeof(*sa));
    sa->sin6_family = AF_INET6;
    memcpy(sa->sin6_addr.s6_addr, ipValue.data(), 16);

    bool ok = false;
    uint scopeNum = m_scopeId.toUInt(&ok);
    if (ok) {
        sa->sin6_scope_id = scopeNum;
    }
    *addrlen = sizeof(struct sockaddr_in6);
    return true;
#else
    Q_UNUSED(addr);
    Q_UNUSED(addrlen);
    return false;
#endif
}

bool IP<UT::IPVersion::IPv6>::fromSockAddr(const struct sockaddr *addr, socklen_t addrlen) {
#ifdef Q_OS_UNIX
    if (!addr || addrlen < (socklen_t)sizeof(struct sockaddr_in6))
        return false;
    const struct sockaddr_in6 *sa = reinterpret_cast<const struct sockaddr_in6*>(addr);
    if (sa->sin6_family != AF_INET6)
        return false;
    for (int i = 0; i < 16; ++i) {
        ipValue[i] = sa->sin6_addr.s6_addr[i];
    }
    if (sa->sin6_scope_id != 0) {
        m_scopeId = QString::number(sa->sin6_scope_id);
    } else {
        m_scopeId.clear();
    }
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
    return true;
#else
    Q_UNUSED(addr);
    Q_UNUSED(addrlen);
    return false;
#endif
}

void IP<UT::IPVersion::IPv6>::setScopeId(const QString &scopeId) {
    m_scopeId = scopeId;
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

QString IP<UT::IPVersion::IPv6>::scopeId() const {
    return m_scopeId;
}

bool IP<UT::IPVersion::IPv6>::matchesSubnet(const AbstractIP &network, int prefixLength) const {
    if (network.version() != UT::IPVersion::IPv6 || prefixLength < 0 || prefixLength > 128)
        return false;
    const IP<UT::IPVersion::IPv6> *net6 = dynamic_cast<const IP<UT::IPVersion::IPv6>*>(&network);
    if (!net6) return false;

    int fullBytes = prefixLength / 8;
    int remainingBits = prefixLength % 8;

    for (int i = 0; i < fullBytes; ++i) {
        if (ipValue[i] != net6->ipValue[i]) return false;
    }

    if (remainingBits > 0) {
        uint8_t mask = 0xFF << (8 - remainingBits);
        if ((ipValue[fullBytes] & mask) != (net6->ipValue[fullBytes] & mask))
            return false;
    }

    return true;
}

bool IP<UT::IPVersion::IPv6>::isLinkLocal() const {
    // fe80::/10
    return (ipValue[0] == 0xFE && (ipValue[1] & 0xC0) == 0x80);
}

bool IP<UT::IPVersion::IPv6>::isPrivateUse() const {
    // fc00::/7
    return (ipValue[0] & 0xFE) == 0xFC;
}

bool IP<UT::IPVersion::IPv6>::isUniqueLocalUnicast() const {
    // same as isPrivateUse()
    return isPrivateUse();
}


QSharedPointer<IPv6_t> IP<UT::IPVersion::IPv6>::applyMask(int prefixLength) const {
    if (prefixLength < 0 || prefixLength > 128) {
        return QSharedPointer<IPv6_t>::create(ipValue, const_cast<IP<UT::IPVersion::IPv6>*>(this)->parent());
    }

    IP<UT::IPVersion::IPv6> temp(this->parent());
    temp.ipValue = ipValue;
    int fullBytes = prefixLength / 8;
    int remainingBits = prefixLength % 8;

    for (int i = fullBytes + (remainingBits > 0 ? 1 : 0); i < 16; ++i) {
        temp.ipValue[i] = 0;
    }

    if (remainingBits > 0) {
        uint8_t mask = 0xFF << (8 - remainingBits);
        temp.ipValue[fullBytes] &= mask;
    }

    return QSharedPointer<IPv6_t>::create(temp.ipValue, temp.parent());
}



QSharedPointer<IPv6_t> IP<UT::IPVersion::IPv6>::previousAddress() const {
 
    bool isMin = true;
    for (auto b : ipValue) {
        if (b != 0x00) {
            isMin = false;
            break;
        }
    }
    if (isMin) {
     
        return QSharedPointer<IPv6_t>::create(ipValue, const_cast<IP<UT::IPVersion::IPv6>*>(this)->parent());
    }

    IP<UT::IPVersion::IPv6> temp(this->parent());
    temp.ipValue = ipValue;

    for (int i = 15; i >= 0; --i) {
        int val = static_cast<int>(temp.ipValue[i]) - 1;
        temp.ipValue[i] = static_cast<uint8_t>(val & 0xFF);
        if (val >= 0) {
            break; 
        } else {
        }
    }

    return QSharedPointer<IPv6_t>::create(temp.ipValue, temp.parent());
}


bool IP<UT::IPVersion::IPv6>::operator==(const IP<UT::IPVersion::IPv6> &other) const {
    return ipValue == other.ipValue && m_scopeId == other.m_scopeId;
}

bool IP<UT::IPVersion::IPv6>::operator<(const IP<UT::IPVersion::IPv6> &other) const {
    if (ipValue < other.ipValue) return true;
    if (ipValue > other.ipValue) return false;
    return m_scopeId < other.m_scopeId;
}

bool IP<UT::IPVersion::IPv6>::operator>(const IP<UT::IPVersion::IPv6> &other) const {
    return other < *this;
}
