#pragma once

extern void memfault_platform_device_info_boot(void);

static void initialize_nvs() {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
}

extern void memfault_init() {
   memfault_boot();
   memfault_platform_device_info_boot();
   memfault_device_info_dump();
}
