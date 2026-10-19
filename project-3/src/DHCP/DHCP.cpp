#include "./../DHCP/DHCP.h"
#include <QDebug>

DHCP::DHCP(int asNumber, QObject *parent)
    : QObject(parent),
    m_enabled(false),
    m_asNumber(asNumber)
{
    m_nextIPv4 = QSharedPointer<HostAddress>::create(QString("192.168.0.100"));
    m_nextIPv6 = QSharedPointer<HostAddress>::create(QString("2001:db8::100"));

    QString logFileName = QString("dhcp_as_%1.log").arg(m_asNumber);
    m_logFile.setFileName(logFileName);
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "[DHCP] Could not open log file:" << logFileName;
    }
}

DHCP::~DHCP() {}

void DHCP::setEnabled(bool enabled)
{
    m_enabled = enabled;
    qDebug() << "[DHCP] DHCP for AS" << m_asNumber
             << (m_enabled ? "ENABLED" : "DISABLED");
}

void DHCP::onDhcpDiscover(const DHCPRequest &request)
{
    if (!m_enabled) {
        return; 
    }

    qDebug() << "[DHCP] DISCOVER from Node:" << request.nodeID
             << "MAC:" << request.macAddress->toString()
             << "AS:" << request.asNumber
             << "wantsIPv6:" << request.wantsIPv6;

    QSharedPointer<HostAddress> offeredIP = request.wantsIPv6
                                                ? getNextAvailableIPv6()
                                                : getNextAvailableIPv4();


    Q_EMIT dhcpOfferSent(request, offeredIP);
}

void DHCP::onDhcpRequest(const DHCPRequest &request)
{
    if (!m_enabled) {
        return;
    }

    qDebug() << "[DHCP] REQUEST from Node:" << request.nodeID
             << "MAC:" << request.macAddress->toString()
             << "AS:" << request.asNumber
             << "wantsIPv6:" << request.wantsIPv6;

    QSharedPointer<HostAddress> assignedIP = assignIPAddress(request);

    Q_EMIT dhcpAckSent(request, assignedIP);

    logAssignment(request.nodeID, *assignedIP, request.macAddress, request.asNumber);
}

QSharedPointer<HostAddress> DHCP::assignIPAddress(const DHCPRequest &request)
{
    QString macStr = request.macAddress->toString();

    if (m_assignedIPs.contains(macStr)) {
        return m_assignedIPs.value(macStr);
    }

    QSharedPointer<HostAddress> newIP = request.wantsIPv6
                                            ? getNextAvailableIPv6()
                                            : getNextAvailableIPv4();

    m_assignedIPs.insert(macStr, newIP);
    return newIP;
}

void DHCP::logAssignment(const QString &nodeID,
                         const HostAddress &ip,
                         const QSharedPointer<MACAddress> &mac,
                         int asNumber)
{
    if (!m_logFile.isOpen()) {
        return;
    }

    QTextStream out(&m_logFile);
    out << QDateTime::currentDateTime().toString(Qt::ISODate) << ", "
        << nodeID << ", "
        << ip.toString() << ", "
        << mac->toString() << ", "
        << "AS:" << asNumber << "\n";
    out.flush();
}

QSharedPointer<HostAddress> DHCP::getNextAvailableIPv4()
{
    QSharedPointer<HostAddress> current = m_nextIPv4;

    QSharedPointer<HostAddress> next = m_nextIPv4->nextAddress();
    if (next) {
        m_nextIPv4 = next;
    }

    return current;
}

QSharedPointer<HostAddress> DHCP::getNextAvailableIPv6()
{
    QSharedPointer<HostAddress> current = m_nextIPv6;

    QSharedPointer<HostAddress> next = m_nextIPv6->nextAddress();
    if (next) {
        m_nextIPv6 = next;
    }

    return current;
}


void DHCP::releaseIP(const QSharedPointer<MACAddress>& macAddress)
{
    if (!m_enabled) {
        qDebug() << "[DHCP] releaseIP called, but DHCP is disabled. Ignoring.";
        return;
    }

    if (m_assignedIPs.contains(macAddress->toString())) {
        qDebug() << "[DHCP] Releasing IP for MAC:" << macAddress->toString()
                 << "Assigned IP was:" << m_assignedIPs.value(macAddress->toString())->toString();
        m_assignedIPs.remove(macAddress->toString());
    } else {
        qDebug() << "[DHCP] No IP found for MAC:" << macAddress->toString();
    }
}

void DHCP::saveRecordsToFile(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = m_assignedIPs.begin(); it != m_assignedIPs.end(); ++it) {
            out << it.key() << "," << it.value()->toString() << "\n";
        }
        file.close();
    }
}


void DHCP::loadRecordsFromFile(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                QString mac = parts[0];
                QSharedPointer<HostAddress> ip(new HostAddress(parts[1]));
                m_assignedIPs[mac] = ip;
            }
        }
        file.close();
    }
}

