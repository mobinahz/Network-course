#include "webrtc.h"
#include <QtEndian>
#include <QJsonDocument>

#pragma pack(push, 1)
struct RtpHeader {
    uint8_t first;
    uint8_t marker:1;
    uint8_t payloadType:7;
    uint16_t sequenceNumber;
    uint32_t timestamp;
    uint32_t ssrc;
};
#pragma pack(pop)


WebRTC::WebRTC(QObject *parent)
    : QObject{parent},
    m_audio("Audio")
{
    connect(this, &WebRTC::gatheringComplited, [this] (const QString &peerID) {

        m_localDescription = descriptionToJson(m_peerConnections[peerID]->localDescription().value());
        Q_EMIT localDescriptionGenerated(peerID, m_localDescription);

        if (m_isOfferer)
            Q_EMIT this->offerIsReady(peerID, m_localDescription);
        else
            Q_EMIT this->answerIsReady(peerID, m_localDescription);
    });
}

WebRTC::~WebRTC()
{}


void WebRTC::init(const QString &id, bool isOfferer)
{
    m_localId = id;
    m_isOfferer = isOfferer;

    m_config = rtc::Configuration();
    m_audio = rtc::Description::Audio("audio");
    m_audio.setBitrate(m_bitRate);
    Q_EMIT isOffererChanged();
}

void WebRTC::addPeer(const QString &peerId)
{
    auto newPeer = std::make_shared<rtc::PeerConnection>(m_config);
    m_peerConnections[peerId] = newPeer;
    newPeer->onLocalDescription([this, peerId](const rtc::Description &description) {
        QString localDescriptionJson = descriptionToJson(description);
        m_localDescription = localDescriptionJson;
    });

    newPeer->onLocalCandidate([this, peerId](rtc::Candidate candidate) {
        QString candidateString = QString::fromStdString(candidate.candidate());
        Q_EMIT  this->localCandidateGenerated(peerId, candidateString, QString::fromStdString(candidate.mid()));
    });
    newPeer->onStateChange([this, peerId](rtc::PeerConnection::State state) {
        switch (state) {
        case rtc::PeerConnection::State::New:
            qDebug() << "Peer" << peerId << "state: New";
            break;
        case rtc::PeerConnection::State::Connecting:
            qDebug() << "Peer" << peerId << "state: Connecting";
            break;
        case rtc::PeerConnection::State::Connected:
            qDebug() << "Peer" << peerId << "state: Connected";
            break;
        case rtc::PeerConnection::State::Disconnected:
            qDebug() << "Peer" << peerId << "state: Disconnected";
            break;
        case rtc::PeerConnection::State::Failed:
            qDebug() << "Peer" << peerId << "state: Failed";
            break;
        case rtc::PeerConnection::State::Closed:
            qDebug() << "Peer" << peerId << "state: Closed";
            break;
        default:
            break;
        }
    });
    newPeer->onGatheringStateChange([this, peerId](rtc::PeerConnection::GatheringState state) {
        switch(state) {
        case rtc::PeerConnection::GatheringState::New:
            qDebug() << "Gathering state" << peerId << "state: New";
            break;
        case rtc::PeerConnection::GatheringState::InProgress:
            qDebug() << "Gathering state" << peerId << "state: InProgress";
            break;
        case rtc::PeerConnection::GatheringState::Complete:
            m_gatheringComplited = true;
            emit gatheringComplited(peerId);
            qDebug() << "Gathering state" << peerId << "state: Complete";
            break;
        }
    });
    newPeer->onTrack([this, peerId] (std::shared_ptr<rtc::Track> track) {
        m_peerTracks[peerId] = track;

        track->onMessage([this, peerId](rtc::message_variant data) {
            QByteArray packet = readVariant(data);
            Q_EMIT incommingPacket(peerId, packet, packet.size());
        });

        qDebug() << "Incoming track received for peer" << peerId;
    });
    addAudioTrack(peerId, "m=audio 49170 UDP/TLS/RTP/SAVPF 111");
}

