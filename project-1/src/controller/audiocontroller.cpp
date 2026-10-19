#include "audiocontroller.h"


AudioController::AudioController(QObject *parent, QString type, WebRTC* webrtcOffer, WebRTC* webrtcReceive, Peer* offerPeer, Peer* receivePeer){
    this->type = type;
    this->audioinput = new AudioInput(nullptr);
    this->audiooutput = new AudioOutput(nullptr);
    this->webrtcOffer = webrtcOffer;
    this->webrtcReceive = webrtcReceive;
    this->offerPeer = offerPeer;
    this->receivePeer = receivePeer;
}

void AudioController::Handle(){
    if (type == "offerer"){
        connect(audioinput, &AudioInput::bufferReady, [this](const QByteArray data){
            this->webrtcOffer->sendTrack("receiver", data);
        });
        audioinput->start();
    }

    else if(type == "answerer"){
        connect(this->webrtcReceive, &WebRTC::incommingPacket, audiooutput, &AudioOutput::addData);
    }
}
