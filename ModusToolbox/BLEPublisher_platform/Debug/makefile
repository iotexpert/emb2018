################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RTE/WICED_PLATFORM/subdir.mk
-include RTE/WICED_MCU/subdir.mk
-include RTE/WICED_LIB_SECURITY/subdir.mk
-include RTE/WICED_LIB_BASE/subdir.mk
-include RTE/WICED_DRIVER_BASE/subdir.mk
-include RTE/WICED_DAEMON_BASE/subdir.mk
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
all: libBLEPublisher_platform.a

# Tool invocations
libBLEPublisher_platform.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross Archiver'
	arm-none-eabi-ar -r  "libBLEPublisher_platform.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) libBLEPublisher_platform.a
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
