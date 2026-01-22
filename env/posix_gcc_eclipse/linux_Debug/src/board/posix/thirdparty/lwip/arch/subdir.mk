################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip/arch/perf.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip/arch/sys_arch.c 

C_DEPS += \
./src/board/posix/thirdparty/lwip/arch/perf.d \
./src/board/posix/thirdparty/lwip/arch/sys_arch.d 

OBJS += \
./src/board/posix/thirdparty/lwip/arch/perf.o \
./src/board/posix/thirdparty/lwip/arch/sys_arch.o 


# Each subdirectory must supply rules for building sources it contributes
src/board/posix/thirdparty/lwip/arch/perf.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip/arch/perf.c src/board/posix/thirdparty/lwip/arch/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/board/posix/thirdparty/lwip/arch/sys_arch.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip/arch/sys_arch.c src/board/posix/thirdparty/lwip/arch/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-board-2f-posix-2f-thirdparty-2f-lwip-2f-arch

clean-src-2f-board-2f-posix-2f-thirdparty-2f-lwip-2f-arch:
	-$(RM) ./src/board/posix/thirdparty/lwip/arch/perf.d ./src/board/posix/thirdparty/lwip/arch/perf.o ./src/board/posix/thirdparty/lwip/arch/sys_arch.d ./src/board/posix/thirdparty/lwip/arch/sys_arch.o

.PHONY: clean-src-2f-board-2f-posix-2f-thirdparty-2f-lwip-2f-arch

