#include <SPI.h>
#include <Ethernet.h>
#define DEBUG
// define maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64
#include <WebSocket.h>
// define mac and ip address
// mac can be made up
// ip can be determined using DhcpAddressPrinter
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0xE4 };
byte ip[] = { 192, 168, 1 , 202 };
// Create a Websocket server
//WebSocket wsServer; // default is ("/", 80)
WebSocket wsServer("/", 8383); 

int light = 3;

void setup() {
#ifdef DEBUG  
  Serial.begin(57600);
#endif
  Ethernet.begin(mac, ip);
  // set callbacks / interrupt functions
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  
  delay(100); // Give Ethernet time to get ready
  
  pinMode(light, OUTPUT);
  digitalWrite(light, LOW);
}

void loop() {
  // server issues, default port 80
  wsServer.listen();
  
  
  // do other stuff
  delay(100);
  
  
  if (wsServer.isConnected()) {
    wsServer.send("abc123", 6);
    digitalWrite(light, HIGH);
  } else {
    digitalWrite(light, LOW);
  }
}


void onData(WebSocket &socket, char* dataString, byte frameLength) {  
#ifdef DEBUG
  Serial.print("Got data: ");
  Serial.write((unsigned char*)dataString, frameLength);
  Serial.println();
#endif

  // decode the dataString
  
  // Just ECHO back data for fun.
  socket.send(dataString, strlen(dataString));
}

void onConnect(WebSocket &socket) {
  Serial.println("onConnect called");
}
void onDisconnect(WebSocket &socket) {
  Serial.println("onDisconnect called");
}

// Testing light
void blink() {
  digitalWrite(light, HIGH);
  delay(1000);
  digitalWrite(light, LOW);
  delay(1000);
}

