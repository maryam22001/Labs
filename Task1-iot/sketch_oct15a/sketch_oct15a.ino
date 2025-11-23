#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Merna Atef";
const char* password = "123456789";

// WebSocket Server details
#define SERVER  "192.168.1.6"    // Change to your server IP (your computer's IP)
#define PORT    3000             // Change to your server port
#define URL     "/"              // WebSocket endpoint path (keep as "/")

// LED pin - Changed to D1 (GPIO5) which is better for LED control
// D3 (GPIO0) can cause boot issues
const int LED_PIN = D2;  // or use pin 5

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Connect to WebSocket server
  webSocket.begin(SERVER, PORT, URL);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(1000);
  
  Serial.println("Connecting to WebSocket server...");
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WebSocket] Disconnected!");
      break;
      
    case WStype_CONNECTED:
      Serial.printf("[WebSocket] Connected to server: %s\n", payload);
      // Send connection confirmation
      webSocket.sendTXT("NodeMCU Connected");
      break;
    
    case WStype_TEXT:
      Serial.printf("[WebSocket] Received: %s\n", payload);
      handleMessage((char*)payload);
      break;
      
    case WStype_ERROR:
      Serial.println("[WebSocket] Error!");
      break;
  }
}

void handleMessage(char* payload) {
  String message = String(payload);
  
  // Simple text commands (like "on" and "off")
  if (message == "on") {
    analogWrite(LED_PIN, 1023); // Max brightness
    Serial.println("LED ON");
    webSocket.sendTXT("LED is ON");
  }
  else if (message == "off") {
    analogWrite(LED_PIN, 0);
    Serial.println("LED OFF");
    webSocket.sendTXT("LED is OFF");
  }
  // JSON format commands
  else if (message.indexOf("{") != -1) {
    // Parse JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      const char* command = doc["command"];
      
      if (strcmp(command, "ON") == 0) {
        analogWrite(LED_PIN, 1023);
        Serial.println("LED ON");
        webSocket.sendTXT("{\"status\":\"LED is ON\"}");
      }
      else if (strcmp(command, "OFF") == 0) {
        analogWrite(LED_PIN, 0);
        Serial.println("LED OFF");
        webSocket.sendTXT("{\"status\":\"LED is OFF\"}");
      }
      else if (strcmp(command, "BRIGHTNESS") == 0) {
        int brightness = doc["value"];
        int pwmValue = map(brightness, 0, 255, 0, 1023);
        analogWrite(LED_PIN, pwmValue);
        Serial.print("Brightness set to: ");
        Serial.println(brightness);
        
        String response = "{\"status\":\"Brightness: " + String(brightness) + "\"}";
        webSocket.sendTXT(response);
      }
    }
  }
}