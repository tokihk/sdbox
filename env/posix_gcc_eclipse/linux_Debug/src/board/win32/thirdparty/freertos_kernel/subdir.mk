################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/freertos_kernel/heap_3.c \
/media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/freertos_kernel/heap_4.c \
/media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/freertos_kernel/port.c 

C_DEPS += \
./src/board/win32/thirdparty/freertos_kernel/heap_3.d \
./src/board/win32/thirdparty/freertos_kernel/heap_4.d \
./src/board/win32/thirdparty/freertos_kernel/port.d 

OBJS += \
./src/board/win32/thirdparty/freertos_kernel/heap_3.o \
./src/board/win32/thirdparty/freertos_kernel/heap_4.o \
./src/board/win32/thirdparty/freertos_kernel/port.o 


# Each subdirectory must supply rules for building sources it contributes
src/board/win32/thirdparty/freertos_kernel/heap_3.o: /media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/freertos_kernel/heap_3.c src/board/win32/thirdparty/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/src/include" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/board/win32/thirdparty/freertos_kernel/heap_4.o: /media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/freertos_kernel/heap_4.c src/board/win32/thirdparty/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/src/include" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/board/win32/thirdparty/freertos_kernel/port.o: /media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/board/win32/thirdparty/freertos_kernel/port.c src/board/win32/thirdparty/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/src/include" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-board-2f-win32-2f-thirdparty-2f-freertos_kernel

clean-src-2f-board-2f-win32-2f-thirdparty-2f-freertos_kernel:
	-$(RM) ./src/board/win32/thirdparty/freertos_kernel/heap_3.d ./src/board/win32/thirdparty/freertos_kernel/heap_3.o ./src/board/win32/thirdparty/freertos_kernel/heap_4.d ./src/board/win32/thirdparty/freertos_kernel/heap_4.o ./src/board/win32/thirdparty/freertos_kernel/port.d ./src/board/win32/thirdparty/freertos_kernel/port.o

.PHONY: clean-src-2f-board-2f-win32-2f-thirdparty-2f-freertos_kernel

