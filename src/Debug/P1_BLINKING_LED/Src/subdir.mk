################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../P1_BLINKING_LED/Src/main.c \
../P1_BLINKING_LED/Src/syscalls.c \
../P1_BLINKING_LED/Src/sysmem.c 

OBJS += \
./P1_BLINKING_LED/Src/main.o \
./P1_BLINKING_LED/Src/syscalls.o \
./P1_BLINKING_LED/Src/sysmem.o 

C_DEPS += \
./P1_BLINKING_LED/Src/main.d \
./P1_BLINKING_LED/Src/syscalls.d \
./P1_BLINKING_LED/Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
P1_BLINKING_LED/Src/main.o: ../P1_BLINKING_LED/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"P1_BLINKING_LED/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
P1_BLINKING_LED/Src/syscalls.o: ../P1_BLINKING_LED/Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"P1_BLINKING_LED/Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
P1_BLINKING_LED/Src/sysmem.o: ../P1_BLINKING_LED/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"P1_BLINKING_LED/Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

