################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/altcp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/altcp_alloc.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/altcp_tcp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/def.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/dns.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/inet_chksum.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/init.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ip.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/mem.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/memp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/netif.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/pbuf.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/raw.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/stats.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/sys.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/tcp.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/tcp_in.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/tcp_out.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/timeouts.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/udp.c 

C_DEPS += \
./src/thirdparty/lwip/core/altcp.d \
./src/thirdparty/lwip/core/altcp_alloc.d \
./src/thirdparty/lwip/core/altcp_tcp.d \
./src/thirdparty/lwip/core/def.d \
./src/thirdparty/lwip/core/dns.d \
./src/thirdparty/lwip/core/inet_chksum.d \
./src/thirdparty/lwip/core/init.d \
./src/thirdparty/lwip/core/ip.d \
./src/thirdparty/lwip/core/mem.d \
./src/thirdparty/lwip/core/memp.d \
./src/thirdparty/lwip/core/netif.d \
./src/thirdparty/lwip/core/pbuf.d \
./src/thirdparty/lwip/core/raw.d \
./src/thirdparty/lwip/core/stats.d \
./src/thirdparty/lwip/core/sys.d \
./src/thirdparty/lwip/core/tcp.d \
./src/thirdparty/lwip/core/tcp_in.d \
./src/thirdparty/lwip/core/tcp_out.d \
./src/thirdparty/lwip/core/timeouts.d \
./src/thirdparty/lwip/core/udp.d 

OBJS += \
./src/thirdparty/lwip/core/altcp.o \
./src/thirdparty/lwip/core/altcp_alloc.o \
./src/thirdparty/lwip/core/altcp_tcp.o \
./src/thirdparty/lwip/core/def.o \
./src/thirdparty/lwip/core/dns.o \
./src/thirdparty/lwip/core/inet_chksum.o \
./src/thirdparty/lwip/core/init.o \
./src/thirdparty/lwip/core/ip.o \
./src/thirdparty/lwip/core/mem.o \
./src/thirdparty/lwip/core/memp.o \
./src/thirdparty/lwip/core/netif.o \
./src/thirdparty/lwip/core/pbuf.o \
./src/thirdparty/lwip/core/raw.o \
./src/thirdparty/lwip/core/stats.o \
./src/thirdparty/lwip/core/sys.o \
./src/thirdparty/lwip/core/tcp.o \
./src/thirdparty/lwip/core/tcp_in.o \
./src/thirdparty/lwip/core/tcp_out.o \
./src/thirdparty/lwip/core/timeouts.o \
./src/thirdparty/lwip/core/udp.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/lwip/core/altcp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/altcp.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/altcp_alloc.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/altcp_alloc.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/altcp_tcp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/altcp_tcp.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/def.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/def.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/dns.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/dns.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/inet_chksum.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/inet_chksum.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/init.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/init.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/ip.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/ip.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/mem.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/mem.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/memp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/memp.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/netif.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/netif.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/pbuf.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/pbuf.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/raw.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/raw.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/stats.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/stats.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/sys.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/sys.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/tcp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/tcp.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/tcp_in.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/tcp_in.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/tcp_out.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/tcp_out.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/timeouts.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/timeouts.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/core/udp.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/core/udp.c src/thirdparty/lwip/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-lwip-2f-core

clean-src-2f-thirdparty-2f-lwip-2f-core:
	-$(RM) ./src/thirdparty/lwip/core/altcp.d ./src/thirdparty/lwip/core/altcp.o ./src/thirdparty/lwip/core/altcp_alloc.d ./src/thirdparty/lwip/core/altcp_alloc.o ./src/thirdparty/lwip/core/altcp_tcp.d ./src/thirdparty/lwip/core/altcp_tcp.o ./src/thirdparty/lwip/core/def.d ./src/thirdparty/lwip/core/def.o ./src/thirdparty/lwip/core/dns.d ./src/thirdparty/lwip/core/dns.o ./src/thirdparty/lwip/core/inet_chksum.d ./src/thirdparty/lwip/core/inet_chksum.o ./src/thirdparty/lwip/core/init.d ./src/thirdparty/lwip/core/init.o ./src/thirdparty/lwip/core/ip.d ./src/thirdparty/lwip/core/ip.o ./src/thirdparty/lwip/core/mem.d ./src/thirdparty/lwip/core/mem.o ./src/thirdparty/lwip/core/memp.d ./src/thirdparty/lwip/core/memp.o ./src/thirdparty/lwip/core/netif.d ./src/thirdparty/lwip/core/netif.o ./src/thirdparty/lwip/core/pbuf.d ./src/thirdparty/lwip/core/pbuf.o ./src/thirdparty/lwip/core/raw.d ./src/thirdparty/lwip/core/raw.o ./src/thirdparty/lwip/core/stats.d ./src/thirdparty/lwip/core/stats.o ./src/thirdparty/lwip/core/sys.d ./src/thirdparty/lwip/core/sys.o ./src/thirdparty/lwip/core/tcp.d ./src/thirdparty/lwip/core/tcp.o ./src/thirdparty/lwip/core/tcp_in.d ./src/thirdparty/lwip/core/tcp_in.o ./src/thirdparty/lwip/core/tcp_out.d ./src/thirdparty/lwip/core/tcp_out.o ./src/thirdparty/lwip/core/timeouts.d ./src/thirdparty/lwip/core/timeouts.o ./src/thirdparty/lwip/core/udp.d ./src/thirdparty/lwip/core/udp.o

.PHONY: clean-src-2f-thirdparty-2f-lwip-2f-core

