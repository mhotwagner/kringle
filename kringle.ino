#include <Ornament.h>
#include "Config.h"

#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Adafruit_NeoPixel.h>

const int serverPort = 80;

ESP8266WebServer server(serverPort);

using namespace websockets;
WebsocketsClient socketClient;

HTTPClient webClient;

Config conf;
bool configured = false;

int dataPin = D8;
int ledCount = 6;
Ornament ornament = Ornament(dataPin, &Serial, false);

void(* reboot) (void) = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("[INFO] Initializing ornament");

  Serial.print("[INFO] Initalizing SPIFFS filesystem...");
  if (initializeFS()) {
    Serial.println("OK");
  } else {
    Serial.println("ERROR");
    exit(1);
  }

  conf = Config();

  if (conf.boot_to_config) {
    Serial.println("[WARN] Encountered boot_to_config directive");
    Serial.println("[WARN] Skipping rest of config check and");
    Serial.println("[WARN] Running Configuration server");
    initializeConfigServer();
    return;
  }

  if (!conf.wifiConfigured()) {
    Serial.println("[WARN] Wifi is NOT CONFIGURED");
  } else {
    Serial.print("[INFO] Wifi is configured to ");
    Serial.println(conf.wifi_ssid);
  }

  if (!conf.apiConfigured()) {
    Serial.println("[WARN] API is NOT CONFIGURED");
  } else {
    Serial.print("[INFO] API is configured to ");
    Serial.println(conf.api_host);
  }

  if (conf.apiConfigured()) {
    initializeApi(conf.api_host);
  }

  if (conf.wifiConfigured() && conf.apiConfigured()) {
    ornament.on(Ornament::c_green, 50);
    Serial.println("[INFO] Congifuration loaded successfully");
    Serial.println("[INFO] Starting Ornament socket server");
    initializeWifiClient(conf.wifi_ssid, conf.wifi_password);
    configured = true;
  } else {
    // ornament.on(Ornament::c_yellow, 50);
    Serial.println("[WARN] Congifuration failed to load");
    Serial.println("[WARN] Running configuration server");
    initializeConfigServer();
  }

}

void loop() {
  if (configured) {
    socketClient.poll();
  } else {
    server.handleClient();
  }
}
