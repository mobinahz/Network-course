#include "IPV6.h"
#include "IPV4.h"
#include "IPV6.h"
#include <QStringList>
#include <QByteArray>
#include <cstring>
#ifdef Q_OS_UNIX
#include <netinet/in.h>
#include <sys/socket.h>
#endif


IP<UT::IPVersion::IPv4>::IP(QObject *parent)
    : AbstractIP(parent), ipValue(0)
{
}

IP<UT::IPVersion::IPv4>::IP(const QString &ipString, QObject *parent)
    : AbstractIP(parent), ipValue(0)
{
    fromString(ipString);
}

IP<UT::IPVersion::IPv4>::IP(uint32_t ipVal, QObject *parent)
    : AbstractIP(parent), ipValue(ipVal)
{
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

IP<UT::IPVersion::IPv4>::~IP() {}

QString IP<UT::IPVersion::IPv4>::toString() const {
    uint8_t b1 = (ipValue >> 24) & 0xFF;
    uint8_t b2 = (ipValue >> 16) & 0xFF;
    uint8_t b3 = (ipValue >> 8) & 0xFF;
    uint8_t b4 = ipValue & 0xFF;
    return QString("%1.%2.%3.%4").arg(b1).arg(b2).arg(b3).arg(b4);
}

uint64_t IP<UT::IPVersion::IPv4>::toValue() const {
    return static_cast<uint64_t>(ipValue);
}

void IP<UT::IPVersion::IPv4>::fromString(const QString &ipString) {
    QStringList parts = ipString.split('.');
    if (parts.size() == 4) {
        bool ok = true;
        uint32_t val = 0;
        for (int i = 0; i < 4; ++i) {
            int byteVal = parts[i].toInt(&ok);
            if (!ok || byteVal < 0 || byteVal > 255) {
                val = 0; // invalid
                ok = false;
                break;
            }
            val = (val << 8) + static_cast<uint32_t>(byteVal);
        }
        ipValue = val;
        Q_EMIT valueChanged();
        if (!ok) {
            Q_EMIT errorOccurred("Invalid IPv4 address format: " + ipString);
        } else {
            Q_EMIT addressChanged();
        }
    } else {
        ipValue = 0;
        Q_EMIT valueChanged();
        Q_EMIT errorOccurred("Invalid IPv4 address format: " + ipString);
    }
}

bool IP<UT::IPVersion::IPv4>::isLoopback() const {
    return ((ipValue & 0xFF000000) == 0x7F000000);
}

bool IP<UT::IPVersion::IPv4>::isMulticast() const {
    uint8_t firstByte = (ipValue >> 24) & 0xFF;
    return (firstByte >= 224 && firstByte <= 239);
}

UT::IPVersion IP<UT::IPVersion::IPv4>::version() const {
    return UT::IPVersion::IPv4;
}

bool IP<UT::IPVersion::IPv4>::isNull() const {
    return (ipValue == 0);
}

bool IP<UT::IPVersion::IPv4>::isValid() const {
    return true; 
}

void IP<UT::IPVersion::IPv4>::clear() {
    ipValue = 0;
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

void IP<UT::IPVersion::IPv4>::fromSpecialAddress(SpecialAddress addr) {
    switch (addr) {
    case SpecialAddress::Null:
    case SpecialAddress::Any:
    case SpecialAddress::AnyIPv4:
        fromString("0.0.0.0");
        break;
    case SpecialAddress::LocalHost:
        fromString("127.0.0.1");
        break;
    case SpecialAddress::LocalHostIPv6:
        fromString("127.0.0.1");
        break;
    case SpecialAddress::Broadcast:
        fromString("255.255.255.255");
        break;
    case SpecialAddress::AnyIPv6:
        fromString("0.0.0.0");
        break;
    }
}

void IP<UT::IPVersion::IPv4>::fromValue(uint32_t val) {
    ipValue = val;
    Q_EMIT valueChanged();
    Q_EMIT addressChanged();
}

void IP<UT::IPVersion::IPv4>::fromBytes(const std::array<uint8_t,4> &bytes) {
    uint32_t val = 0;
    for (uint8_t b : bytes) {
        val = (val << 8) | b;
    }
    fromValue(val);
}

std::array<uint8_t,4> IP<UT::IPVersion::IPv4>::toBytes() const {
    return {
        static_cast<uint8_t>((ipValue >> 24) & 0xFF),
        static_cast<uint8_t>((ipValue >> 16) & 0xFF),
        static_cast<uint8_t>((ipValue >> 8) & 0xFF),
        static_cast<uint8_t>(ipValue & 0xFF)
    };
}

bool IP<UT::IPVersion::IPv4>::isBroadcast() const {
    return ipValue == 0xFFFFFFFF;
}

IPv6_t IP<UT::IPVersion::IPv4>::toIPv6Mapped() const {
    std::array<uint8_t,16> v6Bytes = {0};
    v6Bytes[10] = 0xFF;
    v6Bytes[11] = 0xFF;
    v6Bytes[12] = (ipValue >> 24) & 0xFF;
    v6Bytes[13] = (ipValue >> 16) & 0xFF;
    v6Bytes[14] = (ipValue >> 8) & 0xFF;
    v6Bytes[15] = ipValue & 0xFF;
    return IPv6_t(v6Bytes);
}

IPv6_t IP<UT::IPVersion::IPv4>::toIPv6Compatible() const {
    std::array<uint8_t,16> v6Bytes = {0};
    v6Bytes[12] = (ipValue >> 24) & 0xFF;
    v6Bytes[13] = (ipValue >> 16) & 0xFF;
    v6Bytes[14] = (ipValue >> 8) & 0xFF;
    v6Bytes[15] = ipValue & 0xFF;
    return IPv6_t(v6Bytes);
}

QByteArray IP<UT::IPVersion::IPv4>::toByteArray() const {
    QByteArray arr(4, 0);
    arr[0] = static_cast<char>((ipValue >> 24) & 0xFF);
    arr[1] = static_cast<char>((ipValue >> 16) & 0xFF);
    arr[2] = static_cast<char>((ipValue >> 8) & 0xFF);
    arr[3] = static_cast<char>(ipValue & 0xFF);
    return arr;
}

void IP<UT::IPVersion::IPv4>::fromByteArray(const QByteArray &data) {
    if (data.size() != 4) {
        Q_EMIT errorOccurred("Invalid IPv4 byte array size");
        return;
    }
    uint32_t val = (static_cast<uint8_t>(data[0]) << 24) |
                   (static_cast<uint8_t>(data[1]) << 16) |
                   (static_cast<uint8_t>(data[2]) << 8)  |
                   static_cast<uint8_t>(data[3]);
    fromValue(val);
}

bool IP<UT::IPVersion::IPv4>::toSockAddr(struct sockaddr *addr, socklen_t *addrlen) const {
#ifdef Q_OS_UNIX
    if (!addr || !addrlen || *addrlen < (socklen_t)sizeof(struct sockaddr_in))
        return false;

    struct sockaddr_in *sa = reinterpret_cast<struct sockaddr_in*>(addr);
    memset(sa, 0, sizeof(*sa));
    sa->sin_family = AF_INET;
    sa->sin_addr.s_addr = htonl(ipValue);
    *addrlen = sizeof(struct sockaddr_in);
    return true;
#else
    Q_UNUSED(addr);
    Q_UNUSED(addrlen);
    return false;
#endif
}

bool IP<UT::IPVersion::IPv4>::fromSockAddr(const struct sockaddr *addr, socklen_t addrlen) {
#ifdef Q_OS_UNIX
    if (!addr || addrlen < (socklen_t)sizeof(struct sockaddr_in))
        return false;
    const struct sockaddr_in *sa = reinterpret_cast<const struct sockaddr_in*>(addr);
    if (sa->sin_family != AF_INET)
        return false;
    uint32_t val = ntohl(sa->sin_addr.s_addr);
    fromValue(val);
    return true;
#else
    Q_UNUSED(addr);
    Q_UNUSED(addrlen);
    return false;
#endif
}

bool IP<UT::IPVersion::IPv4>::matchesSubnet(const AbstractIP &network, int prefixLength) const {
    if (network.version() != UT::IPVersion::IPv4 || prefixLength < 0 || prefixLength > 32)
        return false;

    const IP<UT::IPVersion::IPv4> *net4 = dynamic_cast<const IP<UT::IPVersion::IPv4>*>(&network);
    if (!net4) return false;

    uint32_t mask = (prefixLength == 0) ? 0 : 0xFFFFFFFF << (32 - prefixLength);
    uint32_t thisVal = ipValue & mask;
    uint32_t netVal  = net4->ipValue & mask;
    return (thisVal == netVal);
}

bool IP<UT::IPVersion::IPv4>::isLinkLocal() const {
    // 169.254.0.0/16
    return ((ipValue & 0xFFFF0000) == 0xA9FE0000);
}

bool IP<UT::IPVersion::IPv4>::isPrivateUse() const {
    uint8_t first = (ipValue >> 24) & 0xFF;
    uint8_t second = (ipValue >> 16) & 0xFF;

    if (first == 10) return true;
    if (first == 172 && (second >= 16 && second <= 31)) return true;
    if (first == 192 && second == 168) return true;
    return false;
}

bool IP<UT::IPVersion::IPv4>::isUniqueLocalUnicast() const {
    return isPrivateUse();
}

QSharedPointer<IPv4_t> IP<UT::IPVersion::IPv4>::networkAddress(int prefixLength) const {
    if (prefixLength < 0 || prefixLength > 32) {
        return QSharedPointer<IPv4_t>::create(ipValue);
    }

    uint32_t mask = (prefixLength == 0) ? 0 : 0xFFFFFFFF << (32 - prefixLength);
    uint32_t net = ipValue & mask;
    return QSharedPointer<IPv4_t>::create(net);
}

QSharedPointer<IPv4_t> IP<UT::IPVersion::IPv4>::broadcastAddress(int prefixLength) const {
    if (prefixLength < 0 || prefixLength > 32) {
        return QSharedPointer<IPv4_t>::create(ipValue);
    }

    uint32_t mask = (prefixLength == 0) ? 0 : 0xFFFFFFFF << (32 - prefixLength);
    uint32_t net = ipValue & mask;
    uint32_t broadcast = net | ~mask;
    return QSharedPointer<IPv4_t>::create(broadcast);
}

QSharedPointer<AbstractIP> IP<UT::IPVersion::IPv4>::nextAddress() const {
    if (ipValue == 0xFFFFFFFF) {
        return nullptr; 
    }
    uint32_t nextValue = ipValue + 1;
    return QSharedPointer<AbstractIP>(new IP<UT::IPVersion::IPv4>(nextValue));
}


QSharedPointer<IPv4_t> IP<UT::IPVersion::IPv4>::previousAddress() const {
    if (ipValue == 0x00000000) {
       
        return QSharedPointer<IPv4_t>::create(ipValue);
    }
    return QSharedPointer<IPv4_t>::create(ipValue - 1);
}



bool IP<UT::IPVersion::IPv4>::operator==(const IP<UT::IPVersion::IPv4> &other) const {
    return ipValue == other.ipValue;
}

bool IP<UT::IPVersion::IPv4>::operator<(const IP<UT::IPVersion::IPv4> &other) const {
    return ipValue < other.ipValue;
}

bool IP<UT::IPVersion::IPv4>::operator>(const IP<UT::IPVersion::IPv4> &other) const {
    return ipValue > other.ipValue;
}

IP<UT::IPVersion::IPv4>& IP<UT::IPVersion::IPv4>::operator=(uint32_t val) {
    fromValue(val);
    return *this;
}
