#include "memfault/esp_port/core.h"

extern "C" void app_main() {
    memfault_boot();
}