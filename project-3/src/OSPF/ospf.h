#ifndef OSPF_H
#define OSPF_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include "HostAddress.h"
#include "Port.h"
#include "../Globals/Globals.h"


class Router; 

class OSPF : public QObject
{
    Q_OBJECT
public:
    explicit OSPF(Router* parentRouter, QObject* parent = nullptr);
    virtual ~OSPF();

    void initialize(const QSharedPointer<HostAddress>& routerIP, 
                    const QVector<QSharedPointer<Port>>& ports);

    void discoverNeighbors();

    void sendLSAs();

    void processLSA(const QSharedPointer<HostAddress>& neighbor, 
                    const LSA& lsa);

    void runDijkstra();

    void updateRoutingTable();

    void printLSDB() const;
    PacketPtr_t convertLSAToPacket(const LSA& lsa, const QString& source, const QString& destination);
    LSA convertPacketToLSA(const PacketPtr_t& packet);

private:
    Router* router;
    QSharedPointer<HostAddress> routerIP;
    QVector<QSharedPointer<Port>> ports;
    QMap<QString, int> dist; 
    QMap<QString, QString> previous;

    QMap<QSharedPointer<HostAddress>, int> neighborCosts;

   
    QMap<QString, QVector<LinkState>> lsdb;

    static const int INFINITY_COST = 999999;

    void floodLSA(const LSA& lsa);
};

#endif // OSPF_H