void WebRTC::generateOfferSDP(const QString &peerId)
{
    qDebug() << "111";
    auto it = m_peerConnections.find(peerId);
    if (it != m_peerConnections.end()) {
        auto peerConnection = it.value();
        peerConnection->onLocalDescription([this, peerId](const rtc::Description &description) {
    m_localDescription = descriptionToJson(description);
            Q_EMIT localDescriptionGenerated(peerId, m_localDescription);
        });

        peerConnection->setLocalDescription(rtc::Description::Type::Offer);
        qDebug() << "Generated SDP offer for peer ID:" << peerId;
    } else {
        qWarning() << "Peer connection not found for peerId:" << peerId;
    }
}

void WebRTC::generateAnswerSDP(const QString &peerId)
{
    auto it = m_peerConnections.find(peerId);
    if (it != m_peerConnections.end()) {
        auto peerConnection = it.value();

        peerConnection->onLocalDescription([this, peerId](const rtc::Description &description) {
            m_localDescription = descriptionToJson(description);
            Q_EMIT localDescriptionGenerated(peerId, m_localDescription);
        });
        peerConnection->localDescription()->generateSdp();
        qDebug() << "Generated SDP answer for peer ID:" << peerId;
    } else {
        qWarning() << "Peer connection not found for peerId:" << peerId;
    }
}

void WebRTC::addAudioTrack(const QString &peerId, const QString &trackName)
{
    auto it = m_peerConnections.find(peerId);
    if (it != m_peerConnections.end()) {
        auto peerConnection = it.value();
        auto track = peerConnection->addTrack(rtc::Description::Media(trackName.toStdString(), "audio", rtc::Description::Direction::SendOnly));
        m_peerTracks[peerId] = track;

        track->onMessage([this, peerId](rtc::message_variant data) {
            QByteArray messageData = readVariant(data);
            emit incommingPacket(peerId, messageData, messageData.size());
        });

        track->onFrame([this](rtc::binary frame, rtc::FrameInfo info) {
            qDebug() << "Received audio frame with timestamp:" << info.timestamp;
        });
    } else {
        qWarning() << "Peer connection not found for peerId:" << peerId;
    }
}


void WebRTC::sendTrack(const QString &peerId, const QByteArray &buffer)
{
    auto peerConnectionIt = m_peerConnections.find(peerId);
    auto peerTrackIt = m_peerTracks.find(peerId);
    if (peerConnectionIt == m_peerConnections.end() || peerTrackIt == m_peerTracks.end()) {
        qWarning() << "Peer connection or track not found for peerID:" << peerId;
        return;
    }

    RtpHeader rtpHeader;
    rtpHeader.first = 0x80;
    rtpHeader.marker = 0;
    rtpHeader.payloadType = m_payloadType;
    rtpHeader.sequenceNumber = qToBigEndian(m_sequenceNumber++);
    rtpHeader.timestamp = qToBigEndian(getCurrentTimestamp());
    rtpHeader.ssrc = qToBigEndian(m_ssrc);

    QByteArray rtpPacket;
    rtpPacket.append(reinterpret_cast<const char*>(&rtpHeader), sizeof(RtpHeader));  // Append the RTP header
    rtpPacket.append(buffer);  // Append the payload (audio data)


    std::vector<std::byte> packetData(rtpPacket.size());
    std::transform(rtpPacket.begin(), rtpPacket.end(), packetData.begin(), [](char c) {
        return static_cast<std::byte>(c);
    });

    try {
        peerTrackIt.value()->send(packetData);
    } catch (const std::exception &e) {
        qWarning() << "Error sending RTP packet to peer" << peerId << ":" << e.what();
    }

}

