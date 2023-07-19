list(APPEND
   COMPONENT_SRCS
        main.cpp
        memfault/memfault_platform_port.c
)

set(COMPONENT_ADD_INCLUDEDIRS
   .
   memfault
)

register_component()