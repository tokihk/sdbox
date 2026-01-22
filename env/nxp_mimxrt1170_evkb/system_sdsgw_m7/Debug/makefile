################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include xip/subdir.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include src/thirdparty/ntshell/subdir.mk
-include src/thirdparty/lwip/src/netif/ppp/polarssl/subdir.mk
-include src/thirdparty/lwip/src/netif/ppp/subdir.mk
-include src/thirdparty/lwip/src/netif/subdir.mk
-include src/thirdparty/lwip/src/core/ipv6/subdir.mk
-include src/thirdparty/lwip/src/core/ipv4/subdir.mk
-include src/thirdparty/lwip/src/core/subdir.mk
-include src/thirdparty/lwip/src/api/subdir.mk
-include src/thirdparty/lwip/contrib/apps/tcpecho/subdir.mk
-include src/thirdparty/freertos_kernel/subdir.mk
-include src/protocol/subdir.mk
-include src/prompt/subdir.mk
-include src/general/subdir.mk
-include src/gate/subdir.mk
-include src/driver/subdir.mk
-include src/core/subdir.mk
-include src/config/subdir.mk
-include src/board/nxp_mimxrt1170_evkb/thirdparty/lwip/arch/subdir.mk
-include src/board/nxp_mimxrt1170_evkb/thirdparty/lwip/subdir.mk
-include src/board/nxp_mimxrt1170_evkb/thirdparty/freertos_kernel/subdir.mk
-include src/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/phy/subdir.mk
-include component/lists/subdir.mk
-include component/i2c/subdir.mk
-include codec/port/wm8962/subdir.mk
-include codec/subdir.mk
-include board/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := system_sdsgw_m7
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: system_sdsgw_m7.axf

# Tool invocations
system_sdsgw_m7.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker -Map="system_sdsgw_m7.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -T system_sdsgw_m7_Debug.ld -u _printf_float -o "system_sdsgw_m7.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) system_sdsgw_m7.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "system_sdsgw_m7.axf"; # arm-none-eabi-objcopy -v -O binary "system_sdsgw_m7.axf" "system_sdsgw_m7.bin" ; # checksum -p MIMXRT1176xxxxx -d "system_sdsgw_m7.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
