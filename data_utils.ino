String renderFile(String path, JsonObject data) {
  String content = readFile(path);
  if (content) {
    Serial.println("conf data?");
    Serial.println(data);
    Serial.println("is null?");
    Serial.println(data.isNull());
    for (JsonPair kv : data) {
      Serial.println(kv.key().c_str());
    }
  }
  return content;
}

bool fileExists(String path) {
  if (SPIFFS.exists(path)) {
    return true;
  }
  return false;
}

File getFile(String path) {
  Serial.println("[INFO] Getting file " + path);
  if (fileExists(path)) {
    File file  = SPIFFS.open(path, "r");
    return file;
  }
  Serial.println("[ERROR] File Not Found");
  File file;
  return file;
}

String readFile(String path) {
  File file = getFile(path);
  if (file) {
    Serial.println("[INFO] Reading file " + path);
    String data = file.readString();
    file.close();
    return data;
  }
  return "";
}

bool writeFile(String path, DynamicJsonDocument data) {
  Serial.println("[INFO] Writing file " + path);
  File file = SPIFFS.open("/config.json", "w");
  if (!file) {
    Serial.println("Error opening file for writing");
    return false;
  }
  int status = serializeJson(data, file);
  if (status == 0) {
     Serial.println("[ERROR] Failed to write to " + path);
  } else {
    Serial.print("[INFO] Configuration saved");
  }
  file.close();
}

// Start the file system
bool initializeFS() {
  if(SPIFFS.begin()) {
    return true;
  } else {
    return false;
  }
}
