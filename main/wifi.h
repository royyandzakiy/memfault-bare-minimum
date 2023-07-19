#pragma once


// Attempt to join a wifi network
extern void wifi_creds_nvs(char * ssid, char * pass);
extern bool wifi_join(const char* ssid, const char* pass);
extern void wifi_load_creds(char** ssid, char** password);
extern void memfault_esp_port_wifi_autojoin();