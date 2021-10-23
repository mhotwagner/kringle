bool initializeWifiClient(String wifi_ssid, String wifi_pass) {
  logger.log(wifi_ssid);
  logger.log("[INFO] Initializing wifi client");
  logger.log("[INFO] Connecting to " + wifi_ssid);
  logger.log("...");
  WiFi.begin(wifi_ssid, wifi_pass);
  int n = 0;
  while (WiFi.status() != WL_CONNECTED && n < 120) {
    delay(500);
    logger.log(".");
    n++;
    if (n % 10 == 0) {
      ornament.info_blink(1);
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  logger.log("[INFO] Connected");
  logger.log("[INFO] IP: ");
  logger.log(WiFi.localIP());
  return true;
}
