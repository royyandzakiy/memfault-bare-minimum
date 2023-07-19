# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(MEMFAULT_FIRMWARE_SDK C:/Users/koeip/Documents/Royyan/project-coding/iot/memfault-firmware-sdk) # comment this out if you are not Royyan!
# set(MEMFAULT_FIRMWARE_SDK path/to/memfault-firmware-sdk) # insert your memfault-firmware-sdk path here!
include(${MEMFAULT_FIRMWARE_SDK}/ports/esp_idf/memfault.cmake)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(memfault-bare-minimum)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}.elf
  POST_BUILD
  COMMAND
  python ${MEMFAULT_FIRMWARE_SDK}/scripts/fw_build_id.py ${CMAKE_PROJECT_NAME}.elf)