#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QAudioSink>
#include <QAudioFormat>
#include <QIODevice>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <opus.h>

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = nullptr);
    ~AudioOutput();

    void addData(const QString &peerId, const QByteArray &data, qint64 len);

signals:
    void newPacket();

private slots:
    void play();

private:
    QAudioSink *audioSink;
    QIODevice *audioDevice;
    QQueue<QByteArray> packetQueue;
    QMutex mutex;
    OpusDecoder *decoder;
    int frameSize;
};

#endif // AUDIOOUTPUT_H
