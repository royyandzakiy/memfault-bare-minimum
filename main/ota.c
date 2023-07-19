#include "ota.h"

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

void memfault_ota() {
  prv_memfault_ota();
}