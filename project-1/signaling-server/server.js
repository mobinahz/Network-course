const WebSocket = require('ws');
const server = new WebSocket.Server({ port: 3000 });
const clients = new Set();

server.on('connection', (socket) => {
    // Add new client to the set
    clients.add(socket);
    console.log("New client connected. Total clients:", clients.size);

    // When a message is received, broadcast it to everyone except the sender
    socket.on('message', (message) => {
        // Convert the message buffer to a string
        const messageString = message.toString();

        try {
            // Parse it as JSON (if the message is JSON formatted)
            const parsedMessage = JSON.parse(messageString);
            console.log("Received JSON message:", parsedMessage);

            // Broadcast the message to other clients
            clients.forEach((client) => {
                if (client !== socket && client.readyState === WebSocket.OPEN) {
                    client.send(JSON.stringify(parsedMessage));
                }
            });
        } catch (error) {
            console.log("Received non-JSON message:", messageString);
        }
    });

    // Remove client from the set when it disconnects
    socket.on('close', () => {
        clients.delete(socket);
        console.log("Client disconnected. Total clients:", clients.size);
    });
});

console.log("WebSocket server is running on ws://localhost:3000");
