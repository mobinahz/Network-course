#ifndef EVENTSCOORDINATOR_H
#define EVENTSCOORDINATOR_H

#include <chrono>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "ConfigurationManager.h"
#include "../DataGenerator/DataGenerator.h"

class EventsCoordinator : public QThread
{
    Q_OBJECT

public:
    typedef std::chrono::milliseconds Millis;

    explicit EventsCoordinator(QThread *parent = nullptr, const QVector<QSharedPointer<PC>>& pcs = {});
    ~EventsCoordinator() override;

    static EventsCoordinator *instance(QThread *parent = nullptr);
    static void release();
    void setPcs(const QVector<QSharedPointer<PC>>& pcs);
    void startCoordinator();
    void stopCoordinator();
    DataGenerator dataGenerator;
    const QVector<QSharedPointer<PC>>& pcs;
    int duration;
    int packetsPerCycle;

Q_SIGNALS:
    void eventTriggered();

protected:
    void run() override;

private:
    inline static EventsCoordinator *m_self = nullptr;
    bool m_running = false;
    int m_interval;
    static QMutex m_mutex;
};

#endif    // EVENTSCOORDINATOR_H