void WebRTC::setRemoteDescription(const QString &peerID, const QString &sdp)
{
    auto it = m_peerConnections.find(peerID);
    if (it != m_peerConnections.end()) {
        auto peerConnection = it.value();

        std::string sdpStr = sdp.toStdString();

        rtc::Description remoteDescription(sdpStr, m_isOfferer ? rtc::Description::Type::Answer : rtc::Description::Type::Offer);
        peerConnection->setRemoteDescription(remoteDescription);
        qDebug() << "Set remote description for peerID:" << peerID;
    } else {
        qWarning() << "Peer connection not found for peerId:" << peerID;
    }
}

void WebRTC::setRemoteCandidate(const QString &peerID, const QString &candidate, const QString &sdpMid)
{
    auto it = m_peerConnections.find(peerID);
    if (it != m_peerConnections.end()) {
        auto peerConnection = it.value();

        rtc::Candidate rtcCandidate(candidate.toStdString(), sdpMid.toStdString());

        peerConnection->addRemoteCandidate(rtcCandidate);

        qDebug() << "Added remote candidate for peerID:" << peerID
                 << " with candidate:" << candidate
                 << " and sdpMid:" << sdpMid;
    } else {
        qWarning() << "Peer connection not found for peerId:" << peerID;
    }
}

QByteArray WebRTC::readVariant(const rtc::message_variant &data)
{
    QByteArray result;

    if (std::holds_alternative<std::string>(data)) {
        const auto &str = std::get<std::string>(data);
        result = QByteArray::fromStdString(str);
    }
    else if (std::holds_alternative<rtc::binary>(data)) {
        const rtc::binary &binaryData = std::get<rtc::binary>(data);
        result = QByteArray(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    } else {
        qWarning() << "Unsupported data type in rtc::message_variant";
        return result;
    }

    const int rtpHeaderSize = sizeof(RtpHeader);

    if (result.size() >= rtpHeaderSize) {
        result = result.mid(rtpHeaderSize);
    } else {
        qWarning() << "Data size is smaller than RTP header size. Unable to remove header.";
    }

    return result;
}

QString WebRTC::descriptionToJson(const rtc::Description &description)
{
    QString sdpType = QString::fromStdString(description.typeString());
    QString sdpContent = QString::fromStdString(description.generateSdp());

    QString jsonString = QString("{\"type\": \"%1\", \"sdp\": \"%2\"}")
                             .arg(sdpType)
                             .arg(sdpContent.replace("\"", "\\\""));


    return jsonString;
}

int WebRTC::bitRate() const
{
    return m_bitRate;
}

void WebRTC::setBitRate(int newBitRate)
{
    if (m_bitRate != newBitRate) {
        m_bitRate = newBitRate;
        emit bitRateChanged();
    }
}

void WebRTC::resetBitRate()
{
    const int defaultBitRate = 48000;
    if (m_bitRate != defaultBitRate) {
        m_bitRate = defaultBitRate;
        emit bitRateChanged();
    }
}


int WebRTC::payloadType() const
{
    return m_payloadType;
}

void WebRTC::setPayloadType(int newPayloadType)
{
    if (m_payloadType != newPayloadType) {
        m_payloadType = newPayloadType;
        emit payloadTypeChanged();
    }
}

void WebRTC::resetPayloadType()
{
    const int defaultPayloadType = 111;
    if (m_payloadType != defaultPayloadType) {
        m_payloadType = defaultPayloadType;
        emit payloadTypeChanged();
    }
}

rtc::SSRC WebRTC::ssrc() const
{
    return m_ssrc;
}

void WebRTC::setSsrc(rtc::SSRC newSsrc)
{
    if (m_ssrc != newSsrc) {
        m_ssrc = newSsrc;
        emit ssrcChanged();
    }
}

void WebRTC::resetSsrc()
{
    if (m_ssrc != 2) {
        m_ssrc = 2;
        emit ssrcChanged();
    }
}


bool WebRTC::isOfferer() const
{
    return m_isOfferer;
}

void WebRTC::setIsOfferer(bool newIsOfferer)
{
    m_isOfferer = newIsOfferer;
    Q_EMIT isOffererChanged();
}

void WebRTC::resetIsOfferer()
{
    m_isOfferer = false;
    Q_EMIT isOffererChanged();
}


