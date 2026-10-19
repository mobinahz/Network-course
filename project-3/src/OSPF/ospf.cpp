#include "OSPF.h"
#include "Router.h"
#include <QDebug>
#include <QSet>
#include <limits>

OSPF::OSPF(Router* parentRouter, QObject* parent)
    : QObject(parent), 
      router(parentRouter)
{
}

OSPF::~OSPF()
{
}

void OSPF::initialize(const QSharedPointer<HostAddress>& routerIP, 
                      const QVector<QSharedPointer<Port>>& ports)
{
    this->routerIP = routerIP;
    this->ports    = ports;

    discoverNeighbors();


    QVector<LinkState> myLinks;
    for (auto it = neighborCosts.begin(); it != neighborCosts.end(); ++it) {
        LinkState ls;
        ls.neighbor = it.key();
        ls.cost     = it.value();
        myLinks.append(ls);
    }
    lsdb[this->routerIP->toString()] = myLinks;

    sendLSAs();
}

void OSPF::discoverNeighbors()
{
    qDebug() << "Here in discover Nieghbor";
    for (const auto& port : ports) {
        QSharedPointer<HostAddress> connectedHost = port->getConnectedHost();
        qDebug() << connectedHost;
        if (!connectedHost.isNull()) {
            qDebug() << "Found Port";
            neighborCosts[connectedHost] = 1; 
        }
    }
}

PacketPtr_t OSPF::convertLSAToPacket(const LSA& lsa, const QString& source, const QString& destination) {
    QByteArray payload;

    PacketPtr_t packet = QSharedPointer<Packet>::create(source, destination, payload.size(), Packet::PacketType::OSPF);
    packet->setLSA(lsa);
    return packet;
}


LSA OSPF::convertPacketToLSA(const PacketPtr_t& packet) {
    
    return packet->getLSA();
}


void OSPF::sendLSAs()
{
    LSA lsa;
    lsa.originRouter = this->routerIP;
    
    QVector<LinkState> linkStates;
    for (auto it = neighborCosts.begin(); it != neighborCosts.end(); ++it) {
        linkStates.push_back(LinkState{ it.key(), it.value() });
    }
    lsa.links = linkStates;

    floodLSA(lsa);
}

void OSPF::floodLSA(const LSA& lsa)
{
    qDebug() << "[OSPF] Sending LSA from" << lsa.originRouter->toString();
    PacketPtr_t lsaPacket = this->convertLSAToPacket(lsa, lsa.originRouter->toString(), "");
    for (auto& port:router->getPorts()){
        qDebug() << "Here From Router " << router->getId() << ' ' << port->getPortNumber();
        port->sendPacket(lsaPacket);
    }

}


void OSPF::processLSA(const QSharedPointer<HostAddress>& neighbor, 
                      const LSA& lsa)
{

    if (!lsdb.contains(lsa.originRouter->toString())) {
        lsdb[lsa.originRouter->toString()] = lsa.links;
        qDebug() << "[OSPF] Learned new LSA "
                 << "about" << lsa.originRouter->toString() << "in Router " << router->getId();
        qDebug() << "[OSPF] Links for router" << lsa.originRouter->toString() << ":";
        for (const auto& link : lsa.links) {
            qDebug() << "   -> Neighbor:" << link.neighbor->toString()
            << "Cost:" << link.cost;
        }
        floodLSA(lsa);
    } else {
       
    }
}

void OSPF::runDijkstra()
{
    QSet<QString> nodes;
    for (auto it = lsdb.begin(); it != lsdb.end(); ++it) {
        nodes.insert(it.key()); 
        for (const LinkState& link : it.value()) {
            nodes.insert(link.neighbor->toString());
        }
    }
    nodes.insert(this->routerIP->toString());

    dist.clear();
    previous.clear();
    QMap<QString, bool> visited;

    for (const auto& node : nodes) {
        dist[node] = INFINITY_COST;
        visited[node] = false;
        previous[node] = QString();
    }
    dist[this->routerIP->toString()] = 0; 

    while (true) {
        QString current;
        int currentDist = INFINITY_COST;
        for (const auto& node : nodes) {
            if (!visited[node] && dist[node] < currentDist) {
                currentDist = dist[node];
                current = node;
            }
        }
        if (current.isEmpty()) {
            break;
        }
        visited[current] = true;

        if (lsdb.contains(current)) {
            for (const LinkState& link : lsdb[current]) {
                QString nbr = link.neighbor->toString();
                if (!visited[nbr]) {
                    int alt = dist[current] + link.cost;
                    if (alt < dist[nbr]) {
                        dist[nbr] = alt;
                        previous[nbr] = current;
                    }
                }
            }
        }
    }


    for (const auto& node : nodes) {
        if (node == this->routerIP->toString()) continue;
        if (dist[node] >= INFINITY_COST) {
            continue;
        }

        QString pathWalker = node;
        QString parent;
        while (true) {
            parent = previous[pathWalker];
            if (parent.isEmpty() || parent == this->routerIP->toString()) {
                break;
            }
            pathWalker = parent;
        }
 
        router->addRoute(
            QSharedPointer<HostAddress>::create(node),   
            QSharedPointer<HostAddress>::create("255.255.255.0"), 
            QSharedPointer<HostAddress>::create(pathWalker), 
            dist[node], 
            "OSPF"      
            );
    }

    qDebug() << "[OSPF] Dijkstra completed for Router" << router->getId();
    qDebug() << "=== Dijkstra Results for Router" << router->getId() << "===";
    qDebug() << "Distances:";
    for (const auto& node : dist.keys()) {
        qDebug() << "Router:" << node << "Distance:" << dist[node];
    }
    updateRoutingTable();
}


void OSPF::updateRoutingTable()
{
    if (dist.isEmpty() || previous.isEmpty()) {
        qDebug() << "[OSPF] Cannot update routing table. Dijkstra results are empty.";
        return;
    }

    qDebug() << "[OSPF] Updating routing table from Dijkstra results...";
    for (auto it = dist.begin(); it != dist.end(); ++it) {
        const QString& destination = it.key(); 
        int cost = it.value();              

        if (cost >= INFINITY_COST) {
            continue;
        }

        QString nextHop = destination;
        while (previous.contains(nextHop) && previous[nextHop] != this->routerIP->toString()) {
            nextHop = previous[nextHop];
        }

        QSharedPointer<HostAddress> destAddress = QSharedPointer<HostAddress>::create(destination);
        QSharedPointer<HostAddress> nextHopAddress = QSharedPointer<HostAddress>::create(nextHop);

        router->addRoute(destAddress, nullptr, nextHopAddress, cost, "OSPF");

        qDebug() << "[OSPF] Route added: Destination =" << destination
                 << ", Next Hop =" << nextHop
                 << ", Cost =" << cost;
    }

    qDebug() << "[OSPF] Routing table updated successfully.";
}


void OSPF::printLSDB() const
{
    qDebug() << "[OSPF] Current Link-State Database (LSDB):";
    for (auto it = lsdb.begin(); it != lsdb.end(); ++it) {
        auto routerAddr = it.key();
        qDebug() << " Router:" << routerAddr;
        for (const LinkState& link : it.value()) {
            qDebug() << "   -> " << link.neighbor->toString() 
                     << "cost =" << link.cost;
        }
    }
}
