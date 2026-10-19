#ifndef PACKET_H
#define PACKET_H

#include <QString>
#include <QList>
#include <QObject>
#include <QSharedPointer>
#include "../HostAddress/HostAddress.h"

struct LinkState {
    QSharedPointer<HostAddress> neighbor;  
    int cost;                             
};

struct LSA {
    QSharedPointer<HostAddress> originRouter;   
    QVector<LinkState> links;                  
};

class Packet : public QObject {
public:
    enum class PacketType {
        Data,
        Control,
        DHCPRequest,
        DHCPResponse,
        RIP,
        OSPF
    };

    Packet(const QString& source, const QString& destination, int size, PacketType type);

    QString getSource() const;
    QString getDestination() const;
    int getSize() const;
    PacketType getType() const;
    int pathLen;
    void setSource(const QString& source);
    void setDestination(const QString& destination);
    void setSize(int size);
    void setType(PacketType type);

    void addNodeToPath(const QString& nodeIP);
    QList<QString> getPath() const;

    void incrementTotalCycles();
    void incrementBufferCycles();
    int getTotalCycles() const;
    int getBufferCycles() const;

    QString packetTypeToString(PacketType type);
    QByteArray getPayload() const;
    void setPayload(const QByteArray& payload);
    void setLSA(const LSA& lsa);
    LSA getLSA();

private:
    QByteArray payload;
    QString source;
    QString destination;
    int size;
    PacketType type;
    LSA lsaContent;
    QList<QString> path;
    int totalCycles;
    int bufferCycles;

};

typedef QSharedPointer<Packet> PacketPtr_t;

#endif // PACKET_H
