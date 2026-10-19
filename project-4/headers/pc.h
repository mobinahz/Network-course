#ifndef PC_H
#define PC_H

#include <QThread>

#include "packet.h"
#include "port.h"
#include "ip.h"
#include "common.h"
#include "eventHandler.h"
#include <unordered_map>
#include <set>

class PC : public QObject
{
    Q_OBJECT
public:
    explicit PC(int _id, IP* _ip, Port* _port, IPVersion _ipVer=IPV4, QObject *parent = nullptr);
    explicit PC(int _id, Port* _port, IPVersion _ipVer=IPV4, QObject *parent = nullptr);
    IP* ip;
    int id;
    int rtt;
    int rto;
    int cwnd;
    int available_cwnd;
    int tick_cnt;
    double cwndFraction;
    Port* port;
    QVector<QSharedPointer<Packet>> buffer;

    void recievePacket(QSharedPointer<Packet> packet);
    void sendPacket(QSharedPointer<Packet> packet);
    void start();
    bool hasIP();
    void connectTick(EventHandler* eventHandler);
    void addPacket(QSharedPointer<Packet> packet);
    void sendTcpPackets();
    void writePacketsToFile();
    void ackReceived(int ackNum);

signals:
    void packetReceived(QSharedPointer<Packet> packet);
    void packetSent();
    void ackSent(int seqNum);
    void missingPacketDetected(int expectedSeqNum, int receivedSeqNum);

private:
    IPVersion ipVer;
    int clk;
    int last_send_packet;
    int ssthresh;
    int dupAckCount;
    int lastAckNumber;
    int first_waiting_packet;
    int lastDhcpRq;
    bool dhcp;
    bool ipSet;
    void sendHello();
    void sendDhcpRequest();
    void tick(int time);
    void sendLease();
    void processPacket(QSharedPointer<Packet> packet);
    void processTCPpacket(QSharedPointer<Packet> packet);
    void processOtherPackets(QSharedPointer<Packet> packet);
    void sendAck(int seqNum, QSharedPointer<Packet> packet);
    QVector<QSharedPointer<Packet>> packets;
    QVector<QSharedPointer<Packet>> need_to_send_packets;
    QVector<int> packet_last_send_time;
    QVector<QSharedPointer<Packet>> received_packets;
    std::unordered_map<QString, std::set<int>> receivedSequenceNumbers;  //new

};

#endif // PC_H
