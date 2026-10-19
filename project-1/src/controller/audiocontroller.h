#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>
#include "../network/webrtc.h"
#include "../audio/audiooutput.h"
#include "../audio/audioinput.h"
#include "../network/peer.h"

class AudioController : public QObject
{
    Q_OBJECT
public:
    explicit AudioController(QObject *parent = nullptr, QString type = nullptr, WebRTC *webrtcOffer = nullptr, WebRTC *webrtcReceive = nullptr, Peer *answerPeer = nullptr, Peer *receivePeer = nullptr);
    void Handle();
private:
    WebRTC* webrtcOffer;
    WebRTC* webrtcReceive;
    AudioInput *audioinput;
    AudioOutput *audiooutput;
    Peer *offerPeer;
    Peer *receivePeer;
    QString type;
};


#endif // AUDIOINPUT_H
