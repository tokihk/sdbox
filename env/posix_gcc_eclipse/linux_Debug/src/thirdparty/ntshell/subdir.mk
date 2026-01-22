################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntlibc.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntopt.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntshell.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntstdio.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/text_editor.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/text_history.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/vtrecv.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/vtsend.c 

C_DEPS += \
./src/thirdparty/ntshell/ntlibc.d \
./src/thirdparty/ntshell/ntopt.d \
./src/thirdparty/ntshell/ntshell.d \
./src/thirdparty/ntshell/ntstdio.d \
./src/thirdparty/ntshell/text_editor.d \
./src/thirdparty/ntshell/text_history.d \
./src/thirdparty/ntshell/vtrecv.d \
./src/thirdparty/ntshell/vtsend.d 

OBJS += \
./src/thirdparty/ntshell/ntlibc.o \
./src/thirdparty/ntshell/ntopt.o \
./src/thirdparty/ntshell/ntshell.o \
./src/thirdparty/ntshell/ntstdio.o \
./src/thirdparty/ntshell/text_editor.o \
./src/thirdparty/ntshell/text_history.o \
./src/thirdparty/ntshell/vtrecv.o \
./src/thirdparty/ntshell/vtsend.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/ntshell/ntlibc.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntlibc.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/ntopt.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntopt.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/ntshell.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntshell.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/ntstdio.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/ntstdio.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/text_editor.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/text_editor.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/text_history.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/text_history.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/vtrecv.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/vtrecv.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/ntshell/vtsend.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/ntshell/vtsend.c src/thirdparty/ntshell/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-ntshell

clean-src-2f-thirdparty-2f-ntshell:
	-$(RM) ./src/thirdparty/ntshell/ntlibc.d ./src/thirdparty/ntshell/ntlibc.o ./src/thirdparty/ntshell/ntopt.d ./src/thirdparty/ntshell/ntopt.o ./src/thirdparty/ntshell/ntshell.d ./src/thirdparty/ntshell/ntshell.o ./src/thirdparty/ntshell/ntstdio.d ./src/thirdparty/ntshell/ntstdio.o ./src/thirdparty/ntshell/text_editor.d ./src/thirdparty/ntshell/text_editor.o ./src/thirdparty/ntshell/text_history.d ./src/thirdparty/ntshell/text_history.o ./src/thirdparty/ntshell/vtrecv.d ./src/thirdparty/ntshell/vtrecv.o ./src/thirdparty/ntshell/vtsend.d ./src/thirdparty/ntshell/vtsend.o

.PHONY: clean-src-2f-thirdparty-2f-ntshell

