#ifndef TCPHEADER_H
#define TCPHEADER_H

#include <QtGlobal>
#include <QByteArray>

class TCPHeader
{
public:
    TCPHeader();
    TCPHeader(quint16 sourcePort,
              quint16 destPort,
              quint32 seqNumber,
              quint32 ackNumber,
              quint8 dataOffset,
              quint8 flags,
              quint16 window,
              quint16 checksum,
              quint16 urgentPointer);


    quint16 sourcePort() const;
    quint16 destinationPort() const;
    quint32 sequenceNumber() const;
    quint32 acknowledgmentNumber() const;
    quint8 dataOffset() const;
    quint8 flags() const;
    quint16 windowSize() const;
    quint16 checksum() const;
    quint16 urgentPointer() const;


    void setSourcePort(quint16 port);
    void setDestinationPort(quint16 port);
    void setSequenceNumber(quint32 seq);
    void setAcknowledgmentNumber(quint32 ack);
    void setDataOffset(quint8 offset);
    void setFlags(quint8 flags);
    void setWindowSize(quint16 size);
    void setChecksum(quint16 checksum);
    void setUrgentPointer(quint16 pointer);

    QByteArray toByteArray() const;

private:
    quint16 m_sourcePort;
    quint16 m_destinationPort;
    quint32 m_sequenceNumber;
    quint32 m_ackNumber;
    quint8  m_dataOffset;
    quint8  m_flags;
    quint16 m_window;
    quint16 m_checksum;
    quint16 m_urgentPointer;
};

#endif
