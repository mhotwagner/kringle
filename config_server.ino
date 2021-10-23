
String getContentType(String path) {
  if (path.endsWith("json")) {
    return "application/json";
  } else if (path.endsWith("js")) {
    return "application/javascript";
  } else if (path.endsWith("gif")) {
    return "image/gif";
  } else if (path.endsWith("jpg") || path.endsWith("jpeg")) {
    return "image/jpeg";
  } else if (path.endsWith("png")) {
    return "image/png";
  } else if ((path.endsWith("html")) || path.endsWith("htm")) {
    return "text/html";
  } else if (path.endsWith("css")) {
    return "text/css";
  }
  return "text/plain";
}


String renderAuthTemplate(String _template, Config _conf) {
  _template.replace("{{boot_to_config}}", _conf.boot_to_config ? "checked" : "");
  _template.replace("{{wifi_ssid}}", _conf.wifi_ssid);
  _template.replace("{{wifi_password}}", _conf.wifi_password);
  _template.replace("{{api_host}}", _conf.api_host);
  return _template;
}

void handleAuthDash() {
  logger.log("[INFO] GET /");
  File file = getFile("/config.html");
  String ct = getContentType("/config.html");
  logger.log(server.streamFile(file, ct));
  file.close();
}

bool processAuthUpdate(String _doc) {
  logger.log("[INFO] POST /config/");
  DynamicJsonDocument doc(512);
  deserializeJson(doc, _doc);
  if (!doc.containsKey("wifi_ssid") || !doc.containsKey("wifi_password") || !doc.containsKey("api_host")) {
    server.send(500, "text/plain", "[ERROR] MUST INCLUDE wifi_ssid AND wifi_password AND api_host ARGUMENTS");
  }
  bool boot_to_config = doc.containsKey("boot_to_config") ? doc["boot_to_config"].as<bool>() : false;
  String ssid = doc["wifi_ssid"];
  String pass = doc["wifi_password"];
  String api_host = doc["api_host"];
  String msg = "[INFO] Storing config: { boot_to_config: ";
  msg.concat(boot_to_config ? "Y" : "N");
  msg.concat(", wifi_ssid: '");
  msg.concat(ssid);
  msg.concat("', wifi_password: '"); 
  msg.concat(pass);
  msg.concat("', api_host: '");
  msg.concat(api_host);
  msg.concat("' }");
  logger.log(msg);

  bool success = writeFile("/config.json", doc);
  if (success) {
    logger.log("[INFO] Success");
    return true;

  } else {
    logger.error("[ERROR] Failed to save configuration");
    return false;
  }
}


void handleAuthUpdate() {
  logger.log("[INFO] POST /config/");
  bool success = processAuthUpdate(server.arg(0));
  if (success) {
    logger.log("[INFO] Success");
    delay(1000);
    server.send(200, "text/plain", "ok");
    reboot();
  } else {
    logger.error("[ERROR] Failed to save configuration");
    server.send(500, "text/plain", "failed to save config");
  }
}

size_t streamFile(File file, String contentType) {
  int fileSize = file.size();
  server.setContentLength(fileSize);
  String filename = file.name();
  if (filename.endsWith("gz")) {
    server.sendHeader("Content-Encoding", "gzip");
  }
  server.sendHeader("Connection", "keep-alive");
  server.send(200, contentType, "");

  char buffer[1024];
  int sentSize = 0;
  while (fileSize > 0) {
    logger.log(".");
    size_t length = min((int)(sizeof(buffer) - 1), fileSize);
    file.read((uint8_t *)buffer, length);
    int _sentSize = server.client().write((const char*)buffer, length);
    sentSize += _sentSize;
    logger.log(_sentSize);
    fileSize -= length;
  }
  // server.sendHeader("Connection", "close");
  return sentSize;
}

class AssetHandler : public RequestHandler {
  public:
    AssetHandler() {
    }

    bool canHandle(HTTPMethod method, const String& uri) {
      return uri.startsWith("/assets/") ? true : false;
    }
    
    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, const String& requestUri) {
      logger.log("[INFO] GET " + requestUri);
      String path = requestUri.substring(7);
      logger.log("[INFO] Attempting to stream " + path);
  
      if (!fileExists(path)) {
        handleNotFound();
        return false;
      }
  
      File file = getFile(path);
      String ct = getContentType(path);
      // Serial.print("actual size: ");
      // Serial.println(file.size());
      size_t sentSize = streamFile(file, ct);
      // Serial.print("sent size: ");
      // Serial.println(sentSize);
      file.close();
  
      return true;
    }
};

bool streamConfig() {
  File file = getFile("/config.json");
  String ct = getContentType("/config.json");
  size_t sentSize = streamFile(file, ct);
  bool success = sentSize == file.size();
  file.close();
  return success;
}

class ConfigApiHandler : public RequestHandler {  
  public:
    bool canHandle(HTTPMethod requestMethod, const String& uri) override {
      return uri.startsWith("/api/config");
    }
  
    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, const String& requestUri) {
      String path = requestUri.substring(5);
      if (requestMethod==HTTP_GET) {
        logger.log("[INFO] GET /api/");
  
        if (path=="config") {  
          return streamConfig();
        } else {
            server.send(404, "text/plain", requestUri + " not found");
            return false;
        }
      }
      if (requestMethod==HTTP_POST) {
        logger.log("[INFO] POST /api/config/");
        bool success = processAuthUpdate(server.arg(0));
        if (success) {
          server.send(200, "text/plain", "OK");
          return true;
        } else {
          server.send(500);
          return false;
        }
      }
    }
};

void handleNotFound() {
  String msg = "[WARN] ";
  msg.concat(server.method());
  msg.concat(" ");
  msg.concat(server.uri());
  msg.concat(" NOT FOUND");
  logger.warn(msg);
  server.send(404);
}

// Config Server
void initializeConfigServer() {
  logger.log("[INFO] Starting config server over wifi");

  const char* ssid = "northpoler";  // Enter SSID here
  const char* password = "";  //Enter Password here

  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid);

  server.addHandler(new AssetHandler());
  server.addHandler(new ConfigApiHandler());

  server.on("/", handleAuthDash);
  server.on("/config/", HTTP_POST, handleAuthUpdate);
  server.onNotFound(handleNotFound);

  server.begin();

  String msg = "[INFO] Broadcasting as SSID <";
  msg.concat(ssid);
  msg.concat(">");
  logger.log(msg);
  msg = "[INFO] Accepting connections at <";
  msg.concat(local_ip.toString());
  msg.concat(">");
  logger.log(msg);
}
