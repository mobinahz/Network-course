#include "TCPHeader.h"
#include <QtEndian>

TCPHeader::TCPHeader()
    : m_sourcePort(0),
    m_destinationPort(0),
    m_sequenceNumber(0),
    m_ackNumber(0),
    m_dataOffset(5),
    m_flags(0),
    m_window(0),
    m_checksum(0),
    m_urgentPointer(0)
{
}

TCPHeader::TCPHeader(quint16 sourcePort,
                     quint16 destPort,
                     quint32 seqNumber,
                     quint32 ackNumber,
                     quint8 dataOffset,
                     quint8 flags,
                     quint16 window,
                     quint16 checksum,
                     quint16 urgentPointer)
    : m_sourcePort(sourcePort),
    m_destinationPort(destPort),
    m_sequenceNumber(seqNumber),
    m_ackNumber(ackNumber),
    m_dataOffset(dataOffset),
    m_flags(flags),
    m_window(window),
    m_checksum(checksum),
    m_urgentPointer(urgentPointer)
{
}


quint16 TCPHeader::sourcePort() const {
    return m_sourcePort;
}

quint16 TCPHeader::destinationPort() const {
    return m_destinationPort;
}

quint32 TCPHeader::sequenceNumber() const {
    return m_sequenceNumber;
}

quint32 TCPHeader::acknowledgmentNumber() const {
    return m_ackNumber;
}

quint8 TCPHeader::dataOffset() const {
    return m_dataOffset;
}

quint8 TCPHeader::flags() const {
    return m_flags;
}

quint16 TCPHeader::windowSize() const {
    return m_window;
}

quint16 TCPHeader::checksum() const {
    return m_checksum;
}

quint16 TCPHeader::urgentPointer() const {
    return m_urgentPointer;
}

void TCPHeader::setSourcePort(quint16 port) {
    m_sourcePort = port;
}

void TCPHeader::setDestinationPort(quint16 port) {
    m_destinationPort = port;
}

void TCPHeader::setSequenceNumber(quint32 seq) {
    m_sequenceNumber = seq;
}

void TCPHeader::setAcknowledgmentNumber(quint32 ack) {
    m_ackNumber = ack;
}

void TCPHeader::setDataOffset(quint8 offset) {
    m_dataOffset = offset;
}

void TCPHeader::setFlags(quint8 flags) {
    m_flags = flags;
}

void TCPHeader::setWindowSize(quint16 size) {
    m_window = size;
}

void TCPHeader::setChecksum(quint16 checksum) {
    m_checksum = checksum;
}

void TCPHeader::setUrgentPointer(quint16 pointer) {
    m_urgentPointer = pointer;
}

QByteArray TCPHeader::toByteArray() const
{
    QByteArray buffer;
    buffer.resize(20);

    *reinterpret_cast<quint16*>(buffer.data() + 0) = qToBigEndian(m_sourcePort);
    *reinterpret_cast<quint16*>(buffer.data() + 2) = qToBigEndian(m_destinationPort);
    *reinterpret_cast<quint32*>(buffer.data() + 4) = qToBigEndian(m_sequenceNumber);
    *reinterpret_cast<quint32*>(buffer.data() + 8) = qToBigEndian(m_ackNumber);

    quint8 offsetAndFlagsHi = (m_dataOffset << 4) & 0xF0;

    buffer[12] = static_cast<char>(offsetAndFlagsHi);
    buffer[13] = static_cast<char>(m_flags);

    *reinterpret_cast<quint16*>(buffer.data() + 14) = qToBigEndian(m_window);
    *reinterpret_cast<quint16*>(buffer.data() + 16) = qToBigEndian(m_checksum);
    *reinterpret_cast<quint16*>(buffer.data() + 18) = qToBigEndian(m_urgentPointer);

    return buffer;
}
