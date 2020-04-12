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

class ConfigHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/config/");
  }
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {

    
    
    server.send(200);
    return true;
  }
} configHandler;

String header;


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
