################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RTE/WICED_WAF_APPS/subdir.mk
-include RTE/WICED_PLATFORM/subdir.mk
-include RTE/WICED_MCU/subdir.mk
-include RTE/WICED_LIB_BASE/subdir.mk
-include RTE/WICED_DRIVER_BASE/subdir.mk
-include RTE/WICED_BASE/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BLEPublisher_bootloader.elf secondary-outputs

# Tool invocations
BLEPublisher_bootloader.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall  -g3 -T "/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/GCC/bootloader.ld" -nostartfiles -Xlinker --gc-sections -L"/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/lib" -L"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6" -L"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/GCC/cy8c6x7" -L"/Users/arh/Documents/iotexpert-projects/emb2018/ModusToolbox/BLEPublisher_resources/Debug" -Wl,-Map,"BLEPublisher_bootloader.map" -Wl,-A,thumb -Wl,-z,max-page-size=0x10 -Wl,-z,common-page-size=0x10 -Wl,--defsym,__STACKSIZE__=800 -Wl,--defsym,FIQ_STACK_SIZE=0 -Wl,--defsym,IRQ_STACK_SIZE=256 -Wl,--defsym,SYS_STACK_SIZE=0 -o "BLEPublisher_bootloader.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) BLEPublisher_bootloader.elf
	-@echo ' '

post-build:
	-"/Applications/ModusToolbox 1.0/tools/modus-shell-1.0/bin/bash" "--norc" "/Applications/ModusToolbox 1.0/tools/wiced-scripts-1.0/process_app_trx.bash" "/Applications/ModusToolbox 1.0/tools/modus-shell-1.0" "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/bin" "/Applications/ModusToolbox 1.0/tools/wiced-scripts-1.0" . "BLEPublisher_bootloader.elf"
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents post-build

-include ../makefile.targets
