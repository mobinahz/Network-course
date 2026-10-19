#ifndef TOPOLOGYCONTROLLER_H
#define TOPOLOGYCONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include "TopologyBuilder.h"
#include "./../AutonomousSystem/AutonomousSystem.h"
#include "Globals.h" 

class TopologyController : public QObject {
    Q_OBJECT

public:
    explicit TopologyController(QObject* parent = nullptr);
    virtual ~TopologyController();

    void setupAS(const QSharedPointer<AutonomousSystem>& as, UT::TopologyType topologyType);

private:
    QSharedPointer<TopologyBuilder> builder;
};

#endif // TOPOLOGYCONTROLLER_H
