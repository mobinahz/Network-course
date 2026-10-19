#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QDebug>
#include "network/webrtc.h"
#include "network/peer.h"
#include <QTimer>
#include <QQmlContext>
#include "controller/Controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QString type = argv[1];

    QUrl signalingServerUrl("ws://localhost:3000");
    Peer peer1("offerer", "receiver", signalingServerUrl, true, nullptr);
    Peer peer2("receiver", "offerer", signalingServerUrl, false, nullptr);
    WebRTC webrtc1;
    WebRTC webrtc2;

    if (type == "offerer"){
        peer1.setWebRTC(&webrtc1);
        qDebug() << "Registering";
        engine.load(QUrl::fromLocalFile("../../src/ui/Main.qml"));
        Controller *controller = new Controller(nullptr, type, &webrtc1, nullptr, &peer1, nullptr);
        engine.rootContext()->setContextProperty("controller", controller);
    }
    if (type == "answerer"){
        peer2.setWebRTC(&webrtc2);
        engine.load(QUrl::fromLocalFile("../../src/ui/Main.qml"));
        Controller *controller = new Controller(nullptr, type, nullptr, &webrtc2, nullptr, &peer2);
        engine.rootContext()->setContextProperty("controller", controller);
    }
    return app.exec();
}
