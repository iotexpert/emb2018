################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../4343WA1_bin.c \
../4343WA1_clm_blob.c \
../aws_config_html.c \
../client_cer.c \
../privkey_cer.c \
../rootca_cer.c 

OBJS += \
./4343WA1_bin.o \
./4343WA1_clm_blob.o \
./aws_config_html.o \
./client_cer.o \
./privkey_cer.o \
./rootca_cer.o 

C_DEPS += \
./4343WA1_bin.d \
./4343WA1_clm_blob.d \
./aws_config_html.d \
./client_cer.d \
./privkey_cer.d \
./rootca_cer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall  -g3 -DWWD_DIRECT_RESOURCES -DWICED_VERSION='"Wiced_ModusToolbox_001.000.000"' -DWICED_DISABLE_MCU_POWERSAVE -DHSE_VALUE=26000000 -DBUS='"SDIO"' -DWWD_DOWNLOAD_CLM_BLOB -DBOOTLOADER_MAGIC_NUMBER=0x4d435242 -DSFLASH_APPS_HEADER_LOC=0x0000 -DWICED_DCT_INCLUDE_BT_CONFIG -Dwifi_firmware_image=resources_firmware_DIR_4343W_DIR_4343WA1_bin -Dwifi_firmware_clm_blob=resources_firmware_DIR_4343W_DIR_4343WA1_clm_blob -DWWD_STARTUP_DELAY=10 -DWWD_BUS_SDIO_AFTER_RESET_DELAY=1000 -DMAX_WATCHDOG_TIMEOUT_SECONDS=2.5 -DDEFAULT_SYSTEM_MONITOR_PERIOD=2500 -DPLATFORM='"CY8CKIT_062"' -DCY8C6247BZI_D54 -DAPPS_CHIP_REVISION='""' -DPLATFORM_POWER_DOWN_THREAD_PREEMPTION_ALLOWED -DWLAN_CHIP_FAMILY=4343W -DWLAN_CHIP=4343W -DCRLF_STDIO_REPLACEMENT -DNX_INCLUDE_USER_DEFINE_FILE -DNetX_VERSION='"vver5.5_sp1"' -DOPENSSL -DWICED_USE_BESL -DNETWORK_NetX=1 -DSYS_TIME_H_AVAILABLE -DNXD_EXTENDED_BSD_SOCKET_SUPPORT -DBUILDCFG -DADD_LWIP_EAPOL_SUPPORT -DThreadX_VERSION='"v5.6"' -D__fd_set_defined -DWICED_BLUETOOTH_PLATFORM -DAPP_NAME='"ble_hello_sensor"' -DADD_NETX_EAPOL_SUPPORT -DSTDC_HEADERS -DNX_FRAGMENT_IMMEDIATE_ASSEMBLY -DWICED_BTE_LIB -DRTOS_ThreadX=1 -DTX_INCLUDE_USER_DEFINE_FILE -D_RTE_ -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_mainapp" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_dct" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_resources" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/RTOS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include/RTOS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/include/platform" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/internal" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/WWD/internal/bus_protocols" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/include" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/internal" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/ARM_CM4" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/ARM_CM4/CMSIS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/GCC" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/MIPS" -I"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/platform/include" -I"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_resources/RTE" -std=gnu11 -MD -fno-common -isystem "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/include" -isystem "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/lib/include" -isystem "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/lib/include-fixed" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


