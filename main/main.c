#include <string.h>

#include "cmd_decl.h"
#include "esp_task.h"

#include "memfault/components.h"
#include "memfault/esp_port/core.h"
#include "memfault/esp_port/http_client.h"
#include "led.h"
#include "nvs.h"
#include "nvs_flash.h"

static bool prv_handle_ota_upload_available(void *user_ctx) {
  // set blue when performing update
  led_set_color(kLedColor_Blue);

  MEMFAULT_LOG_INFO("Starting OTA download ...");
  return true;
}

static bool prv_handle_ota_download_complete(void *user_ctx) {
  MEMFAULT_LOG_INFO("OTA Update Complete, Rebooting System");

  // The pc & lr which result in the reboot can always be *optionally* recorded
  void *pc;
  MEMFAULT_GET_PC(pc);
  void *lr;
  MEMFAULT_GET_LR(lr);
  sMfltRebootTrackingRegInfo reg_info = {
    .pc = (uint32_t)pc,
    .lr = (uint32_t)lr,
  };
  // Note: "reg_info" may be NULL if no register information collection is desired
  memfault_reboot_tracking_mark_reset_imminent(kMfltRebootReason_FirmwareUpdate, &reg_info);

  esp_restart();
  return true;
}

static void prv_memfault_ota(void) {
  if (!memfault_esp_port_wifi_connected()) {
    return;
  }

  sMemfaultOtaUpdateHandler handler = {
    .user_ctx = NULL,
    .handle_update_available = prv_handle_ota_upload_available,
    .handle_download_complete = prv_handle_ota_download_complete,
  };

  MEMFAULT_LOG_INFO("Checking for OTA Update");

  int rv = memfault_esp_port_ota_update(&handler);
  if (rv == 0) {
    MEMFAULT_LOG_INFO("Up to date!");
    led_set_color(kLedColor_Green);
  } else if (rv == 1) {
    MEMFAULT_LOG_INFO("Update available!");
  } else if (rv < 0) {
    MEMFAULT_LOG_ERROR("OTA update failed, rv=%d", rv);
    led_set_color(kLedColor_Red);
  }
}

void memfault_esp_port_wifi_autojoin(void) {
  if (memfault_esp_port_wifi_connected()) {
    return;
  }

  char *ssid, *pass;
  wifi_load_creds(&ssid, &pass);
  if ((ssid == NULL) || (pass == NULL) || (strnlen(ssid, 64) == 0) || (strnlen(pass, 64) == 0)) {
    MEMFAULT_LOG_DEBUG("No WiFi credentials found");
    return;
  }
  MEMFAULT_LOG_DEBUG("Starting WiFi Autojoin ...");
  bool result = wifi_join(ssid, pass);
  if (!result) {
    MEMFAULT_LOG_DEBUG("Failed to join WiFi network");
  }
}

// Periodically post any Memfault data that has not yet been posted.
static void prv_poster_task(void *args) {
  const uint32_t interval_sec = 60;
  const TickType_t delay_ms = (1000 * interval_sec) / portTICK_PERIOD_MS;

  MEMFAULT_LOG_INFO("Data poster task up and running every %" PRIu32 "s.", interval_sec);

  while (true) {
    // count the number of times this task has run
    // memfault_metrics_heartbeat_add(MEMFAULT_METRICS_KEY(PosterTaskNumSchedules), 1);
    // attempt to autojoin wifi, if configured
    memfault_esp_port_wifi_autojoin();

    // if connected, post any memfault data
    if (memfault_esp_port_wifi_connected()) {
      MEMFAULT_LOG_DEBUG("Checking for memfault data to send");
      int err = memfault_esp_port_http_client_post_data();
      // if the check-in succeeded, set green, otherwise clear.
      // gives a quick eyeball check that the app is alive and well
      led_set_color((err == 0) ? kLedColor_Green : kLedColor_Red);
    }

    // check for OTA update
    prv_memfault_ota();

    // sleep
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
  extern void memfault_platform_device_info_boot(void);
  memfault_platform_device_info_boot();
  memfault_device_info_dump();
  
  initialize_nvs();

  const portBASE_TYPE res =
    xTaskCreate(prv_poster_task, "poster", ESP_TASK_MAIN_STACK, NULL, ESP_TASK_MAIN_PRIO, NULL);
  MEMFAULT_ASSERT(res == pdTRUE);

  connect_direct(CONFIG_WIFI_SSID, CONFIG_WIFI_PASS);
}