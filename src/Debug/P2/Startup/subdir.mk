################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../P2/Startup/startup_stm32f407vgtx.s 

OBJS += \
./P2/Startup/startup_stm32f407vgtx.o 


# Each subdirectory must supply rules for building sources it contributes
P2/Startup/%.o: ../P2/Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

