// #ifndef LOGGER_H
// #define LOGGER_H

// #include <QMutex>
// #include <QFile>
// #include <QTextStream>
// #include <QDateTime>

// class Logger {
// public:
//     static void log(const QString& message) {
//         QMutexLocker locker(&mutex);
//         QFile file("../log.txt");
//         if (file.open(QIODevice::Append | QIODevice::Text)) {
//             QTextStream out(&file);
//             out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - " << message << '\n';
//         }
//     }

// private:
//     static QMutex mutex;
// };

// QMutex Logger::mutex;

// #endif
