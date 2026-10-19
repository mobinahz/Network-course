#ifndef NETWORK_H
#define NETWORK_H

#include "./../AutonomousSystem/AutonomousSystem.h"
#include "./../TopologyController/TopologyController.h"

class Network : public QObject
{
    Q_OBJECT

public:

    explicit Network(QObject* parent = nullptr);
    ~Network();

    void setupAS1();
    void setupAS2();
    void connectAS1toAS2();


    QSharedPointer<AutonomousSystem> getAS1();
    QSharedPointer<AutonomousSystem> getAS2();



private:
    QSharedPointer<AutonomousSystem> AS1;
    QSharedPointer<AutonomousSystem> AS2;
    TopologyController topologyController;

};

#endif //NETWORK_H
