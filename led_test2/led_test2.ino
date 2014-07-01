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
#define DATA_PIN 11
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
  if(DEBUG) { Serial.begin(9600); }
  
  // Set the LED controller
  delay(1000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  blackout();
  if(DEBUG) { Serial.println("Lights engaged"); }

}

void loop() {
  loop_lights();

}


void onData(WebSocket &socket, char* dataString, byte frameLength) {  
  if(DEBUG && false) { 
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
  
  if(message.length() != MSG_SIZE) {
    return false;
  }
  int index = (int) (message.substring(0,3)).toInt();
  if(index >= NUM_LEDS) {
    return false;
  }
  String color = message.substring(4, MSG_SIZE);
  
  setColor(color, index);
  return true;
}

void setColor(String color, int index) {
  // set LED color. convert string to hex
  
  if(color.length() != 6) {
    if(DEBUG) { Serial.println("bad color"); }
    return;
  }
  char charBuf[6];
  color.toCharArray(charBuf, 6);
  // RGB, 0xFFAACC, 3 bytes
  unsigned long hexcolor = (getVal(charBuf[0]) << 20) + (getVal(charBuf[1]) << 16)
     + (getVal(charBuf[2]) << 12) + (getVal(charBuf[3]) << 8)
     + (getVal(charBuf[4]) << 4) + getVal(charBuf[5]);
  //int hexcolor = red << 16 + green << 8 + blue;
  
  //leds[index] = CRGB::White;
  leds[index] = hexcolor;
  //FastLED.show();
  
  if(DEBUG) { 
    Serial.print("Color " + String(index) + " : ");
    Serial.println(hexcolor, HEX); 
  }
}

byte getVal(char c) {
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
