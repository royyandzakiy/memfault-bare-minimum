#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Attempt to join a wifi network
void wifi_creds_nvs(char * ssid, char * pass);
bool wifi_join(const char* ssid, const char* pass);
void wifi_load_creds(char** ssid, char** password);
void memfault_esp_port_wifi_autojoin();