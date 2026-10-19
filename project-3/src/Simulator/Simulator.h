#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QVector>
#include <QSharedPointer>
#include "../Network/Network.h"
#include "../EventsCoordinator/EventsCoordinator.h"

class Simulator : public QThread {
    Q_OBJECT

public:
    explicit Simulator(QObject* parent = nullptr);
    ~Simulator();
    void post_process();

protected:
    void run() override;

private:
    EventsCoordinator* coordinator;
    Network* network;
};

#endif
