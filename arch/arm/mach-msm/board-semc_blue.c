/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (C) 2012 Sony Ericsson Mobile Communications AB.
 * Copyright (c) 2012 Cypress Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/i2c/isl9519.h>
#include <linux/gpio.h>
#include <linux/msm_ssbi.h>
#include <linux/regulator/gpio-regulator.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <linux/mfd/pm8xxx/pm8xxx-adc.h>
#include <linux/regulator/consumer.h>
#include <linux/spi/spi.h>
#include <linux/slimbus/slimbus.h>
#include <linux/bootmem.h>
#include <linux/memblock.h>
#include <linux/msm_kgsl.h>
#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#endif
#include <linux/clearpad.h>
#include <linux/dma-mapping.h>
#include <linux/platform_data/qcom_crypto_device.h>
#include <linux/platform_data/qcom_wcnss_device.h>
#include <linux/leds.h>
#include <linux/i2c/atmel_mxt_ts.h>
#include <linux/msm_tsens.h>
#ifdef CONFIG_SEMC_ONESEG_TUNER_PM
#include "oneseg-blue_common.h"
#endif
#include <linux/ks8851.h>
#include <linux/memory.h>

#ifdef CONFIG_INPUT_LSM303DLH_ACCELEROMETER
#include  <linux/lsm303dlh_acc.h>
#endif
#ifdef CONFIG_INPUT_LSM303DLHC_ACCELEROMETER
#include  <linux/lsm303dlhc_acc.h>
#endif
#ifdef CONFIG_INPUT_LSM303DLH_MAGNETOMETER
#include  <linux/lsm303dlh_mag.h>
#endif
#ifdef CONFIG_INPUT_L3G4200D
#include  <linux/l3g4200d_gyr.h>
#endif
#ifdef CONFIG_INPUT_BMA250
#include <linux/bma250.h>
#endif
#ifdef CONFIG_INPUT_BMA250_NG
#include <linux/bma250_ng.h>
#endif
#ifdef CONFIG_INPUT_AKM8972
#include <linux/akm8972.h>
#endif
#if defined CONFIG_SENSORS_MPU3050 || defined CONFIG_SENSORS_MPU6050
#include <linux/mpu.h>
#include "gyro-semc_blue.h"
#endif
#ifdef CONFIG_INPUT_APDS9702
#include <linux/apds9702.h>
#endif
#ifdef CONFIG_LEDS_AS3676
#include <linux/leds-as3676.h>
#endif
#ifdef CONFIG_VIBRATOR_LC898300
#include <linux/vibrator-lc898300.h>
#endif

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
#include <asm/setup.h>
#include <asm/hardware/gic.h>
#include <asm/mach/mmc.h>

#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/msm_spi.h>
#ifdef CONFIG_USB_MSM_OTG_72K
#include <mach/msm_hsusb.h>
#else
#include <linux/usb/msm_hsusb.h>
#endif
#include <linux/usb/android.h>
#include <mach/usbdiag.h>
#include <mach/socinfo.h>
#include <mach/rpm.h>
#include <mach/gpio.h>
#include <mach/gpiomux.h>
#include <mach/msm_bus_board.h>
#include <mach/msm_memtypes.h>
#include <mach/dma.h>
#include <mach/msm_dsps.h>
#include <mach/msm_xo.h>
#include <mach/restart.h>
#include <mach/msm_serial_hs.h>
#include <mach/msm_serial_hs_lite.h>
#include <linux/serial_core.h>

#ifdef CONFIG_WCD9310_CODEC
#include <linux/slimbus/slimbus.h>
#include <linux/mfd/wcd9310/core.h>
#include <linux/mfd/wcd9310/pdata.h>
#endif
#include <mach/pm8921-mic_bias.h>
#include <mach/simple_remote_msm8960_pf.h>

#include <linux/ion.h>
#include <mach/ion.h>
#include <mach/mdm-peripheral.h>
#include <linux/fmem.h>

#include "timer.h"
#include "devices.h"
#include "devices-msm8x60.h"
#include "spm.h"
#include "board-8960.h"
#include <mach/pm.h>
#include <mach/cpuidle.h>
#include "rpm_resources.h"
#include "mpm.h"
#include "acpuclock.h"
#include "rpm_log.h"
#include "smd_private.h"
#include "pm-boot.h"
#include "touch-semc_blue.h"
#include "input-semc_blue.h"
#include "charger-semc_blue.h"
#include "pm8921-gpio-mpp-blue.h"
#include "board-semc_blue-vibrator.h"

#include <linux/leds-lm3533_ng.h>
#include <linux/lm356x.h>
#include <linux/input/noa3402.h>

#ifdef CONFIG_NFC_PN544
#include <linux/pn544.h>
#endif
#ifdef CONFIG_USB_NCP373
#include <linux/usb/ncp373.h>
#endif

#ifdef CONFIG_FB_MSM_MHL_SII8334
#include <linux/mhl_sii8334.h>
#include <linux/mfd/pm8xxx/pm8921-charger.h>
#include <linux/usb/msm_hsusb.h>
#endif

#ifdef CONFIG_SEMC_FELICA_SUPPORT
#include "felica-blue.h"
#endif

#ifdef CONFIG_MSM_GSBI10_UART
#include <linux/tty.h>
#endif

#ifdef CONFIG_NFC_PN544
#define PMIC_GPIO_NFC_EN	(33)
#define MSM_GPIO_NFC_FWDL_EN	(19)
#define MSM_GPIO_NFC_IRQ	(106)
#endif

extern void __init gpiomux_device_install(void);
#include "msm_watchdog.h"

static struct platform_device msm_fm_platform_init = {
	.name = "iris_fm",
	.id   = -1,
};

#if 1 /* def CONFIG_CYPRESS_TTSP */
#include <linux/cyttsp5_bus.h>
#include <linux/cyttsp5_core.h>
#include <linux/cyttsp5_i2c.h>
#include <linux/cyttsp5_btn.h>
#include <linux/cyttsp5_mt.h>
#define CYTTSP5_I2C_TCH_ADR 0x24
#define CYTTSP5_I2C_IRQ_GPIO 70	/* sample value from Blue */
#define CYTTSP5_I2C_RST_GPIO 10	/* sample value from Blue */

static int cyttsp5_xres(struct device *dev)
{
	int rc = 0;

	gpio_set_value(CYTTSP5_I2C_RST_GPIO, 1);
	msleep(20);
	gpio_set_value(CYTTSP5_I2C_RST_GPIO, 0);
	msleep(40);
	gpio_set_value(CYTTSP5_I2C_RST_GPIO, 1);
	msleep(20);
	dev_info(dev,
		"%s: RESET CYTTSP gpio=%d r=%d\n", __func__,
		CYTTSP5_I2C_RST_GPIO, rc);
	return rc;
}

static int cyttsp5_init(int on, struct device *dev)
{
	int rc = 0;

	if (on) {
		rc = gpio_request(CYTTSP5_I2C_RST_GPIO, NULL);
		if (rc < 0) {
			gpio_free(CYTTSP5_I2C_RST_GPIO);
			rc = gpio_request(CYTTSP5_I2C_RST_GPIO, NULL);
		}
		if (rc < 0) {
			dev_err(dev,
				"%s: Fail request gpio=%d\n", __func__,
				CYTTSP5_I2C_RST_GPIO);
		} else {
			rc = gpio_direction_output(CYTTSP5_I2C_RST_GPIO, 1);
			if (rc < 0) {
				pr_err("%s: Fail set output gpio=%d\n",
					__func__, CYTTSP5_I2C_RST_GPIO);
				gpio_free(CYTTSP5_I2C_RST_GPIO);
			} else {
				rc = gpio_request(CYTTSP5_I2C_IRQ_GPIO, NULL);
				if (rc < 0) {
					gpio_free(CYTTSP5_I2C_IRQ_GPIO);
					rc = gpio_request(CYTTSP5_I2C_IRQ_GPIO,
						NULL);
				}
				if (rc < 0) {
					dev_err(dev,
						"%s: Fail request gpio=%d\n",
						__func__, CYTTSP5_I2C_IRQ_GPIO);
					gpio_free(CYTTSP5_I2C_RST_GPIO);
				} else {
					gpio_direction_input
						(CYTTSP5_I2C_IRQ_GPIO);
				}
			}
		}
	} else {
		gpio_free(CYTTSP5_I2C_IRQ_GPIO);
		gpio_free(CYTTSP5_I2C_IRQ_GPIO);
	}

	dev_info(dev,
		"%s: INIT CYTTSP RST gpio=%d and IRQ gpio=%d r=%d\n",
		__func__, CYTTSP5_I2C_IRQ_GPIO, CYTTSP5_I2C_RST_GPIO, rc);
	return rc;
}

static int cyttsp5_wakeup(struct device *dev)
{
	int rc = 0;

	rc = gpio_direction_output(CYTTSP5_I2C_IRQ_GPIO, 0);
	if (rc < 0) {
		dev_err(dev,
			"%s: Fail set output gpio=%d\n",
			__func__, CYTTSP5_I2C_IRQ_GPIO);
	} else {
		udelay(2000);
		rc = gpio_direction_input(CYTTSP5_I2C_IRQ_GPIO);
		if (rc < 0) {
			dev_err(dev,
				"%s: Fail set input gpio=%d\n",
				__func__, CYTTSP5_I2C_IRQ_GPIO);
		}
	}

	dev_info(dev,
		"%s: WAKEUP CYTTSP gpio=%d r=%d\n", __func__,
		CYTTSP5_I2C_IRQ_GPIO, rc);
	return rc;
}

static int cyttsp5_sleep(struct device *dev)
{
	return 0;
}

static int cyttsp5_power(int on, struct device *dev)
{
	if (on)
		return cyttsp5_wakeup(dev);

	return cyttsp5_sleep(dev);
}

/* Button to keycode conversion */
static u16 cyttsp5_btn_keys[] = {
	/* use this table to map buttons to keycodes (see input.h) */
	KEY_HOME,		/* 102 */
	KEY_MENU,		/* 139 */
	KEY_BACK,		/* 158 */
	KEY_SEARCH,		/* 217 */
	KEY_VOLUMEDOWN,		/* 114 */
	KEY_VOLUMEUP,		/* 115 */
	KEY_CAMERA,		/* 212 */
	KEY_POWER		/* 116 */
};

static struct touch_settings cyttsp5_sett_btn_keys = {
	.data = (uint8_t *)&cyttsp5_btn_keys[0],
	.size = ARRAY_SIZE(cyttsp5_btn_keys),
	.tag = 0,
};

static struct cyttsp5_core_platform_data _cyttsp5_core_platform_data = {
	.irq_gpio = CYTTSP5_I2C_IRQ_GPIO,
	.xres = cyttsp5_xres,
	.init = cyttsp5_init,
	.power = cyttsp5_power,
	.sett = {
		NULL,	/* Reserved */
		NULL,	/* Command Registers */
		NULL,	/* Touch Report */
		NULL,	/* Cypress Data Record */
		NULL,	/* Test Record */
		NULL,	/* Panel Configuration Record */
		NULL, /* &cyttsp5_sett_param_regs, */
		NULL, /* &cyttsp5_sett_param_size, */
		NULL,	/* Reserved */
		NULL,	/* Reserved */
		NULL,	/* Operational Configuration Record */
		NULL, /* &cyttsp5_sett_ddata, *//* Design Data Record */
		NULL, /* &cyttsp5_sett_mdata, *//* Manufacturing Data Record */
		NULL,	/* Config and Test Registers */
		&cyttsp5_sett_btn_keys,	/* button-to-keycode table */
	},
};

static struct cyttsp5_core cyttsp5_core_device = {
	.name = CYTTSP5_CORE_NAME,
	.id = "main_ttsp_core",
	.adap_id = CYTTSP5_I2C_NAME,
	.dev = {
		.platform_data = &_cyttsp5_core_platform_data,
	},
};

#define CY_MAXX 880
#define CY_MAXY 1280
#define CY_MINX 0
#define CY_MINY 0

#define CY_ABS_MIN_X CY_MINX
#define CY_ABS_MIN_Y CY_MINY
#define CY_ABS_MAX_X CY_MAXX
#define CY_ABS_MAX_Y CY_MAXY
#define CY_ABS_MIN_P 0
#define CY_ABS_MIN_W 0
#define CY_ABS_MAX_P 255
#define CY_ABS_MAX_W 255

#define CY_ABS_MIN_T 0

#define CY_ABS_MAX_T 15

#define CY_IGNORE_VALUE 0xFFFF

static const uint16_t cyttsp5_abs[] = {
	ABS_MT_POSITION_X, CY_ABS_MIN_X, CY_ABS_MAX_X, 0, 0,
	ABS_MT_POSITION_Y, CY_ABS_MIN_Y, CY_ABS_MAX_Y, 0, 0,
	ABS_MT_PRESSURE, CY_ABS_MIN_P, CY_ABS_MAX_P, 0, 0,
	CY_IGNORE_VALUE, CY_ABS_MIN_W, CY_ABS_MAX_W, 0, 0,
	ABS_MT_TRACKING_ID, CY_ABS_MIN_T, CY_ABS_MAX_T, 0, 0,
};

struct touch_framework cyttsp5_framework = {
	.abs = (uint16_t *)&cyttsp5_abs[0],
	.size = ARRAY_SIZE(cyttsp5_abs),
	.enable_vkeys = 0,
};

static struct cyttsp5_mt_platform_data _cyttsp5_mt_platform_data = {
	.frmwrk = &cyttsp5_framework,
	.flags = 0,
	.inp_dev_name = CYTTSP5_MT_NAME,
};

struct cyttsp5_device cyttsp5_mt_device = {
	.name = CYTTSP5_MT_NAME,
	.core_id = "main_ttsp_core",
	.dev = {
		.platform_data = &_cyttsp5_mt_platform_data,
	}
};

static struct cyttsp5_btn_platform_data _cyttsp5_btn_platform_data = {
	.inp_dev_name = CYTTSP5_BTN_NAME,
};

struct cyttsp5_device cyttsp5_btn_device = {
	.name = CYTTSP5_BTN_NAME,
	.core_id = "main_ttsp_core",
	.dev = {
			.platform_data = &_cyttsp5_btn_platform_data,
	}
};

#endif /* CONFIG_CYPRESS_TTSP */

#define FPGA_CS_GPIO		14
#define KS8851_RST_GPIO		89
#define KS8851_IRQ_GPIO		90

#define MSM_8960_GSBI10_QUP_I2C_BUS_ID 10

/* Macros assume PMIC GPIOs and MPPs start at 1 */
#define PM8921_GPIO_BASE		NR_GPIO_IRQS
#define PM8921_GPIO_PM_TO_SYS(pm_gpio)	(pm_gpio - 1 + PM8921_GPIO_BASE)
#define PM8921_MPP_BASE			(PM8921_GPIO_BASE + PM8921_NR_GPIOS)
#define PM8921_MPP_PM_TO_SYS(pm_gpio)	(pm_gpio - 1 + PM8921_MPP_BASE)
#define PM8921_IRQ_BASE			(NR_MSM_IRQS + NR_GPIO_IRQS)
#define PM8921_MPP_IRQ_BASE		(PM8921_IRQ_BASE + NR_GPIO_IRQS)

#define MSM_PMEM_ADSP_SIZE         0x7800000
#define MSM_PMEM_CAMERA_SIZE       0x6400000
#define MSM_PMEM_AUDIO_SIZE        0x2B4000
#define MSM_PMEM_TZ_SIZE           0x100000
#define MSM_PMEM_SIZE 0x2800000 /* 40 Mbytes */


#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
#define MSM_PMEM_KERNEL_EBI1_SIZE	0xB0C000
#define MSM_ION_SF_SIZE			MSM_PMEM_SIZE
#define MSM_ION_MM_FW_SIZE     0x200000 /* (2MB) */
#ifdef CONFIG_SEMC_SWIQI
#define MSM_ION_SWIQI_SIZE		0x2000000 /* 32MB */
#else
#define MSM_ION_SWIQI_SIZE		0
#endif
#define MSM_ION_MM_SIZE		(MSM_PMEM_ADSP_SIZE + MSM_ION_SWIQI_SIZE)
#define MSM_ION_QSECOM_SIZE	0x100000 /* (1MB) */
#define MSM_ION_MFC_SIZE	SZ_8K
#define MSM_ION_AUDIO_SIZE	MSM_PMEM_AUDIO_SIZE
#define MSM_ION_HEAP_NUM	8
static unsigned int msm_ion_cp_mm_size = MSM_ION_MM_SIZE;
#else
#define MSM_PMEM_KERNEL_EBI1_SIZE  0x110C000
#define MSM_ION_HEAP_NUM	1
#endif

#ifdef CONFIG_KERNEL_PMEM_EBI_REGION
static unsigned pmem_kernel_ebi1_size = MSM_PMEM_KERNEL_EBI1_SIZE;
static int __init pmem_kernel_ebi1_size_setup(char *p)
{
	pmem_kernel_ebi1_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_kernel_ebi1_size", pmem_kernel_ebi1_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
static unsigned pmem_size = MSM_PMEM_SIZE;
static int __init pmem_size_setup(char *p)
{
	pmem_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_size", pmem_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;

static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_camera_size = MSM_PMEM_CAMERA_SIZE;

static int __init pmem_camera_size_setup(char *p)
{
	pmem_camera_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_camera_size", pmem_camera_size_setup);

static unsigned pmem_tz_size = MSM_PMEM_TZ_SIZE;

static int __init pmem_tz_size_setup(char *p)
{
	pmem_tz_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_tz_size", pmem_tz_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;

static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_ALLORNOTHING,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = {.platform_data = &android_pmem_pdata},
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};
static struct platform_device android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};
#endif

static struct android_pmem_platform_data android_pmem_camera_pdata = {
	.name = "pmem_camera",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
	.map_on_demand = 1,
};

static struct platform_device android_pmem_camera_device = {
	.name = "android_pmem",
	.id = 3,
	.dev = { .platform_data = &android_pmem_camera_pdata },
};

static struct android_pmem_platform_data android_pmem_tz_pdata = {
	.name = "pmem_tz",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
	.map_on_demand = 1,
};

static struct platform_device android_pmem_tz_device = {
	.name = "android_pmem",
	.id = 5,
	.dev = { .platform_data = &android_pmem_tz_pdata },
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 4,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};
#endif

struct fmem_platform_data fmem_pdata = {
};

#define DSP_RAM_BASE_8960 0x8da00000
#define DSP_RAM_SIZE_8960 0x1800000
static int dspcrashd_pdata_8960 = 0xDEADDEAD;

static struct resource resources_dspcrashd_8960[] = {
	{
		.name   = "msm_dspcrashd",
		.start  = DSP_RAM_BASE_8960,
		.end    = DSP_RAM_BASE_8960 + DSP_RAM_SIZE_8960,
		.flags  = IORESOURCE_DMA,
	},
};

static struct platform_device msm_device_dspcrashd_8960 = {
	.name           = "msm_dspcrashd",
	.num_resources  = ARRAY_SIZE(resources_dspcrashd_8960),
	.resource       = resources_dspcrashd_8960,
	.dev = { .platform_data = &dspcrashd_pdata_8960 },
};

static struct memtype_reserve msm8960_reserve_table[] __initdata = {
	[MEMTYPE_SMI] = {
	},
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};

static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_pdata.size = pmem_size;
#endif
	android_pmem_camera_pdata.size = pmem_camera_size;
	android_pmem_audio_pdata.size = MSM_PMEM_AUDIO_SIZE;
	android_pmem_tz_pdata.size = pmem_tz_size;
#endif
}

static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	msm8960_reserve_table[p->memory_type].size += p->size;
}

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_pdata);
#endif
	reserve_memory_for(&android_pmem_camera_pdata);
	reserve_memory_for(&android_pmem_audio_pdata);
	reserve_memory_for(&android_pmem_tz_pdata);
	msm8960_reserve_table[MEMTYPE_EBI1].size += pmem_kernel_ebi1_size;
#endif
}

static void __init reserve_fmem_memory(void)
{
}

static int msm8960_paddr_to_memtype(unsigned int paddr)
{
	return MEMTYPE_EBI1;
}

#ifdef CONFIG_ION_MSM
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct ion_cp_heap_pdata cp_mm_ion_pdata = {
	.permission_type = IPT_TYPE_MM_CARVEOUT,
	.align = PAGE_SIZE,
};

static struct ion_cp_heap_pdata cp_mfc_ion_pdata = {
	.permission_type = IPT_TYPE_MFC_SHAREDMEM,
	.align = PAGE_SIZE,
};

static struct ion_co_heap_pdata co_ion_pdata = {
	.adjacent_mem_id = INVALID_HEAP_ID,
	.align = PAGE_SIZE,
};

static struct ion_co_heap_pdata fw_co_ion_pdata = {
	.adjacent_mem_id = ION_CP_MM_HEAP_ID,
	.align = SZ_128K,
};
#endif

static struct ion_platform_data ion_pdata = {
	.nr = MSM_ION_HEAP_NUM,
	.heaps = {
		{
			.id		= ION_SYSTEM_HEAP_ID,
			.type		= ION_HEAP_TYPE_SYSTEM,
			.name		= ION_VMALLOC_HEAP_NAME,
		},
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
		{
			.id		= ION_SF_HEAP_ID,
			.type		= ION_HEAP_TYPE_CARVEOUT,
			.name		= ION_SF_HEAP_NAME,
			.size		= MSM_ION_SF_SIZE,
			.memory_type	= ION_EBI_TYPE,
			.extra_data	= (void *) &co_ion_pdata,
		},
		{
			.id		= ION_CP_MM_HEAP_ID,
			.type		= ION_HEAP_TYPE_CP,
			.name		= ION_MM_HEAP_NAME,
			.size		= MSM_ION_MM_SIZE,
			.memory_type	= ION_EBI_TYPE,
			.extra_data	= (void *) &cp_mm_ion_pdata,
		},
		{
			.id     = ION_MM_FIRMWARE_HEAP_ID,
			.type   = ION_HEAP_TYPE_CARVEOUT,
			.name   = ION_MM_FIRMWARE_HEAP_NAME,
			.size   = MSM_ION_MM_FW_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &fw_co_ion_pdata,
		},
		{
			.id		= ION_CP_MFC_HEAP_ID,
			.type		= ION_HEAP_TYPE_CP,
			.name		= ION_MFC_HEAP_NAME,
			.size		= MSM_ION_MFC_SIZE,
			.memory_type	= ION_EBI_TYPE,
			.extra_data	= (void *) &cp_mfc_ion_pdata,
		},
		{
			.id		= ION_IOMMU_HEAP_ID,
			.type		= ION_HEAP_TYPE_IOMMU,
			.name		= ION_IOMMU_HEAP_NAME,
		},
		{
			.id     = ION_QSECOM_HEAP_ID,
			.type   = ION_HEAP_TYPE_CARVEOUT,
			.name   = ION_QSECOM_HEAP_NAME,
			.size   = MSM_ION_QSECOM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_ion_pdata,
		},
		{
			.id     = ION_AUDIO_HEAP_ID,
			.type   = ION_HEAP_TYPE_CARVEOUT,
			.name   = ION_AUDIO_HEAP_NAME,
			.size   = MSM_ION_AUDIO_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_ion_pdata,
		},

#endif
	}
};

