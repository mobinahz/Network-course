#include "DataGenerator.h"
#include <QRandomGenerator>
#include <QtMath>

DataGenerator::DataGenerator(QObject *parent)
    : QObject(parent),
    xm(1.0),
    alpha(2.0),
    packetsPerClock(5)
{
}

DataGenerator::~DataGenerator() {}

double DataGenerator::generateParetoSize() {
    QRandomGenerator *rng = QRandomGenerator::global();
    double u = rng->generateDouble();
    return xm / qPow(1.0 - u, 1.0 / alpha);
}

QList<QPair<int, QSharedPointer<HostAddress>>> DataGenerator::generatePacketsForPCs() {
    QList<QPair<int, QSharedPointer<HostAddress>>> packets;
    QRandomGenerator *rng = QRandomGenerator::global();
    for (int i = 0; i < packetsPerClock; i++) {
        // Randomly select source and destination within range [24, 38]
        int sourceID = rng->bounded(24, 39); // Inclusive range [24, 38]
        int destinationID = rng->bounded(24, 39);

        while (destinationID == sourceID) {
            destinationID = rng->bounded(24, 39);
        }
        QString source, destination;
        if (sourceID < 32)
            source = QString("192.168.100.%1").arg(sourceID);
        else
            source = QString("192.168.200.%1").arg(sourceID);
        if (destinationID < 32)
            destination = QString("192.168.100.%1").arg(destinationID);
        else
            destination = QString("192.168.200.%1").arg(destinationID);
        double size = generateParetoSize();
        Packet::PacketType type = Packet::PacketType::Data;

        PacketPtr_t packet = QSharedPointer<Packet>::create(source, destination, static_cast<int>(size), type);
        QSharedPointer<HostAddress> destinationIP = QSharedPointer<HostAddress>::create(destination);
        packets.append(qMakePair(sourceID - 24, destinationIP));

        qDebug() << "Here in DataGenerator " << source << ' ' << destination;
    }

    return packets;
}
