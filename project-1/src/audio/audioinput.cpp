#include "audioinput.h"
#include <opus.h>
#include <QaudioFormat>
#include <QMediaDevices>
#include <QDebug>
#include <QObject>

AudioInput::AudioInput(QObject *parent)
    : QIODevice(parent), audioSource(nullptr), bufferSize(4096),
    encoder(nullptr), frameSize(960)
{
    int error;
    encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &error);
    if (error != OPUS_OK) {
        qFatal("Failed to create Opus encoder: %s", opus_strerror(error));
    }

    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QMediaDevices mediaDevices;
    QAudioDevice defaultInputDevice = mediaDevices.defaultAudioInput();

    if (!defaultInputDevice.isFormatSupported(format)) {
        qFatal("Audio format not supported");
    }

    audioSource = new QAudioSource(defaultInputDevice, format, this);
}

AudioInput::~AudioInput()
{
    if (audioSource) delete audioSource;
    if (encoder) opus_encoder_destroy(encoder);
}

void AudioInput::start()
{

    if (!audioSource) return;
    if (!this->open(QIODevice::ReadWrite)) {
        qWarning() << "AudioInput: Failed to open QIODevice for writing";
        return;
    }
    audioSource->start(this);
    if (audioSource->state() != QAudio::ActiveState) {
        qWarning() << "Audio source is not active. State:" << audioSource->state();
    }
}

void AudioInput::stop()
{
    if (audioSource) {
        audioSource->stop();
    }
}

qint64 AudioInput::writeData(const char *data, qint64 len)
{
    if (!encoder) {
        qWarning() << "AudioInput: Opus encoder is not initialized!";
        return -1;
    }
    qDebug() << "AudioInput: Writing audio data, length:" << len;

    const int maxPacketSize = 4000;
    unsigned char encodedData[maxPacketSize];

    int encodedBytes = opus_encode(encoder, reinterpret_cast<const opus_int16*>(data), len / 2, encodedData, maxPacketSize);

    if (encodedBytes < 0) {
        qWarning() << "AudioInput: Opus encoding failed, error code:" << encodedBytes;
        return -1;
    }

    qDebug() << "AudioInput: Audio encoded, encoded bytes:" << encodedBytes;

    // Emit the encoded data as a QByteArray
    QByteArray audioData(reinterpret_cast<const char*>(encodedData), encodedBytes);
    emit bufferReady(audioData);
    qDebug() << "AudioInput: Encoded audio data emitted, length:" << encodedBytes;

    return len;
}

qint64 AudioInput::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}