static struct platform_device ion_dev = {
	.name = "ion-msm",
	.id = 1,
	.dev = { .platform_data = &ion_pdata },
};
#endif

struct platform_device fmem_device = {
	.name = "fmem",
	.id = 1,
	.dev = { .platform_data = &fmem_pdata },
};

static void reserve_ion_memory(void)
{
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	msm8960_reserve_table[MEMTYPE_EBI1].size += msm_ion_cp_mm_size;
	msm8960_reserve_table[MEMTYPE_EBI1].size += MSM_ION_MM_FW_SIZE;
	msm8960_reserve_table[MEMTYPE_EBI1].size += MSM_ION_SF_SIZE;
	msm8960_reserve_table[MEMTYPE_EBI1].size += MSM_ION_MFC_SIZE;
	msm8960_reserve_table[MEMTYPE_EBI1].size += MSM_ION_QSECOM_SIZE;
	msm8960_reserve_table[MEMTYPE_EBI1].size += MSM_ION_AUDIO_SIZE;
#endif
}

static void __init reserve_mdp_memory(void)
{
	msm8960_mdp_writeback(msm8960_reserve_table);
}

static void __init msm8960_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
	reserve_ion_memory();
	reserve_fmem_memory();
	reserve_mdp_memory();
}

static struct reserve_info msm8960_reserve_info __initdata = {
	.memtype_reserve_table = msm8960_reserve_table,
	.calculate_reserve_sizes = msm8960_calculate_reserve_sizes,
	.paddr_to_memtype = msm8960_paddr_to_memtype,
};

static int msm8960_memory_bank_size(void)
{
	return 1<<29;
}

static int msm_sdc3_get_wpswitch(struct device *dev)
{
	/*
	*  Since we don't have an switch on blue for detecting
	*  if sdcard is read only we should always return 0
	*  which means it's never read only.
	*/
	int status = 0;
	return status;
}

static void __init locate_unstable_memory(void)
{
	struct membank *mb = &meminfo.bank[meminfo.nr_banks - 1];
	unsigned long bank_size;
	unsigned long low, high;

	bank_size = msm8960_memory_bank_size();
	low = meminfo.bank[0].start;
	high = mb->start + mb->size;

	/* Check if 32 bit overflow occured */
	if (high < mb->start)
		high = ~0UL;

	low &= ~(bank_size - 1);

	if (high - low <= bank_size)
		return;

	msm8960_reserve_info.low_unstable_address = high -
						MIN_MEMORY_BLOCK_SIZE / 2;
	msm8960_reserve_info.max_unstable_size = MIN_MEMORY_BLOCK_SIZE / 2;

	msm8960_reserve_info.bank_size = bank_size;
	pr_info("low unstable address %lx max size %lx bank size %lx\n",
		msm8960_reserve_info.low_unstable_address,
		msm8960_reserve_info.max_unstable_size,
		msm8960_reserve_info.bank_size);
}

static void __init place_movable_zone(void)
{
	movable_reserved_start = msm8960_reserve_info.low_unstable_address;
	movable_reserved_size = msm8960_reserve_info.max_unstable_size;
	pr_info("movable zone start %lx size %lx\n",
		movable_reserved_start, movable_reserved_size);
}

static void __init msm8960_early_memory(void)
{
	reserve_info = &msm8960_reserve_info;
	locate_unstable_memory();
	place_movable_zone();
}

#define MSM_LASTLOGS_BASE 0x88E00000
#define MSM_LASTLOGS_SIZE SZ_1M

#ifdef CONFIG_ANDROID_RAM_CONSOLE
#define MSM_RAM_CONSOLE_START   (MSM_LASTLOGS_BASE + MSM_LASTLOGS_SIZE \
				 - MSM_RAM_CONSOLE_SIZE)
#define MSM_RAM_CONSOLE_SIZE    (128 * SZ_1K)

