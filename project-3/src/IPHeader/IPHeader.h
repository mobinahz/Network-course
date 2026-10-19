#ifndef IPHEADER_H
#define IPHEADER_H

#include "AbstractIP.h"
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QByteArray>
#include "Globals.h"

class IPHeader : public QObject {
    Q_OBJECT
public:
    explicit IPHeader(QObject *parent = nullptr) : QObject(parent) {}
    IPHeader(UT::IPVersion ipVersion, QObject *parent = nullptr) : QObject(parent), ipVersion(ipVersion) {}
    virtual ~IPHeader() {}

    virtual UT::IPVersion version() const = 0;

    virtual void clear() = 0;
    virtual bool isValid() const = 0;
    virtual QString toString() const = 0;


    virtual QByteArray toByteArray() const { return QByteArray(); }
    virtual bool fromByteArray(const QByteArray &) { return false; }

private:
    UT::IPVersion ipVersion;

Q_SIGNALS:
    void errorOccurred(QString msg);
};

#endif // IPHEADER_H
