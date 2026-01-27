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

static void xboard_pin_init(void)
{
    BOARD_InitBootPins();

    BOARD_InitDebugConsolePins();

    BOARD_InitWM8962Pins();


	CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */
	CLOCK_EnableClock(kCLOCK_Iomuxc_Lpsr);      /* LPCG on: LPCG is ON. */

	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_24_LPUART1_TXD,          /* GPIO_AD_24 is configured as LPUART1_TXD */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_25_LPUART1_RXD,          /* GPIO_AD_25 is configured as LPUART1_RXD */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */

	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_17_SAI1_MCLK,            /* GPIO_AD_17 is configured as SAI1_MCLK */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_17 */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_21_SAI1_TX_DATA00,       /* GPIO_AD_21 is configured as SAI1_TX_DATA00 */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_21 */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_22_SAI1_TX_BCLK,         /* GPIO_AD_22 is configured as SAI1_TX_BCLK */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_22 */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_23_SAI1_TX_SYNC,         /* GPIO_AD_23 is configured as SAI1_TX_SYNC */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_23 */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_24_LPUART1_TXD,          /* GPIO_AD_24 is configured as LPUART1_TXD */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_25_LPUART1_RXD,          /* GPIO_AD_25 is configured as LPUART1_RXD */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_GPR->GPR0 = ((IOMUXC_GPR->GPR0 &
	(~(IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK)))  /* Mask bits to zero which are setting */
	  | IOMUXC_GPR_GPR0_SAI1_MCLK_DIR(0x01U)  /* SAI1_MCLK signal direction control: 0x01U */
	);
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_LPSR_04_LPI2C5_SDA,         /* GPIO_LPSR_04 is configured as LPI2C5_SDA */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_LPSR_04 */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_LPSR_05_LPI2C5_SCL,         /* GPIO_LPSR_05 is configured as LPI2C5_SCL */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_LPSR_05 */


	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_24_LPUART1_TXD,          /* GPIO_AD_24 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_25_LPUART1_RXD,          /* GPIO_AD_25 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */

	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_17_SAI1_MCLK,            /* GPIO_AD_17 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_21_SAI1_TX_DATA00,       /* GPIO_AD_21 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_22_SAI1_TX_BCLK,         /* GPIO_AD_22 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_23_SAI1_TX_SYNC,         /* GPIO_AD_23 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_24_LPUART1_TXD,          /* GPIO_AD_24 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_AD_25_LPUART1_RXD,          /* GPIO_AD_25 PAD functional properties : */
	  0x02U);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_LPSR_04_LPI2C5_SDA,         /* GPIO_LPSR_04 PAD functional properties : */
	  0x0AU);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high driver
												 Pull / Keep Select Field: Pull Disable
												 Pull Up / Down Config. Field: Weak pull up
												 Open Drain LPSR Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_LPSR_05_LPI2C5_SCL,         /* GPIO_LPSR_05 PAD functional properties : */
	  0x0AU);                                 /* Slew Rate Field: Slow Slew Rate
												 Drive Strength Field: high driver
												 Pull / Keep Select Field: Pull Disable
												 Pull Up / Down Config. Field: Weak pull up
												 Open Drain LPSR Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */

}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitEnet1GPins:
- options: {callFromInitBoot: 'false', coreID: cm7, enableClock: 'true'}
- pin_list:
  - {pin_num: E13, peripheral: ENET_1G, signal: enet_rx_en, pin_signal: GPIO_DISP_B1_00}
  - {pin_num: D13, peripheral: ENET_1G, signal: enet_rx_clk, pin_signal: GPIO_DISP_B1_01}
  - {pin_num: D11, peripheral: ENET_1G, signal: 'enet_rdata, 00', pin_signal: GPIO_DISP_B1_02}
  - {pin_num: E11, peripheral: ENET_1G, signal: 'enet_rdata, 01', pin_signal: GPIO_DISP_B1_03}
  - {pin_num: E10, peripheral: ENET_1G, signal: 'enet_rdata, 02', pin_signal: GPIO_DISP_B1_04}
  - {pin_num: C11, peripheral: ENET_1G, signal: 'enet_rdata, 03', pin_signal: GPIO_DISP_B1_05}
  - {pin_num: D10, peripheral: ENET_1G, signal: 'enet_tdata, 03', pin_signal: GPIO_DISP_B1_06}
  - {pin_num: E12, peripheral: ENET_1G, signal: 'enet_tdata, 02', pin_signal: GPIO_DISP_B1_07}
  - {pin_num: A15, peripheral: ENET_1G, signal: 'enet_tdata, 01', pin_signal: GPIO_DISP_B1_08}
  - {pin_num: C13, peripheral: ENET_1G, signal: 'enet_tdata, 00', pin_signal: GPIO_DISP_B1_09}
  - {pin_num: B14, peripheral: ENET_1G, signal: enet_tx_en, pin_signal: GPIO_DISP_B1_10}
  - {pin_num: A14, peripheral: ENET_1G, signal: enet_tx_clk_io, pin_signal: GPIO_DISP_B1_11}
  - {pin_num: A5, peripheral: GPIO11, signal: 'gpio_io, 14', pin_signal: GPIO_DISP_B2_13}
  - {pin_num: U2, peripheral: ENET_1G, signal: enet_mdc, pin_signal: GPIO_EMC_B2_19}
  - {pin_num: R3, peripheral: ENET_1G, signal: enet_mdio, pin_signal: GPIO_EMC_B2_20}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitEnet1GPins, assigned for the Cortex-M7F core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void xboard_pin_enet1g_init(void)
{
//	CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */

	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_00_ENET_1G_RX_EN,   /* GPIO_DISP_B1_00 is configured as ENET_1G_RX_EN */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_01_ENET_1G_RX_CLK,  /* GPIO_DISP_B1_01 is configured as ENET_1G_RX_CLK */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_02_ENET_1G_RX_DATA00,  /* GPIO_DISP_B1_02 is configured as ENET_1G_RX_DATA00 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_03_ENET_1G_RX_DATA01,  /* GPIO_DISP_B1_03 is configured as ENET_1G_RX_DATA01 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_04_ENET_1G_RX_DATA02,  /* GPIO_DISP_B1_04 is configured as ENET_1G_RX_DATA02 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_05_ENET_1G_RX_DATA03,  /* GPIO_DISP_B1_05 is configured as ENET_1G_RX_DATA03 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_06_ENET_1G_TX_DATA03,  /* GPIO_DISP_B1_06 is configured as ENET_1G_TX_DATA03 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_07_ENET_1G_TX_DATA02,  /* GPIO_DISP_B1_07 is configured as ENET_1G_TX_DATA02 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_08_ENET_1G_TX_DATA01,  /* GPIO_DISP_B1_08 is configured as ENET_1G_TX_DATA01 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_09_ENET_1G_TX_DATA00,  /* GPIO_DISP_B1_09 is configured as ENET_1G_TX_DATA00 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_10_ENET_1G_TX_EN,   /* GPIO_DISP_B1_10 is configured as ENET_1G_TX_EN */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B1_11_ENET_1G_TX_CLK_IO,  /* GPIO_DISP_B1_11 is configured as ENET_1G_TX_CLK_IO */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_13_GPIO11_IO14,     /* GPIO_DISP_B2_13 is configured as GPIO11_IO14 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_EMC_B2_19_ENET_1G_MDC,      /* GPIO_EMC_B2_19 is configured as ENET_1G_MDC */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_EMC_B2_20_ENET_1G_MDIO,     /* GPIO_EMC_B2_20 is configured as ENET_1G_MDIO */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitEnetPins:
- options: {callFromInitBoot: 'false', coreID: cm7, enableClock: 'true'}
- pin_list:
  - {pin_num: P17, peripheral: GPIO9, signal: 'gpio_io, 11', pin_signal: GPIO_AD_12}
  - {pin_num: K16, peripheral: ENET, signal: enet_mdc, pin_signal: GPIO_AD_32}
  - {pin_num: H17, peripheral: ENET, signal: enet_mdio, pin_signal: GPIO_AD_33}
  - {pin_num: E9, peripheral: ENET, signal: 'enet_tdata, 00', pin_signal: GPIO_DISP_B2_02}
  - {pin_num: D7, peripheral: ENET, signal: 'enet_tdata, 01', pin_signal: GPIO_DISP_B2_03}
  - {pin_num: C7, peripheral: ENET, signal: enet_tx_en, pin_signal: GPIO_DISP_B2_04}
  - {pin_num: C9, peripheral: ENET, signal: enet_ref_clk, pin_signal: GPIO_DISP_B2_05, software_input_on: Enable, slew_rate: Fast}
  - {pin_num: C6, peripheral: ENET, signal: 'enet_rdata, 00', pin_signal: GPIO_DISP_B2_06}
  - {pin_num: D6, peripheral: ENET, signal: 'enet_rdata, 01', pin_signal: GPIO_DISP_B2_07}
  - {pin_num: B5, peripheral: ENET, signal: enet_rx_en, pin_signal: GPIO_DISP_B2_08}
  - {pin_num: D8, peripheral: ENET, signal: enet_rx_er, pin_signal: GPIO_DISP_B2_09}
  - {pin_num: U5, peripheral: GPIO12, signal: 'gpio_io, 12', pin_signal: GPIO_LPSR_12}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitEnetPins, assigned for the Cortex-M7F core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void xboard_pin_init_enet(void)
{
	CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */
//	CLOCK_EnableClock(kCLOCK_Iomuxc_Lpsr);      /* LPCG on: LPCG is ON. */

	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_12_GPIO9_IO11,           /* GPIO_AD_12 is configured as GPIO9_IO11 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_32_ENET_MDC,             /* GPIO_AD_32 is configured as ENET_MDC */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_AD_33_ENET_MDIO,            /* GPIO_AD_33 is configured as ENET_MDIO */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_02_ENET_TX_DATA00,  /* GPIO_DISP_B2_02 is configured as ENET_TX_DATA00 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_03_ENET_TX_DATA01,  /* GPIO_DISP_B2_03 is configured as ENET_TX_DATA01 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_04_ENET_TX_EN,      /* GPIO_DISP_B2_04 is configured as ENET_TX_EN */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_05_ENET_REF_CLK,    /* GPIO_DISP_B2_05 is configured as ENET_REF_CLK */
	  1U);                                    /* Software Input On Field: Force input path of pad GPIO_DISP_B2_05 */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_06_ENET_RX_DATA00,  /* GPIO_DISP_B2_06 is configured as ENET_RX_DATA00 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_07_ENET_RX_DATA01,  /* GPIO_DISP_B2_07 is configured as ENET_RX_DATA01 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_08_ENET_RX_EN,      /* GPIO_DISP_B2_08 is configured as ENET_RX_EN */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_DISP_B2_09_ENET_RX_ER,      /* GPIO_DISP_B2_09 is configured as ENET_RX_ER */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_GPR->GPR4 = ((IOMUXC_GPR->GPR4 &
	(~(IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK))) /* Mask bits to zero which are setting */
	  | IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR(0x01U) /* ENET_REF_CLK direction control: 0x01U */
	);
	IOMUXC_SetPinMux(
	  IOMUXC_GPIO_LPSR_12_GPIO12_IO12,        /* GPIO_LPSR_12 is configured as GPIO12_IO12 */
	  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinConfig(
	  IOMUXC_GPIO_DISP_B2_05_ENET_REF_CLK,    /* GPIO_DISP_B2_05 PAD functional properties : */
	  0x03U);                                 /* Slew Rate Field: Fast Slew Rate
												 Drive Strength Field: high drive strength
												 Pull / Keep Select Field: Pull Disable, Highz
												 Pull Up / Down Config. Field: Weak pull down
												 Open Drain Field: Disabled
												 Domain write protection: Both cores are allowed
												 Domain write protection lock: Neither of DWP bits is locked */
}

static void xboard_pinmux_init(void)
{
	uint32_t mask;

//	CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */
//	CLOCK_EnableClock(kCLOCK_Iomuxc_Lpsr);      /* LPCG on: LPCG is ON. */

	/* === Debug Port === */
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_00_GPIO8_IO31, 0);		/* J25-15(PIN)			*/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_01_GPIO9_IO00, 0);		/* J25-13(PIN)			*/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_02_GPIO9_IO01, 0);		/* J25-11(PIN)			*/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_03_GPIO9_IO02, 0);		/* J25-9(PIN)			*/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_04_GPIO9_IO03, 0);		/* J25-7(PIN)			*/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_05_GPIO9_IO04, 0);		/* D6(LED), J25-5(PIN)	*/
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_26_GPIO9_IO25, 0);		/* D34(LED)				*/

	mask = ((uint32_t)0u << IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK_SHIFT)	/* Neither of DWP bits is locked	*/
		 | ((uint32_t)0u << IOMUXC_SW_PAD_CTL_PAD_DWP_SHIFT)		/* Both cores are allowed			*/
		 | ((uint32_t)0u << IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT)		/* Open Drain - Disabled			*/
		 | ((uint32_t)1u << IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT)		/* Weak pull up						*/
		 | ((uint32_t)1u << IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT)		/* Pull Enable						*/
		 | ((uint32_t)1u << IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT)		/* normal drive strength			*/
		 | ((uint32_t)1u << IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT);		/* Fast Slew Rate					*/

	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_00_GPIO8_IO31, mask);	/* J25-15(PIN)			*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_01_GPIO9_IO00, mask);	/* J25-13(PIN)			*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_02_GPIO9_IO01, mask);	/* J25-11(PIN)			*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_03_GPIO9_IO02, mask);	/* J25-9(PIN)			*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_04_GPIO9_IO03, mask);	/* J25-7(PIN)			*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_05_GPIO9_IO04, mask);	/* D6(LED), J25-5(PIN)	*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_26_GPIO9_IO25, mask);	/* D34(LED)				*/

