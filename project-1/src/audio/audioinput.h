#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <QAudioSource>
#include <opus.h>

class AudioInput : public QIODevice
{
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = nullptr);
    ~AudioInput();

    void start();
    void stop();

    qint64 writeData(const char *data, qint64 len) override;
    qint64 readData(char *data, qint64 maxlen) override;

signals:
    void bufferReady(const QByteArray &buffer);

private:
    QAudioSource *audioSource;
    QByteArray audioBuffer;
    int bufferSize;
    OpusEncoder *encoder;
    int frameSize;
};

#endif // AUDIOINPUT_H
