################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/erpc_touch_client.cpp 

C_SRCS += \
../source/main_client.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/erpc_touch_client.o \
./source/main_client.o \
./source/mtb.o \
./source/semihost_hardfault.o 

CPP_DEPS += \
./source/erpc_touch_client.d 

C_DEPS += \
./source/main_client.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++11 -DCPU_MKL25Z128VFM4 -DCPU_MKL25Z128VFM4_cm0plus -DDEBUG -DCPU_MKL25Z128VLK4 -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__NEWLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../eRPC_spi_master_c_wrapper -I../eRPC_spi_master_transport -I../eRPC_spi_master_transport/erpc/infra -I../eRPC_spi_master_transport/erpc/transports -I../eRPC_client -I../eRPC_client/erpc/infra -I../eRPC_client/erpc/setup -I../eRPC_port_stdlib -I../eRPC_port_stdlib/erpc/port -I../eRPC_includes -I../eRPC_includes/erpc/infra -I../eRPC_includes/erpc/setup -I../eRPC_common_src -I../startup -I../erpc/service -I../erpc -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VFM4 -DCPU_MKL25Z128VFM4_cm0plus -DDEBUG -DCPU_MKL25Z128VLK4 -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__NEWLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../eRPC_spi_master_c_wrapper -I../eRPC_spi_master_transport -I../eRPC_spi_master_transport/erpc/infra -I../eRPC_spi_master_transport/erpc/transports -I../eRPC_client -I../eRPC_client/erpc/infra -I../eRPC_client/erpc/setup -I../eRPC_port_stdlib -I../eRPC_port_stdlib/erpc/port -I../eRPC_includes -I../eRPC_includes/erpc/infra -I../eRPC_includes/erpc/setup -I../eRPC_common_src -I../startup -I../erpc/service -I../erpc -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


