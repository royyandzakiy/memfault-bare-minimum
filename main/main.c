#include <string.h>

#include "global.h"
#include "esp_task.h"

#include "memfault/components.h"
#include "memfault/esp_port/core.h"
#include "led.h"
#include "nvs.h"
#include "ota.h"
#include "nvs_flash.h"

// Periodically post any Memfault data that has not yet been posted.
static void post_data_task(void *args) {
  const uint32_t interval_sec = 60;
  const TickType_t delay_ms = (1000 * interval_sec) / portTICK_PERIOD_MS;

  MEMFAULT_LOG_INFO("Data poster task up and running every %" PRIu32 "s.", interval_sec);

  while (true) {
    memfault_metrics_heartbeat_add(MEMFAULT_METRICS_KEY(PosterTaskNumSchedules), 1); // count the number of times this task has run

    memfault_esp_port_wifi_autojoin();

    if (memfault_esp_port_wifi_connected()) {
      MEMFAULT_LOG_DEBUG("Checking for memfault data to send");
      int err = memfault_esp_port_http_client_post_data();
      led_set_color((err == 0) ? kLedColor_Green : kLedColor_Red);
    }

    memfault_ota();
    vTaskDelay(delay_ms);
  }
}

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
  memfault_platform_device_info_boot();
  memfault_device_info_dump();
  
  initialize_nvs();
  wifi_creds_nvs(CONFIG_WIFI_SSID, CONFIG_WIFI_PASS);

  const portBASE_TYPE res =
    xTaskCreate(post_data_task, "poster", ESP_TASK_MAIN_STACK, NULL, ESP_TASK_MAIN_PRIO, NULL);
  MEMFAULT_ASSERT(res == pdTRUE);
}