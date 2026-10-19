#ifndef RIP_H
#define RIP_H

#include "HostAddress.h"
#include "../Port/Port.h"
#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QVector>

struct RIPEntry {
    QSharedPointer<HostAddress> destination; 
    QSharedPointer<HostAddress> subnetMask;  
    QSharedPointer<HostAddress> nextHop;    
    int cost;                                

    RIPEntry(QSharedPointer<HostAddress> dest, QSharedPointer<HostAddress> mask,
             QSharedPointer<HostAddress> hop, int c)
        : destination(dest), subnetMask(mask), nextHop(hop), cost(c) {}
};

class RIP : public QObject {
    Q_OBJECT

public:
    explicit RIP(QObject* parent = nullptr);
    virtual ~RIP();

    void initialize(const QSharedPointer<HostAddress>& routerIP, QVector<QSharedPointer<Port>>& ports);
    void sendRoutingUpdates();
    void processRoutingUpdate(const QSharedPointer<HostAddress>& neighbor,
                              const QVector<RIPEntry>& receivedTable);
    void updateRoutingTable();

    QVector<RIPEntry> getRoutingTable() const;

private:
    QSharedPointer<HostAddress> routerIP; 
    QVector<QSharedPointer<Port>> ports;        
    QMap<QSharedPointer<HostAddress>, RIPEntry> routingTable; 
    QMap<QSharedPointer<HostAddress>, int> neighborCosts;     

    void discoverNeighbors();
    void sendPacketToNeighbors(const QVector<RIPEntry>& routingTable);

    static const int INFINITY_COST = 16; 
};

#endif // RIP_H
