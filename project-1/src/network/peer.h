#ifndef PEER_H
#define PEER_H

#include <QWebSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUrl>

#include "webrtc.h"

class Peer : public QObject {
    Q_OBJECT

public:
    Peer(const QString& peerId, const QString& otherPeerId, const QUrl& signalingServerUrl, bool isOfferer, QObject* parent = nullptr);
    ~Peer();
    void sendPlainMessage(const QString& message);
    void setRemotePeerId(const QString& remotePeerId);
    void setWebRTC(WebRTC *webrtc);
    void handleSDP(QString peerId, QString localDescription);
    void setAnswer();
    QString peerId_;
    QString otherPeerId_;
    QWebSocket webSocket_;
    void registerWithSignalingServer();
    void StartConnecting();

signals:
    void sdpReceived(const QString& sdp);
    void iceCandidateReceived(const QString& candidate);

private slots:
    void onDisconnected();
    void onTextMessageReceived(const QString& message);
    void sendSDP(const QString& peerId, const QString& sdp);

private:


    QString remotePeerId_;
    QUrl signalingServerUrl_;
    WebRTC *webrtc_;
    bool isOfferer;
    QString sdp_;
    QString theirSdp_;
    void onConnected();
};

#endif // PEER_H
