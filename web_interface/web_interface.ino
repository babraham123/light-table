#include <SPI.h>
#include <Ethernet.h>
#include <WebSocket.h>

#define LED 3
#define PORT 8383
#define SERVER "www.bereketabraham.com"
#define KEY "amazing1"
#define NUM_LEDS 50
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
  if(DEBUG) { Serial.begin(9600); }

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
  
  //Ethernet.begin(mac, ip);
  // set callbacks / interrupt functions
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  delay(1000);
  if(DEBUG) { Serial.println("websocket server began."); }
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  // server issues, default port 80
  wsServer.listen();
  
  // do other stuff
  delay(100);
  
  if (wsServer.isConnected()) {
    // wsServer.send("abc123", 6);
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}


void onData(WebSocket &socket, char* dataString, byte frameLength) {  
  if(DEBUG) { 
    Serial.print("Got data: ");
    Serial.write((unsigned char*)dataString, frameLength);
    Serial.println();
  }

  // decode the dataString
  if( !processData(String(dataString)) ) {
    if(DEBUG) { Serial.println("Processing failed"); }
  }
  
  // Just ECHO back data for fun.
  if(DEBUG) { socket.send(dataString, strlen(dataString)); }
}

void onConnect(WebSocket &socket) {
  if(DEBUG) { Serial.println("onConnect called"); }
}
void onDisconnect(WebSocket &socket) {
  if(DEBUG) { Serial.println("onDisconnect called"); }
}

boolean processData(String message) {
  // message must a index and 6 letter hex color values
  // ie "001:FF00AA"
  // total chars in message: 10
  
  if(message.length() != 10) {
    return false;
  }
  int index = (int) (message.substring(0,3)).toInt();
  if(index >= NUM_LEDS) {
    return false;
  }
  
  String color = message.substring(4, 10);
  // do something
  
  return true;
}

