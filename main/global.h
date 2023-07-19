/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Attempt to join a wifi network
void wifi_creds_nvs(char * ssid, char * pass);
bool wifi_join(const char* ssid, const char* pass);
void wifi_load_creds(char** ssid, char** password);
void memfault_esp_port_wifi_autojoin();

extern void memfault_platform_device_info_boot(void);