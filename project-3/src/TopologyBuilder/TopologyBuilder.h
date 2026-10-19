#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "Router.h" 

class TopologyBuilder : public QObject {
    Q_OBJECT

public:
    explicit TopologyBuilder(QObject* parent = nullptr);
    virtual ~TopologyBuilder();

    void buildMeshTopology(const QVector<Router*>& routers, const QVector<QSharedPointer<PC>>& pcs);
    void buildRingStarTopology(const QVector<Router*>& routers, const QVector<QSharedPointer<PC>>& pcs);
    void buildTorusTopology(const QVector<Router*>& routers);

private:
    void connectRouters(const QSharedPointer<Router>& r1, const QSharedPointer<Router>& r2);
};

#endif // TOPOLOGYBUILDER_H




