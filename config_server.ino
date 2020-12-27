
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
  } else if (path.endsWith("html")) {
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
  Serial.println("[INFO] GET /");
  File file = getFile("/config.html");
  String ct = getContentType("/config.html");
  logger.log(server.streamFile(file, ct));
  file.close();
}

bool processAuthUpdate(String _doc) {
  Serial.println("[INFO] POST /config/");
  DynamicJsonDocument doc(512);
  deserializeJson(doc, _doc);
  if (!doc.containsKey("wifi_ssid") || !doc.containsKey("wifi_password") || !doc.containsKey("api_host")) {
    server.send(500, "text/plain", "[ERROR] MUST INCLUDE wifi_ssid AND wifi_password AND api_host ARGUMENTS");
  }
  bool boot_to_config = doc.containsKey("boot_to_config") ? doc["boot_to_config"].as<bool>() : false;
  String ssid = doc["wifi_ssid"];
  String pass = doc["wifi_password"];
  String api_host = doc["api_host"];
  Serial.print("[INFO] Storing config: { boot_to_config: ");
  Serial.print(boot_to_config);
  Serial.print(", wifi_ssid: '");
  Serial.print(ssid);
  Serial.print("', wifi_password: '");
  Serial.print(pass);
  Serial.print("', api_host: '");
  Serial.print(api_host);
  Serial.println("' }");

  bool success = writeFile("/config.json", doc);
  if (success) {
    Serial.println("[INFO] Success");
    return true;

  } else {
    Serial.println("[ERROR] Failed to save configuration");
    return false;
  }
}


void handleAuthUpdate() {
  Serial.println("[INFO] POST /config/");
  bool success = processAuthUpdate(server.arg(0));
  if (success) {
    Serial.println("[INFO] Success");
    delay(1000);
    server.send(200, "text/plain", "ok");
    reboot();
  } else {
    Serial.println("[ERROR] Failed to save configuration");
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
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/assets/");
  }
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {
    Serial.println("[INFO] GET " + requestUri);
    String path = requestUri.substring(7);

    if (!fileExists(path)) {
      handleNotFound();
      return false;
    }

    logger.log("1");
    File file = getFile(path);
    logger.log("2");
    String ct = getContentType(path);
    logger.log("3");
    Serial.print("actual size: ");
    Serial.println(file.size());
    size_t sentSize = streamFile(file, ct);
    Serial.print("sent size: ");
    Serial.println(sentSize);
    logger.log("4");
    file.close();

    return true;
  }
} assetHandler;

bool streamConfig() {
  File file = getFile("/config.json");
  String ct = getContentType("/config.json");
  size_t sentSize = streamFile(file, ct);
  bool success = sentSize == file.size();
  file.close();
  return success;
}

class ConfigApiHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/api/config");
  }

  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {
    String path = requestUri.substring(5);
    if (requestMethod==HTTP_GET) {
      Serial.println("[INFO] GET /api/");

      if (path=="config") {  
        return streamConfig();
      } else {
          server.send(404, "text/plain", requestUri + " not found");
          return false;
      }
    }
    if (requestMethod==HTTP_POST) {
      Serial.println("[INFO] POST /api/config/");
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
} configApiHandler;

void handleNotFound() {
  Serial.print("[WARN] ");
  Serial.print(server.method());
  Serial.print(" ");
  Serial.print(server.uri());
  Serial.println(" NOT FOUND");
  server.send(404);
}

// Config Server
void initializeConfigServer() {
  Serial.println("[INFO] Starting config server over wifi");

  const char* ssid = "northpoler";  // Enter SSID here
  const char* password = "";  //Enter Password here

  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.addHandler(&assetHandler);
  server.addHandler(&configApiHandler);

  server.on("/", handleAuthDash);
  server.on("/config/", HTTP_POST, handleAuthUpdate);
  server.onNotFound(handleNotFound);

  server.begin();

  Serial.print("[INFO] Broadcasting as SSID <");
  Serial.print(ssid);
  Serial.println(">");
  Serial.print("[INFO] Accepting connections at <");
  Serial.print(local_ip);
  Serial.println(">");
}
