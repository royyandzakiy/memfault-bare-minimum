# Memfault Bare Minimum

This project is meant to provide a basic memfault template to use and develop further. This is made by implementing all steps from the guide in the [memfault/espressif-esp32-guide](https://docs.memfault.com/docs/mcu/espressif-esp32-guide/), and taking some snippets from the template [esp32-standalone-example](https://github.com/memfault/esp32-standalone-example), into a fully functioning template with bare minimum steps to follow through, just to get coding quickly. If you would like further indepth and fine grained tuning, it is very well recommended to checkout the two references above and adjust accordingly.

## Features
- WiFi
- Memfault OTA
- Memfault Heartbeat Metric Post
- NVS

## Getting Started
1. Prepare the environment
    - install VSCode & ESP IDF Extension
    - setup ESP IDF Extension by downloading `v4.4.0`
    - download [memfault-firmware-sdk](https://github.com/memfault/memfault-firmware-sdk) `git clone https://github.com/memfault/memfault-firmware-sdk`, save the folder path for later

2. set the memfault sdk folder location in `CMakeLists.txt`
    - open `CMakeLists.txt` (in root folder, not in `main`), and change the line next to MEMFAULT_FIRMWARE_SDK with the absolute path of your `memfault-firmware-sdk` root folder 
        ```
        ...
        set(MEMFAULT_FIRMWARE_SDK C:/Users/koeip/Documents/Royyan/project-coding/iot/memfault-firmware-sdk) # comment this out if you are not Royyan!
        # set(MEMFAULT_FIRMWARE_SDK path/to/memfault-firmware-sdk) # insert your memfault-firmware-sdk path here!
        ...
        ```

3. get memfault Project Key
    - create memfault account, login into [app.memfault.com](https://app.memfault.com/)
    - navigate through Settings > Project Key
        
        ![](docs/get-project-key.png)

4. setup sdkconfig and insert Project Key
    - open terminal in root folder `idf.py menuconfig`, or you can do this step by opening the ESP IDF SDK Config Editor in VSCode
        1. navigate through `Component config` > `Memfault` > `Memfault project key` > `<memfault project key>` (insert Memfault Project Key gathered from the Memfault Cloud Dashboard)
        2. navigate through `Component config` > `Core dump` > `Data destination` > choose `Flash`
        3. navigate through `Partition table` > `Partition table` > choose `Custom Partition Table CSV` AND make sure the `Custom partition CSV file` is `partitions.csv` or change accordingly
        4. navigate through `WiFi Credentials Configuration` > fill in `WiFi SSID` AND `WiFi Password`

            <details>
            <summary>Setting through menuconfig</summary>
            <br/>
            <img src="docs/insert-project-key-menuconfig.png" width="700">
            <br/>
            <img src="docs/change-coredump-flash-menuconfig.png" width="700">
            <br/>
            <img src="docs/change-partition-menuconfig.png" width="700">
            </details>

            <details>
            <summary>Setting through SDK config editor</summary>
            <br/>
            <img src="docs/insert-project-key-configeditor.png" width="700">
            <br/>
            <img src="docs/change-coredump-flash-configeditor.png" width="700">
            <br/>
            <img src="docs/change-partition-configeditor.png" width="700">
            </details>            
    - build `idf.py fullclean build`

    <details>
    <summary>Example of output of a successfull build and flash</summary>
    <pre>
    --- Using \\.\COM7 instead...
    --- idf_monitor on \\.\COM7 115200 ---
    --- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
    ets Jun  8 2016 00:22:57

    rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
    configsip: 0, SPIWP:0xee
    clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
    mode:DIO, clock div:2
    load:0x3fff0030,len:6680
    load:0x40078000,len:14848
    load:0x40080400,len:3792
    0x40080400: _init at ??:?

    entry 0x40080694
    I (26) boot: ESP-IDF v4.4.2-1-gce1fde4b3b-dirty 2nd stage bootloader
    I (27) boot: compile time 21:18:10
    I (27) boot: chip revision: 1
    I (31) boot_comm: chip revision: 1, min. bootloader chip revision: 0
    I (38) boot.esp32: SPI Speed      : 40MHz
    I (43) boot.esp32: SPI Mode       : DIO
    I (47) boot.esp32: SPI Flash Size : 4MB
    I (52) boot: Enabling RNG early entropy source...
    I (57) boot: Partition Table:
    I (61) boot: ## Label            Usage          Type ST Offset   Length
    I (68) boot:  0 nvs              WiFi data        01 02 00009000 00004000
    I (76) boot:  1 otadata          OTA data         01 00 0000d000 00002000
    I (83) boot:  2 phy_init         RF data          01 01 0000f000 00001000
    I (90) boot:  3 factory          factory app      00 00 00010000 00100000
    I (98) boot:  4 storage          Unknown data     01 81 00110000 00084000
    I (105) boot:  5 coredump         Unknown data     01 03 00194000 00057800
    I (113) boot:  6 ota_0            OTA app          00 10 001f0000 00100000
    I (121) boot:  7 ota_1            OTA app          00 11 002f0000 00100000
    I (128) boot: End of partition table
    I (132) boot: Defaulting to factory image
    I (137) boot_comm: chip revision: 1, min. application chip revision: 0
    I (144) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=1e1bch (123324) map
    I (197) esp_image: segment 1: paddr=0002e1e4 vaddr=3ffb0000 size=01e34h (  7732) load
    I (201) esp_image: segment 2: paddr=00030020 vaddr=400d0020 size=94e20h (609824) map
    I (424) esp_image: segment 3: paddr=000c4e48 vaddr=3ffb1e34 size=01ee4h (  7908) load
    I (427) esp_image: segment 4: paddr=000c6d34 vaddr=40080000 size=14f14h ( 85780) load
    I (465) esp_image: segment 5: paddr=000dbc50 vaddr=50000000 size=00010h (    16) load
    I (475) boot: Loaded app from partition at offset 0x10000
    I (476) boot: Disabling RNG early entropy source...
    I (487) cpu_start: Pro cpu up.
    I (488) cpu_start: Starting app cpu, entry point is 0x4008123c
    0x4008123c: call_start_cpu1 at C:/Users/koeip/esp/esp-idf-v4.4/components/esp_system/port/cpu_start.c:160

    I (0) cpu_start: App cpu up.
    I (504) cpu_start: Pro cpu start user code
    I (504) cpu_start: cpu freq: 160000000
    I (504) cpu_start: Application information:
    I (508) cpu_start: Project name:     memfault-bare-minimum
    I (515) cpu_start: App version:      9b8580e
    I (519) cpu_start: Compile time:     Jul 19 2023 22:00:58
    I (526) cpu_start: ELF file SHA256:  5a282977ec623f0c...
    I (532) cpu_start: ESP-IDF:          v4.4.2-1-gce1fde4b3b-dirty
    I (538) heap_init: Initializing. RAM available for dynamic allocation:
    I (545) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
    I (551) heap_init: At 3FFB86B8 len 00027948 (158 KiB): DRAM
    I (558) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
    I (564) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
    I (570) heap_init: At 40094F14 len 0000B0EC (44 KiB): IRAM
    I (578) spi_flash: detected chip: generic
    I (581) spi_flash: flash io: dio
    I (587) mflt: Coredumps will be saved to 'coredump' partition
    I (592) mflt: Using entry 0x3ffba588 pointing to address 0x00194000
    I (600) cpu_start: Starting scheduler on PRO CPU.
    I (0) cpu_start: Starting scheduler on APP CPU.
    I (612) mflt: Memfault Build ID: 6e4086e9aa4cebca56da5058f653c367cd0daed4
    I (622) system_api: Base MAC address is not set
    I (622) system_api: read default base MAC address from EFUSE
    I (632) mflt: S/N: C8C9A3C7CEA0
    I (632) mflt: SW type: esp32-main
    I (642) mflt: SW version: 1.0.0-dev
    I (642) mflt: HW version: esp32-proto
    I (692) mflt: Data poster task up and running every 60s.
    I (712) wifi_init: rx ba win: 6
    I (712) wifi_init: tcpip mbox: 32
    I (722) wifi_init: udp mbox: 6
    I (722) wifi_init: tcp mbox: 6
    I (722) wifi_init: tcp tx win: 5744
    I (722) wifi_init: tcp rx win: 5744
    I (732) wifi_init: tcp mss: 1440
    I (732) wifi_init: WiFi IRAM OP enabled
    I (742) wifi_init: WiFi RX IRAM OP enabled
    I (742) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
    W (2252) wifi:<ba-add>idx:0 (ifx:0, a4:91:b1:ed:32:bf), tid:0, ssn:0, winSize:64
    I (3192) esp_netif_handlers: sta ip: 192.168.1.187, mask: 255.255.255.0, gw: 192.168.1.1
    I (5872) mflt: Result: 0
    I (5882) mflt: Checking for OTA Update
    I (8122) HTTP_CLIENT: Body received in fetch header state, 0x3ffd4e53, 173
    I (8132) mflt: Starting OTA download ...
    I (10412) esp_https_ota: Starting OTA...
    I (10422) esp_https_ota: Writing to partition subtype 16 at offset 0x1f0000
    I (59682) esp_image: segment 0: paddr=001f0020 vaddr=3f400020 size=224c8h (140488) map
    I (59732) esp_image: segment 1: paddr=002124f0 vaddr=3ffb0000 size=03da8h ( 15784) 
    I (59742) esp_image: segment 2: paddr=002162a0 vaddr=40080000 size=09d78h ( 40312)
    I (59762) esp_image: segment 3: paddr=00220020 vaddr=400d0020 size=a8464h (689252) map
    I (59982) esp_image: segment 4: paddr=002c848c vaddr=40089d78 size=0cddch ( 52700) 
    I (60002) esp_image: segment 5: paddr=002d5270 vaddr=400c0000 size=00064h (   100)
    I (60002) esp_image: segment 6: paddr=002d52dc vaddr=50000000 size=00010h (    16) 
    I (60022) esp_image: segment 0: paddr=001f0020 vaddr=3f400020 size=224c8h (140488) map
    I (60072) esp_image: segment 1: paddr=002124f0 vaddr=3ffb0000 size=03da8h ( 15784) 
    I (60082) esp_image: segment 2: paddr=002162a0 vaddr=40080000 size=09d78h ( 40312)
    I (60092) esp_image: segment 3: paddr=00220020 vaddr=400d0020 size=a8464h (689252) map
    I (60322) esp_image: segment 4: paddr=002c848c vaddr=40089d78 size=0cddch ( 52700) 
    I (60332) esp_image: segment 5: paddr=002d5270 vaddr=400c0000 size=00064h (   100) 
    I (60342) esp_image: segment 6: paddr=002d52dc vaddr=50000000 size=00010h (    16) 
    I (60402) mflt: OTA Update Complete, Rebooting System
    W (60402) wifi:<ba-del>idx
    W (60402) wifi:hmac tx: ifx0 stop, discard
    ets Jun  8 2016 00:22:57

    rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
    configsip: 0, SPIWP:0xee
    clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
    mode:DIO, clock div:2
    load:0x3fff0030,len:6680
    load:0x40078000,len:14848
    load:0x40080400,len:3792
    0x40080400: _init at ??:?

    entry 0x40080694
    I (27) boot: ESP-IDF v4.4.2-1-gce1fde4b3b-dirty 2nd stage bootloader
    I (27) boot: compile time 21:18:10
    I (27) boot: chip revision: 1
    ...
    </pre>
    </details>  

- enjoy!

## To Do
- change into .cpp
- turn into a git submodule