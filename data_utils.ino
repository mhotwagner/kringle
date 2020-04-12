class Config {
  public:
    const char* wifi_ssid;
    const char* wifi_password;

    void load() {
      Serial.println("[INFO] Reading config");
      String rawConfig = readFile("/config.json");
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, rawConfig);

      wifi_ssid = doc["wifi_ssid"];
      wifi_password = doc["wifi_password"];
      
      Serial.print("[INFO] wifi_ssid - ");
      Serial.println(wifi_ssid);
      Serial.print("[INFO] wifi_password - ");
      Serial.println(wifi_password);
    }
} _config;



bool isWifiConfigured() {
  if (_config.wifi_ssid=="") {
    return false;
  }
  return true;
}

const char* get_wifi_ssid() {
  return _config.wifi_ssid;
}

const char* get_wifi_pass() {
  return _config.wifi_password;
}


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
  Serial.print("[INFO] Initalizing SPIFFS filesystem...");
  if(SPIFFS.begin()) {
    Serial.println("ok");
    _config.load();
  } else {
    Serial.println("ERROR");
  }
}
