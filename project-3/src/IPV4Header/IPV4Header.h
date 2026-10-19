#ifndef IPV4HEADER_H
#define IPV4HEADER_H

#include "IPHeader.h"
#include "Globals.h"
#include "IPV4.h"
#include <QString>
#include <QSharedPointer>

class IPv4Header : public IPHeader {
    Q_OBJECT
public:
    explicit IPv4Header(QObject *parent = nullptr);

    IPv4Header(uint8_t versionHeaderLength,
               uint8_t typeOfService,
               uint16_t totalLength,
               uint16_t identification,
               uint16_t flagsFragmentOffset,
               uint8_t ttl,
               uint8_t protocol,
               uint16_t headerChecksum,
               const IPv4Ptr_t &sourceIp,
               const IPv4Ptr_t &destIp,
               QObject *parent = nullptr);

    UT::IPVersion version() const override { return UT::IPVersion::IPv4; }

    void clear() override;
    bool isValid() const override;
    QString toString() const override;


    void setSourceIp(const IPv4Ptr_t &src) { m_sourceIp = src; }
    void setDestIp(const IPv4Ptr_t &dst) { m_destIp = dst; }

    IPv4Ptr_t sourceIp() const { return m_sourceIp; }
    IPv4Ptr_t destIp() const { return m_destIp; }

    uint8_t headerVersionBits() const { return (m_versionHeaderLength >> 4) & 0x0F; }
    uint8_t headerLength() const { return m_versionHeaderLength & 0x0F; }


private:
    uint8_t m_versionHeaderLength;
    uint8_t m_typeOfService;
    uint16_t m_totalLength;
    uint16_t m_identification;
    uint16_t m_flagsFragmentOffset;
    uint8_t m_ttl;
    uint8_t m_protocol;
    uint16_t m_headerChecksum;
    IPv4Ptr_t m_sourceIp;
    IPv4Ptr_t m_destIp;
};

#endif // IPV4HEADER_H
