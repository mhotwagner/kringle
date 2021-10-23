bool initializeWifiClient(String wifi_ssid, String wifi_pass) {
  logger.log("[INFO] Initializing wifi client, connecting to " + String(wifi_ssid));
  WiFi.begin(wifi_ssid, wifi_pass);
  int n = 0;
  while (WiFi.status() != WL_CONNECTED && n < 60) {
    delay(1000);
    n++;
    if (n % 10 == 0) {
      ornament.info_blink(2);
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  logger.log("[INFO] Connected at " + WiFi.localIP().toString());
  return true;
}
