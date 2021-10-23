StaticJsonDocument<200> jsonData;

void socketOnMessage(WebsocketsMessage message) {
  String msg = "[INFO] Got Message: ";
  msg.concat(message.data());
  logger.log(msg);
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

  if (jsonData.containsKey("breathe")) {
    //jmas(&ornament);
    ornament.breathe();
  }

  if (jsonData.containsKey("breathingRainbow")) {
    //jmas(&ornament);
    ornament.breathingRainbow();
  }

  if (jsonData.containsKey("reboot") && jsonData["reboot"].as<bool>()) {
    reboot();
  }

  if (jsonData.containsKey("config")) {
    socketOnConfigUpdate(jsonData["config"].as<JsonObject>());
  }
}

void socketOnEvent(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        logger.log("[INFO] Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        logger.log("[INFO] Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        logger.log("[INFO] Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        logger.log("[INFO] Got a Pong!");
    }
}

void socketOnConfigUpdate(JsonObject config) {
  logger.log("[INFO] Handling config update via socket");
  DynamicJsonDocument doc(128);
  doc["wifi_ssid"] = config["boot_to_config"];
  doc["wifi_ssid"] = config["wifi_ssid"];
  doc["wifi_password"] = config["wifi_password"];
  doc["api_host"] = config["api_host"];

  bool success = writeFile("/config.json", doc);
  if (success) {
    logger.log("[INFO] Success");
  } else {
    logger.error("[ERROR] Failed to save configuration");
  }
}

void initializeApi(String apiUrlBase) {
  logger.log("[INFO] Initializing api to " + apiUrlBase);
  String macId = WiFi.macAddress();
  String apiUrl= "http://" + apiUrlBase + "/api/ornaments/" + macId + "/";
  logger.log("[INFO] GET " + apiUrl);
  WiFiClient wifiClient;
  webClient.begin(wifiClient, apiUrl);
  int statusCode = webClient.GET();
  
  logger.log("[INFO] status: " + String(statusCode));
  if (statusCode == 200) {
    logger.log("[INFO] API successfully initialized");
    //successBlink(&ornament);
    ornament.success_blink();
  } else if (statusCode == 201) {
    logger.log("[INFO] API successfully initialized and new ornament added");
    //infoBlink(&ornament);
    ornament.info_blink();
  } else {
    logger.error("[ERROR] Failed to initialize API");
    //errorBlink(&ornament);
    ornament.error_blink();
    return;
  }

  socketClient.onMessage(socketOnMessage);
  socketClient.onEvent(socketOnEvent);
  String socketUrl = "wss://" + apiUrlBase + "/ws/ornaments/device/" + macId + "/";
  socketClient.connect(socketUrl);
}
