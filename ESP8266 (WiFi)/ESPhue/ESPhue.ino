#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char ssid[] = "McDonald's FREE WiFi";
const char password[] = "gratiswifi";

const int pinR = 14;
const int pinG = 12;
const int pinB = 13;
int h = 0;
int s = 100;
int l = 50;
int r = 0;
int g = 0;
int b = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Started up!");
  //Setup pins
  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);
  server.begin();//Make sure to start a server.
  asyncConnect();
}

void loop() {
  asyncHandle();
}