#if 0
	/* デバッグポートとして使用する場合、ドライブ速度が遅いことに注意 */
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_00_DIG_GPIO13_IO03, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_01_DIG_GPIO13_IO04, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_02_DIG_GPIO13_IO05, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_03_DIG_GPIO13_IO06, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_04_DIG_GPIO13_IO07, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_05_DIG_GPIO13_IO08, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_06_DIG_GPIO13_IO09, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_07_DIG_GPIO13_IO10, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_08_DIG_GPIO13_IO11, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SNVS_09_DIG_GPIO13_IO12, 0);
#endif
}

static void xboard_module_clock_init(void)
{
	const clock_sys_pll1_config_t sysPll1Config =
	{
		.pllDiv2En = true,
	};

	clock_root_config_t clock_cfg = {0};

	CLOCK_InitSysPll1(&sysPll1Config);

	/* for ENET 100MHz */
	{
		/* Generate 50M root clock. */
		clock_cfg.mux = 4;
		clock_cfg.div = 10;

		CLOCK_SetRootClock(kCLOCK_Root_Enet1, &clock_cfg);
	}

	/* for ENET1G 1000MHz */
	{
		/* Generate 125M root clock. */
		clock_cfg.mux = 4;
		clock_cfg.div = 4;

		CLOCK_SetRootClock(kCLOCK_Root_Enet2, &clock_cfg);
	}
}

