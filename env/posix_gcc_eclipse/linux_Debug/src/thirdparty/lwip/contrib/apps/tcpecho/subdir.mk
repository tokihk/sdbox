################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.c 

C_DEPS += \
./src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.d 

OBJS += \
./src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.o 


# Each subdirectory must supply rules for building sources it contributes
src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.o: /media/hitoshi/Local\ Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.c src/thirdparty/lwip/contrib/apps/tcpecho/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/lwip/src/include" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src" -I"/media/hitoshi/Local Disk/root/wsp/wsp_SDBox/sdbox_system/src/thirdparty/freertos_kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-thirdparty-2f-lwip-2f-contrib-2f-apps-2f-tcpecho

clean-src-2f-thirdparty-2f-lwip-2f-contrib-2f-apps-2f-tcpecho:
	-$(RM) ./src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.d ./src/thirdparty/lwip/contrib/apps/tcpecho/tcpecho.o

.PHONY: clean-src-2f-thirdparty-2f-lwip-2f-contrib-2f-apps-2f-tcpecho

