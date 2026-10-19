#include "IPv4Header.h"
#include <QString>
#include <QDebug>

IPv4Header::IPv4Header(QObject *parent)
    : IPHeader(parent),
      m_versionHeaderLength(0x45),
      m_typeOfService(0),
      m_totalLength(0),
      m_identification(0),
      m_flagsFragmentOffset(0),
      m_ttl(64), 
      m_protocol(0),
      m_headerChecksum(0),
      m_sourceIp(IPv4Ptr_t::create()),
      m_destIp(IPv4Ptr_t::create())
{}


IPv4Header::IPv4Header(uint8_t versionHeaderLength,
                       uint8_t typeOfService,
                       uint16_t totalLength,
                       uint16_t identification,
                       uint16_t flagsFragmentOffset,
                       uint8_t ttl,
                       uint8_t protocol,
                       uint16_t headerChecksum,
                       const IPv4Ptr_t &sourceIp,
                       const IPv4Ptr_t &destIp,
                       QObject *parent)
    : IPHeader(UT::IPVersion::IPv4, parent),
    m_versionHeaderLength(versionHeaderLength),
    m_typeOfService(typeOfService),
    m_totalLength(totalLength),
    m_identification(identification),
    m_flagsFragmentOffset(flagsFragmentOffset),
    m_ttl(ttl),
    m_protocol(protocol),
    m_headerChecksum(headerChecksum),
    m_sourceIp(sourceIp ? sourceIp : IPv4Ptr_t::create()),
    m_destIp(destIp ? destIp : IPv4Ptr_t::create())
{}


void IPv4Header::clear() {
    m_versionHeaderLength = 0x45; 
    m_typeOfService = 0;
    m_totalLength = 0;
    m_identification = 0;
    m_flagsFragmentOffset = 0;
    m_ttl = 64;
    m_protocol = 0;
    m_headerChecksum = 0;
    m_sourceIp = IPv4Ptr_t::create();
    m_destIp = IPv4Ptr_t::create();
}

bool IPv4Header::isValid() const {
    return (headerVersionBits() == 4);
}

QString IPv4Header::toString() const {
    return QString("IPv4Header: ver=%1 ihl=%2 tos=%3 length=%4 ident=%5 flagsFrag=%6 ttl=%7 prot=%8 chksum=%9 src=%10 dst=%11")
            .arg(headerVersionBits())
            .arg(headerLength())
            .arg(m_typeOfService)
            .arg(m_totalLength)
            .arg(m_identification)
            .arg(m_flagsFragmentOffset)
            .arg(m_ttl)
            .arg(m_protocol)
            .arg(m_headerChecksum)
            .arg(m_sourceIp ? m_sourceIp->toString() : "null")
            .arg(m_destIp ? m_destIp->toString() : "null");
}
