#include "./headers/pc.h"
#include <QFile>

using namespace std;

PC::PC(int _id, IP* _ip, Port* _port, IPVersion _ipVer, QObject *parent)
    : QObject{parent}
{
    ipVer = _ipVer;
    id = _id;
    ip = _ip;
    port = _port;
    dhcp = false;
    ipSet = true;
    rtt = 220;
    rto = 440;
    cwnd = 20;
    available_cwnd = 20;
    tick_cnt = 0;
    last_send_packet = 0;
    first_waiting_packet = 0;
    ssthresh = 50;
}

PC::PC(int _id, Port *_port, IPVersion _ipVer, QObject *parent)
    : QObject{parent}
{
    ipVer = _ipVer;
    id = _id;
    port = _port;
    dhcp = true;
    ipSet = false;
    rtt = 50;
    rto = 100;
    cwnd = 20;
    available_cwnd = 20;
    tick_cnt = 0;
    last_send_packet = 0;
    first_waiting_packet = 0;
    ssthresh = 50;
}

void PC::start()
{
    if (dhcp)
        sendDhcpRequest();
    else
        sendHello();
}

void PC::recievePacket(QSharedPointer<Packet> packet)
{
    if (packet->getType() == MSG_PACKET)
    {

        buffer.append(packet);

        QString path;
        for (auto routerID: packet->getPath())
        {
            path.append(routerID);
            path.append("  ");
        }
        processPacket(packet);
        // qDebug() <<"pc "<<id<<" recieved msg: "<<packet->getString().toStdString()
        //          <<" with queue waiting: " << packet->getQueueWaitCycles() << ", total wait: "
        //          << packet->getWaitCycles() <<" through path: "<< path.toStdString();
        emit packetReceived(packet);
    }
    else if (packet->getType() == DHCP_OFFER)
    {
        if (ipSet) return; // ignore broadcasts
        if (packet->getString().split(",")[1].toInt() != id) return;

        ip = new IPv4("255.255.255.255", packet->getString().split(",")[0]);
        qDebug() << "pc" << id << "new ip is:" << ip->getIPStr();
        ipSet = true;
        sendHello();
    }else if(packet->getType() == TCP){
        ackReceived(packet->getSeqNum());
    }
}

void PC::sendPacket(QSharedPointer<Packet> packet)
{
    qDebug() << "Sending Packet in " << ip->ipAddr.toStr() << "with seqNum " << packet->getSeqNum();
    packet->ipVer = ipVer;
    port->write(packet);
    emit packetSent();
}

void PC::sendHello()
{
    QSharedPointer<Packet> packet(new Packet("0", HELLO, IPV6, IPv4(ip->mask, ip->ipAddr), IPv4(ip->mask, ip->ipAddr)));
    port->write(packet);
}

void PC::sendDhcpRequest()
{
    // qDebug() << "sending DHCP req";
    QSharedPointer<Packet> packet(new Packet(QString::number(id), DHCP_REQUEST, IPV4, IPv4("255.255.255.255", "255.255.255.255"),
                                             IPv4("255.255.255.255", "255.255.255.255")));
    port->write(packet);
    lastDhcpRq = clk;
}

bool PC::hasIP()
{
    return ipSet;
}

void PC::connectTick(EventHandler *eventHandler)
{
    QObject::connect(eventHandler, &EventHandler::tick,
                     this, &PC::tick, Qt::ConnectionType::QueuedConnection);
}

void PC::addPacket(QSharedPointer<Packet> packet)
{
    packets.append(packet);
}

void PC::sendTcpPackets()
{
    tick_cnt++;
    if (!ip)
        return;
    if (need_to_send_packets.isEmpty()) {
        int seqCnt = 0;
        for (const QSharedPointer<Packet> &packet : packets) {
            if (!ip)
                continue;
            packet->setSeqNum(++seqCnt);
            if (ip->ipAddr.addrToNum() == packet->getSource().ipAddr.addrToNum()) {
                need_to_send_packets.append(packet);
                packet_last_send_time.append(0);
            }
        }
        first_waiting_packet = 0;
        last_send_packet = -1;
        dupAckCount = 0;
        if (!need_to_send_packets.isEmpty())
            lastAckNumber = need_to_send_packets.first()->getSeqNum();
        if (!need_to_send_packets.isEmpty())
            qDebug() << "pc with ip " << ip->ipAddr.toStr() << " has " << need_to_send_packets.size();

    }
    if (need_to_send_packets.isEmpty())
        return;
    // qDebug() << tick_cnt << ' ' << packet_last_send_time[first_waiting_packet] << ' ' << rto;
    if (tick_cnt - packet_last_send_time[first_waiting_packet] >= rto)
    {
        qDebug() << "Time out happened " << ip->ipAddr.toStr() << ' ' << first_waiting_packet;
        ssthresh = std::max(cwnd / 2, 2);
        cwnd = 1;
        sendPacket(need_to_send_packets[first_waiting_packet]);
        packet_last_send_time[first_waiting_packet] = tick_cnt;
        dupAckCount = 0;
        return;
    }

    int inFlight = last_send_packet - first_waiting_packet + 1;
    int availableSlots = cwnd - inFlight;
    for (int i = last_send_packet + 1;
         i < need_to_send_packets.size() && availableSlots > 0;
         i++)
    {
        sendPacket(need_to_send_packets[i]);
        packet_last_send_time[i] = tick_cnt;
        last_send_packet = i;
        availableSlots--;
    }
}

