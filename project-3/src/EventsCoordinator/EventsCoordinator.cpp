#include "EventsCoordinator.h"
#include <QDebug>
#include <QThread>
#include <chrono>

QMutex EventsCoordinator::m_mutex;

EventsCoordinator::EventsCoordinator(QThread *parent, const QVector<QSharedPointer<PC>>& pcs)
    : QThread(parent), m_running(false), pcs(pcs), dataGenerator(new DataGenerator) {
    ConfigurationManager& config = ConfigurationManager::getInstance();
    config.loadConfiguration("C:/Users/Asus/Documents/Arshia/University/term7/Shabake/Clone Mojadad/CN_CHomeworks_3/assets/config.json");
    m_interval = config.get<int>("cycle_duration");
    duration = config.get<int>("simulation_duration");
    packetsPerCycle = config.get<int>("packets_per_simulation");
    // packetsPerCycle = 10;
    // m_interval = 30;
    // duration = 3000;
    dataGenerator.packetsPerClock = packetsPerCycle;


    if (m_interval <= 0) {
        m_interval = 1000;
        qWarning() << "Invalid cycle_duration in configuration. Using default of 1000ms.";
    }
    qDebug() << "EventsCoordinator created with interval =" << m_interval << "ms";
}

EventsCoordinator::~EventsCoordinator() {
    stopCoordinator();
    qDebug() << "EventsCoordinator destroyed";
}

void EventsCoordinator::setPcs(const QVector<QSharedPointer<PC>>& pcs){

}

EventsCoordinator* EventsCoordinator::instance(QThread *parent) {
    QMutexLocker locker(&m_mutex);
    if (!m_self) {
        m_self = new EventsCoordinator(parent);
    }
    return m_self;
}

void EventsCoordinator::release() {
    QMutexLocker locker(&m_mutex);
    if (m_self) {
        m_self->stopCoordinator();
        delete m_self;
        m_self = nullptr;
    }
}

void EventsCoordinator::run() {
    using namespace std::chrono;
    while (m_running) {
        auto start = high_resolution_clock::now();
        QList<QPair<int, QSharedPointer<HostAddress>>> packets = dataGenerator.generatePacketsForPCs();
        qDebug() << "Here print pc in run ";
        for (auto& pc:pcs){
            qDebug() << pc->getIp()->toString();
        }
        qDebug() << "Finish Here Too";
        for (auto u:packets){
            qDebug() << pcs[u.first]->getIp()->toString() << ' ' << u.second->toString();
            pcs[u.first]->sendPackets(u.second);
        }
        Q_EMIT eventTriggered();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        auto sleep_duration = std::max(0, m_interval - int(duration.count()));
        qDebug() << "Event triggered, sleeping for" << sleep_duration << "ms";
        QThread::msleep(sleep_duration);
    }
    qDebug() << "Stopping event coordination";
}

void EventsCoordinator::startCoordinator() {
    if (m_running) {
        qDebug() << "Coordinator already running.";
        return;
    }
    m_running = true;
    if (!isRunning()) {
        start();
    }
}

void EventsCoordinator::stopCoordinator() {
    if (!m_running) {
        qDebug() << "Coordinator already stopped.";
        return;
    }
    m_running = false;
    if (!wait(3000)) {
        qWarning() << "Forcing thread termination.";
        terminate();
        wait();
    }
}

