<<<<<<< HEAD
=======
// wifi info
const char* wifi_ssid = "Hotswag Manor";
const char* wifi_pass = "xoxo<3!!";

>>>>>>> A working ornament with a socket connection
// Start the wifi
void initializeWifi(const char* wifi_ssid, const char* wifi_pass) {
  Serial.print("[INFO] Connecting to ");
  Serial.print(wifi_ssid);
  Serial.println("...");
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("[INFO] Connected");
  Serial.print("[INFO] IP: ");
  Serial.println(WiFi.localIP());
}