static struct resource ram_console_resources[] = {
	[0] = {
		.start  = MSM_RAM_CONSOLE_START,
		.end    = MSM_RAM_CONSOLE_START + MSM_RAM_CONSOLE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
};

static struct platform_device ram_console_device = {
	.name           = "ram_console",
	.id             = -1,
};
#endif

#ifdef CONFIG_RAMDUMP_CRASH_LOGS
#define MSM_RAMDUMP_INFO_START	MSM_LASTLOGS_BASE
#define MSM_RAMDUMP_INFO_SIZE	(4 * SZ_1K)
#define MSM_AMSS_LOG_START	(MSM_RAMDUMP_INFO_START + MSM_RAMDUMP_INFO_SIZE)
#define MSM_AMSS_LOG_SIZE	(16 * SZ_1K)

static struct resource ramdumplog_resources[] = {
	[0] = {
		.name   = "ramdumpinfo",
		.start  = MSM_RAMDUMP_INFO_START,
		.end    = MSM_RAMDUMP_INFO_START + MSM_RAMDUMP_INFO_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.name   = "amsslog",
		.start  = MSM_AMSS_LOG_START,
		.end    = MSM_AMSS_LOG_START + MSM_AMSS_LOG_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
};

static struct platform_device ramdumplog_device = {
	.name           = "ramdumplog",
	.id             = -1,
};
#endif

static void reserve_memory_lastlogs(void)
{
	if (memblock_reserve(MSM_LASTLOGS_BASE, MSM_LASTLOGS_SIZE)) {
		printk(KERN_ERR "ram_console reserve memory failed\n");
		return;
	}

	memblock_free(MSM_LASTLOGS_BASE, MSM_LASTLOGS_SIZE);
	memblock_remove(MSM_LASTLOGS_BASE, MSM_LASTLOGS_SIZE);
#ifdef CONFIG_ANDROID_RAM_CONSOLE
	ram_console_device.num_resources  = ARRAY_SIZE(ram_console_resources);
	ram_console_device.resource       = ram_console_resources;
#endif
#ifdef CONFIG_RAMDUMP_CRASH_LOGS
	ramdumplog_device.num_resources  = ARRAY_SIZE(ramdumplog_resources);
	ramdumplog_device.resource       = ramdumplog_resources;
#endif
}

static void __init msm8960_reserve(void)
{
	msm_reserve();
	reserve_memory_lastlogs();
	fmem_pdata.phys = reserve_memory_for_fmem(fmem_pdata.size);
}

static int msm8960_change_memory_power(u64 start, u64 size,
	int change_type)
{
	return soc_change_memory_power(start, size, change_type);
}

#ifdef CONFIG_MSM_CAMERA
#if defined(CONFIG_SEMC_VPE)
struct resource semc_vpe_resources[] = {
	{
		.start	= 0x05300000,
		.end	= 0x05300000 + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= VPE_IRQ,
		.end	= VPE_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device semc_vpe_device = {
	.name		= "semc_vpe",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(semc_vpe_resources),
	.resource	= semc_vpe_resources,
};
#endif

#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
static struct gpiomux_setting gpio_2ma_no_pull_low = {
	.func	= GPIOMUX_FUNC_GPIO,
	.drv	= GPIOMUX_DRV_2MA,
	.pull	= GPIOMUX_PULL_NONE,
	.dir	= GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting cam_mclk1 = {
	.func	= GPIOMUX_FUNC_2,
	.drv	= GPIOMUX_DRV_6MA,
	.pull	= GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting cam_mclk0 = {
	.func	= GPIOMUX_FUNC_1,
	.drv	= GPIOMUX_DRV_6MA,
	.pull	= GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi4 = {
	.func	= GPIOMUX_FUNC_1,
	.drv	= GPIOMUX_DRV_4MA,
	.pull	= GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_2ma_pull_down_in = {
	.func	= GPIOMUX_FUNC_GPIO,
	.drv	= GPIOMUX_DRV_2MA,
	.pull	= GPIOMUX_PULL_DOWN,
	.dir	= GPIOMUX_IN,
};

static struct msm_gpiomux_config msm_camera_2d_configs[] = {
	{
		.gpio		= 1,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_no_pull_low,
		},
	},
	{
		.gpio		= 4,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &cam_mclk1,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_no_pull_low,
		},
	},
	{
		.gpio		= 5,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &cam_mclk0,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_no_pull_low,
		},
	},
	{
		.gpio		= 18,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_no_pull_low,
		},
	},
	{
		.gpio		= 20,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &gsbi4,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_pull_down_in,
		},
	},
	{
		.gpio		= 21,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &gsbi4,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_pull_down_in,
		},
	},
/* TODO: need to implement regarding to different hardware */
#if defined(CONFIG_MACH_SEMC_MINT)
	{
		.gpio		= 89,
		.settings	= {
			[GPIOMUX_ACTIVE]	= &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED]	= &gpio_2ma_no_pull_low,
		},
	},
#endif
};

static uint16_t msm_camera_gpio_2d_tbl[] = {
	1,	/* MCAM_RST_N */
	4,	/* CAM_MCLK1 */
	5,	/* CAM_MCLK0 */
	18,	/* CHAT_CAM_RST_N */
	20,	/* I2C_DATA_CAM */
	21,	/* I2C_CLK_CAM */
/* TODO: need to implement regarding to different hardware */
#if defined(CONFIG_MACH_SEMC_MINT)
	89,	/* CAMERA_AF_CE */
#endif
};

static struct msm_camera_gpio_conf msm_camera_gpio_conf_data = {
	.cam_gpiomux_conf_tbl		= msm_camera_2d_configs,
	.cam_gpiomux_conf_tbl_size	= ARRAY_SIZE(msm_camera_2d_configs),
	.cam_gpio_tbl			= msm_camera_gpio_2d_tbl,
	.cam_gpio_tbl_size		= ARRAY_SIZE(msm_camera_gpio_2d_tbl),
};

static struct msm_bus_vectors msm_camera_init_vectors[] = {
	{
		.src	= MSM_BUS_MASTER_VFE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
	{
		.src	= MSM_BUS_MASTER_VPE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
	{
		.src	= MSM_BUS_MASTER_JPEG_ENC,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
};

static struct msm_bus_vectors msm_camera_preview_vectors[] = {
	{
		.src	= MSM_BUS_MASTER_VFE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 27648000,
		.ib	= 110592000,
	},
	{
		.src	= MSM_BUS_MASTER_VPE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
	{
		.src	= MSM_BUS_MASTER_JPEG_ENC,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
};

static struct msm_bus_vectors msm_camera_video_vectors[] = {
	{
		.src	= MSM_BUS_MASTER_VFE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 1521190000,
		.ib	= 1521190000,
	},
	{
		.src	= MSM_BUS_MASTER_VPE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 1521190000,
		.ib	= 1521190000,
	},
	{
		.src	= MSM_BUS_MASTER_JPEG_ENC,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
};

static struct msm_bus_vectors msm_camera_snapshot_vectors[] = {
	{
		.src	= MSM_BUS_MASTER_VFE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 1521190000,
		.ib	= 1521190000,
	},
	{
		.src	= MSM_BUS_MASTER_VPE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
	{
		.src	= MSM_BUS_MASTER_JPEG_ENC,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 1521190000,
		.ib	= 1521190000,
	},
};

static struct msm_bus_vectors msm_camera_zsl_vectors[] = {
	{
		.src	= MSM_BUS_MASTER_VFE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 302071680,
		.ib	= 1208286720,
	},
	{
		.src	= MSM_BUS_MASTER_VPE,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 0,
		.ib	= 0,
	},
	{
		.src	= MSM_BUS_MASTER_JPEG_ENC,
		.dst	= MSM_BUS_SLAVE_EBI_CH0,
		.ab	= 1521190000,
		.ib	= 1521190000,
	},
};

static struct msm_bus_paths msm_camera_bus_client_config[] = {
	{
		ARRAY_SIZE(msm_camera_init_vectors),
		msm_camera_init_vectors,
	},
	{
		ARRAY_SIZE(msm_camera_preview_vectors),
		msm_camera_preview_vectors,
	},
	{
		ARRAY_SIZE(msm_camera_video_vectors),
		msm_camera_video_vectors,
	},
	{
		ARRAY_SIZE(msm_camera_snapshot_vectors),
		msm_camera_snapshot_vectors,
	},
	{
		ARRAY_SIZE(msm_camera_zsl_vectors),
		msm_camera_zsl_vectors,
	},
};

static struct msm_bus_scale_pdata msm_camera_bus_client_pdata = {
	msm_camera_bus_client_config,
	ARRAY_SIZE(msm_camera_bus_client_config),
	.name = "msm_camera",
};

struct msm_camera_device_platform_data msm_camera_csi_device_data[] = {
	{
		.ioext.csiphy		= 0x04800000,
		.ioext.csisz		= 0x00000400,
		.ioext.csiirq		= CSI_0_IRQ,
		.ioclk.mclk_clk_rate	= 8000000,
		.ioclk.vfe_clk_rate	= 228570000,
		.csid_core		= 0,
		.cam_bus_scale_table	= &msm_camera_bus_client_pdata,
	},
	{
		.ioext.csiphy		= 0x04900000,
		.ioext.csisz		= 0x00000400,
		.ioext.csiirq		= CSI_1_IRQ,
		.ioclk.mclk_clk_rate	= 8000000,
		.ioclk.vfe_clk_rate	= 228570000,
		.csid_core		= 1,
		.cam_bus_scale_table	= &msm_camera_bus_client_pdata,
	},
};

static struct msm_camera_sensor_flash_data msm_camera_flash_none = {
	.flash_type	= MSM_CAMERA_FLASH_NONE,
	.flash_src	= NULL,
};
#endif

#ifdef CONFIG_SEMC_CAM_MAIN_V4L2
static struct msm_camera_sensor_platform_info
		msm_camera_semc_sensor_main_platform_info = {
	.mount_angle		= 90,
	.sensor_reset_enable	= 1,
	.sensor_reset		= 1,
	.sensor_pwd		= 1,
	.vcm_pwd		= 0,
	.vcm_enable		= 1,
	.privacy_light		= 0,
	.privacy_light_info	= NULL,
};

static struct msm_camera_sensor_info
		msm_camera_semc_sensor_main_sensor_info = {
	.sensor_name		= "semc_sensor_main",
	.sensor_reset		= 1,
	.sensor_pwd		= 1,
	.mclk			= 5,
	.sensor_platform_info	= &msm_camera_semc_sensor_main_platform_info,
	.pdata			= &msm_camera_csi_device_data[0],
	.flash_data		= &msm_camera_flash_none,
	.csi_if			= 1,
	.gpio_conf		= &msm_camera_gpio_conf_data,
	.camera_type		= BACK_CAMERA_2D,
};
#endif

#ifdef CONFIG_SEMC_CAM_SUB_V4L2
static struct msm_camera_sensor_platform_info
		msm_camera_semc_sensor_sub_platform_info = {
	.mount_angle		= 270,
	.sensor_reset_enable	= 1,
	.sensor_reset		= 18,
	.sensor_pwd		= 1,
	.vcm_pwd		= 0,
	.vcm_enable		= 1,
	.privacy_light		= 0,
	.privacy_light_info	= NULL,
};

static struct msm_camera_sensor_info
		msm_camera_semc_sensor_sub_sensor_info = {
	.sensor_name		= "semc_sensor_sub",
	.sensor_reset		= 18,
	.sensor_pwd		= 1,
	.mclk			= 4,
	.sensor_platform_info	= &msm_camera_semc_sensor_sub_platform_info,
	.pdata			= &msm_camera_csi_device_data[1],
	.flash_data		= &msm_camera_flash_none,
	.csi_if			= 1,
	.gpio_conf		= &msm_camera_gpio_conf_data,
	.camera_type		= FRONT_CAMERA_2D,
};
#endif

void __init msm8960_init_cam(void)
{
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	msm_gpiomux_install(msm_camera_2d_configs,
			ARRAY_SIZE(msm_camera_2d_configs));

	platform_device_register(&msm8960_device_csiphy0);
	platform_device_register(&msm8960_device_csiphy1);
	platform_device_register(&msm8960_device_csid0);
	platform_device_register(&msm8960_device_csid1);
	platform_device_register(&msm8960_device_ispif);
	platform_device_register(&msm8960_device_vfe);
#if defined(CONFIG_SEMC_VPE)
	platform_device_register(&semc_vpe_device);
#else
	platform_device_register(&msm8960_device_vpe);
#endif
#else
	int i;
	struct platform_device *cam_dev[] = {
	};

	for (i = 0; i < ARRAY_SIZE(cam_dev); i++) {
		struct msm_camera_sensor_info *s_info;
		s_info = cam_dev[i]->dev.platform_data;
		msm_get_cam_resources(s_info);
		platform_device_register(cam_dev[i]);
	}
#endif
}
#endif

#ifdef CONFIG_FB_MSM_TRIPLE_BUFFER
#define MSM_FB_PRIM_BUF_SIZE (1280 * 736 * 4 * 3) /* 4 bpp x 3 pages */
#else
#define MSM_FB_PRIM_BUF_SIZE (1280 * 736 * 4 * 2) /* 4 bpp x 2 pages */
#endif


#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
#define MSM_FB_EXT_BUF_SIZE	(1920 * 1088 * 2 * 1) /* 2 bpp x 1 page */
#elif defined(CONFIG_FB_MSM_TVOUT)
#define MSM_FB_EXT_BUF_SIZE (720 * 576 * 2 * 2) /* 2 bpp x 2 pages */
#else
#define MSM_FB_EXT_BUF_SIZE	0
#endif

#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
/* 4 bpp x 2 page HDMI case */
#define MSM_FB_SIZE roundup((1920 * 1088 * 4 * 2), 4096)
#else
/* Note: must be multiple of 4096 */
#define MSM_FB_SIZE roundup(MSM_FB_PRIM_BUF_SIZE + MSM_FB_EXT_BUF_SIZE, 4096)
#endif

#ifdef CONFIG_FB_MSM_OVERLAY0_WRITEBACK
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((1280 * 736 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY0_WRITEBACK */

#ifdef CONFIG_FB_MSM_OVERLAY1_WRITEBACK
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE roundup((1920 * 1088 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY1_WRITEBACK */

#define MDP_VSYNC_GPIO 0

#define PANEL_NAME_MAX_LEN	30
#define MIPI_CMD_NOVATEK_QHD_PANEL_NAME	"mipi_cmd_novatek_qhd"
#define MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME	"mipi_video_novatek_qhd"
#define MIPI_VIDEO_TOSHIBA_WSVGA_PANEL_NAME	"mipi_video_toshiba_wsvga"
#define MIPI_VIDEO_CHIMEI_WXGA_PANEL_NAME	"mipi_video_chimei_wxga"
#define MIPI_VIDEO_SIMULATOR_VGA_PANEL_NAME	"mipi_video_simulator_vga"
#define MIPI_CMD_RENESAS_FWVGA_PANEL_NAME	"mipi_cmd_renesas_fwvga"
#define HDMI_PANEL_NAME	"hdmi_msm"
#define TVOUT_PANEL_NAME	"tvout_msm"

static struct resource msm_fb_resources[] = {
	{
		.flags = IORESOURCE_DMA,
	}
};

static int msm_fb_detect_panel(const char *name)
{
	if (machine_is_msm8960_liquid()) {
		if (!strncmp(name, MIPI_VIDEO_CHIMEI_WXGA_PANEL_NAME,
				strnlen(MIPI_VIDEO_CHIMEI_WXGA_PANEL_NAME,
					PANEL_NAME_MAX_LEN)))
			return 0;
	} else {
		if (!strncmp(name, MIPI_VIDEO_TOSHIBA_WSVGA_PANEL_NAME,
				strnlen(MIPI_VIDEO_TOSHIBA_WSVGA_PANEL_NAME,
					PANEL_NAME_MAX_LEN)))
			return 0;

#ifndef CONFIG_FB_MSM_MIPI_PANEL_DETECT
		if (!strncmp(name, MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME,
				strnlen(MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME,
					PANEL_NAME_MAX_LEN)))
			return 0;

		if (!strncmp(name, MIPI_CMD_NOVATEK_QHD_PANEL_NAME,
				strnlen(MIPI_CMD_NOVATEK_QHD_PANEL_NAME,
					PANEL_NAME_MAX_LEN)))
			return 0;

		if (!strncmp(name, MIPI_VIDEO_SIMULATOR_VGA_PANEL_NAME,
				strnlen(MIPI_VIDEO_SIMULATOR_VGA_PANEL_NAME,
					PANEL_NAME_MAX_LEN)))
			return 0;

		if (!strncmp(name, MIPI_CMD_RENESAS_FWVGA_PANEL_NAME,
				strnlen(MIPI_CMD_RENESAS_FWVGA_PANEL_NAME,
					PANEL_NAME_MAX_LEN)))
			return 0;
#endif
	}

	if (!strncmp(name, HDMI_PANEL_NAME,
			strnlen(HDMI_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	if (!strncmp(name, TVOUT_PANEL_NAME,
			strnlen(TVOUT_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	pr_warning("%s: not supported '%s'", __func__, name);
	return -ENODEV;
}

static struct msm_fb_platform_data msm_fb_pdata = {
	.detect_client = msm_fb_detect_panel,
};

static struct platform_device msm_fb_device = {
	.name   = "msm_fb",
	.id     = 0,
	.num_resources     = ARRAY_SIZE(msm_fb_resources),
	.resource          = msm_fb_resources,
	.dev.platform_data = &msm_fb_pdata,
};

#define MLCD_RESET_N 43
#define LCD_PWR_EN 36
#define LCD_VREG_ON_WAIT_MS 10
#define LCD_RESET_WAIT_MS 10
#define LCD_POWER_WAIT_MS 50

static struct pm_gpio gpio43_param = {
	.direction = PM_GPIO_DIR_OUT,
	.output_buffer = PM_GPIO_OUT_BUF_CMOS,
	.output_value = 0,
	.pull = PM_GPIO_PULL_NO,
	.vin_sel = PM_GPIO_VIN_S4,
	.out_strength = PM_GPIO_STRENGTH_LOW,
	.function = PM_GPIO_FUNC_NORMAL,
	.inv_int_pol = 0,
	.disable_pin = 0,
};

static int gpio_lcd_reset;
static struct regulator *vreg_lcd_vci;
static struct regulator *vreg_lcd_vddio;

static int lcd_gpio_setup(int request)
{
	int rc = 0;

	if (!gpio_lcd_reset)
		gpio_lcd_reset = PM8921_GPIO_PM_TO_SYS(MLCD_RESET_N);

	if (request) {
		/* LCD_PWR_EN */
		rc = gpio_request(LCD_PWR_EN, "lcd power gpio");
		if (rc) {
			pr_err("%s: GPIO %d: request failed. rc=%d\n",
				__func__, LCD_PWR_EN, rc);
			return rc;
		}
		rc = gpio_direction_output(LCD_PWR_EN, 0);
		if (rc) {
			pr_err("%s: GPIO %d: direction out failed. rc=%d\n",
				__func__, LCD_PWR_EN, rc);
			goto out_pwr;
		}

		rc = pm8xxx_gpio_config(gpio_lcd_reset, &gpio43_param);
		if (rc) {
			pr_err("gpio_config 43 failed, rc=%d\n", rc);
			return -EINVAL;
		}

		/* gpio_lcd_reset */
		rc = gpio_request(gpio_lcd_reset, "lcd reset");
		if (rc) {
			pr_err("%s: GPIO %d: request failed. rc=%d\n",
				__func__, gpio_lcd_reset, rc);
			goto out_pwr;
		}
	} else {
		gpio_free(LCD_PWR_EN);
		gpio_free(gpio_lcd_reset);
		gpio_lcd_reset = 0;
	}

	return rc;
out_pwr:
	gpio_free(LCD_PWR_EN);
	return rc;
}

static int lcd_power(int on)
{
	if (on)
		gpio_set_value(LCD_PWR_EN, 1);
	else
		gpio_set_value(LCD_PWR_EN, 0);
	msleep(LCD_POWER_WAIT_MS);

	return 0;
}

static int lcd_reset(int on)
{
	int rc;

	if (!gpio_lcd_reset) {
		rc = lcd_gpio_setup(1);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			return -EINVAL;
		}
	}

	if (on) {
		gpio_set_value_cansleep(gpio_lcd_reset, 0);
		gpio_set_value_cansleep(gpio_lcd_reset, 1);
	} else {
		gpio_set_value_cansleep(gpio_lcd_reset, 1);
		gpio_set_value_cansleep(gpio_lcd_reset, 0);
	}
	msleep(LCD_RESET_WAIT_MS);

	return 0;
}

/*
 * MIPI_DSI only use 8058_LDO2 which need always on
 * therefore it need to be put at low power mode if
 * it was not used instead of turn it off.
 */
static int mipi_dsi_power(int on)
{
	static int curr_power;
	static struct regulator *ldo2;
	int rc;

	if (curr_power == on)
		return 0;

	if (ldo2 == NULL) {	/* init */
		ldo2 = regulator_get(&msm_mipi_dsi1_device.dev,
			"dsi_vdda");
		if (IS_ERR(ldo2)) {
			pr_err("%s: dsi_vdda failed\n", __func__);
			rc = PTR_ERR(ldo2);
			return rc;
		}
	}

	if (on) {
		rc = regulator_set_voltage(ldo2, 1200000, 1200000);
		if (rc) {
			pr_err("%s: Unable to set voltage level for dsi_vdda\n",
				__func__);
			goto out_put;
		}

		rc = regulator_enable(ldo2);
		if (rc) {
			pr_err("%s: Unable to enable dsi_vdda\n", __func__);
			goto out_put;
		}
	} else {
		rc = regulator_disable(ldo2);
		if (rc)
			pr_warning("%s: '%s' regulator disable failed, rc=%d\n",
				__func__, "dsi_vdda", rc);
	}

	curr_power = on;

	return 0;
out_put:
	regulator_put(ldo2);
	ldo2 = NULL;
	return rc;
}

#ifdef CONFIG_FB_MSM_MIPI_DSI_VENDER_DET

#define LCD_VENDOR_DET 66
#define LCD_VENDOR_SAMSUNG 1
#define LCD_VENDOR_RENESAS 0
#define LCD_VENDER_DET_WAIT_MS 50
#define LCD_VENDER_DISABLE_VREG_WAIT_MS 10

static int semc_blue_is_dric_det(void)
{
	int rc = 0;
	static int val = -ENODEV;

	if (val != -ENODEV)
		return val;

	if (!vreg_lcd_vci) {
		vreg_lcd_vci = regulator_get(NULL, "8921_l8");
		if (IS_ERR(vreg_lcd_vci)) {
			pr_err("%s: Unable to get 8921_l8\n", __func__);
			vreg_lcd_vci = NULL;
			return -ENODEV;
		}
	}

	if (!vreg_lcd_vddio) {
		vreg_lcd_vddio = regulator_get(NULL, "8921_l29");
		if (IS_ERR(vreg_lcd_vddio)) {
			pr_err("%s: Unable to get 8921_l29\n", __func__);
			vreg_lcd_vddio = NULL;
			goto out_put_l8;
		}
	}

	rc = regulator_set_voltage(vreg_lcd_vci, 2850000, 2850000);
	if (rc) {
		pr_err("%s:%d unable to set L8 voltage to 2.85V\n",
			__func__, rc);
		goto out_put_l29;
	}

	rc = regulator_set_voltage(vreg_lcd_vddio, 1800000, 1800000);
	if (rc) {
		pr_err("%s:%d unable to set L29 voltage to 1.8V\n",
			__func__, rc);
		goto out_put_l29;
	}

	rc = regulator_enable(vreg_lcd_vci);
	if (rc) {
		pr_err("%s: Enable regulator 8921_l8 failed\n",
			__func__);
		goto out_put_l29;
	}

	rc = regulator_enable(vreg_lcd_vddio);
	if (rc) {
		pr_err("%s: Enable regulator 8921_l29 failed\n",
			__func__);
		goto out_put_l29;
	}

	lcd_gpio_setup(TRUE);

	msleep(LCD_VENDER_DET_WAIT_MS);

	/* LCD_VENDOR_DET */
	rc = gpio_request(LCD_VENDOR_DET, "lcd vendor detect");
	if (rc) {
		pr_err("%s: GPIO %d: request failed. rc=%d\n",
			__func__, LCD_VENDOR_DET, rc);
		goto out_put_l29;
	}
	rc = gpio_direction_input(LCD_VENDOR_DET);
	if (rc) {
		pr_err("%s: GPIO %d: direction in failed. rc=%d\n",
			__func__, LCD_VENDOR_DET, rc);
		goto out_free;
	}

	val = gpio_get_value(LCD_VENDOR_DET);
	pr_info("%s: GPIO:%d\n", __func__, val);

	rc = regulator_disable(vreg_lcd_vddio);
	if (rc)
		pr_err("%s: Enable regulator 8921_l29 failed\n",
			__func__);
	rc = regulator_disable(vreg_lcd_vci);
	if (rc)
		pr_err("%s: Enable regulator 8921_l8 failed\n",
			__func__);

	lcd_gpio_setup(FALSE);

out_free:
	gpio_free(LCD_VENDOR_DET);
out_put_l29:
	regulator_put(vreg_lcd_vddio);
	vreg_lcd_vddio = NULL;
out_put_l8:
	regulator_put(vreg_lcd_vci);
	vreg_lcd_vci = NULL;

	msleep(LCD_VENDER_DISABLE_VREG_WAIT_MS);

	return val;
}

#endif /* CONFIG_FB_MSM_MIPI_DSI_VENDER_DET */

#ifdef CONFIG_FB_MSM_MIPI_DSI_RENESAS_R63306

#define MIPI_DSI_RENESAS_NAME "mipi_renesas_r63306"

static int r63306_vreg_power(int on)
{
	int rc = 0;

	if (!vreg_lcd_vci) {
		vreg_lcd_vci = regulator_get(&msm_mipi_dsi1_device.dev,
			"dsi_vci");
		if (IS_ERR(vreg_lcd_vci)) {
			pr_err("could not get dsi_vci, rc = %ld\n",
				PTR_ERR(vreg_lcd_vci));
			return -ENODEV;
		}
	}

	if (!vreg_lcd_vddio) {
		vreg_lcd_vddio = regulator_get(&msm_mipi_dsi1_device.dev,
			"dsi_vddio");
		if (IS_ERR(vreg_lcd_vddio)) {
			pr_err("%s: Unable to get dsi_vddio\n", __func__);
			vreg_lcd_vddio = NULL;
			goto out_put;
		}
	}

	if (on) {
		rc = regulator_set_voltage(vreg_lcd_vci, 2850000, 2850000);
		if (rc) {
			pr_err("%s:%d unable to set dsi_vci voltage to 2.8V\n",
				__func__, rc);
			goto out_put_all;
		}

		rc = regulator_enable(vreg_lcd_vci);
		if (rc) {
			pr_err("%s: Enable regulator dsi_vci failed\n",
				__func__);
			goto out_put_all;
		}

		rc = regulator_set_voltage(vreg_lcd_vddio, 1800000, 1800000);
		if (rc) {
			pr_err("%s:%d unable to set dsi_vddio voltage to 1.8V\n",
				__func__, rc);
			goto out_disable;
		}

		rc = regulator_enable(vreg_lcd_vddio);
		if (rc) {
			pr_err("%s: Enable regulator dsi_vddio failed\n",
				__func__);
			goto out_disable;
		}

		rc = lcd_gpio_setup(on);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			goto out_disable_all;
		}

		msleep(LCD_VREG_ON_WAIT_MS);

		lcd_reset(on);
	} else {
		msleep(LCD_RESET_WAIT_MS);
		lcd_reset(on);

		rc = lcd_gpio_setup(on);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			return -EINVAL;
		}

		rc = regulator_disable(vreg_lcd_vddio);
		if (rc)
			pr_warning("%s: '%s' regulator disable failed, rc=%d\n",
				__func__, "dsi_vddio", rc);
		rc = regulator_disable(vreg_lcd_vci);
		if (rc)
			pr_warning("%s: '%s' regulator disable failed, rc=%d\n",
				__func__, "dsi_vci", rc);
	}

	return 0;
out_disable_all:
	regulator_disable(vreg_lcd_vddio);
out_disable:
	regulator_disable(vreg_lcd_vci);
out_put_all:
	regulator_put(vreg_lcd_vddio);
	vreg_lcd_vddio = NULL;
out_put:
	regulator_put(vreg_lcd_vci);
	vreg_lcd_vci = NULL;
	return rc;
}

static int r63306_lcd_power(int on)
{
	static int curr_power;
	int rc;

	if (curr_power == on)
		return 0;

	if (on) {
		rc = r63306_vreg_power(on);
		if (!rc)
			rc = mipi_dsi_power(on);
	} else {
		rc = mipi_dsi_power(on);
		if (!rc)
			rc = r63306_vreg_power(on);
	}

	if (!rc)
		curr_power = on;

	return rc;
}

#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDW30
extern const struct panel_id tmd_mdw30_panel_default_no_id;
extern const struct panel_id tmd_mdw30_panel_default_id_old;
extern const struct panel_id tmd_mdw30_panel_default;
extern const struct panel_id tmd_mdw30_panel_id_old;
extern const struct panel_id tmd_mdw30_panel_id_1a;
extern const struct panel_id tmd_mdw30_panel_id_1c;
extern const struct panel_id tmd_mdw30_panel_id_1e;
extern const struct panel_id tmd_mdw30_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDW30 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDX80
extern const struct panel_id tmd_mdx80_panel_default;
extern const struct panel_id tmd_mdx80_panel_id_1a;
extern const struct panel_id tmd_mdx80_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDX80 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_HITACHI_DX12D900VM
extern const struct panel_id hitachi_dx12d900vm_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_HITACHI_DX12D900VM */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS046K3SX01
extern const struct panel_id sharp_ls046k3sx01_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS046K3SX01 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX01
extern const struct panel_id sharp_ls043k3sx01_panel_default_old;
extern const struct panel_id sharp_ls043k3sx01_panel_default;
extern const struct panel_id sharp_ls043k3sx01_panel_id_old;
extern const struct panel_id sharp_ls043k3sx01_panel_id_1a;
extern const struct panel_id sharp_ls043k3sx01_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX01 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX04
extern const struct panel_id sharp_ls043k3sx04_panel_default;
extern const struct panel_id sharp_ls043k3sx04_panel_id_1a;
extern const struct panel_id sharp_ls043k3sx04_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX04 */

static const struct panel_id *default_panel_ids_r63306[] = {
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDW30
	&tmd_mdw30_panel_default_no_id,
	&tmd_mdw30_panel_default_id_old,
	&tmd_mdw30_panel_default,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDW30 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDX80
	&tmd_mdx80_panel_default,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDX80 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX01
	&sharp_ls043k3sx01_panel_default_old,
	&sharp_ls043k3sx01_panel_default,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX01 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX04
	&sharp_ls043k3sx04_panel_default,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX04 */
	NULL,
};

static const struct panel_id *panel_ids_r63306[] = {
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDW30
	&tmd_mdw30_panel_id_old,
	&tmd_mdw30_panel_id_1a,
	&tmd_mdw30_panel_id_1c,
	&tmd_mdw30_panel_id_1e,
	&tmd_mdw30_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDW30 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDX80
	&tmd_mdx80_panel_id_1a,
	&tmd_mdx80_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_TMD_MDX80 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_HITACHI_DX12D900VM
	&hitachi_dx12d900vm_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_HITACHI_DX12D900VM */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS046K3SX01
	&sharp_ls046k3sx01_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS046K3SX01 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX01
	&sharp_ls043k3sx01_panel_id_old,
	&sharp_ls043k3sx01_panel_id_1a,
	&sharp_ls043k3sx01_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX01 */
#ifdef CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX04
	&sharp_ls043k3sx04_panel_id_1a,
	&sharp_ls043k3sx04_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_R63306_PANEL_SHARP_LS043K3SX04 */
	NULL,
};

#endif /* CONFIG_FB_MSM_MIPI_DSI_RENESAS_R63306 */


#ifdef CONFIG_FB_MSM_MIPI_DSI_SAMSUNG_S6D6AA0

#define MIPI_DSI_SAMSUNG_NAME "mipi_samsung_s6d6aa0"

static int s6d6aa0_vreg_power(int on)
{
	int rc = 0;

	if (!vreg_lcd_vddio) {
		vreg_lcd_vddio = regulator_get(&msm_mipi_dsi1_device.dev,
			"dsi_vddio");
		if (IS_ERR(vreg_lcd_vddio)) {
			pr_err("%s: Unable to get 8921_l29\n", __func__);
			vreg_lcd_vddio = NULL;
			return -ENODEV;
		}
	}

	if (on) {
		rc = regulator_set_voltage(vreg_lcd_vddio, 1800000, 1800000);
		if (rc) {
			pr_err("%s:%d unable to set L29 voltage to 1.8V\n",
				__func__, rc);
			goto out_put;
		}

		rc = regulator_enable(vreg_lcd_vddio);
		if (rc) {
			pr_err("%s: Enable regulator 8921_l29 failed\n",
				__func__);
			goto out_put;
		}

		rc = lcd_gpio_setup(on);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			goto out_desable;
		}
		gpio_set_value(LCD_PWR_EN, 1);

		msleep(LCD_POWER_WAIT_MS);

		lcd_reset(on);
	} else {
		lcd_reset(on);

		gpio_set_value(LCD_PWR_EN, 0);
		rc = lcd_gpio_setup(on);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			return -EINVAL;
		}

		rc = regulator_disable(vreg_lcd_vddio);
		if (rc)
			pr_warning("%s: '%s' regulator disable failed, rc=%d\n",
				__func__, "8921_l29", rc);
	}

	return 0;
out_desable:
	regulator_disable(vreg_lcd_vddio);
out_put:
	regulator_put(vreg_lcd_vddio);
	vreg_lcd_vddio = NULL;
	return rc;
}

static int s6d6aa0_lcd_power(int on)
{
	static int curr_power;
	int rc;

	if (curr_power == on)
		return 0;

	if (on) {
		rc = s6d6aa0_vreg_power(on);
		if (!rc)
			rc = mipi_dsi_power(on);
	} else {
		rc = mipi_dsi_power(on);
		if (!rc)
			rc = s6d6aa0_vreg_power(on);
	}

	if (!rc)
		curr_power = on;

	return rc;
}

#ifdef CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX439AKM
extern const struct panel_id sony_acx439akm_panel_default_id_old;
extern const struct panel_id sony_acx439akm_panel_id_old;
extern const struct panel_id sony_acx439akm_panel_id_1a;
extern const struct panel_id sony_acx439akm_panel_id_1e;
#endif /* CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX439AKM */

#ifdef CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX444AKM
extern const struct panel_id sony_acx444akm_panel_default;
extern const struct panel_id sony_acx444akm_panel_id_1b;
extern const struct panel_id sony_acx444akm_panel_id_1c;
extern const struct panel_id sony_acx444akm_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX444AKM */

static const struct panel_id *default_panel_ids_s6d6aa0[] = {
#ifdef CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX439AKM
	&sony_acx439akm_panel_default_id_old,
#endif /* CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX439AKM */
#ifdef CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX444AKM
	&sony_acx444akm_panel_default,
#endif /* CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX444AKM */
	NULL,
};

static const struct panel_id *panel_ids_s6d6aa0[] = {
#ifdef CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX439AKM
	&sony_acx439akm_panel_id_old,
	&sony_acx439akm_panel_id_1a,
	&sony_acx439akm_panel_id_1e,
#endif /* CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX439AKM */
#ifdef CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX444AKM
	&sony_acx444akm_panel_id_1b,
	&sony_acx444akm_panel_id_1c,
	&sony_acx444akm_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_S6D6AA0_PANEL_SONY_ACX444AKM */
	NULL,
};

#endif /* CONFIG_FB_MSM_MIPI_DSI_SAMSUNG_S6D6AA0 */


#ifdef CONFIG_FB_MSM_MIPI_DSI_NOVATEK_NT35565

#define MIPI_DSI_NOVATEK_NAME "mipi_novatek_nt35565"

#define LCD_RESET_H_WAIT_MS_NT35565 120

static int nt35565_vreg_power(int on)
{
	int rc = 0;

	if (!vreg_lcd_vci) {
		vreg_lcd_vci = regulator_get(&msm_mipi_dsi1_device.dev,
			"dsi_vci");
		if (IS_ERR(vreg_lcd_vci)) {
			pr_err("could not get dsi_vci, rc = %ld\n",
				PTR_ERR(vreg_lcd_vci));
			return -ENODEV;
		}
	}

	if (!vreg_lcd_vddio) {
		vreg_lcd_vddio = regulator_get(&msm_mipi_dsi1_device.dev,
			"dsi_vddio");
		if (IS_ERR(vreg_lcd_vddio)) {
			pr_err("%s: Unable to get dsi_vddio\n", __func__);
			vreg_lcd_vddio = NULL;
			goto out_put;
		}
	}

	if (on) {
		rc = regulator_set_voltage(vreg_lcd_vci, 2850000, 2850000);
		if (rc) {
			pr_err("%s:%d unable to set dsi_vci voltage to 2.8V\n",
				__func__, rc);
			goto out_put_all;
		}

		rc = regulator_enable(vreg_lcd_vci);
		if (rc) {
			pr_err("%s: Enable regulator dsi_vci failed\n",
				__func__);
			goto out_put_all;
		}

		rc = regulator_set_voltage(vreg_lcd_vddio, 1800000, 1800000);
		if (rc) {
			pr_err("%s:%d unable to set dsi_vddio voltage to 1.8V\n",
				__func__, rc);
			goto out_disable;
		}

		rc = regulator_enable(vreg_lcd_vddio);
		if (rc) {
			pr_err("%s: Enable regulator dsi_vddio failed\n",
				__func__);
			goto out_disable;
		}

		rc = lcd_gpio_setup(on);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			goto out_disable_all;
		}

		msleep(LCD_VREG_ON_WAIT_MS);

		gpio_set_value_cansleep(gpio_lcd_reset, 1);
		msleep(LCD_RESET_WAIT_MS);
		gpio_set_value_cansleep(gpio_lcd_reset, 0);
		msleep(LCD_RESET_WAIT_MS);
		gpio_set_value_cansleep(gpio_lcd_reset, 1);
		msleep(LCD_RESET_H_WAIT_MS_NT35565);
	} else {
		gpio_set_value_cansleep(gpio_lcd_reset, 0);

		rc = lcd_gpio_setup(on);
		if (rc) {
			pr_err("gpio setup failed , rc=%d\n", rc);
			return -EINVAL;
		}

		rc = regulator_disable(vreg_lcd_vci);
		if (rc)
			pr_warning("%s: '%s' regulator disable failed, rc=%d\n",
				__func__, "dsi_vci", rc);
		rc = regulator_disable(vreg_lcd_vddio);
		if (rc)
			pr_warning("%s: '%s' regulator disable failed, rc=%d\n",
				__func__, "dsi_vddio", rc);
	}

	return 0;
out_disable_all:
	regulator_disable(vreg_lcd_vddio);
out_disable:
	regulator_disable(vreg_lcd_vci);
out_put_all:
	regulator_put(vreg_lcd_vddio);
	vreg_lcd_vddio = NULL;
out_put:
	regulator_put(vreg_lcd_vci);
	vreg_lcd_vci = NULL;
	return rc;
}

static int nt35565_lcd_power(int on)
{
	static int curr_power;
	int rc;

	if (curr_power == on)
		return 0;

	if (on) {
		rc = nt35565_vreg_power(on);
		if (!rc)
			rc = mipi_dsi_power(on);
	} else {
		rc = mipi_dsi_power(on);
		if (!rc)
			rc = nt35565_vreg_power(on);
	}

	if (!rc)
		curr_power = on;

	return rc;
}

#ifdef CONFIG_FB_MSM_MIPI_NT35565_PANEL_SHARP_LS038K3SX01
extern const struct panel_id sharp_ls038k3sx01_panel_default;
extern const struct panel_id sharp_ls038k3sx01_panel_id_1a;
extern const struct panel_id sharp_ls038k3sx01_panel_id_1b;
extern const struct panel_id sharp_ls038k3sx01_panel_id;
#endif /* CONFIG_FB_MSM_MIPI_NT35565_PANEL_SHARP_LS038K3SX01 */

static const struct panel_id *default_panel_ids_nt35565[] = {
#ifdef CONFIG_FB_MSM_MIPI_NT35565_PANEL_SHARP_LS038K3SX01
	&sharp_ls038k3sx01_panel_default,
#endif /* CONFIG_FB_MSM_MIPI_NT35565_PANEL_SHARP_LS038K3SX01 */
	NULL,
};

static const struct panel_id *panel_ids_nt35565[] = {
#ifdef CONFIG_FB_MSM_MIPI_NT35565_PANEL_SHARP_LS038K3SX01
	&sharp_ls038k3sx01_panel_id_1a,
	&sharp_ls038k3sx01_panel_id_1b,
	&sharp_ls038k3sx01_panel_id,
#endif /* CONFIG_FB_MSM_MIPI_NT35565_PANEL_SHARP_LS038K3SX01 */
	NULL,
};

#endif /* CONFIG_FB_MSM_MIPI_DSI_NOVATEK_NT35565 */


#ifdef CONFIG_FB_MSM_MIPI_DSI_VENDER_DET

const struct panel_id **default_panel_ids;
const struct panel_id **panel_ids;

static struct mipi_dsi_platform_data mipi_dsi_pdata;

static struct platform_device semc_blue_lcd_device = {
	.id = 0,
};
#else /* CONFIG_FB_MSM_MIPI_DSI_VENDER_DET */

#ifdef CONFIG_FB_MSM_MIPI_DSI_RENESAS_R63306

const struct panel_id **default_panel_ids = default_panel_ids_r63306;
const struct panel_id **panel_ids = panel_ids_r63306;

static struct platform_device semc_blue_lcd_device = {
	.name = MIPI_DSI_RENESAS_NAME,
	.id = 0,
};

static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.dsi_power_save   = r63306_lcd_power,
};
#endif /* CONFIG_FB_MSM_MIPI_DSI_RENESAS_R63306 */

#ifdef CONFIG_FB_MSM_MIPI_DSI_SAMSUNG_S6D6AA0

const struct panel_id **default_panel_ids = default_panel_ids_s6d6aa0;
const struct panel_id **panel_ids = panel_ids_s6d6aa0;

static struct platform_device semc_blue_lcd_device = {
	.name = MIPI_DSI_SAMSUNG_NAME,
	.id = 0,
};

static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.dsi_power_save   = s6d6aa0_lcd_power,
};
#endif /* CONFIG_FB_MSM_MIPI_DSI_SAMSUNG_S6D6AA0 */

#ifdef CONFIG_FB_MSM_MIPI_DSI_NOVATEK_NT35565

const struct panel_id **default_panel_ids = default_panel_ids_nt35565;
const struct panel_id **panel_ids = panel_ids_nt35565;

static struct platform_device semc_blue_lcd_device = {
	.name = MIPI_DSI_NOVATEK_NAME,
	.id = 0,
};

static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.dsi_power_save   = nt35565_lcd_power,
};
#endif /* CONFIG_FB_MSM_MIPI_DSI_NOVATEK_NT35565 */

#endif /* CONFIG_FB_MSM_MIPI_DSI_VENDER_DET */



void __init semc_blue_add_lcd_device(void)
{
	int rc;
	struct lcd_panel_platform_data *pdata;

	pdata = kmalloc(sizeof(struct lcd_panel_platform_data), GFP_KERNEL);

	pdata->default_panels = default_panel_ids;
	pdata->panels = panel_ids;
	pdata->lcd_power = lcd_power;
	pdata->lcd_reset = lcd_reset;

#ifdef CONFIG_FB_MSM_MIPI_DSI_VENDER_DET
	if (semc_blue_is_dric_det() == LCD_VENDOR_SAMSUNG) {
		semc_blue_lcd_device.name = MIPI_DSI_SAMSUNG_NAME;
		pdata->default_panels = default_panel_ids_s6d6aa0;
		pdata->panels = panel_ids_s6d6aa0;
	} else {
		semc_blue_lcd_device.name = MIPI_DSI_RENESAS_NAME;
		pdata->default_panels = default_panel_ids_r63306;
		pdata->panels = panel_ids_r63306;
	}
#endif /* CONFIG_FB_MSM_MIPI_DSI_VENDER_DET */

	semc_blue_lcd_device.dev.platform_data = pdata;
	rc = platform_device_register(&semc_blue_lcd_device);
	if (rc)
		dev_err(&semc_blue_lcd_device.dev,
			"%s: platform_device_register() failed = %d\n",
			__func__, rc);

}

#ifdef CONFIG_NFC_PN544
static int pn544_chip_config(enum pn544_state state, void *not_used)
{
	switch (state) {
	case PN544_STATE_OFF:
		gpio_set_value(MSM_GPIO_NFC_FWDL_EN, 0);
		gpio_set_value_cansleep(
			PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN), 0);
		usleep(50000);
		break;
	case PN544_STATE_ON:
		gpio_set_value(MSM_GPIO_NFC_FWDL_EN, 0);
		gpio_set_value_cansleep(
			PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN), 1);
		usleep(10000);
		break;
	case PN544_STATE_FWDL:
		gpio_set_value(MSM_GPIO_NFC_FWDL_EN, 1);
		gpio_set_value_cansleep(
			PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN), 0);
		usleep(10000);
		gpio_set_value_cansleep(
			PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN), 1);
		break;
	default:
		pr_err("%s: undefined state %d\n", __func__, state);
		return -EINVAL;
	}
	return 0;
}

static int pn544_gpio_request(void)
{
	int ret;

	ret = gpio_request(MSM_GPIO_NFC_IRQ, "pn544_irq");
	if (ret)
		goto err_irq;
	ret = gpio_request(PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN),
		"pn544_ven");
	if (ret)
		goto err_ven;
	ret = gpio_request(MSM_GPIO_NFC_FWDL_EN, "pn544_fw");
	if (ret)
		goto err_fw;
	return 0;
err_fw:
	gpio_free(PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN));
err_ven:
	gpio_free(MSM_GPIO_NFC_IRQ);
err_irq:
	pr_err("%s: gpio request err %d\n", __func__, ret);
	return ret;
}

static void pn544_gpio_release(void)
{
	gpio_free(PM8921_GPIO_PM_TO_SYS(PMIC_GPIO_NFC_EN));
	gpio_free(MSM_GPIO_NFC_IRQ);
	gpio_free(MSM_GPIO_NFC_FWDL_EN);
}

static struct pn544_i2c_platform_data pn544_pdata = {
	.irq_type = IRQF_TRIGGER_RISING,
	.chip_config = pn544_chip_config,
	.driver_loaded = pn544_gpio_request,
	.driver_unloaded = pn544_gpio_release,
};
#endif

#ifdef CONFIG_FB_MSM_MHL_SII8334

#define MSM_GPIO_MHL_RESET_N	(64)
#define MSM_GPIO_MHL_IRQ_N	(65)
#define MHL_SEMC_ADOPTER_ID	(935)
#define MHL_SEMC_DEVICE_ID	(0x8334)

static int mhl_sii_setup_power(int enable)
{
	int rc;
	static struct regulator *reg_8921_l18;

	if (!reg_8921_l18)
		reg_8921_l18 = regulator_get(NULL, "8921_l18");

	if (enable) {
		rc = regulator_set_voltage(reg_8921_l18, 1200000, 1200000);
		if (rc)
			goto out_setup_power;
		rc = regulator_enable(reg_8921_l18);
		if (rc)
			goto out_setup_power;
		printk(KERN_INFO "%s(on): success\n", __func__);
	} else {
		rc = regulator_disable(reg_8921_l18);
		if (rc)
			goto out_setup_power;
		printk(KERN_INFO "%s(off): success\n", __func__);
	}

	return 0;
out_setup_power:
	printk(KERN_ERR "%s: failed to setup power\n", __func__);
	regulator_put(reg_8921_l18);
	return rc;
}

static int mhl_sii_setup_gpio(int enable)
{
	int rc;

	printk(KERN_INFO "mhl_sii_setup_gpio\n");

	rc = gpio_request(MSM_GPIO_MHL_RESET_N, "sii8334_reset");
	if (rc)
		goto out_setup_gpio;

	rc = gpio_request(MSM_GPIO_MHL_IRQ_N, "sii8334_irq");
	if (rc) {
		gpio_free(MSM_GPIO_MHL_RESET_N);
		goto out_setup_gpio;
	}

	return 0;
out_setup_gpio:
	printk(KERN_ERR "%s: failed to setup gpio\n", __func__);
	return rc;
}

static int mhl_sii_charging_enable(int enable, int max_curr)
{
	/* max_curr not supported for blue platform */

	printk(KERN_INFO "mhl_sii_charging_enable (%s)\n",
		enable ? "enable" : "disable");

	if (enable) {
		pm8921_set_usb_power_supply_type
			(POWER_SUPPLY_TYPE_USB);
		pm8921_charger_vbus_draw(IDEV_CHG_MIN);
	} else {
		pm8921_charger_vbus_draw(0);
	}

	return 0;
}

static struct mhl_sii_platform_data mhl_sii_pdata = {
	.reset = MSM_GPIO_MHL_RESET_N,
	.hpd_pin_mode = PIN_MODE_TTL,
	.int_pin_mode = PIN_MODE_OPENDRAIN_N,
	.adopter_id = MHL_SEMC_ADOPTER_ID,
	.device_id = MHL_SEMC_DEVICE_ID,
	.setup_power = mhl_sii_setup_power,
	.setup_gpio = mhl_sii_setup_gpio,
	.charging_enable = mhl_sii_charging_enable,
};
#endif /* CONFIG_FB_MSM_MHL_SII8334 */

#ifdef CONFIG_MSM_BUS_SCALING

static struct msm_bus_vectors rotator_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ROTATOR,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

static struct msm_bus_vectors rotator_ui_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ROTATOR,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = (1024 * 600 * 4 * 2 * 60),
		.ib  = (1024 * 600 * 4 * 2 * 60 * 1.5),
	},
};

static struct msm_bus_vectors rotator_vga_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ROTATOR,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = (640 * 480 * 2 * 2 * 30),
		.ib  = (640 * 480 * 2 * 2 * 30 * 1.5),
	},
};
static struct msm_bus_vectors rotator_720p_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ROTATOR,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = (1280 * 736 * 2 * 2 * 30),
		.ib  = (1280 * 736 * 2 * 2 * 30 * 1.5),
	},
};

