void initializeWifiClient(String wifi_ssid, String wifi_pass) {
  logger.log(wifi_ssid);
  logger.log("[INFO] Initializing wifi client");
  logger.log("[INFO] Connecting to " + wifi_ssid);
  logger.log("...");
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    logger.log(".");
  }
  logger.log("[INFO] Connected");
  logger.log("[INFO] IP: ");
  logger.log(WiFi.localIP());
}
