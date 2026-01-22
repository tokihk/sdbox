################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel/heap_3.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel/port.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel/wait_for_event.c 

C_DEPS += \
./src/board/posix/thirdparty/freertos_kernel/heap_3.d \
./src/board/posix/thirdparty/freertos_kernel/port.d \
./src/board/posix/thirdparty/freertos_kernel/wait_for_event.d 

OBJS += \
./src/board/posix/thirdparty/freertos_kernel/heap_3.o \
./src/board/posix/thirdparty/freertos_kernel/port.o \
./src/board/posix/thirdparty/freertos_kernel/wait_for_event.o 


# Each subdirectory must supply rules for building sources it contributes
src/board/posix/thirdparty/freertos_kernel/heap_3.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel/heap_3.c src/board/posix/thirdparty/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/board/posix/thirdparty/freertos_kernel/port.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel/port.c src/board/posix/thirdparty/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/board/posix/thirdparty/freertos_kernel/wait_for_event.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel/wait_for_event.c src/board/posix/thirdparty/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-board-2f-posix-2f-thirdparty-2f-freertos_kernel

clean-src-2f-board-2f-posix-2f-thirdparty-2f-freertos_kernel:
	-$(RM) ./src/board/posix/thirdparty/freertos_kernel/heap_3.d ./src/board/posix/thirdparty/freertos_kernel/heap_3.o ./src/board/posix/thirdparty/freertos_kernel/port.d ./src/board/posix/thirdparty/freertos_kernel/port.o ./src/board/posix/thirdparty/freertos_kernel/wait_for_event.d ./src/board/posix/thirdparty/freertos_kernel/wait_for_event.o

.PHONY: clean-src-2f-board-2f-posix-2f-thirdparty-2f-freertos_kernel