static struct msm_bus_vectors rotator_1080p_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ROTATOR,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = (1920 * 1088 * 2 * 2 * 30),
		.ib  = (1920 * 1088 * 2 * 2 * 30 * 1.5),
	},
};

static struct msm_bus_paths rotator_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(rotator_init_vectors),
		rotator_init_vectors,
	},
	{
		ARRAY_SIZE(rotator_ui_vectors),
		rotator_ui_vectors,
	},
	{
		ARRAY_SIZE(rotator_vga_vectors),
		rotator_vga_vectors,
	},
	{
		ARRAY_SIZE(rotator_720p_vectors),
		rotator_720p_vectors,
	},
	{
		ARRAY_SIZE(rotator_1080p_vectors),
		rotator_1080p_vectors,
	},
};

struct msm_bus_scale_pdata rotator_bus_scale_pdata = {
	rotator_bus_scale_usecases,
	ARRAY_SIZE(rotator_bus_scale_usecases),
	.name = "rotator",
};

static struct msm_bus_vectors mdp_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

static struct msm_bus_vectors mdp_ui_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 216000000 * 2,
		.ib = 270000000 * 2,
	},
};

static struct msm_bus_vectors mdp_vga_vectors[] = {
	/* VGA and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 216000000 * 2,
		.ib = 270000000 * 2,
	},
};

static struct msm_bus_vectors mdp_720p_vectors[] = {
	/* 720p and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 230400000 * 2,
		.ib = 288000000 * 2,
	},
};

static struct msm_bus_vectors mdp_1080p_vectors[] = {
	/* 1080p and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 334080000 * 2,
		.ib = 417600000 * 2,
	},
};

static struct msm_bus_paths mdp_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(mdp_init_vectors),
		mdp_init_vectors,
	},
	{
		ARRAY_SIZE(mdp_ui_vectors),
		mdp_ui_vectors,
	},
	{
		ARRAY_SIZE(mdp_ui_vectors),
		mdp_ui_vectors,
	},
	{
		ARRAY_SIZE(mdp_vga_vectors),
		mdp_vga_vectors,
	},
	{
		ARRAY_SIZE(mdp_720p_vectors),
		mdp_720p_vectors,
	},
	{
		ARRAY_SIZE(mdp_1080p_vectors),
		mdp_1080p_vectors,
	},
};

static struct msm_bus_scale_pdata mdp_bus_scale_pdata = {
	mdp_bus_scale_usecases,
	ARRAY_SIZE(mdp_bus_scale_usecases),
	.name = "mdp",
};

#endif

static int mdp_core_clk_rate_table[] = {
	85330000,
	85330000,
	160000000,
	200000000,
};

static struct msm_panel_common_pdata mdp_pdata = {
	.gpio = MDP_VSYNC_GPIO,
	.mdp_core_clk_rate = 85330000,
	.mdp_core_clk_table = mdp_core_clk_rate_table,
	.num_mdp_clk = ARRAY_SIZE(mdp_core_clk_rate_table),
#ifdef CONFIG_MSM_BUS_SCALING
	.mdp_bus_scale_table = &mdp_bus_scale_pdata,
#endif
	.mdp_rev = MDP_REV_42,
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
	.mem_hid = ION_CP_MM_HEAP_ID,
#else
	.mem_hid = MEMTYPE_EBI1,
#endif
};

void __init msm8960_mdp_writeback(struct memtype_reserve* reserve_table)
{
	mdp_pdata.ov0_wb_size = MSM_FB_OVERLAY0_WRITEBACK_SIZE;
	mdp_pdata.ov1_wb_size = MSM_FB_OVERLAY1_WRITEBACK_SIZE;
#if defined(CONFIG_ANDROID_PMEM) && !defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	reserve_table[mdp_pdata.mem_hid].size +=
		mdp_pdata.ov0_wb_size;
	reserve_table[mdp_pdata.mem_hid].size +=
		mdp_pdata.ov1_wb_size;
#endif
}

#ifdef CONFIG_FB_MSM_MIPI_DSI_TOSHIBA
#define LPM_CHANNEL0 0
static int toshiba_gpio[] = {LPM_CHANNEL0};

static struct mipi_dsi_panel_platform_data toshiba_pdata = {
	.gpio = toshiba_gpio,
};

static struct platform_device mipi_dsi_toshiba_panel_device = {
	.name = "mipi_toshiba",
	.id = 0,
	.dev = {
		.platform_data = &toshiba_pdata,
	}
};
#endif

static struct mipi_dsi_phy_ctrl dsi_novatek_cmd_mode_phy_db = {

/* DSI_BIT_CLK at 500MHz, 2 lane, RGB888 */
	{0x0F, 0x0a, 0x04, 0x00, 0x20},	/* regulator */
	/* timing   */
	{0xab, 0x8a, 0x18, 0x00, 0x92, 0x97, 0x1b, 0x8c,
	0x0c, 0x03, 0x04, 0xa0},
	{0x5f, 0x00, 0x00, 0x10},	/* phy ctrl */
	{0xff, 0x00, 0x06, 0x00},	/* strength */
	/* pll control */
	{0x40, 0xf9, 0x30, 0xda, 0x00, 0x40, 0x03, 0x62,
	0x40, 0x07, 0x03,
	0x00, 0x1a, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01},
};

static struct mipi_dsi_panel_platform_data novatek_pdata = {
	.phy_ctrl_settings = &dsi_novatek_cmd_mode_phy_db,
};

static struct platform_device mipi_dsi_novatek_panel_device = {
	.name = "mipi_novatek",
	.id = 0,
	.dev = {
		.platform_data = &novatek_pdata,
	}
};

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct resource hdmi_msm_resources[] = {
	{
		.name  = "hdmi_msm_qfprom_addr",
		.start = 0x00700000,
		.end   = 0x007060FF,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "hdmi_msm_hdmi_addr",
		.start = 0x04A00000,
		.end   = 0x04A00FFF,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "hdmi_msm_irq",
		.start = HDMI_IRQ,
		.end   = HDMI_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

static int hdmi_enable_5v(int on);
static int hdmi_core_power(int on, int show);
static int hdmi_cec_power(int on);

static struct msm_hdmi_platform_data hdmi_msm_data = {
	.irq = HDMI_IRQ,
	.enable_5v = hdmi_enable_5v,
	.core_power = hdmi_core_power,
	.cec_power = hdmi_cec_power,
};

static struct platform_device hdmi_msm_device = {
	.name = "hdmi_msm",
	.id = 0,
	.num_resources = ARRAY_SIZE(hdmi_msm_resources),
	.resource = hdmi_msm_resources,
	.dev.platform_data = &hdmi_msm_data,
};
#endif /* CONFIG_FB_MSM_HDMI_MSM_PANEL */

#ifdef CONFIG_MSM_BUS_SCALING
static struct msm_bus_vectors dtv_bus_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};
static struct msm_bus_vectors dtv_bus_def_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 566092800 * 2,
		.ib = 707616000 * 2,
	},
};
static struct msm_bus_paths dtv_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(dtv_bus_init_vectors),
		dtv_bus_init_vectors,
	},
	{
		ARRAY_SIZE(dtv_bus_def_vectors),
		dtv_bus_def_vectors,
	},
};
static struct msm_bus_scale_pdata dtv_bus_scale_pdata = {
	dtv_bus_scale_usecases,
	ARRAY_SIZE(dtv_bus_scale_usecases),
	.name = "dtv",
};

static struct lcdc_platform_data dtv_pdata = {
	.bus_scale_table = &dtv_bus_scale_pdata,
};
#endif

static void __init msm_fb_add_devices(void)
{
	msm_fb_register_device("mdp", &mdp_pdata);
#ifdef CONFIG_FB_MSM_MIPI_DSI_VENDER_DET
	if (semc_blue_is_dric_det() == LCD_VENDOR_SAMSUNG)
		mipi_dsi_pdata.dsi_power_save = s6d6aa0_lcd_power;
	else
		mipi_dsi_pdata.dsi_power_save = r63306_lcd_power;
#endif /* CONFIG_FB_MSM_MIPI_DSI_VENDER_DET */
	msm_fb_register_device("mipi_dsi", &mipi_dsi_pdata);
#ifdef CONFIG_MSM_BUS_SCALING
	msm_fb_register_device("dtv", &dtv_pdata);
#endif
}

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static int hdmi_enable_5v(int on)
{
	return 0;
}

