#pragma once

#include <string.h>

#include "global.h"
#include "esp_task.h"

#include "memfault/components.h"
#include "memfault/esp_port/core.h"
#include "memfault/esp_port/http_client.h"
#include "led.h"
#include "nvs.h"
#include "nvs_flash.h"

void memfault_ota();