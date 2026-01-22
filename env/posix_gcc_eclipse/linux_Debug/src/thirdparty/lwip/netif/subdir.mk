################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/bridgeif.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/bridgeif_fdb.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/ethernet.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/lowpan6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/lowpan6_ble.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/lowpan6_common.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/slipif.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/zepif.c 

C_DEPS += \
./src/thirdparty/lwip/netif/bridgeif.d \
./src/thirdparty/lwip/netif/bridgeif_fdb.d \
./src/thirdparty/lwip/netif/ethernet.d \
./src/thirdparty/lwip/netif/lowpan6.d \
./src/thirdparty/lwip/netif/lowpan6_ble.d \
./src/thirdparty/lwip/netif/lowpan6_common.d \
./src/thirdparty/lwip/netif/slipif.d \
./src/thirdparty/lwip/netif/zepif.d 

OBJS += \
./src/thirdparty/lwip/netif/bridgeif.o \
./src/thirdparty/lwip/netif/bridgeif_fdb.o \
./src/thirdparty/lwip/netif/ethernet.o \
./src/thirdparty/lwip/netif/lowpan6.o \
./src/thirdparty/lwip/netif/lowpan6_ble.o \
./src/thirdparty/lwip/netif/lowpan6_common.o \
./src/thirdparty/lwip/netif/slipif.o \
./src/thirdparty/lwip/netif/zepif.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/lwip/netif/bridgeif.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/bridgeif.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/bridgeif_fdb.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/bridgeif_fdb.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/ethernet.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/ethernet.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/lowpan6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/lowpan6.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/lowpan6_ble.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/lowpan6_ble.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/lowpan6_common.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/lowpan6_common.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/slipif.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/slipif.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/netif/zepif.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/netif/zepif.c src/thirdparty/lwip/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-lwip-2f-netif

clean-src-2f-thirdparty-2f-lwip-2f-netif:
	-$(RM) ./src/thirdparty/lwip/netif/bridgeif.d ./src/thirdparty/lwip/netif/bridgeif.o ./src/thirdparty/lwip/netif/bridgeif_fdb.d ./src/thirdparty/lwip/netif/bridgeif_fdb.o ./src/thirdparty/lwip/netif/ethernet.d ./src/thirdparty/lwip/netif/ethernet.o ./src/thirdparty/lwip/netif/lowpan6.d ./src/thirdparty/lwip/netif/lowpan6.o ./src/thirdparty/lwip/netif/lowpan6_ble.d ./src/thirdparty/lwip/netif/lowpan6_ble.o ./src/thirdparty/lwip/netif/lowpan6_common.d ./src/thirdparty/lwip/netif/lowpan6_common.o ./src/thirdparty/lwip/netif/slipif.d ./src/thirdparty/lwip/netif/slipif.o ./src/thirdparty/lwip/netif/zepif.d ./src/thirdparty/lwip/netif/zepif.o

.PHONY: clean-src-2f-thirdparty-2f-lwip-2f-netif

