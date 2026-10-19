Computer Networks Course - Fall03
- 
CA1 - Socket Programming - distributed live voice call

Arshiya Ataee - 810100252

Mobina Haghizadeh - 810100127

----------------------------------------------------
- **[Project Title](#project-title-distributed-voice-call-with-webrtc-and-websocket)**
- **[Project Summary](#project-summary)**
- **[Components Overview](#components-overview)**
  - **[SDP and ICE Candidates](#sdp-and-ice-candidates)**
  - **[WebSocket and Node.js Server](#websocket-and-nodejs-server)**
  - **[Signaling Process](#signaling-process)**
  - **[WebRTC (Web Real-Time Communication)](#webrtc-web-real-time-communication)**
  - **[AudioInput and AudioOutput Classes](#audioinput-and-audiooutput-classes)**
- **[Code Breakdown: Node.js Signaling Server](#code-breakdown-nodejs-signaling-server)**
- **[Code Breakdown: AudioInput class](#code-breakdown-audioinput-class)**
- **[Code Breakdown: AudioOutput class](#code-breakdown-audiooutput-class)**
- **[Code Breakdown: WebRTC class](#code-breakdown-webrtc-class)**
  - **[Constructor](#constructor)**
  - **[Init method](#init-method)**
  - **[addPeer method](#addpeer-method)**
  - **[generateOfferSDP method](#generateoffersdp-method)**
  - **[generateAnswerSDP method](#generateanswersdp-method)**
  - **[addAudioTrack method](#addaudiotrack-method)**
  - **[sendTrack method](#sendtrack-method)**
  - **[setRemoteDescription method](#setremotedescription-method)**
  - **[setRemoteCandidate method](#setremotecandidate-method)**
  - **[descriptionToJson method](#descriptiontojson-method)**
  - **[readVariant method](#readvariant-method)**
  - **[Getters and Setters](#getters-and-setters)**
  - [Peer Class](#code-breakdown-peer-class)
  - [Constructor](#constructor)
  - [setWebRTC method](#setwebrtc-method)
  - [onConnected method](#onconnected-method)
  - [onDisconnected method](#ondisconnected-method)
  - [onTextMessageReceived method](#ontextmessagereceived-method)
  - [registerWithSignalingServer method](#registerwithsignalingserver-method)
  - [sendSDP method](#sendsdp-method)
  - [handleSDP method](#handlesdp-method)
  - [handleICE method](#handleice-method)
  - [connectToSignalingServer method](#connecttosignalingserver-method)
  - [setIsOfferer method](#setisofferer-method)

- [Controller Class](#code-breakdown-controller-class)
  - [Constructor](#constructor-1)
  - [connect method](#connect-method)

- [Example: Call Setup Flow](#example-call-setup-flow)
- [Running the Project](#running-the-project)
- [WebRTC and Coturn Technology](#webrtc-and-coturn-technology)
- [Logs](#logs)
- [Challenges](#challenges)

### Project Title: Distributed Voice Call with WebRTC and WebSocket

# Project Summary
This project implements a distributed voice call system using WebRTC and WebSocket technologies. The setup includes two peers (representing two people on a call) who connect via a signaling server powered by WebSocket and Node.js. Each peer first exchanges connection details, including SDP (Session Description Protocol) and ICE Candidates (Interactive Connectivity Establishment), through the signaling server. After the initial handshake, the peers establish a direct peer-to-peer (P2P) connection via WebRTC, through which they can transmit audio data. Audio encoding and decoding are handled by the custom AudioInput and AudioOutput classes.

### Components Overview
Here’s a breakdown of each key component and how it fits into the call setup:
1. SDP and ICE Candidates
- SDP (Session Description Protocol): SDP is a format for describing the multimedia content of communication sessions. When a peer initiates a connection, it generates an SDP message containing details like supported codecs, media type (audio in this case), and other session parameters. This information helps the peers agree on compatible connection settings.
- ICE Candidates: ICE (Interactive Connectivity Establishment) candidates are network information packets exchanged between peers to establish a reliable connection path, which could be direct or relayed (e.g., through a TURN server if needed). ICE candidates help each peer discover the best network route for the P2P connection.

2. WebSocket and Node.js Server
- WebSocket: WebSocket is a communication protocol that provides full-duplex communication channels over a single, long-lived TCP connection. In this project, WebSocket is used for real-time communication between each peer and the signaling server, allowing them to exchange SDP and ICE Candidate messages instantly.
- Node.js Server: The Node.js server acts as a signaling server, mediating the initial exchange of SDP and ICE candidates between the two peers. While WebRTC handles media transport, the Node.js server is crucial in establishing the initial connection, enabling the peers to “find” each other.

3. Signaling Process
The signaling process includes:
- Peer A creates an SDP offer and sends it to the Node.js server via WebSocket.
- The server relays this offer to Peer B, which generates an SDP answer and sends it back to Peer A through the server.
- Both peers exchange ICE candidates through the server until they discover the best path for direct communication.
This signaling step is necessary only for connection setup; afterward, data flows directly between the peers.

4. WebRTC (Web Real-Time Communication)
WebRTC enables real-time, P2P communication between browsers or applications, supporting audio, video, and data sharing. In this project, WebRTC establishes the audio call, allowing peers to add and manage media tracks in real time. Once the signaling process is complete, WebRTC creates a direct channel through which audio data is streamed.

5. AudioInput and AudioOutput Classes
- AudioInput: This class captures and encodes the local audio (voice) of each peer, preparing it for transmission over the WebRTC connection.
- AudioOutput: This class receives and decodes the incoming audio data from the remote peer, making it audible to the user.


--------------------------------------------------------------------------------------------------------------------
# Code Breakdown: Node.js Signaling Server
This Node.js script creates a WebSocket server that handles real-time message exchange between clients. Here’s how it works:
```cpp
const WebSocket = require('ws');
```
This line imports the WebSocket library, which allows us to create a WebSocket server. This server will be responsible for facilitating communication between peers.

```cpp
const server = new WebSocket.Server({ port: 3000 });
```
We create a new WebSocket server instance (wss) and set it to listen on port 3000.
This server will accept incoming WebSocket connections from clients (i.e., the two peers in the call).

```cpp
wss.on('connection', (ws) => {
    console.log('New client connected');
```
The server listens for new connections using the `connection` event. When a new client (peer) connects, the connection event triggers, and the server logs a message indicating that a client has connected.
Each new connection gets a ws object, representing an individual client connection.

```cpp
ws.on('message', (message) => {
    console.log(`Received: ${message}`);
```
For each client, the server listens for incoming messages via the 'message' event. This is how peers send SDP and ICE candidate messages to each other during the signaling phase.
Each time the server receives a message, it logs the message content for reference.

```cpp
wss.clients.forEach(client => {
    if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message);
    }
});
```
This part of the code enables the server to relay (or “broadcast”) the received message to all other connected clients except the sender.
The wss.clients set holds all active connections to the WebSocket server.
For each client in this set, the server checks if the client is not the sender (client !== ws) and that the client’s WebSocket connection is open (client.readyState === WebSocket.OPEN). If both conditions are true, it sends the received message to that client.
This broadcast mechanism is essential in signaling: it ensures that peers can share SDP and ICE candidates to establish a P2P connection.

```cpp
ws.on('close', () => {
    console.log('Client disconnected');
});
```
The server listens for the `close` event on each WebSocket connection to detect when a client disconnects.
When a client disconnects, the server logs the event, which can be helpful for debugging and managing active connections.

```cpp
console.log('WebSocket server is running on ws://localhost:3000');
```
This final line outputs a message to indicate that the WebSocket server is successfully running on `ws://localhost:3000, providing a useful endpoint for client applications to connect.

-------------------------------------------------------------------------------------------------------------------
# Code Breakdown: AudioInput class
This AudioInput class:
1. Sets up and configures audio capture from the microphone.
2. Encodes the audio data into Opus format in real-time.
3. Emits the encoded data, ready for transmission—ideal for real-time applications like voice calls.
   
### Constructor
First, it creates an Opus encoder. Opus is a highly efficient codec, perfect for voice calls. Here, it’s configured to handle 48kHz mono audio, which is great for speech.
```cpp
encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &error);
```
Next, it configures the audio format to match the encoder settings. Finally, it retrieves the default input audio device and checks if it supports the format. If it doesn’t, the code stops with an error. Then, it creates a QAudioSource to actually capture audio from the microphone in this specified format.

### Start and Stop Functions
`start()`: This function kicks off the audio capture:
First, it opens the audio device in read-write mode.
Then, it starts the QAudioSource, which will capture audio and feed it into the AudioInput class.
If it fails to start, a warning is logged.
`stop()`: Stops the audio source, ending audio capture.

### Key Method: writeData
The heart of this class is writeData, where the actual encoding happens:
This function receives raw audio data as it’s being captured.
- First, it checks that the encoder is set up, as encoding is crucial for converting the audio to Opus format.
```cpp
const int maxPacketSize = 4000;
unsigned char encodedData[maxPacketSize];
int encodedBytes = opus_encode(encoder, reinterpret_cast<const opus_int16*>(data), len / 2, encodedData, maxPacketSize);
```
Here, opus_encode compresses the audio data (data) into Opus format, with a maximum size defined by maxPacketSize.
opus_encode takes in raw PCM audio and returns compressed bytes (encoded audio). If the encoding fails, it logs an error.

If encoding is successful, it then emits the encoded data using a custom signal (bufferReady), sending it out as a QByteArray for other parts of the application to use or send over a network.

### readData method
This function is essentially a placeholder. In this class, we’re only focusing on capturing and encoding audio, so we don’t actually need to read any data. The function just returns 0, since there’s no data for it to process here.

### emit bufferReady method
While not directly a method, this signal is used to send the encoded audio data out to any other part of the program that’s ready to handle it. When writeData successfully encodes audio, it emits bufferReady with the compressed audio, which can then be picked up for transmission, making it essential for streaming the encoded audio onward.

These methods keep the class minimal and focused on the core functionality: capturing, encoding, and emitting audio data.

-------------------------------------------------------------------------------------------------------------------
# Code Breakdown: AudioOutput class
This AudioOutput class:
1. The constructor does all the setup, making sure audio settings, the Opus decoder, and the connections are ready to go.
2. queues up incoming audio data.
3. decodes and plays each packet in the queue.
   
### Constructor
The constructor does a lot of the heavy lifting in setting up the audio output:
1. Setting up the audio format
2. Creating the audio sink:
It initializes audioSink using the default audio output device, which will play the decoded audio.
Starts the audio device to ensure everything is ready for playback.
```cpp
audioSink = new QAudioSink(QMediaDevices::defaultAudioOutput(), format, this);
audioDevice = audioSink->start();
```
1. Creating the Opus decoder:
The constructor creates an Opus decoder to handle incoming encoded audio packets and convert them back into raw audio.
If it fails to create the decoder, the program logs a fatal error.

1. Connecting the signal:
Finally, it connects the newPacket signal to the play slot. This connection lets play() start whenever a new packet arrives in the queue.

### Destructor
The destructor cleans up by deleting the audioSink and destroying the Opus decoder.

### Key method: addData
Purpose: This method adds incoming encoded audio data to a queue, making it ready for playback.
#### How it works:
- It locks the mutex to prevent any other function from changing the queue at the same time.
- Adds the data packet to packetQueue, a queue that holds all packets waiting to be played.
- Emits the newPacket signal, which triggers play() to process and play the audio immediately.
  
```cpp
packetQueue.enqueue(data);
emit newPacket();
play();
```

### Key method: Play
This is the core method that decodes and plays the audio:
#### 1-Check the queue:

If the queue is empty, it logs a message and exits. This keeps it from trying to play if there’s nothing ready.

#### 2-Dequeuing a packet:

It retrieves the next audio packet from packetQueue.

#### 3-Decoding the packet:

Using the Opus decoder, it converts the encoded data back into raw audio (PCM format).
opus_decode fills outputBuffer with decoded audio samples. If decoding fails, it logs an error.

```cpp
int decodedSamples = opus_decode(decoder, reinterpret_cast<const unsigned char *>(packet.data()), packet.size(), outputBuffer, frameSize, 0);
```
#### 4-Playing the decoded audio:
The raw audio data in outputBuffer is then written to audioDevice, which plays it through the speakers or headphones.
Logs the number of bytes successfully written or a warning if it fails.
```cpp
audioDevice->write(reinterpret_cast<const char*>(outputBuffer), decodedSamples * sizeof(opus_int16));
```
--------------------------------------------------------------------------------------------------------------------

# Code Breakdown: Webrtc class

The WebRTC class is a central part of managing peer-to-peer audio communications in a Qt application using WebRTC. It encapsulates the necessary functionality to create connections, handle audio streams, and manage SDP and ICE candidates, providing a robust interface for real-time communication.

#### Constructor

The constructor initializes QObject using the parent and creates an m_audio object of type rtc::Description::Audio. It connects the gatheringComplited signal to a slot that, upon completion of the gathering phase, converts localDescription to JSON and emits appropriate signals.

#### Init method

This function initializes WebRTC with a given ID and specifies whether the user is an Offerer. It also configures ICE settings and sets audio with m_bitRate. Finally, it publishes the isOfferer state through the isOffererChanged signal and logs the configuration details with qDebug.

```cpp
rtc::Configuration config;
config.ice_servers.push_back(rtc::IceServer::Create("stun:stun.l.google.com:19302"));
```
`rtc::Configuration config`: Creates a configuration object to hold settings for the peer connection.
`config.ice_servers.push_back(...)`:
Adds an ICE server to the configuration, which is essential for NAT traversal.
`rtc::IceServer::Create("stun:stun.l.google.com:19302")`:
 Specifies a STUN server (used to obtain the public IP address of the client) for establishing peer-to-peer connections.

#### addPeer method

The addPeer function creates a new PeerConnection and stores it for the peerId. It then sets up several callbacks, each related to a specific event:

- onLocalDescription: Updates localDescription when local SDP is generated.
- onLocalCandidate: Sends local ICE candidates to the other peer.
- onStateChange: Checks the state of the connection.
- onGatheringStateChange: Confirms ICE gathering completion.
- onTrack: Receives audio data from the other peer.

```cpp
peerConnection = rtc::PeerConnection::Create(config);
peerConnection->OnTrack = [this](rtc::Track track) {
  
    m_peerTracks[peerId] = track;

    track->onMessage([this, peerId](rtc::message_variant data) {
        QByteArray packet = readVariant(data);
        Q_EMIT incommingPacket(peerId, packet, packet.size());
    });
};
```
`peerConnection = rtc::PeerConnection::Create(config)`: This line creates a new instance of a WebRTC PeerConnection using the previously defined configuration. This establishes the peer-to-peer connection.
`peerConnection->OnTrack = [this](rtc::Track track) {...}`: This line sets a callback function that is triggered when a new media track is received from the remote peer. The lambda captures this, allowing access to class members.
`m_peerTracks[peerId] = track`: Inside the callback, it stores the incoming track associated with a specific peer ID in the m_peerTracks map for later use.
`track->onMessage([this, peerId](rtc::message_variant data) {...})`: This line sets up another callback that is invoked whenever a message is received on the track. The lambda captures this and peerId for further processing.
`QByteArray packet = readVariant(data)`: This line reads the incoming message data (of type rtc::message_variant) and converts it into a QByteArray.
`Q_EMIT incommingPacket(peerId, packet, packet.size())`: Finally, it emits a signal named incommingPacket, passing the peer ID, the packet data, and its size to notify other parts of the application that a new packet has been received.

#### generateOfferSDP method

This function generates an SDP of type Offer. After finding the PeerConnection for peerId, it defines a callback for onLocalDescription and converts the generated SDP to JSON, emitting the relevant signal.

```cpp
rtc::SessionDescription offer = peerConnection->CreateOffer();
peerConnection->SetLocalDescription(offer);
```

#### generateAnswerSDP method

Generates an SDP answer in response to an offer from a peer.

#### addAudioTrack method

Adds an audio track to a peer connection and sets up message and frame handling.

```cpp
rtc::AudioTrack track = rtc::AudioTrack::Create(trackName);
peerConnection->AddTrack(track);

```

#### sendTrack method

The sendTrack method sends audio data to a peer by first checking if the connection and track exist. It constructs an RTP packet with the audio data and necessary headers, then tries to send this packet to the peer, logging success or any errors encountered during the process.

#### setRemoteDescription method

This method converts the remote sdp from the other peer into an rtc::Description object and sets it as the remote description on the corresponding PeerConnection.

```cpp
rtc::SessionDescription remoteDescription = rtc::ParseSDP(sdp);
peerConnection->SetRemoteDescription(remoteDescription);
```

#### setRemoteCandidate method

Adds remote ICE candidates to a peer connection, helping establish the connection path.

```cpp
rtc::IceCandidate iceCandidate = rtc::IceCandidate::Create(candidate, sdpMid);
peerConnection->AddIceCandidate(iceCandidate);
```

#### descriptionToJson method

Converts an SDP description to JSON format for easier handling and transmission.

#### readVariant method

Converts a rtc::message_variant to QByteArray, handling different types of messages.

#### getters and setters for isOfferer and others:

These are used for maintaining internal state and notifying changes. While straightforward, ensuring the consistency of state across the application can be tricky.

#### setBitRate(int newBitRate), resetBitRate(), setPayloadType(int newPayloadType), resetPayloadType(), setSsrc(rtc::SSRC newSsrc), resetSsrc(): 

These methods manage audio parameters like bit rate, payload type, and SSRC. The challenges include validating input values, ensuring that changes take effect correctly, and managing state consistency across peers.

-------------------------------------------------------------------------------------------------------------------

# Code Breakdown: Peer class

The Peer class effectively encapsulates the functionalities required for a WebRTC peer to engage in signaling via WebSockets. By managing connections, handling SDP and ICE candidates, and interfacing with a WebRTC instance, it plays a pivotal role in establishing and maintaining peer-to-peer communications.

#### Constructor
The constructor initializes a Peer object with specific attributes such as peerId, signalingServerUrl, and a boolean flag indicating whether the peer is the offerer. During initialization, it connects several signals related to the WebSocket:

- Connected: When the WebSocket connection is established, the onConnected() method is triggered.
- Disconnected: Logs disconnection events via the onDisconnected() method.
- Text Message Received: Incoming messages are handled by the onTextMessageReceived() method.

```cpp
webSocket_.open(signalingServerUrl_); 
```
This line establishes the WebSocket connection to the signaling server.

#### setWebRTC method

This method associates a WebRTC instance with the Peer object. It connects signals to handle SDP offers and answers depending on the role of the peer (offerer or answerer). It also links the generation of local ICE candidates.

#### onConnected method

Executed upon a successful WebSocket connection, this method registers the peer with the signaling server. If the peer is the offerer, it generates an SDP offer and sends it to the signaling server.

```cpp
sdp_ = webrtc_->generateOfferSDP(peerId_);
```
This line is critical as it creates an SDP offer necessary for establishing the peer connection.

#### onDisconnected method

This method is invoked when the WebSocket disconnects, providing feedback through debug logs.

#### onTextMessageReceived method

Handles incoming messages from the signaling server. It parses the message as a JSON document to check if it contains SDP or ICE candidate information.

```cpp
QString type = json["type"].toString(); 
```
This key line categorizes the message, directing it to appropriate handling functions.

#### registerWithSignalingServer method

Sends a registration message to the signaling server to register the peer with its ID.

```cpp
json["type"] = "register"; 
```
This line indicates the message type to ensure proper processing on the server.

#### sendSDP method
Sends the generated SDP offer or answer to the signaling server. This is essential for establishing the media parameters for the connection.

```cpp
json["type"] = "sdp";
```
This line is vital as it signals that the message contains SDP data.

#### handleSDP method

Processes the received SDP from a remote peer, setting it as the remote description in the WebRTC instance.

#### handleICE method

Handles the received ICE candidates. It emits a signal that can be used by the UI or other components to update the connection status or manage UI elements.

```cpp
emit iceCandidateReceived(candidate); 
```
This line allows other components to react to new ICE candidates.

#### connectToSignalingServer method

Attempts to open a WebSocket connection to the signaling server. It includes commented-out code to check if the connection is already valid before reopening it, ensuring efficiency.

#### setIsOfferer method

Updates the isOfferer status of the peer, allowing for flexibility in peer roles during communication.

--------------------------------------------------------------------------------------------------------------------

# Code Breakdown: Controller class

The Controller class serves as a management entity that coordinates audio input and output for WebRTC connections between peers. It initializes audio handling components and manages the connection logic based on whether it is an offerer or a receiver in the WebRTC communication.

#### Constructor

This constructor initializes the Controller object with several components and parameters:

The constructor creates instances of AudioInput and AudioOutput, initializing them with nullptr to signify that they do not have a parent object at this point.
It assigns the passed parameters (webrtcOffer, webrtcReceive, offerPeer, receivePeer, and type) to corresponding member variables for later use.

#### connect method

This method manages the connection logic for the audio controller:

`Logging`:

It logs a debug message (though the message appears to be a placeholder) using qDebug(), which is a function from the Qt framework for outputting debug information.

`Audio Controller Initialization`:

An instance of AudioController is created with the necessary parameters, including the m_type, webrtcOffer, webrtcReceive, offerPeer, and receivePeer. This component will handle the audio routing and management between peers.

`Connection Logic`:

A timer is set to trigger after 5 seconds, calling a lambda function that checks the type of controller:
If the type is "offerer", it initiates the connection process by invoking StartConnecting() on the offerPeer and logs that the connection process has started.
Another timer is set to trigger after 10 seconds, which calls the Handle() method of the audiocontroller to manage audio processing.

--------------------------------------------------------------------------------------------------------------------
### Example: Call Setup Flow
Here's a brief step-by-step example of how the call setup would look:

1. Connecting to Signaling Server: Each peer connects to the Node.js signaling server through WebSocket.
2. SDP Exchange:
- Peer A creates an SDP offer and sends it to Peer B through the server.
- Peer B responds with an SDP answer, which is relayed back to Peer A.
1. ICE Candidate Exchange:
Each peer sends ICE candidates to the other to determine the optimal path for the P2P connection.
1. Establishing P2P Connection: Once SDP and ICE candidates are exchanged, WebRTC establishes a secure, direct connection between the peers.
2. Audio Streaming: The audio is then streamed through WebRTC:
- AudioInput captures and encodes the audio from each peer's microphone.
- AudioOutput receives the audio stream and plays it for each peer, creating a seamless voice call experience.


### Running the Project
1. Start the Node.js signaling server.
2. Run the client applications (the peers) to initiate the WebSocket connection and begin the signaling process.
3. Once connected, the peers can start a voice call over WebRTC.
-------------------------------------------------------------------------------------------------------------

# Logs

![My Project Logs](https://github.com/mobinahz/CN_CA_1/logs/1.jpeg)

![Second Pic](https://github.com/mobinahz/CN_CA_1/logs/2.jpeg)

-------------------------------------------------------------------------------------------------------------------

# Challenges

1. We struggled with getting the answerSdp to generate properly.
2. We ended up having to hardcode some IDs, which wasn't ideal.
3. When we tried running the two executable files separately and connecting them, the receiver kept crashing.

-------------------------------------------------------------------------------------------------------------------

# WebRTC and Coturn Technology
### What is WebRTC? 
WebRTC, which stands for Web Real-Time Communication, is a technology that lets people connect directly over the internet to share video, audio, or data in real time. Think of it as a way for your browser to make video calls or send files without needing a middleman. This makes it faster and often more private.

### Pros and Cons of WebRTC 
The main advantages of WebRTC are its ability to create direct connections, which helps reduce lag, and its built-in security features that keep your communications safe. However, it can be tricky to set up, especially when it comes to getting two devices to find each other and communicate, especially if they're behind firewalls.

### Key Components: STUN, TURN, and Signaling Server
`Signaling Server`: Before two users can start talking, they need to exchange some information about how to connect. This is where the signaling server comes in. It helps them share details like their connection addresses. However, it doesn’t actually handle any video or audio; it just helps them get started.

`STUN`: STUN helps each device figure out its public IP address and port number when it's behind a router or firewall. Basically, it lets each peer know how to reach the other.

`TURN`: Sometimes, direct connections don’t work, especially if firewalls are strict. That’s where TURN comes into play. It acts like a relay, passing messages between peers if they can’t connect directly. While this is a bit slower because the data has to go through a server, it ensures that the connection remains intact.

### What is Coturn?
Coturn is basically a server that helps people communicate over the internet, especially when they’re using WebRTC (a technology for real-time communication). It plays a crucial role in making sure your audio and video calls work smoothly, even when users are behind tricky network setups like NATs (which is what most home routers use).

### How Does Coturn Work?
`STUN Functionality`:
When you want to connect with someone, Coturn can help you figure out your public IP address and the port that others can use to reach you. This is important because many people have private IP addresses that don’t directly connect to the internet. STUN helps you find out how the world sees you, which is the first step to establishing a connection.

`TURN Functionality`:
If direct connections between users fail (maybe because of strict firewalls), Coturn can also act as a relay. This means it will take your media (like your voice or video) and send it to the other person instead of you connecting directly. While this might add a tiny bit of lag since your data is going through an extra stop, it ensures that you can still have your conversation.

### Why Use Coturn?
- Reliable: Coturn is built to handle lots of connections at once, making it perfect for applications where many people need to talk, like video calls.

- Flexible: It does both STUN and TURN, so it can help in a variety of situations, whether direct connections are possible or not.

- Open-source: Because it’s open-source, developers can tweak it to fit their specific needs. If you want to change something or make it work better for your app, you can!

- Secure: Coturn supports secure connections, which is really important for protecting your conversations from prying eyes.

### Setting It Up

When you set up Coturn, you’ll configure it with some settings like which ports to use and how to authenticate users. Once it’s up and running, your application can connect to it, and it’ll take care of any connection problems that might come up during your calls.

### In a Nutshell
In short, Coturn is a key player in making sure your WebRTC calls go off without a hitch. It helps establish connections and makes sure your conversations can happen even if the network is a bit complicated. Whether it’s helping you find your IP or relaying your video, Coturn makes real-time communication a lot smoother!






