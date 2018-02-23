################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Applications/ModusToolbox\ 1.0/packs/Cypress/WICED_base/0.0.1/platforms/CY8CKIT_062/platform.c 

OBJS += \
./RTE/WICED_PLATFORM/platform.o 

C_DEPS += \
./RTE/WICED_PLATFORM/platform.d 


# Each subdirectory must supply rules for building sources it contributes
RTE/WICED_PLATFORM/platform.o: /Applications/ModusToolbox\ 1.0/packs/Cypress/WICED_base/0.0.1/platforms/CY8CKIT_062/platform.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall  -g3 -DWWD_DIRECT_RESOURCES -DWICED_VERSION='"Wiced_ModusToolbox_001.000.000"' -DWICED_DISABLE_MCU_POWERSAVE -DHSE_VALUE=26000000 -DBUS='"SDIO"' -DWWD_DOWNLOAD_CLM_BLOB -DBOOTLOADER_MAGIC_NUMBER=0x4d435242 -DSFLASH_APPS_HEADER_LOC=0x0000 -DWICED_DCT_INCLUDE_BT_CONFIG -Dwifi_firmware_image=resources_firmware_DIR_4343W_DIR_4343WA1_bin -Dwifi_firmware_clm_blob=resources_firmware_DIR_4343W_DIR_4343WA1_clm_blob -DWWD_STARTUP_DELAY=10 -DWWD_BUS_SDIO_AFTER_RESET_DELAY=1000 -DMAX_WATCHDOG_TIMEOUT_SECONDS=2.5 -DDEFAULT_SYSTEM_MONITOR_PERIOD=2500 -DPLATFORM='"CY8CKIT_062"' -DCY8C6247BZI_D54 -DAPPS_CHIP_REVISION='""' -DPLATFORM_POWER_DOWN_THREAD_PREEMPTION_ALLOWED -DWLAN_CHIP_FAMILY=4343W -DWLAN_CHIP=4343W -DCRLF_STDIO_REPLACEMENT -DNX_INCLUDE_USER_DEFINE_FILE -DNetX_VERSION='"vver5.5_sp1"' -DOPENSSL -DWICED_USE_BESL -DNETWORK_NetX=1 -DSYS_TIME_H_AVAILABLE -DNXD_EXTENDED_BSD_SOCKET_SUPPORT -DBUILDCFG -DADD_LWIP_EAPOL_SUPPORT -DThreadX_VERSION='"v5.6"' -D__fd_set_defined -DWICED_BLUETOOTH_PLATFORM -DAPP_NAME='"ble_hello_sensor"' -DADD_NETX_EAPOL_SUPPORT -DSTDC_HEADERS -DNX_FRAGMENT_IMMEDIATE_ASSEMBLY -DWICED_BTE_LIB -DRTOS_ThreadX=1 -DTX_INCLUDE_USER_DEFINE_FILE -D_RTE_ -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_mainapp" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_dct" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_resources" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/WAF" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/CapSense" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/cmsis/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/devices/psoc6/ip" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/devices/psoc6/psoc62/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/creator" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/crypto" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/ctb" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/ctdac" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/dma" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/efuse" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/flash" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/gpio" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/i2s" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/ipc" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/lpcomp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/lvd" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/mcwdt" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/pdm_pcm" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/profile" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/prot" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/rtc" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/sar" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/scb" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/sdio" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/smif" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/sysanalog" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/sysclk" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/sysint" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/syslib" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/syspm" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/systick" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/tcpwm" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/trigmux" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/libraries/drivers/peripheral/wdt" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/peripherals/spi_flash" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS/ThreadX/WICED" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS/ThreadX/WWD" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS/ThreadX/WWD/CM3_CM4" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS/ThreadX/ver5.6" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS/ThreadX/ver5.6/Cortex_M3_M4" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS/ThreadX/ver5.6/Cortex_M3_M4/GCC" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include/RTOS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include/network" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include/platform" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/internal" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/internal/bus_protocols" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/internal/bus_protocols/SDIO" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/internal/chips/4343x" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/daemons/DHCP_server" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/internal" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/DTLS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/P2P" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/TLS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/WPS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/WPS/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/common" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/common/proto" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/crypto_internal" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/host/WICED" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/mbedtls_open" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/mbedtls_open/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/mbedtls_open/include/mbedtls" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/mbedtls_open/library" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/BESL/supplicant" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/COAP/client" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/DNS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/HTTP" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/HTTP2" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/HTTP_client" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/JSON_parser" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/PostgreSQL/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/SMTP" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/SNTP" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/TLV" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/base64" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/bufmgr" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/button_manager" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/cJSON" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/audio" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/bt" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/bt_hci" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/dct" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/duktape" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/ethernet" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/fs" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/mallinfo" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/p2p" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/ping" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/platform" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/thread" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/trace" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/trace/buffered" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/trace/buffered/print" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/trace/clocktimer" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/trace/gpio" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/tracex" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/traffic_generation" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/wifi" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/command_console/wps" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/crc" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/gpio_button" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/linked_list" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/mini_printf" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/ota2" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/ring_buffer" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/wiced_log" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/wicedfs/src" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/libs/wifi" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/WICED" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/WWD" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/auto_ip" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/dhcp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/dns" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/ftp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/http" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/nat" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/pop3" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/ppp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/smtp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/snmp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/sntp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/telnet" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_applications/tftp" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_bsd_layer" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/network/NetX/ver5.5_sp1/netx_examples" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/ARM_CM4" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/ARM_CM4/CMSIS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/GCC" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/MIPS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platforms/CY8CKIT_062" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_platform/RTE" -std=gnu11 -MD -fno-common -isystem "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/include" -isystem "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/lib/include" -isystem "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/lib/include-fixed" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


