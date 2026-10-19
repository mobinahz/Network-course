#ifndef DATALINKHEADER_H
#define DATALINKHEADER_H

#include <QString>
#include <QObject>

class DataLinkHeader : public QObject{
public:
    DataLinkHeader(const QString& sourceMAC, const QString& destMAC, quint16 type);

    QString getSourceMAC() const;
    QString getDestMAC() const;
    quint16 getType() const;

    void setSourceMAC(const QString& mac);
    void setDestMAC(const QString& mac);
    void setType(quint16 type);

private:
    QString sourceMAC;
    QString destMAC;
    quint16 type; 
};

#endif 
