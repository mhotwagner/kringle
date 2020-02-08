#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <FS.h>

#include <Adafruit_NeoPixel.h>



int dataPin = D8;
int ledCount = 20;

Adafruit_NeoPixel ornament = Adafruit_NeoPixel(ledCount, dataPin, NEO_GRB + NEO_KHZ800);


// Start the shelf lights
void initializeOrnament() {
  Serial.println("[INFO] Initializing ornament");
  ornament.begin();
  ornament.show();
  ornament.setBrightness(50);
  blinkLeds(&ornament, 3);
}

class BlinkHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith( "/blink" );
  }
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {    
    int n;
    if (requestUri == "/blink" || requestUri == "/blink/") {
      n = 1;
    } else {
      n = requestUri.substring(7).toInt();
    }
    blinkLeds(&ornament, n);
    server.send(200);
    return true;
  }
} blinkHandler;

class APIHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/api/");
  }
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {
    bool turnOn = false;
    
    // Handle arguments
    String argument = requestUri.substring(5);
    if (argument.startsWith("on/")) turnOn = true;
    if (argument.startsWith("off/")) {
      ornament.fill();
      ornament.show();
    }

    if (turnOn) {
      ornament.fill(Adafruit_NeoPixel::Color(255, 255, 255));
      ornament.show();
    }
    
    server.send(200);
    return true;
  }
} apiHandler;



// server info
const int serverPort = 80;
ESP8266WebServer server(serverPort);

String header;

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

// Index view
void handleIndex() {
  Serial.println("[INFO] GET /");
  blinkLeds(&ornament, 1);
  String data = readFile("/dashboard.html");
  server.send(200, "text/html", data);
}

// xmas view
void handleXmas() {
  Serial.println("[INFO] GET /xmas/");
  xmas(&ornament);
  server.send(200);
}


// Start the server
void initializeServer() {
  Serial.println("[INFO] Initializaing server...");

  server.addHandler(&blinkHandler);
  server.addHandler(&apiHandler);
  
  server.on("/", handleIndex);
  server.on("/xmas/", handleXmas);
//  server.on("/blink", handleBlink);
//  server.on("/dash", handleDash);

  server.begin();
  
  Serial.print("[INFO] Listening on port ");
  Serial.println(serverPort);
}

// Start the file system
void initializeFS() {
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");
  } else {
    Serial.println("SPIFFS Initialization...failed");
  }
}

void setup() {
  Serial.begin(9600);

  initializeFS();

  initializeWifi();
  initializeServer();

  initializeOrnament();
  
  server.begin();
}


void loop() {
  server.handleClient();
}
