#include "MACAddress.h"
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QStringList>
#include <stdexcept>
#include <iomanip>
#include <sstream>

MACAddress::MACAddress(const QString &macStr)
{
    if (macStr.isEmpty()) {
        generateRandomMAC();
    } else {
        if (!isValidMAC(macStr)) {
            throw std::invalid_argument("Invalid MAC address format: " + macStr.toStdString());
        }
        parseMAC(macStr);
    }
}

void MACAddress::generateRandomMAC()
{
    m_macBytes.resize(6);
    for (int i = 0; i < 6; ++i) {
        m_macBytes[i] = static_cast<quint8>(QRandomGenerator::global()->bounded(256));
    }
    m_macBytes[0] &= 0xFE;
}

void MACAddress::parseMAC(const QString &macStr)
{
    QStringList parts = macStr.split(':');
    m_macBytes.resize(6);

    for (int i = 0; i < 6; ++i) {
        bool ok;
        int byte = parts[i].toInt(&ok, 16);
        if (!ok || byte < 0 || byte > 0xFF) {
            throw std::invalid_argument("Invalid MAC address component: " + parts[i].toStdString());
        }
        m_macBytes[i] = static_cast<quint8>(byte);
    }
}

QString MACAddress::toString() const
{
    QStringList parts;
    for (int i = 0; i < 6; ++i) {
        parts.append(QString("%1").arg(static_cast<int>(m_macBytes[i]), 2, 16, QLatin1Char('0')).toUpper());
    }
    return parts.join(':');
}

bool MACAddress::operator==(const MACAddress &other) const
{
    return m_macBytes == other.m_macBytes;
}

bool MACAddress::operator!=(const MACAddress &other) const
{
    return !(*this == other);
}

bool MACAddress::isValidMAC(const QString &macStr)
{
    static const QRegularExpression macRegex("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");
    return macRegex.match(macStr).hasMatch();
}

QByteArray MACAddress::getBytes() const
{
    return m_macBytes;
}

void MACAddress::setBytes(const QByteArray &bytes)
{
    if (bytes.size() != 6) {
        throw std::invalid_argument("Invalid MAC address byte array size.");
    }
    m_macBytes = bytes;
}

