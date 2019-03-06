################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include erpc/service/subdir.mk
-include erpc/subdir.mk
-include eRPC_spi_master_transport/erpc/transports/subdir.mk
-include eRPC_spi_master_transport/erpc/setup/subdir.mk
-include eRPC_spi_master_transport/erpc/infra/subdir.mk
-include eRPC_spi_master_c_wrapper/erpc/setup/subdir.mk
-include eRPC_port_stdlib/erpc/port/subdir.mk
-include eRPC_common_src/erpc/infra/subdir.mk
-include eRPC_client/erpc/setup/subdir.mk
-include eRPC_client/erpc/infra/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: eRPC_Touch_Update_Client.axf

# Tool invocations
eRPC_Touch_Update_Client.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -g  -Wall  --specs=nano.specs  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -Xlinker -Map="eRPC_Touch_Update_Client.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m0plus -mthumb -T frdmkl25z_multiprocessor_examples_erpc_client_matrix_multiply_spi_Debug.ld -o "eRPC_Touch_Update_Client.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) eRPC_Touch_Update_Client.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "eRPC_Touch_Update_Client.axf"; # arm-none-eabi-objcopy -v -O binary "eRPC_Touch_Update_Client.axf" "eRPC_Touch_Update_Client.bin" ; # checksum -p MKL25Z128xxx4 -d "eRPC_Touch_Update_Client.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
