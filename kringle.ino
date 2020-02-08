#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <FS.h>

#include <Adafruit_NeoPixel.h>

const int serverPort = 80;
ESP8266WebServer server(serverPort);

int dataPin = D8;
int ledCount = 20;

byte mac[6];
String macId;

Adafruit_NeoPixel ornament = Adafruit_NeoPixel(ledCount, dataPin, NEO_GRB + NEO_KHZ800);

// Start the shelf lights
void initializeOrnament() {
  Serial.println("[INFO] Initializing ornament");
  ornament.begin();
  ornament.show();
  ornament.setBrightness(50);
  blinkLeds(&ornament, 3);
}


void setup() {
  Serial.begin(9600);

  initializeFS();

  bool wifiConfigured = isWifiConfigured();

  Serial.print("[INFO] - Wifi is ");
  if (!wifiConfigured) {
    Serial.print("NOT ");
  }
  Serial.println("properly configured");

  if (wifiConfigured) {
    initializeWifi(get_wifi_ssid(), get_wifi_pass());  
  }
  
  //initializeServer();

  initializeOrnament();
}


void loop() {
  Serial.println(".");
}
