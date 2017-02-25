/*
 * arch/arm/mach-tegra/board-touch-cyttsp5_core.c
 *
 * Copyright (c) 2010, NVIDIA Corporation.
 * Modified by: Cypress Semiconductor - 2011-2012
 *    - add auto load image include
 *    - add TMA400 support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>

/* use the following define if the device is for a Cardhu build
#define CY_USE_CARDHU
 */

#ifndef CY_USE_CARDHU
#include "board-ventana.h"
#else
#include "board-cardhu.h"
#endif
#include "gpio-names.h"
#include "touch.h"

#define CYTTSP5_I2C_NAME "cyttsp5_i2c_adapter"

#define CYTTSP5_I2C_TCH_ADR 0x24
#define CYTTSP5_LDR_TCH_ADR 0x24

static const struct i2c_board_info cyttsp5_i2c_info[] = {
	{
		I2C_BOARD_INFO(CYTTSP5_I2C_NAME, CYTTSP5_I2C_TCH_ADR),
		.irq =  TEGRA_GPIO_TO_IRQ(TOUCH_GPIO_IRQ_CYTTSP),
		.platform_data = CYTTSP5_I2C_NAME,
	},
};

struct tegra_touchscreen_init cyttsp_init_data = {
	/* GPIO1 Value for IRQ */
	.irq_gpio = TOUCH_GPIO_IRQ_CYTTSP,
	/* GPIO2 Value for RST */
	.rst_gpio = TOUCH_GPIO_RST_CYTTSP,
	/* Valid, GPIOx, Set value, Delay      */
	.sv_gpio1 = {1, TOUCH_GPIO_RST_CYTTSP, 0, 40},
	/* Valid, GPIOx, Set value, Delay      */
	.sv_gpio2 = {1, TOUCH_GPIO_RST_CYTTSP, 1, 500},
	/* BusNum, BoardInfo, Value */
#ifndef CY_USE_CARDHU
	.ts_boardinfo = {0, cyttsp5_i2c_info, 1}
#else
	.ts_boardinfo = {1, cyttsp5_i2c_info, 1}
#endif
};

