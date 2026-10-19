#include "peer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Peer::Peer(const QString& peerId, const QString& otherPeerId, const QUrl& signalingServerUrl, bool isOfferer, QObject* parent)
    : QObject(parent), peerId_(peerId), otherPeerId_(otherPeerId), signalingServerUrl_(signalingServerUrl), isOfferer(isOfferer) {
    connect(&webSocket_, &QWebSocket::disconnected, this, &Peer::onDisconnected);
    connect(&webSocket_, &QWebSocket::textMessageReceived, this, &Peer::onTextMessageReceived);
    connect(&webSocket_, &QWebSocket::connected, this, &Peer::onConnected);
}


Peer::~Peer() {
    if (webSocket_.isValid()) {
        webSocket_.close();
    }
    disconnect(&webSocket_, nullptr, this, nullptr);
    webrtc_ = nullptr;
    qDebug() << "Peer" << peerId_ << "is destroyed and cleaned up.";
}


void Peer::setWebRTC(WebRTC *webrtc) {
    webSocket_.open(signalingServerUrl_);
    this->webrtc_ = webrtc;
    if (isOfferer){
        webrtc_->init(peerId_, true);
        webrtc_->addPeer(otherPeerId_);
        connect(this->webrtc_, &WebRTC::offerIsReady, this, &Peer::sendSDP);
    }
    else{
        webrtc_->init(peerId_, false);
        webrtc_->addPeer(otherPeerId_);
        connect(this->webrtc_, &WebRTC::answerIsReady, this, &Peer::sendSDP);
    }
}

void Peer::onConnected() {
    registerWithSignalingServer();
}

void Peer::onDisconnected() {
    qDebug() << "Disconnected from signaling server.";
}

void Peer::StartConnecting(){
    if (isOfferer)
        webrtc_->generateOfferSDP(otherPeerId_);
}

void Peer::onTextMessageReceived(const QString& message) {
    qDebug() << "onTextMessageReceived" << peerId_;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isObject()) {
        QJsonObject json = doc.object();
        QString type = json["type"].toString();
        if (type == "offer" || type == "answer") {
            qDebug() << "ASDFDAS ";
            QString remotePeerId = json["peerId"].toString();
            QString remoteSdp = json["sdp"].toString();
            handleSDP(remotePeerId, remoteSdp);
        }
    }
}

void Peer::registerWithSignalingServer() {
    qDebug() << "registerwithsignal" << peerId_;
    QJsonObject json;
    json["type"] = "register";
    json["peerId"] = peerId_;
    QJsonDocument doc(json);
    webSocket_.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void Peer::sendSDP(const QString& peerId, const QString& sdp) {
    qDebug() << "sendSDP" << peerId_;
    QJsonObject json;
    json["type"] = isOfferer?"offer":"answer";
    json["peerId"] = peerId_;
    json["sdp"] = sdp;
    QJsonDocument doc(json);
    webSocket_.sendTextMessage(doc.toJson(QJsonDocument::Compact));

}

void Peer::handleSDP(QString remotePeerId, QString localDescription) {
    if (!isOfferer){
        webrtc_->setRemoteDescription(remotePeerId, localDescription);
        webrtc_->generateAnswerSDP(otherPeerId_);

    }else{
        webrtc_->setRemoteDescription(otherPeerId_, localDescription);
    }
}
