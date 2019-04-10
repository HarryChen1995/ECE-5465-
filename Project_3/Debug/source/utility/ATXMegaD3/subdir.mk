################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/utility/ATXMegaD3/gpio.cpp \
../source/utility/ATXMegaD3/spi.cpp 

C_SRCS += \
../source/utility/ATXMegaD3/compatibility.c \
../source/utility/ATXMegaD3/gpio_helper.c 

OBJS += \
./source/utility/ATXMegaD3/compatibility.o \
./source/utility/ATXMegaD3/gpio.o \
./source/utility/ATXMegaD3/gpio_helper.o \
./source/utility/ATXMegaD3/spi.o 

CPP_DEPS += \
./source/utility/ATXMegaD3/gpio.d \
./source/utility/ATXMegaD3/spi.d 

C_DEPS += \
./source/utility/ATXMegaD3/compatibility.d \
./source/utility/ATXMegaD3/gpio_helper.d 


# Each subdirectory must supply rules for building sources it contributes
source/utility/ATXMegaD3/%.o: ../source/utility/ATXMegaD3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/utility/ATXMegaD3/%.o: ../source/utility/ATXMegaD3/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


