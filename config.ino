Config::Config() {
    Serial.println("[INFO] Loading config");
    String rawConfig = readFile("/config.json");
    
    DynamicJsonDocument _doc(1024);
    deserializeJson(_doc, rawConfig);
    data = _doc.as<JsonObject>();
  
    boot_to_config = _doc["boot_to_config"].as<bool>();
    Serial.print("boot_to_config: ");
    Serial.println(boot_to_config);
    wifi_ssid = _doc["wifi_ssid"].as<const char*>();
    Serial.print("wifi_ssid: ");
    Serial.println(wifi_ssid);
    wifi_password = _doc["wifi_password"].as<const char*>();
    Serial.print("wifi_password: ");
    Serial.println(wifi_password);
    api_host = _doc["api_host"].as<const char*>();
    Serial.print("api_host: ");
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

 String Config::renderTemplate(String _template) {
  for (JsonPair kv : data) {
    Serial.println(kv.key().c_str());
  }
  return _template;
}