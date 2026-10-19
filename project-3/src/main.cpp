#include <QCoreApplication>
#include "../Simulator/Simulator.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Simulator* simulator = new Simulator();
    simulator->start();

    return app.exec();
}
