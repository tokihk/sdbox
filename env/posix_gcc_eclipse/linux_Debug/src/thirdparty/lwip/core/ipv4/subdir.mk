################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/acd.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/autoip.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/dhcp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/etharp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/icmp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/igmp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/ip4.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/ip4_addr.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/ip4_frag.c 

C_DEPS += \
./src/thirdparty/lwip/core/ipv4/acd.d \
./src/thirdparty/lwip/core/ipv4/autoip.d \
./src/thirdparty/lwip/core/ipv4/dhcp.d \
./src/thirdparty/lwip/core/ipv4/etharp.d \
./src/thirdparty/lwip/core/ipv4/icmp.d \
./src/thirdparty/lwip/core/ipv4/igmp.d \
./src/thirdparty/lwip/core/ipv4/ip4.d \
./src/thirdparty/lwip/core/ipv4/ip4_addr.d \
./src/thirdparty/lwip/core/ipv4/ip4_frag.d 

OBJS += \
./src/thirdparty/lwip/core/ipv4/acd.o \
./src/thirdparty/lwip/core/ipv4/autoip.o \
./src/thirdparty/lwip/core/ipv4/dhcp.o \
./src/thirdparty/lwip/core/ipv4/etharp.o \
./src/thirdparty/lwip/core/ipv4/icmp.o \
./src/thirdparty/lwip/core/ipv4/igmp.o \
./src/thirdparty/lwip/core/ipv4/ip4.o \
./src/thirdparty/lwip/core/ipv4/ip4_addr.o \
./src/thirdparty/lwip/core/ipv4/ip4_frag.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/lwip/core/ipv4/acd.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/acd.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/autoip.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/autoip.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/dhcp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/dhcp.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/etharp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/etharp.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/icmp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/icmp.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/igmp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/igmp.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/ip4.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/ip4.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/ip4_addr.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/ip4_addr.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv4/ip4_frag.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv4/ip4_frag.c src/thirdparty/lwip/core/ipv4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-lwip-2f-core-2f-ipv4

clean-src-2f-thirdparty-2f-lwip-2f-core-2f-ipv4:
	-$(RM) ./src/thirdparty/lwip/core/ipv4/acd.d ./src/thirdparty/lwip/core/ipv4/acd.o ./src/thirdparty/lwip/core/ipv4/autoip.d ./src/thirdparty/lwip/core/ipv4/autoip.o ./src/thirdparty/lwip/core/ipv4/dhcp.d ./src/thirdparty/lwip/core/ipv4/dhcp.o ./src/thirdparty/lwip/core/ipv4/etharp.d ./src/thirdparty/lwip/core/ipv4/etharp.o ./src/thirdparty/lwip/core/ipv4/icmp.d ./src/thirdparty/lwip/core/ipv4/icmp.o ./src/thirdparty/lwip/core/ipv4/igmp.d ./src/thirdparty/lwip/core/ipv4/igmp.o ./src/thirdparty/lwip/core/ipv4/ip4.d ./src/thirdparty/lwip/core/ipv4/ip4.o ./src/thirdparty/lwip/core/ipv4/ip4_addr.d ./src/thirdparty/lwip/core/ipv4/ip4_addr.o ./src/thirdparty/lwip/core/ipv4/ip4_frag.d ./src/thirdparty/lwip/core/ipv4/ip4_frag.o

.PHONY: clean-src-2f-thirdparty-2f-lwip-2f-core-2f-ipv4