static int hdmi_core_power(int on, int show)
{
	static struct regulator *reg_8921_l23, *reg_8921_s4;
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	/* TBD: PM8921 regulator instead of 8901 */
	if (!reg_8921_l23) {
		reg_8921_l23 = regulator_get(&hdmi_msm_device.dev, "hdmi_avdd");
		if (IS_ERR(reg_8921_l23)) {
			pr_err("could not get reg_8921_l23, rc = %ld\n",
				PTR_ERR(reg_8921_l23));
			return -ENODEV;
		}
		rc = regulator_set_voltage(reg_8921_l23, 1800000, 1800000);
		if (rc) {
			pr_err("set_voltage failed for 8921_l23, rc=%d\n", rc);
			return -EINVAL;
		}
	}
	if (!reg_8921_s4) {
		reg_8921_s4 = regulator_get(&hdmi_msm_device.dev, "hdmi_vcc");
		if (IS_ERR(reg_8921_s4)) {
			pr_err("could not get reg_8921_s4, rc = %ld\n",
				PTR_ERR(reg_8921_s4));
			return -ENODEV;
		}
		rc = regulator_set_voltage(reg_8921_s4, 1800000, 1800000);
		if (rc) {
			pr_err("set_voltage failed for 8921_s4, rc=%d\n", rc);
			return -EINVAL;
		}
	}

	if (on) {
		rc = regulator_set_optimum_mode(reg_8921_l23, 100000);
		if (rc < 0) {
			pr_err("set_optimum_mode l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_enable(reg_8921_l23);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"hdmi_avdd", rc);
			return rc;
		}
		rc = regulator_enable(reg_8921_s4);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"hdmi_vcc", rc);
			return rc;
		}
		rc = gpio_request(100, "HDMI_DDC_CLK");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_DDC_CLK", 100, rc);
			goto error1;
		}
		rc = gpio_request(101, "HDMI_DDC_DATA");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_DDC_DATA", 101, rc);
			goto error2;
		}
		rc = gpio_request(102, "HDMI_HPD");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_HPD", 102, rc);
			goto error3;
		}
		pr_debug("%s(on): success\n", __func__);
	} else {
		gpio_free(100);
		gpio_free(101);
		gpio_free(102);

		rc = regulator_disable(reg_8921_l23);
		if (rc) {
			pr_err("disable reg_8921_l23 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		rc = regulator_disable(reg_8921_s4);
		if (rc) {
			pr_err("disable reg_8921_s4 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		rc = regulator_set_optimum_mode(reg_8921_l23, 100);
		if (rc < 0) {
			pr_err("set_optimum_mode l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		pr_debug("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;

error3:
	gpio_free(101);
error2:
	gpio_free(100);
error1:
	regulator_disable(reg_8921_l23);
	regulator_disable(reg_8921_s4);
	return rc;
}

static int hdmi_cec_power(int on)
{
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (on) {
		rc = gpio_request(99, "HDMI_CEC_VAR");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_CEC_VAR", 99, rc);
			goto error;
		}
		pr_debug("%s(on): success\n", __func__);
	} else {
		gpio_free(99);
		pr_debug("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;
error:
	return rc;
}
#endif /* CONFIG_FB_MSM_HDMI_MSM_PANEL */

static void __init msm8960_allocate_memory_regions(void)
{
	void *addr;
	unsigned long size;

	size = MSM_FB_SIZE;
	addr = alloc_bootmem_align(size, 0x1000);
	msm_fb_resources[0].start = __pa(addr);
	msm_fb_resources[0].end = msm_fb_resources[0].start + size - 1;
	pr_info("allocating %lu bytes at %p (%lx physical) for fb\n",
			size, addr, __pa(addr));

}
#ifdef CONFIG_WCD9310_CODEC

#define TABLA_INTERRUPT_BASE (NR_MSM_IRQS + NR_GPIO_IRQS + NR_PM8921_IRQS)

/* Micbias setting is based on 8660 CDP/MTP/FLUID requirement
 * 4 micbiases are used to power various analog and digital
 * microphones operating at 1800 mV. Technically, all micbiases
 * can source from single cfilter since all microphones operate
 * at the same voltage level. The arrangement below is to make
 * sure all cfilters are exercised. LDO_H regulator ouput level
 * does not need to be as high as 2.85V. It is choosen for
 * microphone sensitivity purpose.
 */
static struct tabla_pdata tabla_platform_data = {
	.slimbus_slave_device = {
		.name = "tabla-slave",
		.e_addr = {0, 0, 0x10, 0, 0x17, 2},
	},
	.irq = MSM_GPIO_TO_INT(62),
	.irq_base = TABLA_INTERRUPT_BASE,
	.num_irqs = NR_TABLA_IRQS,
	.reset_gpio = PM8921_GPIO_PM_TO_SYS(34),
	.micbias = {
		.ldoh_v = TABLA_LDOH_2P85_V,
		.cfilt1_mv = 1800,
		.cfilt2_mv = 1800,
		.cfilt3_mv = 1800,
		.bias1_cfilt_sel = TABLA_CFILT1_SEL,
		.bias2_cfilt_sel = TABLA_CFILT2_SEL,
		.bias3_cfilt_sel = TABLA_CFILT3_SEL,
		.bias4_cfilt_sel = TABLA_CFILT3_SEL,
	}
};

static struct slim_device msm_slim_tabla = {
	.name = "tabla-slim",
	.e_addr = {0, 1, 0x10, 0, 0x17, 2},
	.dev = {
		.platform_data = &tabla_platform_data,
	},
};

static struct tabla_pdata tabla20_platform_data = {
	.slimbus_slave_device = {
		.name = "tabla-slave",
		.e_addr = {0, 0, 0x60, 0, 0x17, 2},
	},
	.irq = MSM_GPIO_TO_INT(62),
	.irq_base = TABLA_INTERRUPT_BASE,
	.num_irqs = NR_TABLA_IRQS,
	.reset_gpio = PM8921_GPIO_PM_TO_SYS(34),
	.micbias = {
		.ldoh_v = TABLA_LDOH_2P85_V,
		.cfilt1_mv = 1800,
		.cfilt2_mv = 1800,
		.cfilt3_mv = 1800,
		.bias1_cfilt_sel = TABLA_CFILT1_SEL,
		.bias2_cfilt_sel = TABLA_CFILT2_SEL,
		.bias3_cfilt_sel = TABLA_CFILT3_SEL,
		.bias4_cfilt_sel = TABLA_CFILT3_SEL,
	}
};

static struct slim_device msm_slim_tabla20 = {
	.name = "tabla2x-slim",
	.e_addr = {0, 1, 0x60, 0, 0x17, 2},
	.dev = {
		.platform_data = &tabla20_platform_data,
	},
};
#endif

static struct slim_boardinfo msm_slim_devices[] = {
#ifdef CONFIG_WCD9310_CODEC
	{
		.bus_num = 1,
		.slim_slave = &msm_slim_tabla,
	},
	{
		.bus_num = 1,
		.slim_slave = &msm_slim_tabla20,
	},
#endif
	/* add more slimbus slaves as needed */
};

#define MSM_WCNSS_PHYS	0x03000000
#define MSM_WCNSS_SIZE	0x280000

static struct resource resources_wcnss_wlan[] = {
	{
		.start	= RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
		.end	= RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
		.name	= "wcnss_wlanrx_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
		.end	= RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
		.name	= "wcnss_wlantx_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_WCNSS_PHYS,
		.end	= MSM_WCNSS_PHYS + MSM_WCNSS_SIZE - 1,
		.name	= "wcnss_mmio",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= 84,
		.end	= 88,
		.name	= "wcnss_gpios_5wire",
		.flags	= IORESOURCE_IO,
	},
};

static struct qcom_wcnss_opts qcom_wcnss_pdata = {
	.has_48mhz_xo	= 1,
};

static struct platform_device msm_device_wcnss_wlan = {
	.name		= "wcnss_wlan",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_wcnss_wlan),
	.resource	= resources_wcnss_wlan,
	.dev		= {.platform_data = &qcom_wcnss_pdata},
};

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

#define QCE_SIZE		0x10000
#define QCE_0_BASE		0x18500000

#define QCE_HW_KEY_SUPPORT	0
#define QCE_SHA_HMAC_SUPPORT	1
#define QCE_SHARE_CE_RESOURCE	1
#define QCE_CE_SHARED		0

/* Begin Bus scaling definitions */
static struct msm_bus_vectors crypto_hw_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 0,
		.ib = 0,
	},
};

static struct msm_bus_vectors crypto_hw_active_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 70000000UL,
		.ib = 70000000UL,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 2480000000UL,
		.ib = 2480000000UL,
	},
};

static struct msm_bus_paths crypto_hw_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(crypto_hw_init_vectors),
		crypto_hw_init_vectors,
	},
	{
		ARRAY_SIZE(crypto_hw_active_vectors),
		crypto_hw_active_vectors,
	},
};

static struct msm_bus_scale_pdata crypto_hw_bus_scale_pdata = {
		crypto_hw_bus_scale_usecases,
		ARRAY_SIZE(crypto_hw_bus_scale_usecases),
		.name = "cryptohw",
};
/* End Bus Scaling Definitions*/

static struct resource qcrypto_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource qcedev_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)

static struct msm_ce_hw_support qcrypto_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = &crypto_hw_bus_scale_pdata,
};

static struct platform_device qcrypto_device = {
	.name		= "qcrypto",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcrypto_resources),
	.resource	= qcrypto_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcrypto_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

static struct msm_ce_hw_support qcedev_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = &crypto_hw_bus_scale_pdata,
};

static struct platform_device qcedev_device = {
	.name		= "qce",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcedev_resources),
	.resource	= qcedev_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcedev_ce_hw_suppport,
	},
};
#endif


static int __init gpiomux_init(void)
{
	int rc;

	rc = msm_gpiomux_init(NR_GPIO_IRQS);
	if (rc) {
		pr_err(KERN_ERR "msm_gpiomux_init failed %d\n", rc);
		return rc;
	}

	gpiomux_device_install();

	return 0;
}

#define MSM_SHARED_RAM_PHYS 0x80000000

static struct pm8xxx_adc_amux pm8xxx_adc_channels_data[] = {
	{"vcoin", CHANNEL_VCOIN, CHAN_PATH_SCALING2, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"vbat", CHANNEL_VBAT, CHAN_PATH_SCALING2, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"dcin", CHANNEL_DCIN, CHAN_PATH_SCALING4, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"ichg", CHANNEL_ICHG, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"vph_pwr", CHANNEL_VPH_PWR, CHAN_PATH_SCALING2, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"ibat", CHANNEL_IBAT, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"m4", CHANNEL_MPP_1, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"batt_therm", CHANNEL_BATT_THERM, CHAN_PATH_SCALING1, AMUX_RSV2,
		ADC_DECIMATION_TYPE2, ADC_SCALE_BATT_THERM},
	{"batt_id", CHANNEL_BATT_ID, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"usbin", CHANNEL_USBIN, CHAN_PATH_SCALING3, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"pmic_therm", CHANNEL_DIE_TEMP, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_PMIC_THERM},
	{"625mv", CHANNEL_625MV, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"125v", CHANNEL_125V, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"chg_temp", CHANNEL_CHG_TEMP, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_DEFAULT},
	{"pa_therm1", ADC_MPP_1_AMUX8, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_PA_THERM},
	{"xo_therm", CHANNEL_MUXOFF, CHAN_PATH_SCALING1, AMUX_RSV0,
		ADC_DECIMATION_TYPE2, ADC_SCALE_XOTHERM},
	{"pa_therm0", ADC_MPP_1_AMUX3, CHAN_PATH_SCALING1, AMUX_RSV1,
		ADC_DECIMATION_TYPE2, ADC_SCALE_PA_THERM},
};

static struct pm8xxx_adc_properties pm8xxx_adc_data = {
	.adc_vdd_reference	= 1800, /* milli-voltage for this adc */
	.bitresolution		= 15,
	.bipolar		= 0,
};

static struct pm8xxx_adc_platform_data pm8xxx_adc_pdata = {
	.adc_channel		= pm8xxx_adc_channels_data,
	.adc_num_board_channel	= ARRAY_SIZE(pm8xxx_adc_channels_data),
	.adc_prop		= &pm8xxx_adc_data,
	.adc_mpp_base		= PM8921_MPP_PM_TO_SYS(1),
};

static void __init msm8960_map_io(void)
{
	msm_shared_ram_phys = MSM_SHARED_RAM_PHYS;
	msm_map_msm8960_io();

	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
}

static void __init msm8960_init_irq(void)
{
	msm_mpm_irq_extn_init();
	gic_init(0, GIC_PPI_START, MSM_QGIC_DIST_BASE,
						(void *)MSM_QGIC_CPU_BASE);

	/* Edge trigger PPIs except AVS_SVICINT and AVS_SVICINTSWDONE */
	writel_relaxed(0xFFFFD7FF, MSM_QGIC_DIST_BASE + GIC_DIST_CONFIG + 4);

	writel_relaxed(0x0000FFFF, MSM_QGIC_DIST_BASE + GIC_DIST_ENABLE_SET);
	mb();
}

#ifdef CONFIG_MMC_MSM_POWER_SUPPLY_SDC3_VIA_MPP04
static uint32_t msm_sdc3_set_mpp04(struct device*, unsigned int);
#endif

/* MSM8960 has 5 SDCC controllers */
enum sdcc_controllers {
	SDCC1,
	SDCC2,
	SDCC3,
	SDCC4,
	SDCC5,
	MAX_SDCC_CONTROLLER
};

/* All SDCC controllers require VDD/VCC voltage */
static struct msm_mmc_reg_data mmc_vdd_reg_data[MAX_SDCC_CONTROLLER] = {
	/* SDCC1 : eMMC card connected */
	[SDCC1] = {
		.name = "sdc_vdd",
		.high_vol_level = 2950000,
		.low_vol_level = 2950000,
		.always_on = 1,
		.lpm_sup = 1,
		.lpm_uA = 9000,
		.hpm_uA = 200000, /* 200mA */
	},
	/* SDCC3 : External card slot connected */
	[SDCC3] = {
		.name = "sdc_vdd",
		.high_vol_level = 2950000,
		.low_vol_level = 2950000,
		.hpm_uA = 600000, /* 600mA */
	}
};

/* Only slots having eMMC card will require VCCQ voltage */
static struct msm_mmc_reg_data mmc_vccq_reg_data[1] = {
	/* SDCC1 : eMMC card connected */
	[SDCC1] = {
		.name = "sdc_vccq",
		.always_on = 1,
		.high_vol_level = 1800000,
		.low_vol_level = 1800000,
		.hpm_uA = 200000, /* 200mA */
	}
};

/* All SDCC controllers may require voting for VDD PAD voltage */
static struct msm_mmc_reg_data mmc_vddp_reg_data[MAX_SDCC_CONTROLLER] = {
	/* SDCC3 : External card slot connected */
	[SDCC3] = {
		.name = "sdc_vddp",
		.high_vol_level = 2950000,
		.low_vol_level = 1850000,
		.lpm_sup = 1,
		/* Max. Active current required is 16 mA */
		.hpm_uA = 16000,
		/*
		 * Sleep current required is ~300 uA. But min. vote can be
		 * in terms of mA (min. 1 mA). So let's vote for 2 mA
		 * during sleep.
		 */
		.lpm_uA = 2000,
	}
};

static struct msm_mmc_slot_reg_data mmc_slot_vreg_data[MAX_SDCC_CONTROLLER] = {
	/* SDCC1 : eMMC card connected */
	[SDCC1] = {
		.vdd_data = &mmc_vdd_reg_data[SDCC1],
		.vccq_data = &mmc_vccq_reg_data[SDCC1],
	},
	/* SDCC3 : External card slot connected */
	[SDCC3] = {
		.vdd_data = &mmc_vdd_reg_data[SDCC3],
		.vddp_data = &mmc_vddp_reg_data[SDCC3],
	}
};

/* SDC1 pad data */
static struct msm_mmc_pad_drv sdc1_pad_drv_on_cfg[] = {
	{TLMM_HDRV_SDC1_CLK, GPIO_CFG_16MA},
	{TLMM_HDRV_SDC1_CMD, GPIO_CFG_10MA},
	{TLMM_HDRV_SDC1_DATA, GPIO_CFG_10MA}
};

static struct msm_mmc_pad_drv sdc1_pad_drv_off_cfg[] = {
	{TLMM_HDRV_SDC1_CLK, GPIO_CFG_2MA},
	{TLMM_HDRV_SDC1_CMD, GPIO_CFG_2MA},
	{TLMM_HDRV_SDC1_DATA, GPIO_CFG_2MA}
};

static struct msm_mmc_pad_pull sdc1_pad_pull_on_cfg[] = {
	{TLMM_PULL_SDC1_CLK, GPIO_CFG_NO_PULL},
	{TLMM_PULL_SDC1_CMD, GPIO_CFG_PULL_UP},
	{TLMM_PULL_SDC1_DATA, GPIO_CFG_PULL_UP}
};

static struct msm_mmc_pad_pull sdc1_pad_pull_off_cfg[] = {
	{TLMM_PULL_SDC1_CLK, GPIO_CFG_NO_PULL},
	{TLMM_PULL_SDC1_CMD, GPIO_CFG_PULL_UP},
	{TLMM_PULL_SDC1_DATA, GPIO_CFG_PULL_UP}
};

/* SDC3 pad data */
static struct msm_mmc_pad_drv sdc3_pad_drv_on_cfg[] = {
	{TLMM_HDRV_SDC3_CLK, GPIO_CFG_8MA},
	{TLMM_HDRV_SDC3_CMD, GPIO_CFG_8MA},
	{TLMM_HDRV_SDC3_DATA, GPIO_CFG_8MA}
};

static struct msm_mmc_pad_drv sdc3_pad_drv_off_cfg[] = {
	{TLMM_HDRV_SDC3_CLK, GPIO_CFG_2MA},
	{TLMM_HDRV_SDC3_CMD, GPIO_CFG_2MA},
	{TLMM_HDRV_SDC3_DATA, GPIO_CFG_2MA}
};

static struct msm_mmc_pad_pull sdc3_pad_pull_on_cfg[] = {
	{TLMM_PULL_SDC3_CLK, GPIO_CFG_NO_PULL},
	{TLMM_PULL_SDC3_CMD, GPIO_CFG_PULL_UP},
	{TLMM_PULL_SDC3_DATA, GPIO_CFG_PULL_UP}
};

static struct msm_mmc_pad_pull sdc3_pad_pull_off_cfg[] = {
	{TLMM_PULL_SDC3_CLK, GPIO_CFG_NO_PULL},
	/*
	 * SDC3 CMD line should be PULLed UP otherwise fluid platform will
	 * see transitions (1 -> 0 and 0 -> 1) on card detection line,
	 * which would result in false card detection interrupts.
	 */
	{TLMM_PULL_SDC3_CMD, GPIO_CFG_PULL_UP},
	/*
	 * Keeping DATA lines status to PULL UP will make sure that
	 * there is no current leak during sleep if external pull up
	 * is connected to DATA lines.
	 */
	{TLMM_PULL_SDC3_DATA, GPIO_CFG_PULL_UP}
};

struct msm_mmc_pad_pull_data mmc_pad_pull_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.on = sdc1_pad_pull_on_cfg,
		.off = sdc1_pad_pull_off_cfg,
		.size = ARRAY_SIZE(sdc1_pad_pull_on_cfg)
	},
	[SDCC3] = {
		.on = sdc3_pad_pull_on_cfg,
		.off = sdc3_pad_pull_off_cfg,
		.size = ARRAY_SIZE(sdc3_pad_pull_on_cfg)
	},
};

struct msm_mmc_pad_drv_data mmc_pad_drv_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.on = sdc1_pad_drv_on_cfg,
		.off = sdc1_pad_drv_off_cfg,
		.size = ARRAY_SIZE(sdc1_pad_drv_on_cfg)
	},
	[SDCC3] = {
		.on = sdc3_pad_drv_on_cfg,
		.off = sdc3_pad_drv_off_cfg,
		.size = ARRAY_SIZE(sdc3_pad_drv_on_cfg)
	},
};

struct msm_mmc_pad_data mmc_pad_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.pull = &mmc_pad_pull_data[SDCC1],
		.drv = &mmc_pad_drv_data[SDCC1]
	},
	[SDCC3] = {
		.pull = &mmc_pad_pull_data[SDCC3],
		.drv = &mmc_pad_drv_data[SDCC3]
	},
};

struct msm_mmc_pin_data mmc_slot_pin_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.pad_data = &mmc_pad_data[SDCC1],
	},
	[SDCC3] = {
		.pad_data = &mmc_pad_data[SDCC3],
	},
};

static unsigned int sdc1_sup_clk_rates[] = {
	400000, 24000000, 48000000
};

static unsigned int sdc3_sup_clk_rates[] = {
	400000, 24000000, 48000000, 96000000, 192000000
};

#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
static struct mmc_platform_data msm8960_sdc1_data = {
	.ocr_mask       = MMC_VDD_27_28 | MMC_VDD_28_29,
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	.mmc_bus_width  = MMC_CAP_8_BIT_DATA,
#else
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
#endif
	.sup_clk_table	= sdc1_sup_clk_rates,
	.sup_clk_cnt	= ARRAY_SIZE(sdc1_sup_clk_rates),
	.pclk_src_dfab	= 1,
	.nonremovable	= 1,
	.vreg_data	= &mmc_slot_vreg_data[SDCC1],
	.pin_data	= &mmc_slot_pin_data[SDCC1],
};
#endif

#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
static struct mmc_platform_data msm8960_sdc3_data = {
	.ocr_mask       = MMC_VDD_27_28 | MMC_VDD_28_29,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.sup_clk_table	= sdc3_sup_clk_rates,
	.sup_clk_cnt	= ARRAY_SIZE(sdc3_sup_clk_rates),
	.pclk_src_dfab	= 1,
	.vreg_data	= &mmc_slot_vreg_data[SDCC3],
	.pin_data	= &mmc_slot_pin_data[SDCC3],
	.wpswitch	= msm_sdc3_get_wpswitch,
#ifdef CONFIG_MMC_MSM_POWER_SUPPLY_SDC3_VIA_MPP04
	.translate_vdd	= msm_sdc3_set_mpp04,
#endif
#ifdef CONFIG_MMC_MSM_CARD_HW_DETECTION
	.status_gpio	= PM8921_GPIO_PM_TO_SYS(26),
	.status_irq	= PM8921_GPIO_IRQ(PM8921_IRQ_BASE, 26),
	.irq_flags	= IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
	.is_status_gpio_active_low = true,
#endif
	.xpc_cap	= 1,
	.uhs_caps	= (MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
			MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_DDR50 |
			MMC_CAP_UHS_SDR104 | MMC_CAP_MAX_CURRENT_600)
};
#endif

#ifdef CONFIG_MMC_MSM_POWER_SUPPLY_SDC3_VIA_MPP04
#define PM8921_MPP04 0x4
#define SD_PWR_EN PM8921_MPP_PM_TO_SYS(PM8921_MPP04)

static uint32_t msm_sdc3_set_mpp04(struct device *dv, unsigned int vdd)
{
	int rc = 0;
	bool enable = (vdd != 0); /* voltage high */
	static bool is_enabled;
	bool is_always_on = mmc_vddp_reg_data[SDCC3].always_on;

	if (enable) {
		if (is_enabled)
			goto done;
		pr_info("%s: Enable uSD card Load SW\n", __func__);
		/* allocate GPIO for SD_PWR_EN */
		rc = gpio_request(SD_PWR_EN, "PM8921_MPP04");
		if (rc) {
			pr_err("gpio_request(%d, %s) failed %d\n",
				SD_PWR_EN,
				"PM8921_MPP04", rc);
			goto done;
		}
		/* GPIO OUTPUT, might sleep, enable GPIO */
		gpio_set_value_cansleep(SD_PWR_EN, 1);
		is_enabled = true;
	} else {
		if (is_always_on || !is_enabled)
			goto done;
		pr_info("%s: Disable uSD card Load SW\n", __func__);
		/* GPIO OUTPUT, might sleep, disable GPIO */
		gpio_set_value_cansleep(SD_PWR_EN, 0);
		/* release previously-claimed GPIO */
		gpio_free(SD_PWR_EN);
		is_enabled = false;
	}
done:
	return rc;
}
#endif

void __init msm8960_init_mmc(void)
{
#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
	msm8960_sdc1_data.swfi_latency = msm_rpm_get_swfi_latency();
	/* SDC1 : eMMC card connected */
	msm_add_sdcc(1, &msm8960_sdc1_data);
#endif
#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
	msm8960_sdc3_data.swfi_latency = msm_rpm_get_swfi_latency();
	/* SDC3: External card slot */
	msm_add_sdcc(3, &msm8960_sdc3_data);
#endif
}

static void __init msm8960_init_buses(void)
{
#ifdef CONFIG_MSM_BUS_SCALING
	msm_bus_rpm_set_mt_mask();
	msm_bus_8960_apps_fabric_pdata.rpm_enabled = 1;
	msm_bus_8960_sys_fabric_pdata.rpm_enabled = 1;
	msm_bus_8960_mm_fabric_pdata.rpm_enabled = 1;
	msm_bus_apps_fabric.dev.platform_data =
		&msm_bus_8960_apps_fabric_pdata;
	msm_bus_sys_fabric.dev.platform_data = &msm_bus_8960_sys_fabric_pdata;
	msm_bus_mm_fabric.dev.platform_data = &msm_bus_8960_mm_fabric_pdata;
	msm_bus_sys_fpb.dev.platform_data = &msm_bus_8960_sys_fpb_pdata;
	msm_bus_cpss_fpb.dev.platform_data = &msm_bus_8960_cpss_fpb_pdata;
#endif
}

#ifdef CONFIG_USB_MSM_OTG_72K
static struct msm_otg_platform_data msm_otg_pdata;
#else
static int msm_hsusb_vbus_power(bool on)
{
	int ret = 0;
#ifdef CONFIG_USB_NCP373
	if (on) {
		ret = pm8921_disable_source_current(1);
		if (unlikely(ret < 0)) {
			pr_err("%s: failed to notify boost the VBUS ret=%d\n",
								__func__, ret);
			goto do_vbus_off;
		}
		ret = ncp373_vbus_switch(1);
		if (unlikely(ret < 0)) {
			pr_err("%s: failed to switch the vbus load ret=%d\n",
								__func__, ret);
			goto do_vbus_off;
		}
		return ret;
	}
do_vbus_off:
	ncp373_vbus_switch(0);
	pm8921_disable_source_current(0);
#endif
	return ret;
}

static int semc_usb_phy_init_seq[] = {
	0x39, 0x81,
	0x24, 0x82,
	-1
};

