#include <Ornament.h>

#include <ArduinoJson.h>
<<<<<<< HEAD

=======
#include <ArduinoWebsockets.h>
>>>>>>> A working ornament with a socket connection
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <FS.h>

#include <Adafruit_NeoPixel.h>

<<<<<<< HEAD
const int serverPort = 80;
ESP8266WebServer server(serverPort);
=======
using namespace websockets;

HTTPClient webClient;

WebsocketsClient socketClient;

StaticJsonDocument<200> jsonData;
>>>>>>> A working ornament with a socket connection

int dataPin = D8;

int c_red = Adafruit_NeoPixel::Color(0, 255, 0);
int c_green = Adafruit_NeoPixel::Color(255, 0, 0);
int c_blue =  Adafruit_NeoPixel::Color(0, 0, 255);
int c_yellow = Adafruit_NeoPixel::Color(255, 255, 0);
int c_white = Adafruit_NeoPixel::Color(255, 255, 255);

<<<<<<< HEAD
Adafruit_NeoPixel ornament = Adafruit_NeoPixel(ledCount, dataPin, NEO_GRB + NEO_KHZ800);

<<<<<<< HEAD
<<<<<<< HEAD
// Start the shelf lights
=======

=======
Ornament thing = Ornament(&ornament);
>>>>>>> Commit before I break everything
=======
Ornament ornament = Ornament(dataPin);
>>>>>>> WIP

>>>>>>> A working ornament with a socket connection
void initializeOrnament() {
  Serial.println("[INFO] Initializing ornament");
  ornament.init();
}

<<<<<<< HEAD
=======
// Read a file from SPIFFS
String readFile(String path) {
  Serial.println("[INFO] Servfing file " + path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    String data = file.readString();
    file.close();
    return data;
  }
  Serial.println("[ERROR] File Not Found");
  return "";
}


// Start the file system
void initializeFS() {
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");
  } else {
    Serial.println("SPIFFS Initialization...failed");
  }
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
>>>>>>> A working ornament with a socket connection

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

  //turnOnLeds(&ornament, c_white, 50);
  ornament.on(Ornament::c_white, 50);
  
}


void loop() {
//  Serial.println(".");
  socketClient.poll();
}
