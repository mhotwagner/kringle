StaticJsonDocument<200> jsonData;

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

void initializeApi(String apiUrlBase) {
  Serial.println("[INFO] Initializing api");
  String macId = WiFi.macAddress();
  String apiUrl= "http://" + apiUrlBase + "/api/ornaments/" + macId + "/";
  Serial.println("[INFO] GET " + apiUrl);
  webClient.begin(apiUrl);
  int statusCode = webClient.GET();
  
  Serial.println("[INFO] status: " + String(statusCode));
  if (statusCode == 200) {
    Serial.println("[INFO] API successfully initialized");
    //successBlink(&ornament);
    ornament.success_blink();
  } else if (statusCode == 201) {
    Serial.println("[INFO] API successfully initialized and new ornament added");
    //infoBlink(&ornament);
    ornament.info_blink();
  } else {
    Serial.println("[ERROR] Failed to initialize API");
    //errorBlink(&ornament);
    ornament.error_blink();
    return;
  }

  socketClient.onMessage(socketOnMessage);
  socketClient.onEvent(socketOnEvent);
  String socketUrl = "http://" + apiUrlBase + "/ws/ornaments/device/" + macId + "/";
  socketClient.connect(socketUrl);
}
