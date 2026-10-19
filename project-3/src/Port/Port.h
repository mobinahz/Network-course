#ifndef PORT_H
#define PORT_H

#include "HostAddress.h"
#include "Packet.h"
#include <QObject>
#include <QVector>
#include <QQueue>
#include <QSharedPointer>

class Router;

class Port : public QObject, public QEnableSharedFromThis<Port> {
    Q_OBJECT

public:
    explicit Port(int portNumber, Router* parentRouter, int id, QString ip, QObject* parent = nullptr);
    virtual ~Port();

    int getPortNumber();
    QSharedPointer<HostAddress> getConnectedHost() const;
    void connectToRouter(QSharedPointer<Port> otherPort);
    void connectToPC(QSharedPointer<HostAddress> pcAddress);
    void disconnectFromRouter();
    void disconnectFromPC(QSharedPointer<HostAddress> pcAddress);
    void disconnectAll();

    bool isConnectedToRouter() const;
    QVector<QSharedPointer<HostAddress>> getConnectedPCs() const;
    bool isBusyThisTick;
    int id;
    QString ip;
    void sendPacket(const QSharedPointer<Packet>& packet);
    void receivePacket(const QSharedPointer<Packet>& packet);
    QSharedPointer<HostAddress> connectedPC;
    bool isPCConnected = false;
Q_SIGNALS:
    void packetSent(QSharedPointer<Packet> packet);
    void packetReceived(QSharedPointer<Packet> packet);
    void routerConnected(int portNumber, int connectedPortNumber);
    void routerDisconnected(int portNumber);
    void pcConnected(int portNumber, QSharedPointer<HostAddress> pcAddress);
    void pcDisconnected(int portNumber, QSharedPointer<HostAddress> pcAddress);

private:
    int portNumber;
    Router* parentRouter; 
    QSharedPointer<Port> connectedRouterPort; 
    QVector<QSharedPointer<HostAddress>> connectedPCs; 
};

typedef QSharedPointer<Port> PortPtr_t;

#endif // PORT_H




