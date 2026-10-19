#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "Router.h"
#include "Globals.h"
#include "./../PC/PC.h"


class AutonomousSystem : public QObject {
    Q_OBJECT

public:
    explicit AutonomousSystem(int id, QObject* parent = nullptr);
    virtual ~AutonomousSystem();

    void initializeFromConfig();
    void connectToAS(QSharedPointer<AutonomousSystem> otherAS);
    void setupRouters(int numberOfRouters);
    void designateGateways(int asGatewayIndex, int userGatewayIndex);
    void enableDHCPServer(int dhcpServerIndex);
    const QVector<Router*>& getRouters() const;
     const QVector<QSharedPointer<PC>>& getPCs() const;

    void connectPCtoGateway(const QSharedPointer<PC>& pc, const QSharedPointer<Router>& gateway);
    void setupPCs(int numberOfPCs);

    void connectPCtoGateway(QSharedPointer<PC> pc);

    QSharedPointer<Router> getAsGateway() const;
    QSharedPointer<Router> getUserGateway() const;


private:
    int id;
    QVector<Router*> routers;
    QSharedPointer<Router> asGateway;
    QSharedPointer<Router> userGateway;
    QSharedPointer<Router> dhcpServer;
    QVector<QSharedPointer<PC>> pcs;
    UT::TopologyType topologyType;
};

#endif // AUTONOMOUSSYSTEM_H



