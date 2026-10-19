#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QString>
#include <QByteArray>
#include <QObject>

class MACAddress:public QObject
{
public:
    explicit MACAddress(const QString &macStr = "");
    void generateRandomMAC();
    QString toString() const;
    bool operator==(const MACAddress &other) const;
    bool operator!=(const MACAddress &other) const;
    bool isValidMAC(const QString &macStr);
    QByteArray getBytes() const;

    void setBytes(const QByteArray &bytes);

private:
    void parseMAC(const QString &macStr);
    QByteArray m_macBytes; 
};

#endif // MACADDRESS_H

