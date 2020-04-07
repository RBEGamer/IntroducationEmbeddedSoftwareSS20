################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EMS_P1.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EMS_P1.list \

OBJCOPY_BIN += \
EMS_P1.bin \


# All Target
all: EMS_P1.elf secondary-outputs

# Tool invocations
EMS_P1.elf: $(OBJS) $(USER_OBJS) /home/ros/Desktop/EmbeddedSoftwareSS20/src/P1/STM32F767ZITX_FLASH.ld
	arm-none-eabi-gcc -o "EMS_P1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"/home/ros/Desktop/EmbeddedSoftwareSS20/src/P1/STM32F767ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="EMS_P1.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EMS_P1.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EMS_P1.list"
	@echo 'Finished building: $@'
	@echo ' '

EMS_P1.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "EMS_P1.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
