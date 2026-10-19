#include "IPv6Header.h"
#include <QString>
#include <QDebug>

IPv6Header::IPv6Header(QObject *parent)
    : IPHeader(parent),
      m_versionTrafficClassFlowLabel(0x60000000), 
      m_payloadLength(0),
      m_nextHeader(0),
      m_hopLimit(64), 
      m_sourceIP(IPv6Ptr_t::create()),
      m_destIP(IPv6Ptr_t::create())
{}

IPv6Header::IPv6Header(uint32_t versionTrafficClassFlowLabel,
                       uint16_t payloadLength,
                       uint8_t nextHeader,
                       uint8_t hopLimit,
                       const IPv6Ptr_t &sourceIP,
                       const IPv6Ptr_t &destIP,
                       QObject *parent)
    : IPHeader(UT::IPVersion::IPv6 ,parent),
    m_versionTrafficClassFlowLabel(versionTrafficClassFlowLabel),
    m_payloadLength(payloadLength),
    m_nextHeader(nextHeader),
    m_hopLimit(hopLimit),
    m_sourceIP(sourceIP ? sourceIP : IPv6Ptr_t::create()),
    m_destIP(destIP ? destIP : IPv6Ptr_t::create())
{}

void IPv6Header::clear() {
    m_versionTrafficClassFlowLabel = 0x60000000; 
    m_payloadLength = 0;
    m_nextHeader = 0;
    m_hopLimit = 64;
    m_sourceIP = IPv6Ptr_t::create();
    m_destIP = IPv6Ptr_t::create();
}

bool IPv6Header::isValid() const {
    return (headerVersionBits() == 6);
}

QString IPv6Header::toString() const {
    return QString("IPv6Header: ver=%1 flow=0x%2 payloadLen=%3 nextHdr=%4 hopLimit=%5 src=%6 dst=%7")
    .arg(headerVersionBits())
        .arg(QString::number(m_versionTrafficClassFlowLabel, 16))
        .arg(m_payloadLength)
        .arg(m_nextHeader)
        .arg(m_hopLimit)
        .arg(m_sourceIP ? m_sourceIP->toString() : "null")
        .arg(m_destIP ? m_destIP->toString() : "null");
}
