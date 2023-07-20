#include <cstring>
#include "esp_task.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "freertos/semphr.h"

#include "memfault/components.h"
#include "memfault/esp_port/core.h"

#include "misc.h"
#include "wifi.h"
#include "led.h"
#include "ota.h"

static void post_data_task(void *args) {
  const uint32_t interval_sec = 60;
  const TickType_t delay_ms = (1000 * interval_sec) / portTICK_PERIOD_MS;

  MEMFAULT_LOG_INFO("Data poster task up and running every %u s.", interval_sec);

  while (true) {
    memfault_metrics_heartbeat_add(MEMFAULT_METRICS_KEY(PosterTaskNumSchedules), 1);

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

extern "C" void app_main() {
  memfault_init();
  initialize_nvs();
  led_init();

  wifi_creds_nvs(CONFIG_WIFI_SSID, CONFIG_WIFI_PASS);

  const portBASE_TYPE res =
    xTaskCreate((TaskFunction_t)post_data_task, "poster", ESP_TASK_MAIN_STACK, NULL, ESP_TASK_MAIN_PRIO, NULL);
  MEMFAULT_ASSERT(res == pdTRUE);
}