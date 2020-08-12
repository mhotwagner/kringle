#ifndef Config_h
#define Config_h

#include <ArduinoJson.h>
#include <Arduino.h>

class Config {
  public:
  	Config();

  	bool wifiConfigured();
  	bool apiConfigured();

  	String renderTemplate(String _template);

  	JsonObject data;

    bool boot_to_config;
    const char* wifi_ssid;
    const char* wifi_password;
    const char* api_host;
};
#endif
