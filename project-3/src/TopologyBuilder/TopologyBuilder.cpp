#include "TopologyBuilder.h"

TopologyBuilder::TopologyBuilder(QObject* parent) : QObject(parent) {}

TopologyBuilder::~TopologyBuilder() {}

void TopologyBuilder::buildMeshTopology(const QVector<Router*>& routers, const QVector<QSharedPointer<PC>>& pcs){
    int rows = 4;
    int cols = 4;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int currentIndex = r * cols + c;
            if (c + 1 < cols) {
                int rightIndex = currentIndex + 1;
                routers[currentIndex]->connectToRouter(routers[rightIndex]);
            }
            if (r + 1 < rows) {
                int belowIndex = currentIndex + cols;
                routers[currentIndex]->connectToRouter(routers[belowIndex]);
            }
        }
    }
    for (int i = 0; i < 8; i++){
        int idx = i / 2;
        routers[idx]->connectToPC(pcs[i]);
    }
}


void TopologyBuilder::buildRingStarTopology(const QVector<Router*>& routers, const QVector<QSharedPointer<PC>>& pcs) {
    for (int i = 0; i < 5; i += 2){
        qDebug() << "Creating Ring Star " << routers[6]->getId() << ' ' << routers[i]->getId();
        routers[6]->connectToRouter(routers[i]);
    }
    for (int i = 0; i < 5; i++){
        routers[i]->connectToRouter(routers[i + 1]);
    }
    routers[5]->connectToRouter(routers[0]);
    for (int i = 0; i < 3; i++){
        routers[4]->connectToPC(pcs[i]);
    }
    for (int i = 5; i < 7; i++){
        routers[5]->connectToPC(pcs[i]);
    }
    for (int i = 3; i < 5; i++){
        routers[3]->connectToPC(pcs[i]);
    }
}

void TopologyBuilder::buildTorusTopology(const QVector<Router*>& routers) {
    int n = routers.size();
    int side = qSqrt(n);
    if (side * side != n) { 
        qWarning("The number of routers must be a perfect square for a torus topology.");
        return;
    }

    for (int i = 0; i < side; ++i) {
        for (int j = 0; j < side; ++j) {
            int index = i * side + j;
            QVector<int> neighbors = {
                (i * side + ((j + 1) % side)),                
                (i * side + ((j - 1 + side) % side)),   
                (((i + 1) % side) * side + j),            
                (((i - 1 + side) % side) * side + j)         
            };

            for (int neighbor : neighbors) {
                // connectRouters(routers[index], routers[neighbor]);
            }
        }
    }
}

void TopologyBuilder::connectRouters(const QSharedPointer<Router>& r1, const QSharedPointer<Router>& r2) {
    if (!r1 || !r2 || r1 == r2) return;
    auto port1 = r1->getFreePort();
    auto port2 = r2->getFreePort();
    if (port1 && port2) {
        port1->connectToRouter(port2);
    }
}


