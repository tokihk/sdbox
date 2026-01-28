/*
 * xdriver_board.h
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#ifndef XDRIVER_BOARD_H_
#define XDRIVER_BOARD_H_

#include "core/xstddef.h"

#include "fsl_common.h"
#include "fsl_iomuxc.h"

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif

#include "fsl_edma.h"
#include "fsl_gpio.h"

#include "pin_mux.h"


/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDebugConsolePins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDebugConsolePins(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc2);          /* Turn on LPCG: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_08_LPUART1_TX,          /* GPIO_AON_08 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_09_LPUART1_RX,          /* GPIO_AON_09 is configured as LPUART1_RX */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_09 */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_08_LPUART1_TX,          /* GPIO_AON_08 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_09_LPUART1_RX,          /* GPIO_AON_09 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitWM8962Pins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitWM8962Pins(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc2);          /* Turn on LPCG: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_15_LPI2C2_SDA,          /* GPIO_AON_15 is configured as LPI2C2_SDA */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_15 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_16_LPI2C2_SCL,          /* GPIO_AON_16 is configured as LPI2C2_SCL */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_16 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_21_SAI1_TX_DATA00,      /* GPIO_AON_21 is configured as SAI1_TX_DATA00 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_21 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_22_SAI1_TX_SYNC,        /* GPIO_AON_22 is configured as SAI1_TX_SYNC */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_22 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_23_SAI1_TX_BCLK,        /* GPIO_AON_23 is configured as SAI1_TX_BCLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_23 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_24_SAI1_MCLK,           /* GPIO_AON_24 is configured as SAI1_MCLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_24 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_25_SAI1_RX_DATA00,      /* GPIO_AON_25 is configured as SAI1_RX_DATA00 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_25 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_26_SAI1_RX_BCLK,        /* GPIO_AON_26 is configured as SAI1_RX_BCLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_26 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_27_SAI1_RX_SYNC,        /* GPIO_AON_27 is configured as SAI1_RX_SYNC */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_27 */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_15_LPI2C2_SDA,          /* GPIO_AON_15 PAD functional properties : */
      0x1AU);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Disable, Highz
                                                 Pull Up / Down Config. Field: Weak pull up
                                                 Open Drain Field: Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_16_LPI2C2_SCL,          /* GPIO_AON_16 PAD functional properties : */
      0x1AU);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Disable, Highz
                                                 Pull Up / Down Config. Field: Weak pull up
                                                 Open Drain Field: Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_21_SAI1_TX_DATA00,      /* GPIO_AON_21 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_22_SAI1_TX_SYNC,        /* GPIO_AON_22 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_23_SAI1_TX_BCLK,        /* GPIO_AON_23 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_24_SAI1_MCLK,           /* GPIO_AON_24 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_25_SAI1_RX_DATA00,      /* GPIO_AON_25 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_26_SAI1_RX_BCLK,        /* GPIO_AON_26 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_27_SAI1_RX_SYNC,        /* GPIO_AON_27 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPhyAccessPins:
- options: {callFromInitBoot: 'true', coreID: cm7, enableClock: 'true'}
- pin_list:
  - {pin_num: L17, peripheral: NETC, signal: netc_emdc, pin_signal: GPIO_AD_30, pull_up_down_config: Pull_Down, open_drain: Disable}
  - {pin_num: K17, peripheral: NETC, signal: netc_emdio, pin_signal: GPIO_AD_31, identifier: NETC_ECAT_MDIO, pull_up_down_config: Pull_Down, open_drain: Enable}
  - {pin_num: M15, peripheral: RGPIO4, signal: 'gpio_io, 25', pin_signal: GPIO_AD_25, direction: OUTPUT, pull_up_down_config: Pull_Down}
  - {pin_num: B11, peripheral: RGPIO6, signal: 'gpio_io, 13', pin_signal: GPIO_B1_13, direction: OUTPUT}
  - {pin_num: L16, peripheral: RGPIO4, signal: 'gpio_io, 28', pin_signal: GPIO_AD_28, identifier: ENET2_RST_B, direction: OUTPUT}
  - {pin_num: E10, peripheral: RGPIO6, signal: 'gpio_io, 15', pin_signal: GPIO_B2_01, direction: OUTPUT}
  - {pin_num: N15, peripheral: RGPIO4, signal: 'gpio_io, 13', pin_signal: GPIO_AD_13, direction: OUTPUT}
  - {pin_num: P17, peripheral: RGPIO4, signal: 'gpio_io, 12', pin_signal: GPIO_AD_12, direction: INPUT, gpio_interrupt: no_init, gpio_interrupt_output: no_init, pull_up_down_config: no_init,
    open_drain: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPhyAccessPins, assigned for the Cortex-M7F core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPhyAccessPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc1);          /* Turn on LPCG: LPCG is ON. */

  /* GPIO configuration of ENET4_INT_B on GPIO_AD_12 (pin P17) */
  rgpio_pin_config_t ENET4_INT_B_config = {
      .pinDirection = kRGPIO_DigitalInput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_AD_12 (pin P17) */
  RGPIO_PinInit(RGPIO4, 12U, &ENET4_INT_B_config);
  /* Configures GPIO pin interrupt/DMA request on GPIO_AD_12 (pin P17) */
  RGPIO_SetPinInterruptConfig(RGPIO4, 12U, kRGPIO_InterruptOutput0, kRGPIO_InterruptOrDMADisabled);

  /* GPIO configuration of ENET4_RST_B on GPIO_AD_13 (pin N15) */
  rgpio_pin_config_t ENET4_RST_B_config = {
      .pinDirection = kRGPIO_DigitalOutput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_AD_13 (pin N15) */
  RGPIO_PinInit(RGPIO4, 13U, &ENET4_RST_B_config);

  /* GPIO configuration of ENET0_RST_B on GPIO_AD_25 (pin M15) */
  rgpio_pin_config_t ENET0_RST_B_config = {
      .pinDirection = kRGPIO_DigitalOutput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_AD_25 (pin M15) */
  RGPIO_PinInit(RGPIO4, 25U, &ENET0_RST_B_config);

  /* GPIO configuration of ENET2_RST_B on GPIO_AD_28 (pin L16) */
  rgpio_pin_config_t ENET2_RST_B_config = {
      .pinDirection = kRGPIO_DigitalOutput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_AD_28 (pin L16) */
  RGPIO_PinInit(RGPIO4, 28U, &ENET2_RST_B_config);

  /* GPIO configuration of ENET1_RST_B on GPIO_B1_13 (pin B11) */
  rgpio_pin_config_t ENET1_RST_B_config = {
      .pinDirection = kRGPIO_DigitalOutput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_B1_13 (pin B11) */
  RGPIO_PinInit(RGPIO6, 13U, &ENET1_RST_B_config);

  /* GPIO configuration of ENET3_RST_B on GPIO_B2_01 (pin E10) */
  rgpio_pin_config_t ENET3_RST_B_config = {
      .pinDirection = kRGPIO_DigitalOutput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_B2_01 (pin E10) */
  RGPIO_PinInit(RGPIO6, 15U, &ENET3_RST_B_config);

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_12_GPIO4_IO12,           /* GPIO_AD_12 is configured as GPIO4_IO12 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_13_GPIO4_IO13,           /* GPIO_AD_13 is configured as GPIO4_IO13 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_25_GPIO4_IO25,           /* GPIO_AD_25 is configured as GPIO4_IO25 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_28_GPIO4_IO28,           /* GPIO_AD_28 is configured as GPIO4_IO28 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_30_NETC_EMDC,            /* GPIO_AD_30 is configured as NETC_EMDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_31_NETC_EMDIO,           /* GPIO_AD_31 is configured as NETC_EMDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_13_GPIO6_IO13,           /* GPIO_B1_13 is configured as GPIO6_IO13 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B2_01_GPIO6_IO15,           /* GPIO_B2_01 is configured as GPIO6_IO15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_12_GPIO4_IO12,           /* GPIO_AD_12 PAD functional properties : */
      0x0EU);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull up
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_25_GPIO4_IO25,           /* GPIO_AD_25 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_30_NETC_EMDC,            /* GPIO_AD_30 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_31_NETC_EMDIO,           /* GPIO_AD_31 PAD functional properties : */
      0x16U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Enabled
                                                 Force ibe off Field: Disabled */
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitEpPins:
- options: {callFromInitBoot: 'true', coreID: cm7, enableClock: 'true'}
- pin_list:
  - {pin_num: R4, peripheral: NETC_ENETC0_ETH4, signal: TX_EN, pin_signal: GPIO_EMC_B2_15}
  - {pin_num: R7, peripheral: NETC_ENETC0_ETH4, signal: TX_CLK, pin_signal: GPIO_EMC_B2_16, software_input_on: Enable}
  - {pin_num: R6, peripheral: NETC_ENETC0_ETH4, signal: 'TX_DATA, 0', pin_signal: GPIO_EMC_B2_13}
  - {pin_num: N8, peripheral: NETC_ENETC0_ETH4, signal: 'TX_DATA, 1', pin_signal: GPIO_EMC_B2_14}
  - {pin_num: U8, peripheral: NETC_ENETC0_ETH4, signal: RX_EN, pin_signal: GPIO_EMC_B2_19}
  - {pin_num: R8, peripheral: NETC_ENETC0_ETH4, signal: RX_ER, pin_signal: GPIO_EMC_B2_20}
  - {pin_num: U7, peripheral: NETC_ENETC0_ETH4, signal: 'RX_DATA, 0', pin_signal: GPIO_EMC_B2_17}
  - {pin_num: P8, peripheral: NETC_ENETC0_ETH4, signal: 'RX_DATA, 1', pin_signal: GPIO_EMC_B2_18}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitEpPins, assigned for the Cortex-M7F core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitEpPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc1);          /* Turn on LPCG: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_13_NETC_PINMUX_ETH4_TXD00,  /* GPIO_EMC_B2_13 is configured as NETC_PINMUX_ETH4_TXD00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_14_NETC_PINMUX_ETH4_TXD01,  /* GPIO_EMC_B2_14 is configured as NETC_PINMUX_ETH4_TXD01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_15_NETC_PINMUX_ETH4_TX_EN,  /* GPIO_EMC_B2_15 is configured as NETC_PINMUX_ETH4_TX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_16_NETC_PINMUX_ETH4_TX_CLK,  /* GPIO_EMC_B2_16 is configured as NETC_PINMUX_ETH4_TX_CLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_EMC_B2_16 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_17_NETC_PINMUX_ETH4_RXD00,  /* GPIO_EMC_B2_17 is configured as NETC_PINMUX_ETH4_RXD00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_18_NETC_PINMUX_ETH4_RXD01,  /* GPIO_EMC_B2_18 is configured as NETC_PINMUX_ETH4_RXD01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_19_NETC_PINMUX_ETH4_RX_DV,  /* GPIO_EMC_B2_19 is configured as NETC_PINMUX_ETH4_RX_DV */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_B2_20_NETC_PINMUX_ETH4_RX_ER,  /* GPIO_EMC_B2_20 is configured as NETC_PINMUX_ETH4_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
}


static void xboard_pin_init(void)
{
    BOARD_InitPins();

    BOARD_InitWM8962Pins();

    BOARD_InitPhyAccessPins();
    BOARD_InitEpPins();
}

static void xboard_pll_init(void)
{
    BOARD_BootClockRUN();
}

static void xboard_dma_init(void)
{
	/* DMAMUX0 initialize */
	DMAMUX_Init(DMAMUX0);

	/* DMA0 initialize */
	{
		edma_config_t edma_conf;

		EDMA_GetDefaultConfig(&edma_conf);

		EDMA_Init(DMA0, &edma_conf);
	}
}

static inline void xdriver_init_board(void)
{
	BOARD_CommonSetting();

	BOARD_ConfigMPU();

	xboard_pinmux_init();
	xboard_pin_init();

	xboard_pll_init();
}

static inline void xdriver_wait_us_board(uint32_t time_us)
{
	/*
	 * cycle_time = 1 / BOARD_BOOTCLOCKRUN_CORE_CLOCK
	 *            = 1 / 996000000Hz
	 *            = 1ns
	 *
	 * nop  : 1 cycle
	 *
	 * subs : 1 cycle
	 * bge  : 1 cycle
	 *
	 * 1us  = 2 cycle * 498 loop
	 * */

	__asm volatile
	(
		"loop1:									\n"
		"		mov		r1, #820				\n"
		"loop2:									\n"
		"		subs	r1, r1, #1 				\n"
		"		bge		loop2					\n"
		"		subs	r0, r0, #1 				\n"
		"		bge		loop1					\n"
	);
}

#endif /* XDRIVER_BOARD_H_ */
