#pragma once

#include "esp_task.h"

#include "memfault/components.h"
#include "memfault/esp_port/http_client.h"
#include "led.h"

#ifdef __cplusplus
extern "C" {
#endif

void memfault_ota();

#ifdef __cplusplus
}
#endif