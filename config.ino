Config::Config() {
    Serial.println("[INFO] Loading config");
    String rawConfig = readFile("/config.json");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, rawConfig);

    boot_to_config = doc["boot_to_config"].as<bool>();
    wifi_ssid = doc["wifi_ssid"].as<const char*>();
    wifi_password = doc["wifi_password"].as<const char*>();
    api_host = doc["api_host"].as<const char*>();
    
    Serial.print("[INFO] wifi_ssid - ");
    Serial.println(wifi_ssid);
    Serial.print("[INFO] wifi_password - ");
    Serial.println(wifi_password);
    Serial.print("[INFO] api_host - ");
    Serial.println(api_host);
}

bool Config::wifiConfigured() {
  if (strlen(wifi_ssid)) {
    return true;
  }
  return false;
}

bool Config::apiConfigured() {
  if (strlen(api_host)) {
    return true;
  }
  return false;
}