#ifndef Config_h
#define Config_h

class Config {
  public:
  	Config();

  	bool wifiConfigured();
  	bool apiConfigured();

    bool boot_to_config;
    const char* wifi_ssid;
    const char* wifi_password;
    const char* api_host;
};
#endif
