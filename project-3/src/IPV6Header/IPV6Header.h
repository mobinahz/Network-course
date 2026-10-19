#ifndef IPV6HEADER_H
#define IPV6HEADER_H

#include "IPHeader.h"
#include "Globals.h"
#include "IPV6.h"
#include <QString>
#include <QSharedPointer>

class IPv6Header : public IPHeader {
    Q_OBJECT
public:
    explicit IPv6Header(QObject *parent = nullptr);

    IPv6Header(uint32_t versionTrafficClassFlowLabel,
               uint16_t payloadLength,
               uint8_t nextHeader,
               uint8_t hopLimit,
               const IPv6Ptr_t &sourceIP,
               const IPv6Ptr_t &destIP,
               QObject *parent = nullptr);

    UT::IPVersion version() const override { return UT::IPVersion::IPv6; }

    void clear() override;
    bool isValid() const override;
    QString toString() const override;

    void setSourceIP(const IPv6Ptr_t &src) { m_sourceIP = src; }
    void setDestIP(const IPv6Ptr_t &dst) { m_destIP = dst; }

    IPv6Ptr_t sourceIP() const { return m_sourceIP; }
    IPv6Ptr_t destIP() const { return m_destIP; }

    uint32_t versionTrafficClassFlowLabel() const { return m_versionTrafficClassFlowLabel; }
    uint8_t headerVersionBits() const { return (uint8_t)((m_versionTrafficClassFlowLabel >> 28) & 0x0F); }

private:
    uint32_t m_versionTrafficClassFlowLabel;
    uint16_t m_payloadLength;
    uint8_t m_nextHeader;
    uint8_t m_hopLimit;
    IPv6Ptr_t m_sourceIP;
    IPv6Ptr_t m_destIP;
};

#endif // IPV6HEADER_H
