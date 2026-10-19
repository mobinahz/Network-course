#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QList>
#include <QRandomGenerator>
#include <cmath>
#include "../Packet/Packet.h"
#include "../PC/PC.h"

class DataGenerator : public QObject {
    Q_OBJECT

public:
    explicit DataGenerator(QObject *parent = nullptr);
    ~DataGenerator();

    QList<QPair<int, QSharedPointer<HostAddress>>> generatePacketsForPCs();
    int packetsPerClock;

Q_SIGNALS:
    void packetsGenerated(const QList<PacketPtr_t>& packets);

private:
    double generateParetoSize();

    double xm;
    double alpha;
    int maxPackets;
    int sourceRange;
    int destinationRange;
    int packetTypeRange;

};

#endif // DATAGENERATOR_H
