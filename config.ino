Config::Config() {
    logger.log("[INFO] Loading config");
    String rawConfig = readFile("/config.json");
    
    DynamicJsonDocument _doc(1024);
    deserializeJson(_doc, rawConfig);
    data = _doc.as<JsonObject>();
  
    boot_to_config = _doc["boot_to_config"].as<bool>();
    wifi_ssid = _doc["wifi_ssid"].as<const char*>();
    wifi_password = _doc["wifi_password"].as<const char*>();
    api_host = _doc["api_host"].as<const char*>();
    
    logger.log("boot_to_config: " + boot_to_config);
    logger.log("wifi_ssid: " + String(wifi_ssid));
    logger.log("wifi_password: "  + String(wifi_password));
    logger.log("api_host: " + String(api_host));
}

bool Config::wifiConfigured() {
  if (!wifi_ssid) {
    return false;
  }
  if (strlen(wifi_ssid)) {
    return true;
  }
  return false;
}

bool Config::apiConfigured() {
  if (!api_host) {
    return false;
  }
  if (strlen(api_host)) {
    return true;
  }
  return false;
}

 String Config::renderTemplate(String _template) {
  for (JsonPair kv : data) {
    logger.log(kv.key().c_str());
  }
  return _template;
}