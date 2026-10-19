#include "TopologyController.h"

TopologyController::TopologyController(QObject* parent)
    : QObject(parent), builder(new TopologyBuilder(this)) {}

TopologyController::~TopologyController() {
}

void TopologyController::setupAS(const QSharedPointer<AutonomousSystem>& as, UT::TopologyType topologyType) {
    if (!as) {
        qWarning("Invalid Autonomous System pointer");
        return;
    }

    const QVector<Router*>& routers = as->getRouters();
    const QVector<QSharedPointer<PC>>& pcs = as->getPCs();
    switch (topologyType) {
    case UT::TopologyType::Mesh:
        builder->buildMeshTopology(routers, pcs);
        break;
    case UT::TopologyType::RingStar:
        builder->buildRingStarTopology(routers, pcs);
        break;
    case UT::TopologyType::Torus:
        builder->buildTorusTopology(routers);
        break;
    default:
        qWarning("Unknown topology type specified");
        break;
    }
}