static void xboard_enet_clock_init(void)
{
	IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK;	/* 50M ENET_REF_CLOCK output to PHY and ENET module. */
	IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK;	/* bit1:iomuxc_gpr_enet_clk_dir */
}

static void xboard_pll_init(void)
{

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

static void xboard_enet_init(void)
{
	const gpio_pin_config_t gpio_config =
	{
		.direction = kGPIO_DigitalOutput,
		.outputLogic = 0,
		.interruptMode = kGPIO_NoIntmode,
	};

	xboard_pin_init_enet();

	GPIO_PinInit(GPIO12, 12, &gpio_config);

	SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));

	GPIO_WritePinOutput(GPIO12, 12, 1);

	SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));
}

static void xboard_enet1g_init(void)
{
	const gpio_pin_config_t gpio_config =
	{
		.direction = kGPIO_DigitalOutput,
		.outputLogic = 0,
		.interruptMode = kGPIO_NoIntmode,
	};

	xboard_pin_enet1g_init();

	GPIO_PinInit(GPIO11, 14, &gpio_config);

	/* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
	 * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
	SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));

	GPIO_WritePinOutput(GPIO11, 14, 1);

	SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));
}

static inline void xdriver_init_board(void)
{
	BOARD_CommonSetting();

	BOARD_ConfigMPU();

	xboard_pinmux_init();
	xboard_pin_init();

	xboard_pll_init();

	xboard_module_clock_init();
	xboard_enet_clock_init();

	xboard_dma_init();

	xboard_enet1g_init();
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
