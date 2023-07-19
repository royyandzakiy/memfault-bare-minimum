//! @file
//!
//! Copyright 2023 Memfault, Inc
//!
//! Licensed under the Apache License, Version 2.0 (the "License");
//! you may not use this file except in compliance with the License.
//! You may obtain a copy of the License at
//!
//!     http://www.apache.org/licenses/LICENSE-2.0
//!
//! Unless required by applicable law or agreed to in writing, software
//! distributed under the License is distributed on an "AS IS" BASIS,
//! WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//! See the License for the specific language governing permissions and
//! limitations under the License.
//!
//! Glue layer between the Memfault SDK and the underlying platform
//!
//! TODO: Fill in FIXMEs below for your platform

#include "memfault/components.h"
#include "memfault/esp_port/version.h"
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 0)
  #include "esp_mac.h"
#endif
// #include <string.h>

// #include "esp_system.h"

// #include <stdbool.h>

#ifndef MEMFAULT_ESP32_SOFTWARE_TYPE
  #define MEMFAULT_ESP32_SOFTWARE_TYPE "esp32-main"
#endif

#ifndef MEMFAULT_ESP32_HW_REVISION
  #define MEMFAULT_ESP32_HW_REVISION CONFIG_IDF_TARGET "-proto"
#endif

static char s_device_serial[32];

// NOTE: Some versions of the esp-idf use locking when reading mac info
// so this isn't safe to call from an interrupt
static void prv_get_device_serial(char *buf, size_t buf_len) {
  // Use the ESP32's MAC address as unique device id:
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);

  size_t curr_idx = 0;
  for (size_t i = 0; i < sizeof(mac); i++) {
    size_t space_left = buf_len - curr_idx;
    int bytes_written = snprintf(&buf[curr_idx], space_left, "%02X", (int)mac[i]);
    if (bytes_written < space_left) {
      curr_idx += bytes_written;
    } else {  // we are out of space, return what we got, it's NULL terminated
      return;
    }
  }
}

void memfault_platform_device_info_boot(void) {
  prv_get_device_serial(s_device_serial, sizeof(s_device_serial));
}

void memfault_platform_get_device_info(struct MemfaultDeviceInfo *info) {
  *info = (struct MemfaultDeviceInfo){
    .device_serial = s_device_serial,
    .hardware_version = MEMFAULT_ESP32_HW_REVISION,
    .software_version = CONFIG_MEMFAULT_ESP32_MAIN_FIRMWARE_VERSION,
    .software_type = MEMFAULT_ESP32_SOFTWARE_TYPE,
  };
}

//! Last function called after a coredump is saved. Should perform
//! any final cleanup and then reset the device
void memfault_platform_reboot(void) {
  // !FIXME: Perform any final system cleanup here

  // !FIXME: Reset System
  // NVIC_SystemReset()
  while (1) { } // unreachable
}

bool memfault_platform_time_get_current(sMemfaultCurrentTime *time) {
  // !FIXME: If the device tracks real time, update 'unix_timestamp_secs' with seconds since epoch
  // This will cause events logged by the SDK to be timestamped on the device rather than when they
  // arrive on the server
  *time = (sMemfaultCurrentTime) {
    .type = kMemfaultCurrentTimeType_UnixEpochTimeSec,
    .info = {
      .unix_timestamp_secs = 0
    },
  };

  // !FIXME: If device does not track time, return false, else return true if time is valid
  return false;
}

size_t memfault_platform_sanitize_address_range(void *start_addr, size_t desired_size) {
  static const struct {
    uint32_t start_addr;
    size_t length;
  } s_mcu_mem_regions[] = {
    // !FIXME: Update with list of valid memory banks to collect in a coredump
    {.start_addr = 0x00000000, .length = 0xFFFFFFFF},
  };

  for (size_t i = 0; i < MEMFAULT_ARRAY_SIZE(s_mcu_mem_regions); i++) {
    const uint32_t lower_addr = s_mcu_mem_regions[i].start_addr;
    const uint32_t upper_addr = lower_addr + s_mcu_mem_regions[i].length;
    if ((uint32_t)start_addr >= lower_addr && ((uint32_t)start_addr < upper_addr)) {
      return MEMFAULT_MIN(desired_size, upper_addr - (uint32_t)start_addr);
    }
  }

  return 0;
}

//! !FIXME: This function _must_ be called by your main() routine prior
//! to starting an RTOS or baremetal loop.
int memfault_platform_boot(void) {
  // !FIXME: Add init to any platform specific ports here.
  // (This will be done in later steps in the getting started Guide)

  memfault_build_info_dump();
  memfault_device_info_dump();
  memfault_platform_reboot_tracking_boot();

  // initialize the event storage buffer
  static uint8_t s_event_storage[1024];
  const sMemfaultEventStorageImpl *evt_storage =
    memfault_events_storage_boot(s_event_storage, sizeof(s_event_storage));

  // configure trace events to store into the buffer
  memfault_trace_event_boot(evt_storage);

  // record the current reboot reason
  memfault_reboot_tracking_collect_reset_info(evt_storage);

  // configure the metrics component to store into the buffer
  sMemfaultMetricBootInfo boot_info = {
    .unexpected_reboot_count = memfault_reboot_tracking_get_crash_count(),
  };
  memfault_metrics_boot(evt_storage, &boot_info);

  MEMFAULT_LOG_INFO("Memfault Initialized!");

  return 0;
}
