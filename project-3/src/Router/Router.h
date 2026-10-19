#ifndef ROUTER_H
#define ROUTER_H

#include "HostAddress.h"
#include "../Node/Node.h"
#include "../Port/Port.h"
#include "../IP/IP.h"
#include "../DHCP/DHCP.h"
#include "../RIP/RIP.h"
#include "../OSPF/ospf.h"
#include <QObject>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include "PortBindingManager.h"
#include "../PC/PC.h"

class Router;

struct RouteEntry {
    QSharedPointer<HostAddress> destinationNetwork;
    QSharedPointer<HostAddress> subnetMask;
    QSharedPointer<HostAddress> gateway;
    int metric;
    QString protocol;

    RouteEntry()
        : destinationNetwork(nullptr), subnetMask(nullptr), gateway(nullptr), metric(0), protocol("") {}

    RouteEntry(QSharedPointer<HostAddress> dest, QSharedPointer<HostAddress> subnet, QSharedPointer<HostAddress> gate, int m, const QString& proto)
        : destinationNetwork(dest), subnetMask(subnet), gateway(gate), metric(m), protocol(proto) {}
};



class Router : public Node {
    Q_OBJECT

public:
    explicit Router(int nodeID, const QSharedPointer<HostAddress>& ipAddress, int portCount, UT::GatewayType gatewayType, UT::IPVersion ipVersion, int bufferSize, QObject *parent);
    virtual ~Router();

    void updateRoutingTable(const QSharedPointer<HostAddress>& destination);
    void routePacket(const PacketPtr_t& packet);

    void addRoute(const QSharedPointer<HostAddress>& network, const QSharedPointer<HostAddress>& mask, const QSharedPointer<HostAddress>& gateway, int metric, const QString& protocol);
    void updateRoute(const QSharedPointer<HostAddress>& network, const QSharedPointer<HostAddress>& mask, const QSharedPointer<HostAddress>& gateway, int metric, const QString& protocol);
    void deleteRoute(const QSharedPointer<HostAddress>& network);
    QVector<QSharedPointer<Port>> getPorts();
    void connectToRouter(Router* otherRouter);
    void disconnectRouter(Router* otherRouter);
    void printRoutingTable();
    void initializeOSPF();

    void processOSPFUpdate(const QSharedPointer<HostAddress>& neighbor,
                           const LSA& lsa);
    QSharedPointer<HostAddress> getIPAddress() const;

    void enableDHCPServer();
    void disableDHCPServer();
    QSharedPointer<HostAddress> assignIPAddressToNode(int nodeID, const QString& macAddress, int asNumber);
    void releaseIPAddressFromNode(const QString& macAddress);
    void saveDHCPRecords(const QString& filePath);
    void loadDHCPRecords(const QString& filePath);

    void initializeRIP();

    void processRIPUpdate(const QSharedPointer<HostAddress>& neighbor,
                                  const QVector<RIPEntry>& receivedTable);
    QSharedPointer<Port> getFreePort();
    QSharedPointer<Port> findNextHopPort(const PacketPtr_t& packet);

    void requestIPViaDHCP(int asNumber, bool wantsIPv6) override;
    void setMACAddress(const QSharedPointer<MACAddress>& mac);
    int pcPort;
    int freePort;
    void packetReceived(const PacketPtr_t packet);
    void printLSDB();
    OSPF* getOspfProtocol();
    void connectToPC(QSharedPointer<PC> pc);
    int cntArrival;
    int packetLoss;

public Q_SLOTS:
    void onClockTick();
    void onDhcpDiscoverFromPC(const DHCPRequest &request);
    void onDhcpRequestFromPC(const DHCPRequest &request);
    void onDhcpAckReceived(const DHCPRequest &request, const QSharedPointer<HostAddress> &assignedIP);


Q_SIGNALS:
    void dhcpOfferSignal(const DHCPRequest &request, const QSharedPointer<HostAddress> &offeredIP);
    void dhcpAckSignal(const DHCPRequest &request, const QSharedPointer<HostAddress> &assignedIP);

private:
    QSharedPointer<HostAddress> ipAddress; 
    QVector<QSharedPointer<Port>> ports;

    QMap<QString, RouteEntry> routingTable;

    UT::IPVersion ipVersion;
    QVector<PacketPtr_t> buffer;
    int bufferSize;
    UT::GatewayType gatewayType;

    
    QMap<Router*, QPair<QSharedPointer<Port>, QSharedPointer<Port>>> connectedRouters;
    QMap<QSharedPointer<PC>, QPair<QSharedPointer<Port>, QSharedPointer<Port>>> connectedPCs;
    PortBindingManager* bindingManager;


    DHCP* dhcpServer;
    RIP* ripProtocol;
    OSPF* ospfProtocol;
    bool isDHCPEnabled;

    void createPorts(int portCount);
};

typedef QSharedPointer<Router> RouterPtr_t;

#endif

