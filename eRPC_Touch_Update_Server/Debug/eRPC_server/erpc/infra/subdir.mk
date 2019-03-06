################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../eRPC_server/erpc/infra/server.cpp \
../eRPC_server/erpc/infra/simple_server.cpp 

OBJS += \
./eRPC_server/erpc/infra/server.o \
./eRPC_server/erpc/infra/simple_server.o 

CPP_DEPS += \
./eRPC_server/erpc/infra/server.d \
./eRPC_server/erpc/infra/simple_server.d 


# Each subdirectory must supply rules for building sources it contributes
eRPC_server/erpc/infra/%.o: ../eRPC_server/erpc/infra/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++11 -DCPU_MKL25Z128VFM4 -DCPU_MKL25Z128VFM4_cm0plus -DDEBUG -DCPU_MKL25Z128VLK4 -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__NEWLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../eRPC_spi_slave_c_wrapper -I../eRPC_spi_slave_transport -I../eRPC_spi_slave_transport/erpc/infra -I../eRPC_spi_slave_transport/erpc/transports -I../eRPC_server -I../eRPC_server/erpc/infra -I../eRPC_server/erpc/setup -I../eRPC_port_stdlib -I../eRPC_port_stdlib/erpc/port -I../eRPC_includes -I../eRPC_includes/erpc/infra -I../eRPC_includes/erpc/setup -I../eRPC_common_src -I../startup -I../erpc/service -I../erpc -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


