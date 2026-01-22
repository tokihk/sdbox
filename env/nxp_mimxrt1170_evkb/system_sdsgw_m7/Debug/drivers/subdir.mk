################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_anatop_ai.c \
../drivers/fsl_cache.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_dcdc.c \
../drivers/fsl_dmamux.c \
../drivers/fsl_edma.c \
../drivers/fsl_enet.c \
../drivers/fsl_gpc.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_lpuart_edma.c \
../drivers/fsl_mu.c \
../drivers/fsl_pmu.c \
../drivers/fsl_sai.c \
../drivers/fsl_sai_edma.c \
../drivers/fsl_soc_src.c 

C_DEPS += \
./drivers/fsl_anatop_ai.d \
./drivers/fsl_cache.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_dcdc.d \
./drivers/fsl_dmamux.d \
./drivers/fsl_edma.d \
./drivers/fsl_enet.d \
./drivers/fsl_gpc.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_lpuart_edma.d \
./drivers/fsl_mu.d \
./drivers/fsl_pmu.d \
./drivers/fsl_sai.d \
./drivers/fsl_sai_edma.d \
./drivers/fsl_soc_src.d 

OBJS += \
./drivers/fsl_anatop_ai.o \
./drivers/fsl_cache.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_dcdc.o \
./drivers/fsl_dmamux.o \
./drivers/fsl_edma.o \
./drivers/fsl_enet.o \
./drivers/fsl_gpc.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_lpuart_edma.o \
./drivers/fsl_mu.o \
./drivers/fsl_pmu.o \
./drivers/fsl_sai.o \
./drivers/fsl_sai_edma.o \
./drivers/fsl_soc_src.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DXIP_BOOT_HEADER_DCD_ENABLE=1 -DMCUXPRESSO_SDK -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DCODEC_WM8962_ENABLE -DSDK_I2C_BASED_COMPONENT_USED=1 -DUSE_SDRAM -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\source" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\drivers" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\device" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\utilities" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\uart" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\serial_manager" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\lists" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\startup" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\xip" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\CMSIS" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\board" -I"C:/root/wsp/wsp_SDSGW/system_sdsgw/env/nxp_mimxrt1170_evkb/system_sdsgw_m7/../../../../common/nxp_mimxrt1170_evkb/include" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\board\nxp_mimxrt1170_evkb\thirdparty\lwip" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\board\nxp_mimxrt1170_evkb\thirdparty\freertos_kernel" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\codec" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\codec\port" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\codec\port\wm8962" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\i2c" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\thirdparty\freertos_kernel\include" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\thirdparty\lwip\src\include" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\src\board\nxp_mimxrt1170_evkb" -I"C:\root\wsp\wsp_SDSGW\system_sdsgw\env\nxp_mimxrt1170_evkb\system_sdsgw_m7\component\phy" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_anatop_ai.d ./drivers/fsl_anatop_ai.o ./drivers/fsl_cache.d ./drivers/fsl_cache.o ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_dcdc.d ./drivers/fsl_dcdc.o ./drivers/fsl_dmamux.d ./drivers/fsl_dmamux.o ./drivers/fsl_edma.d ./drivers/fsl_edma.o ./drivers/fsl_enet.d ./drivers/fsl_enet.o ./drivers/fsl_gpc.d ./drivers/fsl_gpc.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpi2c.d ./drivers/fsl_lpi2c.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_lpuart_edma.d ./drivers/fsl_lpuart_edma.o ./drivers/fsl_mu.d ./drivers/fsl_mu.o ./drivers/fsl_pmu.d ./drivers/fsl_pmu.o ./drivers/fsl_sai.d ./drivers/fsl_sai.o ./drivers/fsl_sai_edma.d ./drivers/fsl_sai_edma.o ./drivers/fsl_soc_src.d ./drivers/fsl_soc_src.o

.PHONY: clean-drivers

