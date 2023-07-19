#include "memfault/esp_port/core.h"
#include "cmd_decl.h"
#include "nvs.h"
#include "nvs_flash.h"

// #include <inttypes.h>
// #include <stdio.h>
// #include <string.h>

// #include "argtable3/argtable3.h"
// #include "cmd_decl.h"
// #include "driver/uart.h"
// #include "esp_console.h"
// #include "esp_log.h"
// #include "esp_system.h"
// #include "esp_task.h"
// #include "esp_vfs_dev.h"
// #include "esp_vfs_fat.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/semphr.h"
// #include "freertos/timers.h"
// #include "led.h"
// #include "linenoise/linenoise.h"
// #include "memfault/components.h"
// #include "memfault/esp_port/cli.h"
// #include "memfault/esp_port/core.h"
// #include "memfault/esp_port/http_client.h"
// #include "memfault/esp_port/version.h"
// #include "nvs.h"
// #include "nvs_flash.h"

static void initialize_nvs() {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
}

void app_main() {
  memfault_boot();
  initialize_nvs();

  connect_direct("TNCAPED32BF", "PXakKXP6YfHTfhEA");
}