################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/root/wsp/wsp_SDSGW/system_sdsgw/src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.c 

C_DEPS += \
./src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.d 

OBJS += \
./src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.o 


# Each subdirectory must supply rules for building sources it contributes
src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.o: C:/root/wsp/wsp_SDSGW/system_sdsgw/src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.c src/board/gcc_win32/thirdparty/lwip/arch/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DXIP_BOOT_HEADER_DCD_ENABLE=1 -DMCUXPRESSO_SDK -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DCODEC_WM8962_ENABLE -DSDK_I2C_BASED_COMPONENT_USED=1 -DUSE_SDRAM -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\source" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\drivers" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\device" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\utilities" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\uart" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\serial_manager" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\lists" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\startup" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\xip" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\CMSIS" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\board" -I"C:/root/wsp/wsp_SDSGW/system_sdsgw/env/nxp_mimxrt1170_evkb/system_sdsgw_m7/../../../../common/nxp_mimxrt1170_evkb/include" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\board\nxp_mimxrt1170_evkb\thirdparty\lwip" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\board\nxp_mimxrt1170_evkb\thirdparty\freertos_kernel" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\codec" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\codec\port" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\codec\port\wm8962" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\i2c" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\thirdparty\freertos_kernel\include" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\thirdparty\lwip\src\include" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\board\nxp_mimxrt1170_evkb" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\phy" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-board-2f-gcc_win32-2f-thirdparty-2f-lwip-2f-arch

clean-src-2f-board-2f-gcc_win32-2f-thirdparty-2f-lwip-2f-arch:
	-$(RM) ./src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.d ./src/board/gcc_win32/thirdparty/lwip/arch/sys_arch.o

.PHONY: clean-src-2f-board-2f-gcc_win32-2f-thirdparty-2f-lwip-2f-arch

