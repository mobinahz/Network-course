#include "controller.h"
#include <QTimer>

Controller::Controller(QObject *parent, QString type, WebRTC* webrtcOffer, WebRTC* webrtcReceive, Peer* offerPeer, Peer* receivePeer){
    this->audioinput = new AudioInput(nullptr);
    this->audiooutput = new AudioOutput(nullptr);
    this->webrtcOffer = webrtcOffer;
    this->webrtcReceive = webrtcReceive;
    this->offerPeer = offerPeer;
    this->receivePeer = receivePeer;
    this->m_type = type;
}

void Controller::connect(){
    qDebug() << "AFSFSDFAS";
    this->audiocontroller = new AudioController(nullptr, this->m_type, this->webrtcOffer, this->webrtcReceive, this->offerPeer, this->receivePeer);
    QTimer::singleShot(3000, [this]() {
        if (this->m_type == "offerer"){
            this->offerPeer->StartConnecting();
            qDebug() << "Started Connecting after 5 seconds";
        }
    });
    QTimer::singleShot(3000, [this]() {
        this->audiocontroller->Handle();
    });

}

void Controller::disconnect(){
    if (this->m_type == "offerer"){
        delete this->offerPeer;
    }
}
