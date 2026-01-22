################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/lwip/arch/sys_arch.c 

C_DEPS += \
./src/board/win32/thirdparty/lwip/arch/sys_arch.d 

OBJS += \
./src/board/win32/thirdparty/lwip/arch/sys_arch.o 


# Each subdirectory must supply rules for building sources it contributes
src/board/win32/thirdparty/lwip/arch/sys_arch.o: /media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/lwip/arch/sys_arch.c src/board/win32/thirdparty/lwip/arch/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/src/include" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-board-2f-win32-2f-thirdparty-2f-lwip-2f-arch

clean-src-2f-board-2f-win32-2f-thirdparty-2f-lwip-2f-arch:
	-$(RM) ./src/board/win32/thirdparty/lwip/arch/sys_arch.d ./src/board/win32/thirdparty/lwip/arch/sys_arch.o

.PHONY: clean-src-2f-board-2f-win32-2f-thirdparty-2f-lwip-2f-arch

