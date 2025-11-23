const WebSocket = require('ws');
const express = require('express');
const path = require('path');

const app = express();
const PORT = 3000;

// Serve static files (HTML)
app.use(express.static('public'));

// Explicitly serve index.html for root path
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Create HTTP server
const server = app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
  console.log('WebSocket server running on ws://localhost:3000');
});

// Create WebSocket server
const wss = new WebSocket.Server({ server });

// Store all connected clients
let clients = new Set();

wss.on('connection', (ws, req) => {
  const clientIP = req.socket.remoteAddress;
  console.log(`New client connected from ${clientIP}`);
  
  clients.add(ws);
  
  // Send welcome message
  ws.send(JSON.stringify({ 
    type: 'connection', 
    message: 'Connected to server' 
  }));

  ws.on('message', (message) => {
    console.log('Received:', message.toString());
    
    try {
      // Try to parse as JSON
      const data = JSON.parse(message.toString());
      console.log('Parsed data:', data);
      
      // Broadcast to all connected clients
      clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
          client.send(JSON.stringify({
            type: 'status',
            data: data
          }));
        }
      });
      
    } catch (e) {
      // Plain text message
      console.log('Plain text:', message.toString());
      
      // Broadcast to all clients
      clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
          client.send(JSON.stringify({
            type: 'status',
            message: message.toString()
          }));
        }
      });
    }
  });

  ws.on('close', () => {
    console.log('Client disconnected');
    clients.delete(ws);
  });

  ws.on('error', (error) => {
    console.error('WebSocket error:', error);
    clients.delete(ws);
  });
});

// Broadcast function
function broadcast(message) {
  clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      if (typeof message === 'string') {
        client.send(message);
      } else {
        client.send(JSON.stringify(message));
      }
    }
  });
}

// API endpoint to send commands to NodeMCU
app.get('/api/command/:cmd', (req, res) => {
  const command = req.params.cmd;
  console.log('API command:', command);
  
  broadcast(command);
  
  res.json({ success: true, command: command });
});

console.log('\n Server started successfully!');
console.log('Open http://localhost:3000 in your browser');
console.log(' NodeMCU should connect automatically\n');