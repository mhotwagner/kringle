/*
// This is the legachy HTTP server used for initial testing
// this server can server as a backup to a live socket connection
// in the case that internet access is not available
// (or, more specifically, that a northpoler api is unreachable)
//
// To use, be sure to call initializeServer somewhere in the setup()
// NOTE, this currently shares a namespace w/the config server
// so it's one or the other
//
// Finally, if this is really meant to serve as a backup, its
// endpoints should be kept up to date w/the socket endpoints
*/

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
    ornament.blink(n);
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
      ornament.off();
    }

    if (turnOn) {
      ornament.on(ornament.c_white);
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
  ornament.blink();
  String data = readFile("/dashboard.html");
  server.send(200, "text/html", data);
}

// xmas view
void handleXmas() {
  Serial.println("[INFO] GET /xmas/");
  ornament.xmas();
  server.send(200);
}

// jmas view
void handleJmas() {
  Serial.println("[INFO] GET /xmas/");
  ornament.jmas();
  server.send(200);
}


// Start the server
void initializeServer() {
  Serial.println("[INFO] Initializaing server...");

  server.addHandler(&blinkHandler);
  server.addHandler(&apiHandler);
  
  server.on("/", handleIndex);
  server.on("/xmas/", handleXmas);
  server.on("/jmas/", handleJmas);
//  server.on("/blink", handleBlink);
//  server.on("/dash", handleDash);

  server.begin();
  
  Serial.print("[INFO] Listening on port ");
  Serial.println(serverPort);
}
