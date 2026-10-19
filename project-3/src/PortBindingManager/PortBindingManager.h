#ifndef PORTBINDINGMANAGER_H
#define PORTBINDINGMANAGER_H

#include <QObject>
#include <QMap>
#include "Port.h"

class PortBindingManager : public QObject {
    Q_OBJECT

public:
    explicit PortBindingManager(QObject* parent = nullptr);
    ~PortBindingManager();

    bool bindPorts(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2);
    void unbindPorts(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2);

    bool arePortsBound(const QSharedPointer<Port>& port1, const QSharedPointer<Port>& port2) const;

private:
    QMap<QSharedPointer<Port>, QSharedPointer<Port>> bindings; 

    void establishConnection(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2);
    void terminateConnection(QSharedPointer<Port>& port1, QSharedPointer<Port>& port2);

};

#endif // PORTBINDINGMANAGER_H

