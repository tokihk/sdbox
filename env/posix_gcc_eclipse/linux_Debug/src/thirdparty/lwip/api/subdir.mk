################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/api_lib.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/api_msg.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/err.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/if_api.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/netbuf.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/netdb.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/netifapi.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/sockets.c \
/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/tcpip.c 

C_DEPS += \
./src/thirdparty/lwip/api/api_lib.d \
./src/thirdparty/lwip/api/api_msg.d \
./src/thirdparty/lwip/api/err.d \
./src/thirdparty/lwip/api/if_api.d \
./src/thirdparty/lwip/api/netbuf.d \
./src/thirdparty/lwip/api/netdb.d \
./src/thirdparty/lwip/api/netifapi.d \
./src/thirdparty/lwip/api/sockets.d \
./src/thirdparty/lwip/api/tcpip.d 

OBJS += \
./src/thirdparty/lwip/api/api_lib.o \
./src/thirdparty/lwip/api/api_msg.o \
./src/thirdparty/lwip/api/err.o \
./src/thirdparty/lwip/api/if_api.o \
./src/thirdparty/lwip/api/netbuf.o \
./src/thirdparty/lwip/api/netdb.o \
./src/thirdparty/lwip/api/netifapi.o \
./src/thirdparty/lwip/api/sockets.o \
./src/thirdparty/lwip/api/tcpip.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/lwip/api/api_lib.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/api_lib.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/api_msg.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/api_msg.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/err.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/err.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/if_api.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/if_api.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/netbuf.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/netbuf.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/netdb.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/netdb.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/netifapi.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/netifapi.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/sockets.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/sockets.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/thirdparty/lwip/api/tcpip.o: /mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/api/tcpip.c src/thirdparty/lwip/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/include" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/lwip" -I"/mnt/windows/root/wsp/wsp_SDBox/sdbox_system/src/board/posix/thirdparty/freertos_kernel" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-lwip-2f-api

clean-src-2f-thirdparty-2f-lwip-2f-api:
	-$(RM) ./src/thirdparty/lwip/api/api_lib.d ./src/thirdparty/lwip/api/api_lib.o ./src/thirdparty/lwip/api/api_msg.d ./src/thirdparty/lwip/api/api_msg.o ./src/thirdparty/lwip/api/err.d ./src/thirdparty/lwip/api/err.o ./src/thirdparty/lwip/api/if_api.d ./src/thirdparty/lwip/api/if_api.o ./src/thirdparty/lwip/api/netbuf.d ./src/thirdparty/lwip/api/netbuf.o ./src/thirdparty/lwip/api/netdb.d ./src/thirdparty/lwip/api/netdb.o ./src/thirdparty/lwip/api/netifapi.d ./src/thirdparty/lwip/api/netifapi.o ./src/thirdparty/lwip/api/sockets.d ./src/thirdparty/lwip/api/sockets.o ./src/thirdparty/lwip/api/tcpip.d ./src/thirdparty/lwip/api/tcpip.o

.PHONY: clean-src-2f-thirdparty-2f-lwip-2f-api