void PC::ackReceived(int ackNum)
{
    qDebug() << "Ack Received in " << ip->ipAddr.toStr() << ' ' << ackNum;
    if (ackNum > lastAckNumber) {
        int packetsAcked = 0;
        while (first_waiting_packet < need_to_send_packets.size() &&
               need_to_send_packets[first_waiting_packet]->getSeqNum() < ackNum)
        {
            packetsAcked++;
            first_waiting_packet++;
        }
        dupAckCount = 0;
        lastAckNumber = ackNum;

        if (cwnd < ssthresh) {
            cwnd += packetsAcked;
        } else {
            double increment = (double)packetsAcked / cwnd;
            cwndFraction += increment;
            int add = static_cast<int>(cwndFraction);
            if (add > 0) {
                cwnd += add;
                cwndFraction -= add;
            }
        }

        if (first_waiting_packet > last_send_packet) {
            first_waiting_packet = 0;
            last_send_packet = -1;
        }

    } else if (ackNum == lastAckNumber) {
        dupAckCount++;
        if (dupAckCount == 3) {
            ssthresh = std::max(cwnd / 2, 2);
            cwnd = ssthresh + 3;
            if (first_waiting_packet < need_to_send_packets.size()) {
                sendPacket(need_to_send_packets[first_waiting_packet]);
                packet_last_send_time[first_waiting_packet] = tick_cnt;
            }
        } else if (dupAckCount > 3) {
            cwnd++;
        }
    }
}

void PC::tick(int time)
{
    clk = time;
    if (!hasIP()&&time > lastDhcpRq + 100)
        sendDhcpRequest();
    if (hasIP() && time % SEND_LEASE == 0)
        sendLease();

    if (((int)time%MESSAGING_SYSTEM_SEND_PERIOD)==0 && time!=0) {
        sendTcpPackets();
    }
}

void PC::sendLease()
{
    QSharedPointer<Packet> packet(new Packet(QString::number(id), DHCP_LEASE, IPV4, IPv4("255.255.255.255", "255.255.255.255"),
                                             IPv4("255.255.255.255", "255.255.255.255")));
    port->write(packet);
}

//new


void PC::processPacket(QSharedPointer<Packet> packet) {

    bool isNotReceivedBefore = true;
    for (const auto& p : received_packets) {
        if (p->getFileSeqNum() == packet->getFileSeqNum()) {
            isNotReceivedBefore = false;
            break;
        }
    }

    if (isNotReceivedBefore) {
        received_packets.append(packet);
        processTCPpacket(packet);
        emit packetReceived(packet);
    } else {
        std::cout << "Duplicate or irrelevant packet received, ignored: " << packet->getFileSeqNum() << std::endl;
    }


}


void PC::processTCPpacket(QSharedPointer<Packet> packet) {
    QString senderIP = packet->getSource().getIPStr();
    qDebug() << "Here in pc " << ip->ipAddr.toStr() << ' ' << packet->getSource().getIPStr() << ' ' << packet->getSeqNum();
    if (receivedSequenceNumbers.find(senderIP) == receivedSequenceNumbers.end()) {
        receivedSequenceNumbers[senderIP] = std::set<int>();
    }

    int seqNum = packet->getSeqNum();
    auto& seqNums = receivedSequenceNumbers[senderIP];
    auto insertionResult = seqNums.insert(seqNum);

    if (insertionResult.second) {
        emit packetReceived(packet);
    }

    int expectedSeqNum = 1;
    for (auto it = seqNums.begin(); it != seqNums.end() && *it == expectedSeqNum; ++it) {
        expectedSeqNum++;
    }

    if (seqNum > expectedSeqNum) {
        emit missingPacketDetected(expectedSeqNum - 1, seqNum);
        std::cout << "Missing packet detected. Expected: " << expectedSeqNum - 1 << ", but received: " << seqNum << std::endl;
    }

    sendAck(expectedSeqNum, packet);
    std::cout << "Sending ACK for: " << expectedSeqNum << std::endl;
}

void PC::sendAck(int seqNum, QSharedPointer<Packet> packet) {

    IPv4* ipv4Source = dynamic_cast<IPv4*>(ip);
    if (ipv4Source) {
        QSharedPointer<Packet> ackPacket(new Packet(QString::number(seqNum), TCP, ipVer, *ipv4Source, packet->getSource()));
        ackPacket->setSeqNum(seqNum);
        qDebug() << "send ack to " << ackPacket->getDest().ipAddr.toStr() << seqNum;
        sendPacket(ackPacket);
        emit ackSent(seqNum);
    } else {
        std::cerr << "IP is not of type IPv4" << std::endl;
    }

}
 //end

void PC::writePacketsToFile()
{
    if (!received_packets.length()) return;
    std::sort(received_packets.begin(), received_packets.end(),
              [](const QSharedPointer<Packet>& a, const QSharedPointer<Packet>& b) {
                  return a->getFileSeqNum()< b->getFileSeqNum();
              });

    QString filename = QString("%1receiver_output.mp3").arg(OUTPUT_FILE_PATH, id);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing");
        return;
    }

    for (const auto& packet : received_packets) {
        QByteArray byteArray = packet->getPayload();
        file.write(byteArray);
    }
    cout<<"file saved!\n";
    file.close();
}
