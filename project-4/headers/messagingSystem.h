#ifndef MESSAGINGSYSTEM_H
#define MESSAGINGSYSTEM_H

#include <iostream>
#include <random>
#include <chrono>
#include <thread>

#include "../headers/pc.h"

class MessagingSystem // poisson
{
private:
    int lambda;
    QVector<PC*> receivers, senders;
    QVector<QSharedPointer<Packet>> packets;
    QByteArray readFile(QString);

public:
    MessagingSystem(int lambda, const QVector<PC *> &receivers, const QVector<PC *> &senders);
    int numOfPackets();
    QVector<QSharedPointer<Packet>> generatePackets();
    QVector<QSharedPointer<Packet>> generatePacketsForFile(QString filePath);     //esmesh avaz she behtare

};

#endif // MESSAGINGSYSTEM_H
