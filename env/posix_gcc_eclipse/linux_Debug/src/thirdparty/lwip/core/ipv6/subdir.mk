################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/dhcp6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ethip6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/icmp6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/inet6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ip6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ip6_addr.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ip6_frag.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/mld6.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/nd6.c 

C_DEPS += \
./src/thirdparty/lwip/core/ipv6/dhcp6.d \
./src/thirdparty/lwip/core/ipv6/ethip6.d \
./src/thirdparty/lwip/core/ipv6/icmp6.d \
./src/thirdparty/lwip/core/ipv6/inet6.d \
./src/thirdparty/lwip/core/ipv6/ip6.d \
./src/thirdparty/lwip/core/ipv6/ip6_addr.d \
./src/thirdparty/lwip/core/ipv6/ip6_frag.d \
./src/thirdparty/lwip/core/ipv6/mld6.d \
./src/thirdparty/lwip/core/ipv6/nd6.d 

OBJS += \
./src/thirdparty/lwip/core/ipv6/dhcp6.o \
./src/thirdparty/lwip/core/ipv6/ethip6.o \
./src/thirdparty/lwip/core/ipv6/icmp6.o \
./src/thirdparty/lwip/core/ipv6/inet6.o \
./src/thirdparty/lwip/core/ipv6/ip6.o \
./src/thirdparty/lwip/core/ipv6/ip6_addr.o \
./src/thirdparty/lwip/core/ipv6/ip6_frag.o \
./src/thirdparty/lwip/core/ipv6/mld6.o \
./src/thirdparty/lwip/core/ipv6/nd6.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/lwip/core/ipv6/dhcp6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/dhcp6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/ethip6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ethip6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/icmp6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/icmp6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/inet6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/inet6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/ip6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ip6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/ip6_addr.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ip6_addr.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/ip6_frag.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/ip6_frag.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/mld6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/mld6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ipv6/nd6.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ipv6/nd6.c src/thirdparty/lwip/core/ipv6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-lwip-2f-core-2f-ipv6

clean-src-2f-thirdparty-2f-lwip-2f-core-2f-ipv6:
	-$(RM) ./src/thirdparty/lwip/core/ipv6/dhcp6.d ./src/thirdparty/lwip/core/ipv6/dhcp6.o ./src/thirdparty/lwip/core/ipv6/ethip6.d ./src/thirdparty/lwip/core/ipv6/ethip6.o ./src/thirdparty/lwip/core/ipv6/icmp6.d ./src/thirdparty/lwip/core/ipv6/icmp6.o ./src/thirdparty/lwip/core/ipv6/inet6.d ./src/thirdparty/lwip/core/ipv6/inet6.o ./src/thirdparty/lwip/core/ipv6/ip6.d ./src/thirdparty/lwip/core/ipv6/ip6.o ./src/thirdparty/lwip/core/ipv6/ip6_addr.d ./src/thirdparty/lwip/core/ipv6/ip6_addr.o ./src/thirdparty/lwip/core/ipv6/ip6_frag.d ./src/thirdparty/lwip/core/ipv6/ip6_frag.o ./src/thirdparty/lwip/core/ipv6/mld6.d ./src/thirdparty/lwip/core/ipv6/mld6.o ./src/thirdparty/lwip/core/ipv6/nd6.d ./src/thirdparty/lwip/core/ipv6/nd6.o

.PHONY: clean-src-2f-thirdparty-2f-lwip-2f-core-2f-ipv6

