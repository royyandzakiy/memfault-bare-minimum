list(APPEND
   COMPONENT_SRCS
        main.c
        memfault-config/memfault_platform_port.c
        cmd_wifi_legacy.c
        led.c
)

set(COMPONENT_ADD_INCLUDEDIRS
   .
   memfault-config
   ${MEMFAULT_FIRMWARE_SDK}/ports/include
)

register_component()