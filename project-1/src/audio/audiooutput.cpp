#include "audiooutput.h"
#include <QMediaDevices>
#include <QAudioFormat>
#include <QDebug>

AudioOutput::AudioOutput(QObject *parent)
    : QObject(parent), audioSink(nullptr), audioDevice(nullptr), frameSize(960)
{
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    if (!QMediaDevices::defaultAudioOutput().isFormatSupported(format)) {
        qFatal("Audio format not supported");
    }

    audioSink = new QAudioSink(QMediaDevices::defaultAudioOutput(), format, this);

    int error;
    decoder = opus_decoder_create(48000, 1, &error);
    if (error != OPUS_OK) {
        qFatal("Failed to create Opus decoder: %s", opus_strerror(error));
    }

    audioDevice = audioSink->start();
    if (audioDevice) {
        qDebug() << "AudioOutput: Audio device started successfully";
    } else {
        qWarning() << "AudioOutput: Failed to start audio device";
    }

    bool connectionStatus = connect(this, &AudioOutput::newPacket, this, &AudioOutput::play);
    if (!connectionStatus) {
        qWarning() << "AudioOutput: Failed to connect newPacket signal to play slot";
    } else {
        qDebug() << "AudioOutput: Successfully connected newPacket signal to play slot";
    }
    qDebug() << "AudioOutput: newPacket signal connected to play slot";
}

AudioOutput::~AudioOutput()
{
    if (audioSink) delete audioSink;
    if (decoder) opus_decoder_destroy(decoder);
}

void AudioOutput::addData(const QString &peerId, const QByteArray &data, qint64 len)
{
    QMutexLocker locker(&mutex);
    qDebug() << "AudioOutput: Adding new data to the queue. Data length:" << data.size();
    packetQueue.enqueue(data);

    emit newPacket();
    qDebug() << "AudioOutput: newPacket signal emitted";
}

void AudioOutput::play()
{

    if (packetQueue.isEmpty()) {
        qDebug() << "AudioOutput: No data to play. Queue is empty";
        return;
    }

    QByteArray packet = packetQueue.dequeue();
    qDebug() << "AudioOutput: Playing data from queue. Encoded data length:" << packet.size();

    if (packet.size() <= 0) {
        qWarning() << "AudioOutput: Invalid encoded data size";
        return;
    }

    opus_int16 outputBuffer[frameSize * 2];

    int decodedSamples = opus_decode(decoder, reinterpret_cast<const unsigned char *>(packet.data()), packet.size(), outputBuffer, frameSize, 0);

    if (decodedSamples < 0) {
        qWarning() << "opus decoder failed" << opus_strerror(decodedSamples);
        return;
    }
    qDebug() << "AudioOutput: Opus decoded successfully. Decoded samples:" << decodedSamples;
    qint64 bytesWritten = audioDevice->write(reinterpret_cast<const char*>(outputBuffer), decodedSamples * sizeof(opus_int16));
    if (bytesWritten > 0) {
        qDebug() << "AudioOutput: Audio written to device. Bytes written:" << bytesWritten;
    } else {
        qWarning() << "AudioOutput: Failed to write audio data to device. Bytes written:" << bytesWritten;
    }
}