static struct msm_otg_platform_data msm_otg_pdata = {
	.mode			= USB_OTG,
	.otg_control		= OTG_PMIC_CONTROL,
	.phy_type		= SNPS_28NM_INTEGRATED_PHY,
	.pmic_id_irq		= PM8921_USB_ID_IN_IRQ(PM8921_IRQ_BASE),
	.vbus_power		= msm_hsusb_vbus_power,
	.power_budget		= CONFIG_USB_MSM_OTG_POWER_BUDGET,
	.phy_init_seq		= semc_usb_phy_init_seq,
#ifdef CONFIG_FB_MSM_MHL_SII8334
	.mhl_dev_name		= SII_DEV_NAME,
#endif
};
#endif

#ifdef CONFIG_USB_NCP373
#define GPIO_USB_OTG_EN		51
#define GPIO_OTG_OVRCUR_DET_N	37
#define GPIO_OTG_OVP_CNTL	PM8921_GPIO_PM_TO_SYS(42)

struct ncp373_res_hdl {
	int en;
	int in;
	int flg;
};
static struct ncp373_res_hdl ncp373_hdl;

static int ncp373_gpio_request(int gpio, char *name)
{
	int ret;

	ret = gpio_request(gpio, name);

	if (unlikely(ret < 0)) {
		pr_err("%s: failed to request gpio=%d name=%s\n", __func__,
								gpio, name);
		ret = 0;
	} else {
		pr_debug("%s: got gpio gpio=%d name=%s\n", __func__,
								gpio, name);
		ret = gpio;
	}

	return ret;
}

static int ncp373_en_request(void)
{
	return ncp373_gpio_request(GPIO_OTG_OVP_CNTL, "ncp373_en");
}

static int ncp373_in_request(void)
{
	return ncp373_gpio_request(GPIO_USB_OTG_EN, "ncp373_in");
}

static int ncp373_flg_request(void)
{
	return ncp373_gpio_request(GPIO_OTG_OVRCUR_DET_N, "ncp373_flg");
}

static int ncp373_probe(struct platform_device *pdev)
{
	/* It may not be got a resource here,
	 * due to the timeliness of the device initialization.
	*/
	if (likely(!ncp373_hdl.en))
		ncp373_hdl.en = ncp373_en_request();

	if (likely(!ncp373_hdl.in))
		ncp373_hdl.in = ncp373_in_request();

	if (likely(!ncp373_hdl.flg))
		ncp373_hdl.flg = ncp373_flg_request();

	return 0;
}

static int ncp373_en_set(int on)
{
	int ret = -EIO;
	struct pm_gpio param = {
		.direction	= PM_GPIO_DIR_OUT,
		.output_buffer	= PM_GPIO_OUT_BUF_CMOS,
		.pull		= PM_GPIO_PULL_NO,
		.vin_sel	= PM_GPIO_VIN_VPH,
		.out_strength	= PM_GPIO_STRENGTH_LOW,
		.function	= PM_GPIO_FUNC_NORMAL,
	};

	if (unlikely(!ncp373_hdl.en))
		ncp373_hdl.en = ncp373_en_request();

	if (likely(ncp373_hdl.en)) {
		param.output_value = !on;
		ret = pm8xxx_gpio_config(ncp373_hdl.en, &param);

		if (unlikely(ret < 0))
			pr_err("%s: failed to switch %s gpio %d ret=%d\n",
						__func__, on ? "on " : "off",
						ncp373_hdl.en, ret);
	}

	return ret;
}

static int ncp373_in_set(int on)
{
	int ret = -EIO;

	if (unlikely(!ncp373_hdl.in))
		ncp373_hdl.in = ncp373_in_request();

	if (likely(ncp373_hdl.in)) {
		gpio_set_value(ncp373_hdl.in, !!on);
		ret = 0;
	}

	return ret;
}

static int ncp373_flg_get(void)
{
	int ret = -EIO;

	if (unlikely(!ncp373_hdl.flg))
		ncp373_hdl.flg = ncp373_flg_request();

	if (likely(ncp373_hdl.flg)) {
		ret = gpio_get_value(ncp373_hdl.flg);
		if (unlikely(ret < 0))
			pr_err("%s: failed to read GPIO=%d ret=%d\n",
						__func__, ncp373_hdl.flg, ret);
	}

	return ret;
}

static void ncp373_remove(void)
{
	if (likely(ncp373_hdl.en))
		gpio_free(ncp373_hdl.en);

	if (likely(ncp373_hdl.in))
		gpio_free(ncp373_hdl.in);

	if (likely(ncp373_hdl.flg))
		gpio_free(ncp373_hdl.flg);

	ncp373_hdl.en = 0;
	ncp373_hdl.in = 0;
	ncp373_hdl.flg = 0;
}

static void ncp373_notify_flg_int(void)
{
	pr_info("%s: received over current notify\n", __func__);
	msm_otg_notify_vbus_drop();
}

static void ncp373_check_pin_state(void)
{
	int en = -1;
	int in = -1;
	int flg = -1;

	if (likely(ncp373_hdl.en))
		en = gpio_get_value(ncp373_hdl.en);

	if (likely(ncp373_hdl.in))
		in = gpio_get_value(ncp373_hdl.in);

	if (likely(ncp373_hdl.flg))
		flg = gpio_get_value(ncp373_hdl.flg);

	pr_debug("%s: EN=%d, IN=%d, FLG=%d\n", __func__, en, in, flg);
}

struct ncp373_platform_data ncp373_pdata = {
	.probe		= ncp373_probe,
	.remove		= ncp373_remove,
	.en_set		= ncp373_en_set,
	.in_set		= ncp373_in_set,
	.flg_get	= ncp373_flg_get,
	.notify_flg_int	= ncp373_notify_flg_int,
	.check_pin_state = ncp373_check_pin_state,
	.oc_delay_time	= 300000,
};

static struct resource ncp373_resources[] = {
	/* OTG_OVERCUR_INT */
	{
		.start	= MSM_GPIO_TO_INT(GPIO_OTG_OVRCUR_DET_N),
		.end	= MSM_GPIO_TO_INT(GPIO_OTG_OVRCUR_DET_N),
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device ncp373_device = {
	.name		= NCP373_DRIVER_NAME,
	.id		= -1,
	.num_resources	= ARRAY_SIZE(ncp373_resources),
	.resource	= ncp373_resources,
	.dev		= {
		.platform_data = &ncp373_pdata,
	},
};
#endif
#ifdef CONFIG_USB_EHCI_MSM_HSIC
#define HSIC_HUB_RESET_GPIO	91
static struct msm_hsic_host_platform_data msm_hsic_pdata = {
	.strobe		= 150,
	.data		= 151,
};
#else
static struct msm_hsic_host_platform_data msm_hsic_pdata;
#endif

#define PID_MAGIC_ID		0x71432909
#define SERIAL_NUM_MAGIC_ID	0x61945374
#define SERIAL_NUMBER_LENGTH	127
#define DLOAD_USB_BASE_ADD	0x2A03F0C8

struct magic_num_struct {
	uint32_t pid;
	uint32_t serial_num;
};

struct dload_struct {
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
	uint16_t	reserved4;
	uint16_t	pid;
	char		serial_number[SERIAL_NUMBER_LENGTH];
	uint16_t	reserved5;
	struct magic_num_struct magic_struct;
};

static int usb_diag_update_pid_and_serial_num(uint32_t pid, const char *snum)
{
	struct dload_struct __iomem *dload = 0;

	dload = ioremap(DLOAD_USB_BASE_ADD, sizeof(*dload));
	if (!dload) {
		pr_err("%s: cannot remap I/O memory region: %08x\n",
					__func__, DLOAD_USB_BASE_ADD);
		return -ENXIO;
	}

	pr_debug("%s: dload:%p pid:%x serial_num:%s\n",
				__func__, dload, pid, snum);
	/* update pid */
	dload->magic_struct.pid = PID_MAGIC_ID;
	dload->pid = pid;

	/* update serial number */
	dload->magic_struct.serial_num = 0;
	if (!snum) {
		memset(dload->serial_number, 0, SERIAL_NUMBER_LENGTH);
		goto out;
	}

	dload->magic_struct.serial_num = SERIAL_NUM_MAGIC_ID;
	strlcpy(dload->serial_number, snum, SERIAL_NUMBER_LENGTH);
out:
	iounmap(dload);
	return 0;
}

static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
	.can_stall = 1,
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};

#ifdef CONFIG_USB_ANDROID_GG
#define STARTUP_REASON_INDUS_LOG	(1<<29)
static int __init startup_reason_setup(char *startup)
{
	unsigned long startup_reason = 0;
	int rval = 0;

	rval = strict_strtoul(startup, 0, &startup_reason);
	if (!rval) {
		pr_info("%s: 0x%lx\n", __func__, startup_reason);
		if (startup_reason & STARTUP_REASON_INDUS_LOG)
			android_enable_usb_gg(0x0FCE, 0xD14C);
	}
	return 1;
}
__setup("startup=", startup_reason_setup);
#endif


static uint8_t spm_wfi_cmd_sequence[] __initdata = {
			0x03, 0x0f,
};

static uint8_t spm_power_collapse_without_rpm[] __initdata = {
			0x00, 0x24, 0x54, 0x10,
			0x09, 0x03, 0x01,
			0x10, 0x54, 0x30, 0x0C,
			0x24, 0x30, 0x0f,
};

static uint8_t spm_power_collapse_with_rpm[] __initdata = {
			0x00, 0x24, 0x54, 0x10,
			0x09, 0x07, 0x01, 0x0B,
			0x10, 0x54, 0x30, 0x0C,
			0x24, 0x30, 0x0f,
};

static struct msm_spm_seq_entry msm_spm_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
};

static struct msm_spm_platform_data msm_spm_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW0_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_SECURE] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
		.reg_init_values[MSM_SPM_REG_SAW2_VCTL] = 0xB0,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020202,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0060009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x0000001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_seq_list),
		.modes = msm_spm_seq_list,
	},
	[1] = {
		.reg_base_addr = MSM_SAW1_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_SECURE] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
		.reg_init_values[MSM_SPM_REG_SAW2_VCTL] = 0xB0,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020202,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0060009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x0000001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_seq_list),
		.modes = msm_spm_seq_list,
	},
};

static uint8_t l2_spm_wfi_cmd_sequence[] __initdata = {
			0x00, 0x20, 0x03, 0x20,
			0x00, 0x0f,
};

static uint8_t l2_spm_gdhs_cmd_sequence[] __initdata = {
			0x00, 0x20, 0x34, 0x64,
			0x48, 0x07, 0x48, 0x20,
			0x50, 0x64, 0x04, 0x34,
			0x50, 0x0f,
};
static uint8_t l2_spm_power_off_cmd_sequence[] __initdata = {
			0x00, 0x10, 0x34, 0x64,
			0x48, 0x07, 0x48, 0x10,
			0x50, 0x64, 0x04, 0x34,
			0x50, 0x0F,
};

static struct msm_spm_seq_entry msm_spm_l2_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_L2_MODE_RETENTION,
		.notify_rpm = false,
		.cmd = l2_spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_L2_MODE_GDHS,
		.notify_rpm = true,
		.cmd = l2_spm_gdhs_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_L2_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = l2_spm_power_off_cmd_sequence,
	},
};


static struct msm_spm_platform_data msm_spm_l2_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW_L2_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_SECURE] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020202,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x00A000AE,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A00020,
		.modes = msm_spm_l2_seq_list,
		.num_modes = ARRAY_SIZE(msm_spm_l2_seq_list),
	},
};

static struct regulator *vreg_touch_vdd;

static int clearpad_vreg_low_power_mode(int enable)
{
	int rc = 0;

	if (IS_ERR(vreg_touch_vdd)) {
		pr_err("%s: vreg_touch_vdd is not initialized\n", __func__);
		return -ENODEV;
	}

	if (enable)
		rc = regulator_set_optimum_mode(vreg_touch_vdd, 1000);
	else
		rc = regulator_set_optimum_mode(vreg_touch_vdd, 15000);

	if (rc < 0) {
		pr_err("%s: vdd: set mode (%s) failed, rc=%d\n",
			__func__, (enable ? "LPM" : "HPM"), rc);
		return rc;
	} else {
		pr_debug("%s: vdd: set mode (%s) ok, new mode=%d\n",
				__func__, (enable ? "LPM" : "HPM"), rc);
		return 0;
	}
}

static int clearpad_vreg_configure(int enable)
{
	int rc = 0;

	vreg_touch_vdd = regulator_get(NULL, "8921_l17");
	if (IS_ERR(vreg_touch_vdd)) {
		pr_err("%s: get vdd failed\n", __func__);
		return -ENODEV;
	}

	if (enable) {
		rc = regulator_set_voltage(vreg_touch_vdd, 3000000, 3000000);
		if (rc) {
			pr_err("%s: set voltage failed, rc=%d\n", __func__, rc);
			goto clearpad_vreg_configure_err;
		}
		rc = regulator_enable(vreg_touch_vdd);
		if (rc) {
			pr_err("%s: enable vdd failed, rc=%d\n", __func__, rc);
			goto clearpad_vreg_configure_err;
		}
		rc = clearpad_vreg_low_power_mode(0);
		if (rc) {
			pr_err("%s: set vdd mode failed, rc=%d\n",
				__func__, rc);
			goto clearpad_vreg_configure_err;
		}
	} else {
		rc = regulator_disable(vreg_touch_vdd);
		if (rc)
			pr_err("%s: disable vdd failed, rc=%d\n",
								__func__, rc);
	}
	return rc;
clearpad_vreg_configure_err:
	regulator_put(vreg_touch_vdd);
	return rc;
}

#define SYNAPTICS_TOUCH_GPIO_IRQ	(11)
static int clearpad_gpio_configure(int enable)
{
	int rc = 0;

	if (enable) {
		rc = gpio_request(SYNAPTICS_TOUCH_GPIO_IRQ, CLEARPAD_NAME);
		if (rc)
			pr_err("%s: gpio_requeset failed, "
					"rc=%d\n", __func__, rc);
	} else {
		gpio_free(SYNAPTICS_TOUCH_GPIO_IRQ);
	}
	return rc;
}

static struct clearpad_platform_data clearpad_platform_data = {
	.irq = MSM_GPIO_TO_INT(SYNAPTICS_TOUCH_GPIO_IRQ),
	.funcarea_get = clearpad_funcarea_get,
	.vreg_configure = clearpad_vreg_configure,
	.vreg_suspend = clearpad_vreg_low_power_mode,
	.gpio_configure = clearpad_gpio_configure,
};


#define LM3533_HWEN_GPIO 54
static int lm3533_setup(struct device *dev)
{
	int rc = gpio_request(LM3533_HWEN_GPIO, "lm3533_hwen");
	if (rc)
		dev_err(dev, "failed to request gpio %d\n", LM3533_HWEN_GPIO);
	gpio_direction_output(LM3533_HWEN_GPIO, 0);
	return rc;
}
static void lm3533_teardown(struct device *dev)
{
	gpio_free(LM3533_HWEN_GPIO);
	return;
}
static int lm3533_power_on(struct device *dev)
{
	gpio_set_value(LM3533_HWEN_GPIO, 1);
	return 0;
}
static int lm3533_power_off(struct device *dev)
{
	gpio_set_value(LM3533_HWEN_GPIO, 0);
	return 0;
}

static struct lm3533_platform_data lm3533_pdata = {
	.b_cnf = {
		[LM3533_CBNKA] = {
			.pwm = 0,
			.ctl = LM3533_HVA_MAP_LIN | LM3533_HVA_BR_CTL,
			.fsc =  I_UA_TO_FSC(20000),
			.iname = "lcd-backlight",
		},
		[LM3533_CBNKC] = {
			.pwm = 0,
			.ctl = LM3533_LV_MAP_LIN | LM3533_LV_BR_CTL,
			.fsc =  I_UA_TO_FSC(20000),
			.iname = "red",
		},
		[LM3533_CBNKD] = {
			.pwm = 0,
			.ctl = LM3533_LV_MAP_LIN | LM3533_LV_BR_CTL,
			.fsc =  I_UA_TO_FSC(20000),
			.iname = "green",
		},
		[LM3533_CBNKE] = {
			.pwm = 0,
			.ctl = LM3533_LV_MAP_LIN | LM3533_LV_BR_CTL,
			.fsc =  I_UA_TO_FSC(20000),
			.iname = "blue",
		},
	},
	.l_cnf = {
		[LM3533_HVLED1] = {
			.connected = true, .cpout = true, .bank =  LM3533_CBNKA,
		},
		[LM3533_LVLED1] = {
			.connected = true, .cpout = true, .bank =  LM3533_CBNKC,
		},
		[LM3533_LVLED2] = {
			.connected = true, .cpout = true, .bank =  LM3533_CBNKD,
		},
		[LM3533_LVLED3] = {
			.connected = true, .cpout = true, .bank =  LM3533_CBNKE,
		},
	},
	.ovp_boost_pwm = LM3533_BOOST_500KHZ | LM3533_OVP_32V | LM3533_PWM_HIGH,
	.setup = lm3533_setup,
	.teardown = lm3533_teardown,
	.power_on = lm3533_power_on,
	.power_off = lm3533_power_off,
};

#define LM356X_HW_RESET_GPIO 2
int lm356x_request_gpio_pins(void)
{
	int result;

	result = gpio_request(LM356X_HW_RESET_GPIO, "LM356X hw reset");
	if (result)
		return result;

	gpio_direction_output(LM356X_HW_RESET_GPIO, 1);

	udelay(20);
	return result;
}

int lm356x_release_gpio_pins(void)
{
	gpio_set_value(LM356X_HW_RESET_GPIO, 0);
	gpio_free(LM356X_HW_RESET_GPIO);

	return 0;
}

static struct lm356x_platform_data lm3560_platform_data = {
	.hw_enable		= lm356x_request_gpio_pins,
	.hw_disable		= lm356x_release_gpio_pins,
	.led_nums		= 2,
	.strobe_trigger		= LM356X_STROBE_TRIGGER_EDGE,
	.privacy_terminate	= LM356X_PRIVACY_MODE_TURN_BACK,
	.privacy_led_nums	= 1,
	.privacy_blink_period	= 0, /* No bliking */
	.current_limit		= 2300000, /* uA */
	.flash_sync		= LM356X_SYNC_OFF,
	.strobe_polarity	= LM356X_STROBE_POLARITY_HIGH,
	.ledintc_pin_setting	= LM356X_LEDINTC_NTC_THERMISTOR_INPUT,
	.tx1_polarity		= LM356X_TX1_POLARITY_HIGH,
	.tx2_polarity		= LM356X_TX2_POLARITY_HIGH,
	.hw_torch_mode		= LM356X_HW_TORCH_MODE_DISABLE,
};

#define PM8921_HSED_MIC_BIAS 0xA1

struct pm8921_mic_bias_platform_data pm8921_mic_bias_pdata = {
	.mic_bias_addr = PM8921_HSED_MIC_BIAS,
};

#define PLUG_DET_ENA_PIN  -1
#define PLUG_DET_READ_PIN PM8921_GPIO_PM_TO_SYS(38)
#define BUTTON_DET_PIN    -1
#define PM8921_HSED_CNTR_BASE 0x134

int simple_remote_pf_initialize_gpio(struct simple_remote_platform_data *data)
{
	int err = 0;
	if (!data ||
	    -1 == data->headset_detect_read_pin) {
		pr_err("%s: Invalid inparameter (GPIO Pins)."
		       " Aborting!\n", __func__);
		return -EIO;
	}

	err = gpio_request(data->headset_detect_read_pin,
			   "Simple_remote_plug_detect_read");
	if (err) {
		pr_err("%s: %d Request hs-detect_read pin",
		       __func__, err);
		goto out;
	}

	err = gpio_direction_input(data->headset_detect_read_pin);
	if (err) {
		pr_err("%s: %d Set hs-detect pin as input\n",
		       __func__, err);
		goto out_hs_det_read;
	}

	return err;

out_hs_det_read:
	gpio_free(data->headset_detect_read_pin);
out:
	return err;
}

void simple_remote_pf_deinitialize_gpio(
	struct simple_remote_platform_data *data)
{
	gpio_free(data->headset_detect_read_pin);
}

struct simple_remote_platform_data simple_remote_pf_data = {
	.headset_detect_enable_pin = PLUG_DET_ENA_PIN,
	.headset_detect_read_pin = PLUG_DET_READ_PIN,
	.button_detect_irq = BUTTON_DET_PIN,
	.adc_channel = CHANNEL_MPP_1,
	.hsed_base = PM8921_HSED_CNTR_BASE,
	.initialize = &simple_remote_pf_initialize_gpio,
	.deinitialize = &simple_remote_pf_deinitialize_gpio,
#ifdef CONFIG_SIMPLE_REMOTE_INVERT_PLUG_DETECTION_STATE
	.invert_plug_det = 1,
#else
	.invert_plug_det = 0,
#endif
};

#if defined CONFIG_INPUT_BMA250_NG || defined CONFIG_INPUT_BMA250
#define BMA250_GPIO 10
#define BMA250_DEFAULT_RATE 50
static int bma250_gpio_setup(struct device *dev)
{
	return 0;
}

static void bma250_gpio_teardown(struct device *dev)
{
	return;
}

static void bma250_hw_config(int enable)
{
	return;
}

static struct registers bma250_reg_setup = {
	.range                = BMA250_RANGE_2G,
	.bw_sel               = BMA250_BW_250HZ,
};

static struct bma250_platform_data bma250_platform_data = {
	.setup                = bma250_gpio_setup,
	.teardown             = bma250_gpio_teardown,
	.hw_config            = bma250_hw_config,
	.reg                  = &bma250_reg_setup,
#ifdef CONFIG_INPUT_BMA250_NG
	.bypass_state         = mpu3050_bypassmode,
	.read_axis_data       = bma250_read_axis_from_mpu3050,
	.check_sleep_status   = check_bma250_sleep_state,
	.vote_sleep_status    = vote_bma250_sleep_state,
#endif
	.rate                 = BMA250_DEFAULT_RATE,
};
#endif

#ifdef CONFIG_INPUT_AKM8972
//#define AKM897X_GPIO 70
#define AKM897X_GPIO 6
static int akm897x_gpio_setup(void)
{
	int rc;

	rc = gpio_request(AKM897X_GPIO, "akm8972_drdy");
	if (rc)
		pr_err("%s: gpio_request failed rc=%d\n", __func__, rc);
	return rc;
}

static void akm897x_gpio_shutdown(void)
{
	gpio_free(AKM897X_GPIO);
}

