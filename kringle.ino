#include <Ornament.h>

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

HTTPClient webClient;

WebsocketsClient socketClient;

StaticJsonDocument<200> jsonData;

int dataPin = D8;
int ledCount = 6;

Ornament ornament = Ornament(dataPin);

void initializeOrnament() {
  Serial.println("[INFO] Initializing ornament");
  ornament.init();
}

void socketOnMessage(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
  deserializeJson(jsonData, message.data());

  if (jsonData.containsKey("brightness")) {
    //setBrightness(&ornament, jsonData["brightness"]);
    ornament.set_brightness(jsonData["brightness"]);
  }

  if (jsonData.containsKey("red") && jsonData.containsKey("green") && jsonData.containsKey("blue")) {
    int c = Adafruit_NeoPixel::Color(jsonData["green"], jsonData["red"], jsonData["blue"]);
    //setColor(&ornament, c);
    ornament.set_color(c);
  }

  if (jsonData.containsKey("xmas")) {
    //xmas(&ornament);
    ornament.xmas();
  }

  if (jsonData.containsKey("jmas")) {
    //jmas(&ornament);
    ornament.jmas();
  } 
}

void socketOnEvent(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void initializeApi() {
  Serial.println("[INFO] Initializing api");
  String macId = WiFi.macAddress();
  String apiUrl= "http://staging.northpoler.com/api/ornaments/" + macId + "/";
  Serial.println("[INFO] GET " + apiUrl);
  webClient.begin(apiUrl);
  int statusCode = webClient.GET();
  
  Serial.println("[INFO] status: " + String(statusCode));
  if (statusCode == 200) {
    //successBlink(&ornament);
    ornament.success_blink();
  } else if (statusCode == 201) {
    //infoBlink(&ornament);
    ornament.info_blink();
  } else {
    //errorBlink(&ornament);
    ornament.error_blink();
    return;
  }

  socketClient.onMessage(socketOnMessage);
  socketClient.onEvent(socketOnEvent);
  String socketUrl = "http://staging.northpoler.com/ws/ornaments/device/" + macId + "/";
  socketClient.connect(socketUrl);
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

  initializeApi();

  ornament.on(Ornament::c_white, 50);
  
}


void loop() {
//  Serial.println(".");
  socketClient.poll();
}
