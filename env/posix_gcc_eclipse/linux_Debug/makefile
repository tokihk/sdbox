################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/thirdparty/ntshell/subdir.mk
-include src/thirdparty/lwip/netif/ppp/polarssl/subdir.mk
-include src/thirdparty/lwip/netif/ppp/subdir.mk
-include src/thirdparty/lwip/netif/subdir.mk
-include src/thirdparty/lwip/core/ipv6/subdir.mk
-include src/thirdparty/lwip/core/ipv4/subdir.mk
-include src/thirdparty/lwip/core/subdir.mk
-include src/thirdparty/lwip/api/subdir.mk
-include src/thirdparty/freertos_kernel/subdir.mk
-include src/protocol/subdir.mk
-include src/prompt/subdir.mk
-include src/general/subdir.mk
-include src/gate/subdir.mk
-include src/driver/subdir.mk
-include src/core/subdir.mk
-include src/config/subdir.mk
-include src/board/posix/thirdparty/lwip/netif/subdir.mk
-include src/board/posix/thirdparty/lwip/arch/subdir.mk
-include src/board/posix/thirdparty/freertos_kernel/subdir.mk
-include src/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := sdbox_system
BUILD_ARTIFACT_EXTENSION := exe
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: sdbox_system.exe

# Tool invocations
sdbox_system.exe: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/env/eclipse_gcc_posix" -o "sdbox_system.exe" $(OBJS) $(USER_OBJS) $(LIBS) -lasound -lm -lpcap -lvdeplug
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) sdbox_system.exe
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