static void akm897x_hw_config(int enable)
{
	return;
}

static struct akm8972_platform_data akm8972_platform_data = {
	.setup		= akm897x_gpio_setup,
	.shutdown	= akm897x_gpio_shutdown,
	.hw_config	= akm897x_hw_config,
};
#endif

#ifdef CONFIG_SENSORS_MPU3050
#define MPU3050_GPIO 69
int mpu3050_gpio_setup(struct device *dev, int enable)
{
	int rc = 0;

	if (enable) {
		rc = gpio_request(MPU3050_GPIO, "MPUIRQ");
		if (rc)
			pr_err("%s: gpio_request failed. rc=%d\n",
					__func__, rc);
	} else {
		gpio_free(MPU3050_GPIO);
	}

	return rc;
}

void mpu3050_hw_config(int enable)
{
	return;
}

extern struct mpu3050_platform_data mpu_data;
#endif

#ifdef CONFIG_SENSORS_MPU6050
#define MPU6050_GPIO 69
#define AKM897X_GPIO 70
int mpu6050_gpio_setup(struct device *dev, int enable)
{
	int rc = 0;

	if (enable) {
		rc = gpio_request(MPU6050_GPIO, "MPUIRQ");
		if (rc)
			pr_err("%s: gpio_request failed. rc=%d\n",
					__func__, rc);
	} else
		gpio_free(MPU6050_GPIO);

	return rc;
}
void mpu6050_hw_config(int enable)
{
	return;
}

extern struct mpu_platform_data mpu6050_data;
extern struct ext_slave_platform_data mpu_compass_data;
#endif /* CONFIG_SENSORS_MPU6050 */

#ifdef CONFIG_INPUT_APDS9702
#define APDS9702_DOUT_GPIO   49

static int apds9702_gpio_setup(int request)
{
	if (request) {
		return gpio_request(APDS9702_DOUT_GPIO, "apds9702_dout");
	} else {
		gpio_free(APDS9702_DOUT_GPIO);
		return 0;
	}
}

static void apds9702_hw_config(int enable)
{
	return;
}

static struct apds9702_platform_data apds9702_pdata = {
	.gpio_dout      = APDS9702_DOUT_GPIO,
	.is_irq_wakeup  = 1,
	.hw_config      = apds9702_hw_config,
	.gpio_setup     = apds9702_gpio_setup,
	.ctl_reg = {
		.trg   = 1,
		.pwr   = 1,
		.burst = 7,
		.frq   = 3,
		.dur   = 2,
		.th    = 15,
		.rfilt = 0,
	},
	.phys_dev_path = "/sys/devices/i2c-12/12-0054"
};
#endif

#ifdef CONFIG_LEDS_AS3676
extern struct as3676_platform_data as3676_platform_data;
#endif

#ifdef CONFIG_VIBRATOR_LC898300
#define VIB_RSTB 79
#define VIB_EN 53

static struct regulator *ldo16;

static int lc898300_rstb_gpio_setup(bool value)
{
	if (ldo16 == NULL)
		return -EINVAL;
	gpio_set_value(VIB_RSTB, value);
	return 0;
}

static int lc898300_en_gpio_setup(bool value)
{
	if (ldo16 == NULL)
		return -EINVAL;
	gpio_set_value(VIB_EN, value);
	return 0;
}

static int lc898300_gpio_allocate(struct device *dev)
{
	int rc;

	rc = gpio_request(VIB_RSTB, "vibrator reset_gpio");
	if (rc) {
		dev_err(dev, "%s: GPIO %d: request failed. rc=%d\n",
			      __func__, VIB_RSTB, rc);
		return rc;
	}

	rc = gpio_direction_output(VIB_RSTB, 0);
	if (rc) {
		dev_err(dev, "%s: GPIO %d: direction out failed rc=%d\n",
			      __func__, VIB_EN, rc);
		goto error1;
	}

	rc = gpio_request(VIB_EN, "vibrator enable_gpio");
	if (rc) {
		dev_err(dev, "%s: GPIO %d: request failed. rc=%d\n",
			      __func__, VIB_EN, rc);
		goto error1;
	}

	rc = gpio_direction_output(VIB_EN, 0);
	if (rc) {
		dev_err(dev, "%s: GPIO %d: direction out failed rc=%d\n",
			__func__, VIB_EN, rc);
		goto error2;
	}

	return rc;

error2:
	gpio_free(VIB_EN);
error1:
	gpio_free(VIB_RSTB);
	return rc;
}

static void lc898300_gpio_release(struct device *dev)
{
	gpio_free(VIB_RSTB);
	gpio_free(VIB_EN);
}

static int lc898300_power_config(struct device *dev, bool on)
{
	int rc = 0;

	if (on) {
		ldo16 = regulator_get(NULL, "8921_l16");
		if (IS_ERR(ldo16)) {
			dev_err(dev, "%s: could not get 8921_l16, rc = %ld\n",
				__func__, PTR_ERR(ldo16));
			return -ENODEV;
		}
		rc = regulator_set_voltage(ldo16, 3050000, 3050000);
		if (rc) {
			dev_err(dev, "%s: unable to set L16 voltage to "
					"3.075V, rc = %d!\n", __func__, rc);
			goto error;
		}
	} else {
		goto error;
	}

	return rc;
error:
	regulator_put(ldo16);
	ldo16 = NULL;
	return rc;
}

static int lc898300_power_enable(struct device *dev, bool on)
{
	int rc = 0;

	if (ldo16 == NULL) {
		dev_err(dev, "%s: ldo16 = NULL\n", __func__);
		return -EINVAL;
	}

	if (on) {
		rc = regulator_enable(ldo16);
		if (rc) {
			dev_err(dev, "%s: enable regulator 8921_l16 failed\n",
					__func__);
		}
	} else {
		rc = regulator_disable(ldo16);
		if (rc)
			dev_err(dev, "'%s: regulator 8921_l16 disable failed"
					", rc=%d\n", __func__, rc);
	}

	return rc;
}

static struct lc898300_platform_data lc898300_platform_data = {
	.name = "vibrator",
	.power_config = lc898300_power_config,
	.power_enable = lc898300_power_enable,
	.gpio_allocate = lc898300_gpio_allocate,
	.gpio_release = lc898300_gpio_release,
	.rstb_gpio_setup = lc898300_rstb_gpio_setup,
	.en_gpio_setup = lc898300_en_gpio_setup,
	.vib_cmd_info = &lc898300_vib_cmd_data,
};
#endif

#ifdef CONFIG_INPUT_LSM303DLH_ACCELEROMETER
static struct lsm303dlh_acc_platform_data lsm303dlh_acc_platform_data = {
	.range = LSM303_RANGE_8G,
	.poll_interval_ms = 100,
	.irq_pad = 10,
	.power_on = NULL,
	.power_off = NULL,
};
#endif

#ifdef CONFIG_INPUT_LSM303DLHC_ACCELEROMETER
static struct lsm303dlhc_acc_platform_data lsm303dlhc_acc_platform_data = {
	.range = 8,
	.poll_interval_ms = 100,
	.mode = MODE_INTERRUPT,
	.irq_pad = 10,
	.power = NULL,
};
#endif

#ifdef CONFIG_INPUT_LSM303DLH_MAGNETOMETER
static struct lsm303dlh_mag_platform_data lsm303dlh_mag_platform_data = {
	.range = LSM303_RANGE_8200mG,
	.poll_interval_ms = 100,
	.power_on = NULL,
	.power_off = NULL,
};
#endif

#ifdef CONFIG_INPUT_L3G4200D
static struct l3g4200d_gyr_platform_data l3g4200d_gyro_platform_data = {
	.poll_interval = 100,
	.min_interval = 40,

	.fs_range = L3G4200D_FS_2000DPS,

	.axis_map_x =  0,
	.axis_map_y = 1,
	.axis_map_z = 2,

	.negate_x = 0,
	.negate_y = 0,
	.negate_z = 0,

	.init = NULL,
	.exit = NULL,
	.power_on = NULL,
	.power_off = NULL,
};
#endif

#define NOA3402_GPIO	49
static int noa3402_gpio_setup(int request)
{
	int rc = 0;

	if (request) {
		rc = gpio_request(NOA3402_GPIO, "noa3402");
		if (rc) {
			pr_err("%s: gpio_request failed!", __func__);
			return rc;
		}

		rc = gpio_direction_input(NOA3402_GPIO);
		if (rc) {
			pr_err(KERN_ERR "%s: gpio_direction_input failed!",
								__func__);
			goto exit;
		}
		return rc;
	}
exit:
	gpio_free(NOA3402_GPIO);
	return rc;
}

static int noa3402_hw_config(int enable)
{
	return 0;
}

static struct noa3402_platform_data noa3402_pdata = {
	.gpio				= NOA3402_GPIO,
	.pwm_sensitivity		= PWM_SENSITIVITY_STD,
	.pwm_res			= PWM_RES_8_BIT,
	.pwm_type			= PWM_TYPE_LOG,
	.ps_filter_nbr_correct		= 1,
	.ps_filter_nbr_measurements	= 1,
	.ps_led_current			= LED_CURRENT_MA_TO_REG(160),
	.ps_integration_time		= PS_INTEGRATION_300_US,
	.ps_interval			= PS_INTERVAL_MS_TO_REG(50),
	.als_integration_time		= ALS_INTEGRATION_100_MS,
	.als_interval			= ALS_INTERVAL_MS_TO_REG(500),
	.is_irq_wakeup			= 1,
	.phys_dev_path			= "/sys/bus/i2c/devices/3-0037",
	.hw_config			= noa3402_hw_config,
	.gpio_setup			= noa3402_gpio_setup,
};

static struct i2c_board_info gsbi3_peripherals_info[] __initdata = {
	{
		/* Config-spec is 8-bit = 0x58, src-code need 7-bit => 0x2c */
		I2C_BOARD_INFO(CLEARPADI2C_NAME, 0x58 >> 1),
		.platform_data = &clearpad_platform_data,
	}
};

static struct i2c_board_info gsbi10_peripherals_info[] __initdata = {
	{
		/* Config-spec is 8-bit = 0xa6, src-code need 7-bit => 0x53 */
		I2C_BOARD_INFO("lm3560", 0xa6 >> 1),
		.platform_data = &lm3560_platform_data,
	},
#ifdef CONFIG_LEDS_AS3676
	{
		/* Config-spec is 8-bit = 0x80, src-code need 7-bit => 0x40 */
		I2C_BOARD_INFO("as3676", 0x80 >> 1),
		.platform_data = &as3676_platform_data,
	},
#endif
	{
		/* Config-spec is 8-bit = 0x6c, src-code need 7-bit => 0x36 */
		I2C_BOARD_INFO(LM3533_DEV_NAME, 0x6c >> 1),
		.platform_data = &lm3533_pdata,
		.irq = -1
	},
#ifdef CONFIG_NFC_PN544
	{
		/* Config-spec is 8-bit = 0x50, src-code need 7-bit => 0x28 */
		I2C_BOARD_INFO(PN544_DEVICE_NAME, 0x50 >> 1),
		.irq = MSM_GPIO_TO_INT(MSM_GPIO_NFC_IRQ),
		.platform_data = &pn544_pdata,
	},
#endif
#ifdef CONFIG_FB_MSM_MHL_SII8334
	{
		I2C_BOARD_INFO("sii8334", 0x72 >> 1),
		.irq = MSM_GPIO_TO_INT(MSM_GPIO_MHL_IRQ_N),
		.platform_data = &mhl_sii_pdata,
	},
#endif /* CONFIG_FB_MSM_MHL_SII8334 */
};

static struct i2c_board_info gsbi12_peripherals_info[] __initdata = {
#ifdef CONFIG_INPUT_LSM303DLH_ACCELEROMETER
	{
		/* Config-spec is 8-bit = 0x30, src-code need 7-bit => 0x18 */
		I2C_BOARD_INFO(LSM303DLH_ACC_DEV_NAME, 0x30 >> 1),
		.platform_data = &lsm303dlh_acc_platform_data,
	},
#endif
#ifdef CONFIG_INPUT_LSM303DLHC_ACCELEROMETER
	{
		/* Config-spec is 8-bit = 0x32, src-code need 7-bit => 0x19 */
		I2C_BOARD_INFO(LSM303DLHC_ACC_DEV_NAME, 0x32 >> 1),
		.platform_data = &lsm303dlhc_acc_platform_data,
	},
#endif
#ifdef CONFIG_INPUT_LSM303DLH_MAGNETOMETER
	{
		/* Config-spec is 8-bit = 0x3c, src-code need 7-bit => 0x1e */
		I2C_BOARD_INFO(LSM303DLH_MAG_DEV_NAME, 0x3c >> 1),
		.platform_data = &lsm303dlh_mag_platform_data,
	},
#endif
#if defined CONFIG_INPUT_BMA250 || defined CONFIG_INPUT_BMA250_NG
	{
		/* Config-spec is 8-bit = 0x30, src-code need 7-bit => 0x18 */
		I2C_BOARD_INFO("bma250", 0x30 >> 1),
		.irq = MSM_GPIO_TO_INT(BMA250_GPIO),
		.platform_data = &bma250_platform_data,
	},
#endif
#ifdef CONFIG_VIBRATOR_LC898300
	{
		/* Config-spec is 8-bit = 0x92, src-code need 7-bit => 0x49 */
		I2C_BOARD_INFO(LC898300_I2C_NAME, 0x92 >> 1),
		.platform_data = &lc898300_platform_data,
	},
#endif
#ifdef CONFIG_INPUT_AKM8972
	{
		/* Config-spec is 8-bit = 0x18, src-code need 7-bit => 0x0c */
		I2C_BOARD_INFO(AKM8972_I2C_NAME, 0x18 >> 1),
		.irq = MSM_GPIO_TO_INT(AKM897X_GPIO),
		.platform_data = &akm8972_platform_data,
	},
#endif
#ifdef CONFIG_SENSORS_MPU3050
	{
		/* Config-spec is 8-bit = 0xD0, src-code need 7-bit => 0x68 */
		I2C_BOARD_INFO(MPU_NAME, 0xD0 >> 1),
		.irq = MSM_GPIO_TO_INT(MPU3050_GPIO),
		.platform_data = &mpu_data,
	},
#endif
#ifdef CONFIG_SENSORS_MPU6050
	{
		/* Config-spec is 8-bit = 0xD0, src-code need 7-bit => 0x68 */
		I2C_BOARD_INFO("mpu6050", 0xD0 >> 1),
		.irq = MSM_GPIO_TO_INT(MPU6050_GPIO),
		.platform_data = &mpu6050_data,
	},
#endif
#ifdef CONFIG_MPU_SENSORS_AK8972
	{
		/* Config-spec is 8-bit = 0x18, src-code need 7-bit => 0x0c */
		I2C_BOARD_INFO("ak8972", 0x18 >> 1),
		.irq = MSM_GPIO_TO_INT(AKM897X_GPIO),
		.platform_data = &mpu_compass_data,
	},
#endif
#ifdef CONFIG_INPUT_APDS9702
	{
		/* Config-spec is 8-bit = 0xA8, src-code need 7-bit => 0x54 */
		I2C_BOARD_INFO(APDS9702_NAME, 0xA8 >> 1),
		.platform_data = &apds9702_pdata,
	},
#endif
#ifdef CONFIG_INPUT_BMP180
	{
		/* Config-spec is 8-bit = 0xee, src-code need 7-bit => 0x77 */
		I2C_BOARD_INFO("bmp180", 0xee >> 1)
	},
#endif
#ifdef CONFIG_INPUT_L3G4200D
	{
		/* Config-spec is 8-bit = 0xd0, src-code need 7-bit => 0x68 */
		I2C_BOARD_INFO(L3G4200D_DEV_NAME, 0xd0 >> 1),
		.platform_data = &l3g4200d_gyro_platform_data,
	},
#endif
	{
		/* Config-spec is 8-bit = 0x6e, src-code need 7-bit => 0x37 */
		I2C_BOARD_INFO(NOA3402_NAME, 0x6e >> 1),
		.platform_data = &noa3402_pdata
	},
#if 0 // TODO: remove this link to old cyttsp
	{ //Midholt TMA4xx
		I2C_BOARD_INFO(TTSP_I2C_NAME, TTSP_I2C_TCH_ADR),
		.irq =  MSM_GPIO_TO_INT(CYTTSP5_I2C_IRQ_GPIO),
		.platform_data = TTSP_I2C_NAME,
	},
#else
	{ // KEVKEV
		I2C_BOARD_INFO(CYTTSP5_I2C_NAME, CYTTSP5_I2C_TCH_ADR),
		.irq =  MSM_GPIO_TO_INT(CYTTSP5_I2C_IRQ_GPIO),
		.platform_data = CYTTSP5_I2C_NAME,
	},
#endif
};

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi4_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi3_pdata = {
	.clk_freq = 400000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi10_pdata = {
	.clk_freq = 355000,
	.src_clk_rate = 24000000,
#ifdef CONFIG_MSM_GSBI10_UART
	.use_gsbi_shared_mode = 1,
#endif
};

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi12_pdata = {
	.clk_freq = 355000,
	.src_clk_rate = 24000000,
#ifdef CONFIG_MSM_GSBI12_UART
	.use_gsbi_shared_mode = 1,
#endif
};

static struct msm_rpm_platform_data msm_rpm_data = {
	.reg_base_addrs = {
		[MSM_RPM_PAGE_STATUS] = MSM_RPM_BASE,
		[MSM_RPM_PAGE_CTRL] = MSM_RPM_BASE + 0x400,
		[MSM_RPM_PAGE_REQ] = MSM_RPM_BASE + 0x600,
		[MSM_RPM_PAGE_ACK] = MSM_RPM_BASE + 0xa00,
	},

	.irq_ack = RPM_APCC_CPU0_GP_HIGH_IRQ,
	.irq_err = RPM_APCC_CPU0_GP_LOW_IRQ,
	.irq_vmpm = RPM_APCC_CPU0_GP_MEDIUM_IRQ,
	.msm_apps_ipc_rpm_reg = MSM_APCS_GCC_BASE + 0x008,
	.msm_apps_ipc_rpm_val = 4,
};

static struct spi_board_info spi_board_info[] __initdata = {
	/* No devices yet */
};

static struct platform_device msm_device_saw_core0 = {
	.name          = "saw-regulator",
	.id            = 0,
	.dev	= {
		.platform_data = &msm_saw_regulator_pdata_s5,
	},
};

static struct platform_device msm_device_saw_core1 = {
	.name          = "saw-regulator",
	.id            = 1,
	.dev	= {
		.platform_data = &msm_saw_regulator_pdata_s6,
	},
};

#ifdef CONFIG_FB_MSM_WRITEBACK_MSM_PANEL
static struct platform_device wfd_device = {
	.name          = "msm_wfd",
	.id            = -1,
};
#endif

#if 0
static struct tsens_platform_data msm_tsens_pdata  = {
		.slope			= 910,
		.tsens_factor		= 1000,
		.hw_type		= MSM_8960,
		.tsens_num_sensor	= 5,
};
#endif

#ifdef CONFIG_MSM_FAKE_BATTERY
static struct platform_device fish_battery_device = {
	.name = "fish_battery",
};
#endif

static struct platform_device msm8960_device_rpm_regulator __devinitdata = {
	.name	= "rpm-regulator",
	.id	= -1,
	.dev	= {
		.platform_data = &msm_rpm_regulator_pdata,
	},
};

static struct msm_rpm_log_platform_data msm_rpm_log_pdata = {
	.phys_addr_base = 0x0010C000,
	.reg_offsets = {
		[MSM_RPM_LOG_PAGE_INDICES] = 0x00000080,
		[MSM_RPM_LOG_PAGE_BUFFER]  = 0x000000A0,
	},
	.phys_size = SZ_8K,
	.log_len = 4096,		  /* log's buffer length in bytes */
	.log_len_mask = (4096 >> 2) - 1,  /* length mask in units of u32 */
};

static struct platform_device msm_rpm_log_device = {
	.name	= "msm_rpm_log",
	.id	= -1,
	.dev	= {
		.platform_data = &msm_rpm_log_pdata,
	},
};

#ifdef CONFIG_MSM_GSBI10_UART
static int felica_uart_pre_startup(struct uart_port *uport)
{
	struct tty_struct *tty = uport->state->port.tty;

	mutex_lock(&tty->termios_mutex);
	tty->termios->c_ispeed = 460800;
	tty->termios->c_ospeed = 460800;
	tty->termios->c_cflag |= B460800;
	tty->termios->c_cflag &= ~0xD;
	tty->termios->c_cflag |= (CLOCAL | CREAD);
	tty->termios->c_cflag &= ~PARENB;
	tty->termios->c_cflag &= ~CSTOPB;
	tty->termios->c_cflag &= ~CSIZE;
	tty->termios->c_cflag &= ~PARODD;
	tty->termios->c_cflag |= CS8;
	tty->termios->c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);
	tty->termios->c_oflag &= ~OPOST;
	tty->termios->c_iflag &= ~(ICRNL | INPCK | ISTRIP |
						IXON | BRKINT);
	tty->termios->c_cc[VMIN] = 0;
	tty->termios->c_cc[VTIME] = 0;
	tty->ldisc->ops->set_termios(tty, NULL);
	mutex_unlock(&tty->termios_mutex);

	return 0;
}

static int configure_uart_dm10_gpios(int on)
{
	int ret = 0, i;
	int uart_gpios[] = {71, 72};

	for (i = 0; i < ARRAY_SIZE(uart_gpios); i++) {
		if (on) {
			ret = msm_gpiomux_get(uart_gpios[i]);
			if (unlikely(ret))
				break;
		} else {
			ret = msm_gpiomux_put(uart_gpios[i]);
			if (unlikely(ret))
				return ret;
		}
	}
	if (ret)
		for (; i >= 0; i--)
			msm_gpiomux_put(uart_gpios[i]);
	return ret;
}
static struct msm_serial_hs_platform_data uart_gsbi10_pdata = {
	.gpio_config = configure_uart_dm10_gpios,
	.inject_rx_on_wakeup = 0,
	.pre_startup	= felica_uart_pre_startup,
};
#endif

#ifdef CONFIG_MSM_GSBI12_UART
static struct msm_serial_hslite_platform_data msm_uart_gsbi12_pdata = {
	.config_gpio	= 1,
	.uart_tx_gpio	= 42,
	.uart_rx_gpio	= 43,
	.type		= PORT_IRDA,
};
#endif

