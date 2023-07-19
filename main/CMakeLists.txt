list(APPEND
   COMPONENT_SRCS
        main.cpp
        memfault-config/memfault_platform_port.c
        wifi.c
        led.c
        ota.c
)

set(COMPONENT_ADD_INCLUDEDIRS
   .
   memfault-config
   ${MEMFAULT_FIRMWARE_SDK}/ports/include
)

register_component()