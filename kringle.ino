#include <Logger.h>
#include <Ornament.h>
#include <Pumpkin.h>
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

String WIFI_SSID, WIFI_PASS, API_HOST;
Logger logger = Logger(WiFi.macAddress(), &webClient, &Serial, Logger::INFO, Logger::DEBUG);

Config conf;
bool configured = false;

int dataPin = D8;
int ledCount = 6;
Pumpkin ornament = Pumpkin(dataPin, &logger, true);

void(* reboot) (void) = 0;

void setup() {
  Serial.begin(9600);
  
  logger.log("[INFO] Initializing ornament");

  logger.log("[INFO] Initalizing SPIFFS filesystem...");
  if (initializeFS()) {
    logger.log("[INFO] OK");
  } else {
    logger.error("[ERROR] Failed to initialize SPIFFS");
    exit(1);
  }

  conf = Config();

  if (conf.boot_to_config) {
    logger.warn("[WARN] boot_to_config=TRUE, running config server");
    initializeConfigServer();
    return;
  }

  if (conf.wifiConfigured()) {
    WIFI_SSID = conf.wifi_ssid;
    WIFI_PASS = conf.wifi_password;
  }
  if (conf.apiConfigured()) {
    API_HOST = conf.api_host;
  }

  if (!conf.wifiConfigured()) {
    logger.warn("[WARN] Wifi is NOT CONFIGURED");
  } else {
    logger.log("[INFO] Wifi is configured to " + WIFI_SSID);
  }

  if (!conf.apiConfigured()) {
    logger.warn("[WARN] API is NOT CONFIGURED");
  } else {
    logger.log("[INFO] API is configured to " + API_HOST);
  }


  logger.log(conf.wifi_ssid);
  if (conf.wifiConfigured() && conf.apiConfigured()) {
    // ornament.on(Ornament::c_green, 50);
    logger.log("[INFO] Congifuration loaded successfully");
    // logger.log(WIFI_SSID);
    // logger.log(WIFI_PASS);
    // logger.log(API_HOST);
    // logger.log("[INFO] Starting Ornament socket server");
    if (initializeWifiClient(WIFI_SSID, WIFI_PASS)) {
      logger.setApi(API_HOST);
      initializeApi(API_HOST);
      configured = true;
    }
  }

  if (!configured) {
    // ornament.on(Ornament::c_yellow, 50);
    logger.log("[WARN] Congifuration failed to load, running configuration server", Logger::WARN);
    initializeConfigServer();
  }

}

void loop() {
  if (configured) {
    socketClient.poll();
    ornament.execute();
  } else {
    server.handleClient();
  }
}