static struct platform_device *common_devices[] __initdata = {
	&msm8960_device_dmov,
	&msm_device_smd,
	&msm8960_device_uart_gsbi8,
#ifdef CONFIG_MSM_GSBI12_UART
	&msm8960_device_uart_gsbi12,
#endif
	&msm_device_uart_dm6,
#ifdef CONFIG_MSM_GSBI10_UART
	&msm_device_uart_dm10,
#endif
	&msm_device_saw_core0,
	&msm_device_saw_core1,
	&msm8960_device_ssbi_pmic,
	&msm8960_device_qup_i2c_gsbi3,
	&msm8960_device_qup_i2c_gsbi4,
	&msm8960_device_qup_i2c_gsbi10,
#ifndef CONFIG_MSM_DSPS
	&msm8960_device_qup_i2c_gsbi12,
#endif
	&msm_slim_ctrl,
	&msm_device_wcnss_wlan,
#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)
	&qcrypto_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)
	&qcedev_device,
#endif
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
	&msm_device_sps,
#ifdef CONFIG_MSM_FAKE_BATTERY
	&fish_battery_device,
#endif
	&fmem_device,
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	&android_pmem_device,
	&android_pmem_adsp_device,
#endif
	&android_pmem_camera_device,
	&android_pmem_audio_device,
	&android_pmem_tz_device,
#endif
	&msm_fb_device,
	&msm_device_vidc,
	&msm_device_bam_dmux,
	&msm_fm_platform_init,

#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)
	&msm_device_tsif,
#endif
#ifdef CONFIG_SEMC_ONESEG_TUNER_PM
	&oneseg_tunerpm_device,
#endif
#ifdef CONFIG_HW_RANDOM_MSM
	&msm_device_rng,
#endif
	&msm_rpm_device,
#ifdef CONFIG_ION_MSM
	&ion_dev,
#endif
	&msm_rpm_log_device,
	&msm_rpm_stat_device,
	&msm_device_tz_log,

#ifdef CONFIG_MSM_QDSS
	&msm_etb_device,
	&msm_tpiu_device,
	&msm_funnel_device,
	&msm_ptm_device,
#endif
	&msm_device_dspcrashd_8960,
	&msm8960_device_watchdog,
#ifdef CONFIG_FB_MSM_WRITEBACK_MSM_PANEL
	&wfd_panel_device,
	&wfd_device,
#endif
#ifdef CONFIG_ANDROID_RAM_CONSOLE
	&ram_console_device,
#endif
#ifdef CONFIG_RAMDUMP_CRASH_LOGS
	&ramdumplog_device,
#endif
#ifdef CONFIG_SEMC_FELICA_SUPPORT
	&semc_felica_device,
#endif
};

static struct platform_device *msm8960_devices[] __initdata = {
#ifdef CONFIG_USB_NCP373
	&ncp373_device,
#endif
	&msm_8960_q6_lpass,
	&msm_8960_q6_mss_fw,
	&msm_8960_q6_mss_sw,
	&msm_8960_riva,
	&msm_pil_tzapps,
	&msm8960_device_otg,
	&msm8960_device_gadget_peripheral,
	&msm_device_hsusb_host,
	&msm_device_hsic_host,
	&android_usb_device,
	&msm_pcm,
	&msm_pcm_routing,
	&msm_cpudai0,
	&msm_cpudai1,
	&msm_cpudai_hdmi_rx,
	&msm_cpudai_bt_rx,
	&msm_cpudai_bt_tx,
	&msm_cpudai_fm_rx,
	&msm_cpudai_fm_tx,
	&msm_cpudai_auxpcm_rx,
	&msm_cpudai_auxpcm_tx,
	&msm_cpu_fe,
	&msm_stub_codec,
	&msm_kgsl_3d0,
#ifdef CONFIG_MSM_KGSL_2D
	&msm_kgsl_2d0,
	&msm_kgsl_2d1,
#endif
	&mipi_dsi_novatek_panel_device,
#ifdef CONFIG_FB_MSM_MIPI_DSI_TOSHIBA
	&mipi_dsi_toshiba_panel_device,
#endif
#ifdef CONFIG_MSM_GEMINI
	&msm8960_gemini_device,
#endif
	&msm_voice,
	&msm_voip,
	&msm_lpa_pcm,
	&msm_compr_dsp,
	&msm_cpudai_incall_music_rx,
	&msm_cpudai_incall_record_rx,
	&msm_cpudai_incall_record_tx,
	&msm_cpudai_afe_01_rx,
	&msm_cpudai_afe_01_tx,
	&msm_cpudai_afe_02_rx,
	&msm_cpudai_afe_02_tx,
	&msm_pcm_afe,
#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
	&hdmi_msm_device,
#endif
	&msm_pcm_hostless,
	&msm_bus_apps_fabric,
	&msm_bus_sys_fabric,
	&msm_bus_mm_fabric,
	&msm_bus_sys_fpb,
	&msm_bus_cpss_fpb,
};

static void __init msm8960_i2c_init(void)
{
#ifdef CONFIG_MSM_GSBI10_UART
	void *gsbi10_mem = ioremap_nocache(0x1A200000, 4);
#endif
#ifdef CONFIG_MSM_GSBI12_UART
	void *gsbi12_mem = ioremap_nocache(0x12480000, 4);
#endif

#ifdef CONFIG_MSM_GSBI10_UART
	/* Setting protocol code to 0x60 for dual UART/I2C in GSBI10 */
	writel_relaxed(0x6 << 4, gsbi10_mem);
	iounmap(gsbi10_mem);
#endif
#ifdef CONFIG_MSM_GSBI12_UART
	/* Setting protocol code to 0x60 for dual UART/I2C in GSBI12 */
	writel_relaxed(0x6 << 4, gsbi12_mem);
	iounmap(gsbi12_mem);
#endif

	msm8960_device_qup_i2c_gsbi4.dev.platform_data =
					&msm8960_i2c_qup_gsbi4_pdata;

	msm8960_device_qup_i2c_gsbi3.dev.platform_data =
					&msm8960_i2c_qup_gsbi3_pdata;

	msm8960_device_qup_i2c_gsbi10.dev.platform_data =
					&msm8960_i2c_qup_gsbi10_pdata;

	msm8960_device_qup_i2c_gsbi12.dev.platform_data =
					&msm8960_i2c_qup_gsbi12_pdata;
}

static void __init msm8960_gfx_init(void)
{
	uint32_t soc_platform_version = socinfo_get_version();
	if (SOCINFO_VERSION_MAJOR(soc_platform_version) == 1) {
		struct kgsl_device_platform_data *kgsl_3d0_pdata =
				msm_kgsl_3d0.dev.platform_data;
		kgsl_3d0_pdata->pwrlevel[0].gpu_freq = 320000000;
		kgsl_3d0_pdata->pwrlevel[1].gpu_freq = 266667000;
	}
}

static struct pm8xxx_irq_platform_data pm8xxx_irq_pdata __devinitdata = {
	.irq_base		= PM8921_IRQ_BASE,
	.devirq			= MSM_GPIO_TO_INT(104),
	.irq_trigger_flag	= IRQF_TRIGGER_LOW,
};

static struct pm8xxx_gpio_platform_data pm8xxx_gpio_pdata __devinitdata = {
	.gpio_base	= PM8921_GPIO_PM_TO_SYS(1),
};

static struct pm8xxx_mpp_platform_data pm8xxx_mpp_pdata __devinitdata = {
	.mpp_base	= PM8921_MPP_PM_TO_SYS(1),
};

static struct pm8xxx_rtc_platform_data pm8xxx_rtc_pdata __devinitdata = {
	.rtc_write_enable       = false,
	.rtc_alarm_powerup	= false,
};

static struct pm8xxx_pwrkey_platform_data pm8xxx_pwrkey_pdata = {
	.pull_up		= 1,
	.kpd_trigger_delay_us	= 15625,
	.wakeup			= 1,
};

static struct pm8xxx_misc_platform_data pm8xxx_misc_pdata = {
	.priority		= 0,
};

static struct pm8xxx_ccadc_platform_data pm8xxx_ccadc_pdata = {
	.r_sense		= 10,
};

static struct pm8921_platform_data pm8921_platform_data __devinitdata = {
	.irq_pdata		= &pm8xxx_irq_pdata,
	.gpio_pdata		= &pm8xxx_gpio_pdata,
	.mpp_pdata		= &pm8xxx_mpp_pdata,
	.rtc_pdata              = &pm8xxx_rtc_pdata,
	.pwrkey_pdata		= &pm8xxx_pwrkey_pdata,
	.misc_pdata		= &pm8xxx_misc_pdata,
	.regulator_pdatas	= msm_pm8921_regulator_pdata,
	.charger_pdata		= &pm8921_chg_pdata,
	.bms_pdata		= &pm8921_bms_pdata,
	.adc_pdata		= &pm8xxx_adc_pdata,
	.ccadc_pdata		= &pm8xxx_ccadc_pdata,
#ifdef CONFIG_PMIC8XXX_VIBRATOR
	.vibrator_pdata		= &pm8xxx_vibrator_pdata,
#endif
	.simple_remote_pdata	= &simple_remote_pf_data,
	.mic_bias_pdata		= &pm8921_mic_bias_pdata,
};

static struct msm_ssbi_platform_data msm8960_ssbi_pm8921_pdata __devinitdata = {
	.controller_type = MSM_SBI_CTRL_PMIC_ARBITER,
	.slave	= {
		.name			= "pm8921-core",
		.platform_data		= &pm8921_platform_data,
	},
};

static struct msm_cpuidle_state msm_cstates[] __initdata = {
	{0, 0, "C0", "WFI",
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT},

	{0, 1, "C1", "STANDALONE_POWER_COLLAPSE",
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE},

	{0, 2, "C2", "POWER_COLLAPSE",
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE},

	{1, 0, "C0", "WFI",
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT},

	{1, 1, "C1", "STANDALONE_POWER_COLLAPSE",
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE},
};

static struct msm_pm_platform_data msm_pm_data[MSM_PM_SLEEP_MODE_NR * 2] = {
	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_POWER_COLLAPSE)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 0,
		.suspend_enabled = 0,
	},

	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 0,
		.suspend_enabled = 0,
	},

	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 1,
		.suspend_enabled = 1,
	},

	[MSM_PM_MODE(1, MSM_PM_SLEEP_MODE_POWER_COLLAPSE)] = {
		.idle_supported = 0,
		.suspend_supported = 1,
		.idle_enabled = 0,
		.suspend_enabled = 0,
	},

	[MSM_PM_MODE(1, MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 0,
		.suspend_enabled = 0,
	},

	[MSM_PM_MODE(1, MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT)] = {
		.idle_supported = 1,
		.suspend_supported = 0,
		.idle_enabled = 1,
		.suspend_enabled = 0,
	},
};

static struct msm_rpmrs_level msm_rpmrs_levels[] = {
	{
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		100, 8000, 100000, 1,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		2000, 6000, 60100000, 3000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, GDHS, MAX, ACTIVE),
		false,
		4200, 5000, 60350000, 3500,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, HSFS_OPEN, MAX, ACTIVE),
		false,
		6300, 4500, 65350000, 4800,
	},
	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		7000, 3500, 66600000, 5150,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, GDHS, MAX, ACTIVE),
		false,
		11700, 2500, 67850000, 5500,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, MAX, ACTIVE),
		false,
		13800, 2000, 71850000, 6800,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		29700, 500, 75850000, 8800,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, RET_HIGH, RET_LOW),
		false,
		29700, 0, 76350000, 9800,
	},
};

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_TZ,
};

uint32_t msm_rpm_get_swfi_latency(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(msm_rpmrs_levels); i++) {
		if (msm_rpmrs_levels[i].sleep_mode ==
			MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT)
			return msm_rpmrs_levels[i].latency_us;
	}

	return 0;
}

#ifdef CONFIG_I2C
#define MSM_8960_GSBI12_QUP_I2C_BUS_ID 12
#define MSM_8960_GSBI10_QUP_I2C_BUS_ID 10
#define MSM_8960_GSBI4_QUP_I2C_BUS_ID 4
#define MSM_8960_GSBI3_QUP_I2C_BUS_ID 3

struct i2c_registry {
	u8                     machs;
	int                    bus;
	struct i2c_board_info *info;
	int                    len;
};

#ifdef CONFIG_MSM_CAMERA
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
static struct i2c_board_info msm8960_camera_i2c_boardinfo[] = {
#ifdef CONFIG_SEMC_CAM_MAIN_V4L2
	{
		/* Config-spec is 8-bit = 0x34, src-code need 7-bit => 0x1A */
		I2C_BOARD_INFO("semc_sensor_main", 0x34 >> 1),
		.platform_data = &msm_camera_semc_sensor_main_sensor_info,
	},
#endif
#ifdef CONFIG_SEMC_CAM_SUB_V4L2
	{
		/* Config-spec is 8-bit = 0x90, src-code need 7-bit => 0x48 */
		I2C_BOARD_INFO("semc_sensor_sub", 0x90 >> 1),
		.platform_data = &msm_camera_semc_sensor_sub_sensor_info,
	},
#endif
};

struct msm_camera_board_info msm8960_camera_board_info = {
	.board_info = msm8960_camera_i2c_boardinfo,
	.num_i2c_board_info = ARRAY_SIZE(msm8960_camera_i2c_boardinfo),
};
#else
static struct i2c_board_info msm_camera_boardinfo[] __initdata = {
#ifdef CONFIG_IMX074
	{
	/* Config-spec is 8-bit = 0x34, src-code need 7-bit => 0x1A */
	I2C_BOARD_INFO("imx074", 0x34 >> 1),
	},
#endif
#ifdef CONFIG_OV2720
	{
	/* Config-spec is 8-bit = 0xD8, src-code need 7-bit => 0x6C */
	I2C_BOARD_INFO("ov2720", 0xD8 >> 1),
	},
#endif
	{
	/* Config-spec is 8-bit = 0x6C, src-code need 7-bit => 0x36 */
	I2C_BOARD_INFO("qs_mt9p017", 0x6C >> 1),
	},
#ifdef CONFIG_MSM_CAMERA_FLASH_SC628A
	{
	/* Config-spec is 8-bit = 0xDC, src-code need 7-bit => 0x6E */
	I2C_BOARD_INFO("sc628a", 0xDC >> 1),
	},
#endif
};
#endif
#endif

/* Sensors DSPS platform data */
#ifdef CONFIG_MSM_DSPS
#define DSPS_PIL_GENERIC_NAME		"dsps"
#endif /* CONFIG_MSM_DSPS */

static void __init msm8960_init_dsps(void)
{
#ifdef CONFIG_MSM_DSPS
	struct msm_dsps_platform_data *pdata =
		msm_dsps_device.dev.platform_data;
	pdata->pil_name = DSPS_PIL_GENERIC_NAME;
	pdata->gpios = NULL;
	pdata->gpios_num = 0;

	platform_device_register(&msm_dsps_device);
#endif /* CONFIG_MSM_DSPS */
}

static int hsic_peripheral_status = 1;
static DEFINE_MUTEX(hsic_status_lock);

void peripheral_connect()
{
	mutex_lock(&hsic_status_lock);
	if (hsic_peripheral_status)
		goto out;
	platform_device_add(&msm_device_hsic_host);
	hsic_peripheral_status = 1;
out:
	mutex_unlock(&hsic_status_lock);
}
EXPORT_SYMBOL(peripheral_connect);

void peripheral_disconnect()
{
	mutex_lock(&hsic_status_lock);
	if (!hsic_peripheral_status)
		goto out;
	platform_device_del(&msm_device_hsic_host);
	hsic_peripheral_status = 0;
out:
	mutex_unlock(&hsic_status_lock);
}
EXPORT_SYMBOL(peripheral_disconnect);


static struct i2c_registry msm8960_i2c_devices[] __initdata = {
#if !defined(CONFIG_SEMC_CAM_MAIN_V4L2) && !defined(CONFIG_SEMC_CAM_SUB_V4L2)
	{
		0,
		MSM_8960_GSBI4_QUP_I2C_BUS_ID,
		msm_camera_boardinfo,
		ARRAY_SIZE(msm_camera_boardinfo),
	},
#endif
	{
		0,
		MSM_8960_GSBI3_QUP_I2C_BUS_ID,
		gsbi3_peripherals_info,
		ARRAY_SIZE(gsbi3_peripherals_info),
	},
	{
		0,
		MSM_8960_GSBI10_QUP_I2C_BUS_ID,
		gsbi10_peripherals_info,
		ARRAY_SIZE(gsbi10_peripherals_info),
	},
	{
		0,
		MSM_8960_GSBI12_QUP_I2C_BUS_ID,
		gsbi12_peripherals_info,
		ARRAY_SIZE(gsbi12_peripherals_info),
	}
};
#endif /* CONFIG_I2C */

static void __init register_i2c_devices(void)
{
#ifdef CONFIG_I2C
	int i;
#ifdef CONFIG_MSM_CAMERA
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	struct i2c_registry msm8960_camera_i2c_devices = {
		0,
		MSM_8960_GSBI4_QUP_I2C_BUS_ID,
		msm8960_camera_board_info.board_info,
		msm8960_camera_board_info.num_i2c_board_info,
	};
#endif
#endif

	/* Run the array and install devices as appropriate */
	for (i = 0; i < ARRAY_SIZE(msm8960_i2c_devices); ++i) {
		i2c_register_board_info(msm8960_i2c_devices[i].bus,
				msm8960_i2c_devices[i].info,
				msm8960_i2c_devices[i].len);
	}
#ifdef CONFIG_MSM_CAMERA
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	i2c_register_board_info(msm8960_camera_i2c_devices.bus,
		msm8960_camera_i2c_devices.info,
		msm8960_camera_i2c_devices.len);
#endif
#endif
#endif
}

static void setup_early_vregs(void)
{
	struct regulator *vreg;
	int rc;

	vreg = regulator_get(NULL, "8921_l9");
	if (IS_ERR(vreg)) {
		pr_err("%s: failed to get 8921_l9! rc = %ld\n", __func__,
			PTR_ERR(vreg));
		return;
	}

	rc = regulator_set_voltage(vreg, 2850000, 2850000);
	if (rc) {
		pr_err("%s: failed to set voltage on 8921_l9!\n", __func__);
		return;
	}

	rc = regulator_enable(vreg);
	if (rc) {
		pr_err("%s: failed to enable 8921_l9!\n", __func__);
		return;
	}

	vreg = regulator_get(NULL, "8921_lvs4");
	if (IS_ERR(vreg)) {
		pr_err("%s: failed to get vreg 8921_lvs4!\n", __func__);
		return;
	}

	rc = regulator_enable(vreg);
	if (rc) {
		pr_err("%s: failed to enable 8921_lvs4!\n", __func__);
		return;
	}
}

static void __init msm8960_init(void)
{
	if (meminfo_init(SYS_MEMORY, SZ_256M) < 0)
		pr_err("meminfo_init() failed!\n");

	//msm_tsens_early_init(&msm_tsens_pdata);
	BUG_ON(msm_rpm_init(&msm_rpm_data));
	BUG_ON(msm_rpmrs_levels_init(msm_rpmrs_levels,
				ARRAY_SIZE(msm_rpmrs_levels)));

	pmic_reset_irq = PM8921_IRQ_BASE + PM8921_RESOUT_IRQ;
	regulator_suppress_info_printing();
	if (msm_xo_init())
		pr_err("Failed to initialize XO votes\n");
	platform_device_register(&msm8960_device_rpm_regulator);
	msm_clock_init(&msm8960_clock_init_data);
	msm8960_device_otg.dev.platform_data = &msm_otg_pdata;
	msm8960_device_gadget_peripheral.dev.parent = &msm8960_device_otg.dev;
	msm_device_hsusb_host.dev.parent = &msm8960_device_otg.dev;
	msm_device_hsic_host.dev.platform_data = &msm_hsic_pdata;
	gpiomux_init();
	spi_register_board_info(spi_board_info, ARRAY_SIZE(spi_board_info));
	pm8921_platform_data.keypad_pdata = get_keypad_data();
	msm8960_device_ssbi_pmic.dev.platform_data =
				&msm8960_ssbi_pm8921_pdata;
	pm8921_platform_data.num_regulators = msm_pm8921_regulator_pdata_len;
	msm8960_i2c_init();
	msm8960_gfx_init();
	msm_spm_init(msm_spm_data, ARRAY_SIZE(msm_spm_data));
	msm_spm_l2_init(msm_spm_l2_data);
	msm8960_init_buses();
	platform_add_devices(msm_footswitch_devices,
		msm_num_footswitch_devices);
#ifdef CONFIG_MSM_GSBI10_UART
	msm_device_uart_dm10.dev.platform_data = &uart_gsbi10_pdata;
#endif
#ifdef CONFIG_MSM_GSBI12_UART
	msm8960_device_uart_gsbi12.dev.platform_data = &msm_uart_gsbi12_pdata;
#endif
	platform_add_devices(common_devices, ARRAY_SIZE(common_devices));
	pm8921_gpio_mpp_init();
	platform_add_devices(msm8960_devices, ARRAY_SIZE(msm8960_devices));
	msm8960_init_cam();
	msm8960_init_mmc();
	acpuclk_init(&acpuclk_8960_soc_data);
	setup_early_vregs();
	register_i2c_devices();
#if defined(CONFIG_FB_MSM_MIPI_DSI_RENESAS_R63306) || \
		defined(CONFIG_FB_MSM_MIPI_DSI_SAMSUNG_S6D6AA0) ||\
		defined(CONFIG_FB_MSM_MIPI_DSI_NOVATEK_NT35565)
	semc_blue_add_lcd_device();
#endif
	msm_fb_add_devices();
	slim_register_board_info(msm_slim_devices,
		ARRAY_SIZE(msm_slim_devices));
	msm8960_init_dsps();
	msm_pm_set_platform_data(msm_pm_data, ARRAY_SIZE(msm_pm_data));
	msm_pm_set_rpm_wakeup_irq(RPM_APCC_CPU0_WAKE_UP_IRQ);
	msm_cpuidle_set_states(msm_cstates, ARRAY_SIZE(msm_cstates),
				msm_pm_data);
	change_memory_power = &msm8960_change_memory_power;

	cyttsp5_register_core_device(&cyttsp5_core_device);
	cyttsp5_register_device(&cyttsp5_mt_device);
	cyttsp5_register_device(&cyttsp5_btn_device);

	BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));
}

MACHINE_START(SEMC_BLUE_CDB, "SEMC BLUE CDB")
	.map_io = msm8960_map_io,
	.reserve = msm8960_reserve,
	.init_irq = msm8960_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = msm8960_init,
	.init_early = msm8960_allocate_memory_regions,
	.init_very_early = msm8960_early_memory,
MACHINE_END
