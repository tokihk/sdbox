################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_object.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_speaker.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_udp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_wave_gen.c 

C_DEPS += \
./src/gate/xgate.d \
./src/gate/xgate_object.d \
./src/gate/xgate_speaker.d \
./src/gate/xgate_udp.d \
./src/gate/xgate_wave_gen.d 

OBJS += \
./src/gate/xgate.o \
./src/gate/xgate_object.o \
./src/gate/xgate_speaker.o \
./src/gate/xgate_udp.o \
./src/gate/xgate_wave_gen.o 


# Each subdirectory must supply rules for building sources it contributes
src/gate/xgate.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate.c src/gate/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gate/xgate_object.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_object.c src/gate/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gate/xgate_speaker.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_speaker.c src/gate/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gate/xgate_udp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_udp.c src/gate/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gate/xgate_wave_gen.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/gate/xgate_wave_gen.c src/gate/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-gate

clean-src-2f-gate:
	-$(RM) ./src/gate/xgate.d ./src/gate/xgate.o ./src/gate/xgate_object.d ./src/gate/xgate_object.o ./src/gate/xgate_speaker.d ./src/gate/xgate_speaker.o ./src/gate/xgate_udp.d ./src/gate/xgate_udp.o ./src/gate/xgate_wave_gen.d ./src/gate/xgate_wave_gen.o

.PHONY: clean-src-2f-gate

