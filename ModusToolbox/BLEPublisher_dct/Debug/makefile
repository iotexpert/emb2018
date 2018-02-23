################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RTE/WICED_WAF_APPS/subdir.mk
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
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: BLEPublisher_dct.elf secondary-outputs

# Tool invocations
BLEPublisher_dct.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall  -g3 -T "/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/GCC/cy8c6x7/dct.ld" -Xlinker --gc-sections -L"/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/lib" -L"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6" -L"/Applications/ModusToolbox 1.0/packs/Cypress/WICED_base/0.0.1/MCU/PSoC6/GCC/cy8c6x7" -Wl,-Map,"BLEPublisher_dct.map" -Wl,-A,thumb -Wl,-z,max-page-size=0x10 -Wl,-z,common-page-size=0x10 -Wl,--defsym,START_STACK_SIZE=4000 -Wl,--defsym,FIQ_STACK_SIZE=0 -Wl,--defsym,IRQ_STACK_SIZE=1024 -Wl,--defsym,SYS_STACK_SIZE=0 -o "BLEPublisher_dct.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) BLEPublisher_dct.elf
	-@echo ' '

pre-build:
	-"/Applications/ModusToolbox 1.0/tools/modus-shell-1.0/bin/perl" "/Applications/ModusToolbox 1.0/tools/wiced-tools-1.0/scripting/mac_generator/mac_generator.pl"
	-@echo ' '

post-build:
	-"/Applications/ModusToolbox 1.0/tools/modus-shell-1.0/bin/bash" "--norc" "/Applications/ModusToolbox 1.0/tools/wiced-scripts-1.0/process_elf.bash" "/Applications/ModusToolbox 1.0/tools/gcc-4.9.3-1.0/bin" . "BLEPublisher_dct.elf"
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents main-build pre-build post-build

-include ../makefile.targets
