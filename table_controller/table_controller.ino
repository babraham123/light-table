#include <SPI.h>
#include <Ethernet.h>
#include <WebSocket.h>
#include "FastSPI_LED2.h"

#define STATUS 3
#define PORT 8383
#define SERVER "www.bereketabraham.com"
#define KEY "amazing1"
#define MSG_SIZE 10

#define NUM_LEDS 50
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

#define DEBUG true

// define maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 256
// define mac and ip address, mac can be made up (unique)
// ip can be determined using DhcpAddressPrinter
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0xE4 };
byte ip[] = { 192, 168, 1 , 202 };
// Create a Websocket server
//WebSocket wsServer; // default is ("/", 80)
WebSocket wsServer("/", PORT); 

// define variables for initial server call
//IPAddress server(162,248,10,57);  // no DNS
char server[] = SERVER;
EthernetClient client;

void setup() {
  Serial.begin(9600);

  // Set the LED controller
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  blackout();
  if(DEBUG) { Serial.println("Lights engaged"); }


  //  if (Ethernet.begin(mac) == 0) {
  //    Serial.println("Failed to configure Ethernet using DHCP");
  //  }
  // try to congifure using IP address instead of DHCP:
  Ethernet.begin(mac, ip);
  delay(1000);
  if(DEBUG) { Serial.println("ethernet"); }

  if (client.connect(server, 80)) {
    if(DEBUG) { Serial.println("connected"); }

    // Make the HTTP request:
    // trailing slash is important for django
    String request = "GET /lab/initialize/?key=";
    request = request + String(KEY) + String("&port=");
    request = request + String(PORT) + String(" HTTP/1.1");
    client.println(request);
    
    String host = String("Host: ") + String(SERVER);
    client.println(host);
    client.println("Connection: close");
    client.println();
    
    client.stop();
    delay(100);
    if(DEBUG) { Serial.println(request + host); }
  } 
  else {
    if(DEBUG) { Serial.println("connection failed"); }
  }
  
  // Set up websocket server
  //Ethernet.begin(mac, ip);
  // set callbacks / interrupt functions
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  delay(1000);
  if(DEBUG) { Serial.println("websocket server began"); }
  
  // Set the status light
  pinMode(STATUS, OUTPUT);
  digitalWrite(STATUS, LOW);
}

void loop() {
  // server issues, default port 80
  wsServer.listen();
  
  // do other stuff
  FastLED.show();
  //delay(100);
  
  if (wsServer.isConnected()) {
    // wsServer.send("abc123", 6);
    digitalWrite(STATUS, HIGH);
  } else {
    digitalWrite(STATUS, LOW);
  }
}


void onData(WebSocket &socket, char* dataString, byte frameLength) {  
//  if(DEBUG) { 
//    Serial.print("Got data: ");
//    Serial.write((unsigned char*)dataString, frameLength);
//    Serial.println();
//  }

  // decode the dataString
  processData(String(dataString));
  
  // Just ECHO back data for fun.
  if(DEBUG) { socket.send(dataString, strlen(dataString)); }
}

void onConnect(WebSocket &socket) {
  if(DEBUG) { Serial.println("onConnect called"); }
}
void onDisconnect(WebSocket &socket) {
  if(DEBUG) { Serial.println("onDisconnect called"); }
}

void processData(String message) {
  // message must a index and 6 letter hex color values
  // ie "001:FF00AA"
  // total chars in message: 10
  
  if(message.length() != MSG_SIZE) {
    if(DEBUG) { Serial.println("Message wrong size"); }
    return;
  }
  int index = (int) (message.substring(0,3)).toInt();
  if(index >= NUM_LEDS) {
    if(DEBUG) { Serial.println("Index out of range"); }
    return;
  }
  String color = message.substring(4, MSG_SIZE);
  
  setColor(color, index);
}

void setColor(String color, int index) {
  // set LED color. convert string to hex
  
  // -4 from the index, +1 for the null terminator
  char charBuf[MSG_SIZE - 4 + 1];
  color.toCharArray(charBuf, MSG_SIZE - 4 + 1);
  
  // RGB, 0xFFAACC, 3 bytes
  unsigned long hexcolor = (getVal(charBuf[0]) << 20) + (getVal(charBuf[1]) << 16)
     + (getVal(charBuf[2]) << 12) + (getVal(charBuf[3]) << 8)
     + (getVal(charBuf[4]) << 4) + getVal(charBuf[5]);
  
  //leds[index] = CRGB::White;
  leds[index] = hexcolor;
  //FastLED.show();
  
  if(DEBUG) { 
    Serial.print("Color " + String(index) + " : ");
    Serial.println(hexcolor, HEX); 
  }
}

unsigned long getVal(char c) {
  if(c >= '0' && c <= '9') {
    return (byte)(c - '0'); 
  } else {
    return (byte)(c-'A'+10);
  }
}

void blackout() {
  for(int i = 0; i < NUM_LEDS; i++) 
  { leds[i] = CRGB::Black; }
  
  FastLED.show();
  delay(2000);
}

void loop_lights() {
  // Move a single white led 
  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
    leds[whiteLed] = CRGB::White;
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();
  
    delay(200);
    leds[whiteLed] = CRGB::Black;
  }
}
