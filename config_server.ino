
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

void handleAuthDash() {
  Serial.println("[INFO] GET /");
  String data = readFile("/config.html");
  server.send(200, "text/html", data);
}

void handleAuthUpdate() {
  Serial.println("[INFO] POST /config/");
  DynamicJsonDocument doc(128);
  deserializeJson(doc, server.arg(0));
  if (!doc.containsKey("wifi_ssid") || !doc.containsKey("wifi_password") || !doc.containsKey("api_host")) {
    server.send(500, "text/plain", "[ERROR] MUST INCLUDE wifi_ssid AND wifi_password AND api_host ARGUMENTS");
  }
  String ssid = doc["wifi_ssid"];
  String pass = doc["wifi_password"];
  String api_host = doc["api_host"];
  Serial.print("[INFO] Storing config: { wifi_ssid: '");
  Serial.print(ssid);
  Serial.print("', wifi_password: '");
  Serial.print(pass);
  Serial.print("', api_host: '");
  Serial.print(api_host);
  Serial.println("' }");

  bool success = writeFile("/config.json", doc);
  if (success) {
    Serial.println("[INFO] Success");
    server.send(200);
    delay(1000);
    reboot();
  } else {
    Serial.println("[ERROR] Failed to save configuration");
    server.send(500);
  }
}

void handleNotFound() {
  Serial.print("[WARN] ");
  Serial.print(server.method());
  Serial.print(" ");
  Serial.print(server.uri());
  Serial.println(" NOT FOUND");
  server.send(400);
}
