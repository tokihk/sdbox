################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/protocol/xprotocol.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/protocol/xprotocol_softflow.c 

C_DEPS += \
./src/protocol/xprotocol.d \
./src/protocol/xprotocol_softflow.d 

OBJS += \
./src/protocol/xprotocol.o \
./src/protocol/xprotocol_softflow.o 


# Each subdirectory must supply rules for building sources it contributes
src/protocol/xprotocol.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/protocol/xprotocol.c src/protocol/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/protocol/xprotocol_softflow.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/protocol/xprotocol_softflow.c src/protocol/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-protocol

clean-src-2f-protocol:
	-$(RM) ./src/protocol/xprotocol.d ./src/protocol/xprotocol.o ./src/protocol/xprotocol_softflow.d ./src/protocol/xprotocol_softflow.o

.PHONY: clean-src-2f-protocol

