#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "../network/webrtc.h"
#include "../audio/audiooutput.h"
#include "../audio/audioinput.h"
#include "../network/peer.h"
#include "audiocontroller.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr, QString type = nullptr, WebRTC *webrtcOffer = nullptr, WebRTC *webrtcReceive = nullptr, Peer *answerPeer = nullptr, Peer *receivePeer = nullptr);
    Q_INVOKABLE void connect();
    QString type()const{return m_type;}
    Q_INVOKABLE void disconnect();

signals:
    void typeChanged();
private:
    WebRTC* webrtcOffer;
    WebRTC* webrtcReceive;
    AudioInput *audioinput;
    AudioOutput *audiooutput;
    Peer *offerPeer;
    Peer *receivePeer;
    AudioController *audiocontroller;

    QString m_type;
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
};


#endif // AUDIOINPUT_H
