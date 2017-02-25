/* arch/arm/mach-msm/board-semc_blue_cdb.c
 *
 * Copyright (C) 2012 Sony Ericsson Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include <asm/mach-types.h>
#include <linux/clearpad.h>
#include <linux/gpio.h>
#include <linux/gpio_event.h>
#include <linux/gpio_keys.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/leds-as3676.h>
#include <linux/mpu.h>
#include <linux/platform_device.h>
#include <linux/input/pmic8xxx-keypad.h>
#include <linux/mfd/core.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <linux/regulator/gpio-regulator.h>
#include <mach/board.h>
#include <mach/gpiomux.h>
#include <mach/irqs.h>
#include <mach/rpm-regulator.h>

#include "board-8960.h"
#include "charger-semc_blue.h"
#include "gyro-semc_blue.h"
#include "pm8921-gpio-mpp-blue.h"
#include "board-semc_blue-vibrator.h"

/* Section: Vibrator */
struct pm8xxx_vibrator_platform_data pm8xxx_vibrator_pdata = {
	.initial_vibrate_ms = 0,
	.max_timeout_ms = 15000,
	.level_mV = 3100,
};

/* Section: Touch */
struct synaptics_funcarea clearpad_funcarea_array[] = {
	{
		{ 0, 0, 719, 1279 }, { 0, 0, 719, 1299 },
		SYN_FUNCAREA_POINTER, NULL
	},
	{ .func = SYN_FUNCAREA_END }
};

struct synaptics_funcarea *clearpad_funcarea_get(u8 module_id, u8 rev)
{
	return clearpad_funcarea_array;
}

/* Section: Charging */
static int pm8921_therm_mitigation[] = {
	1100,
	700,
	600,
	325,
};

#define MAX_VOLTAGE_MV		4200

struct pm8921_charger_platform_data pm8921_chg_pdata __devinitdata = {
	.safety_time		= 512,
	.ttrkl_time		= 64,
	.update_time		= 60000,
	.max_voltage		= MAX_VOLTAGE_MV,
	.min_voltage		= 3200,
	.resume_voltage_delta	= 100,
	.resume_soc		= 99,
	.term_current		= 70,
	.cold_temp		= 5,
	.cool_temp		= 10,
	.warm_temp		= 45,
	.hot_temp		= 55,
	.hysterisis_temp	= 3,
	.temp_check_period	= 1,
	.safe_current		= 1475,
	.max_bat_chg_current	= 1425,
	.cool_bat_chg_current	= 1425,
	.warm_bat_chg_current	= 375,
	.cool_bat_voltage	= 4200,
	.warm_bat_voltage	= 4000,
	.thermal_mitigation	= pm8921_therm_mitigation,
	.thermal_levels		= ARRAY_SIZE(pm8921_therm_mitigation),
};

struct pm8921_bms_platform_data pm8921_bms_pdata __devinitdata = {
	.battery_data		= &pm8921_battery_data,
	.r_sense		= 10,
	.i_test			= 500,
	.v_failure		= 3200,
	.calib_delay_ms		= 600000,
	.max_voltage_uv         = MAX_VOLTAGE_MV * 1000,
};

/* Section: Gyro */
#ifdef CONFIG_SENSORS_MPU3050

#define GYRO_ORIENTATION {  1,  0,  0,  0,  1,  0,  0,  0,  1 }
#define ACCEL_ORIENTATION { 0, -1,  0,  1,  0,  0,  0,  0,  1 }
#define COMPASS_ORIENTATION {  0, -1,  0,  1,  0,  0,  0,  0, -1 }
#define PRESSURE_ORIENTATION {  1,  0,  0,  0,  1,  0,  0,  0,  1 }

struct mpu3050_platform_data mpu_data = {
	.int_config  = BIT_INT_ANYRD_2CLEAR,
	.orientation = GYRO_ORIENTATION,
	.accel = {
		 .get_slave_descr = get_accel_slave_descr,
		 .adapt_num   = 12,
		 .bus         = EXT_SLAVE_BUS_SECONDARY,
		 .address     = (0x30 >> 1),
		 .orientation = ACCEL_ORIENTATION,
		 .bypass_state = mpu3050_bypassmode,
		 .check_sleep_status = check_bma250_sleep_state,
		 .vote_sleep_status = vote_bma250_sleep_state,
	 },
	.compass = {
		 .get_slave_descr = get_compass_slave_descr,
		 .adapt_num   = 12,
		 .bus         = EXT_SLAVE_BUS_PRIMARY,
		 .address     = (0x18 >> 1),
		 .orientation = COMPASS_ORIENTATION,
	 },
	.pressure = {
		 .get_slave_descr = NULL,
		 .adapt_num   = 0,
		 .bus         = EXT_SLAVE_BUS_INVALID,
		 .address     = 0,
		 .orientation = PRESSURE_ORIENTATION,
	 },
	.setup   = mpu3050_gpio_setup,
	.hw_config  = mpu3050_hw_config,
};
#endif /* CONFIG_SENSORS_MPU3050 */

#ifdef CONFIG_SENSORS_MPU6050

#define MPU_ORIENTATION { 1, 0, 0, 0, 1, 0, 0, 0, 1 }
#define COMPASS_ORIENTATION { 0, 1, 0, -1, 0, 0, 0, 0, 1 }

struct mpu_platform_data mpu6050_data = {
	.int_config  = 0x10,
	.level_shifter = 0,
	.orientation = MPU_ORIENTATION,
	.setup	 = mpu6050_gpio_setup,
	.hw_config  = mpu6050_hw_config,
};

struct ext_slave_platform_data mpu_compass_data = {
	.address     = (0x18 >> 1),
	.adapt_num   = 12,
	.bus         = EXT_SLAVE_BUS_SECONDARY,
	.orientation = COMPASS_ORIENTATION,
};
#endif /* CONFIG_SENSORS_MPU6050 */

/* Section: Keypad */
#ifdef CONFIG_GPIO_SLIDER
static const struct gpio_event_direct_entry slider_switch_gpio_map[] = {
	{92,
	SW_LID},
};

static struct gpio_event_input_info slider_switch_gpio_info = {
	.info.func = gpio_event_input_func,
	.flags = 0, /* GPIO event active low*/
	.type = EV_SW,
	.keymap = slider_switch_gpio_map,
	.keymap_size = ARRAY_SIZE(slider_switch_gpio_map),
};

static struct gpio_event_info *slider_info[] = {
	&slider_switch_gpio_info.info,
};

static struct gpio_event_platform_data slider_data = {
	.name		= "slider-gpio",
	.info		= slider_info,
	.info_count	= ARRAY_SIZE(slider_info),
};

struct platform_device slider_device = {
	.name	= GPIO_EVENT_DEV_NAME,
	.id	= 0,
	.dev	= {
		.platform_data	= &slider_data,
	},
};
#endif

#define SINGLE_IRQ_RESOURCE(_name, _irq) \
{ \
	.name	= _name, \
	.start	= _irq, \
	.end	= _irq, \
	.flags	= IORESOURCE_IRQ, \
}

static const unsigned int keymap[] = {
	KEY(0, 0, KEY_BACK),
	KEY(0, 1, KEY_HOME),
	KEY(0, 2, KEY_MENU),
	KEY(1, 0, KEY_CAMERA_FOCUS),
	KEY(1, 1, KEY_CAMERA_SNAPSHOT),
};

static struct matrix_keymap_data keymap_data = {
	.keymap_size    = ARRAY_SIZE(keymap),
	.keymap         = keymap,
};

static struct pm8xxx_keypad_platform_data keypad_data = {
	.input_name             = "keypad_8960",
	.input_phys_device      = "keypad_8960/input0",
	.num_rows               = 2,
	.num_cols               = 5,
	.rows_gpio_start	= PM8921_GPIO_PM_TO_SYS(9),
	.cols_gpio_start	= PM8921_GPIO_PM_TO_SYS(1),
	.debounce_ms            = 15,
	.scan_delay_ms          = 32,
	.row_hold_ns            = 91500,
	.wakeup                 = 1,
	.keymap_data            = &keymap_data,
};

struct pm8xxx_keypad_platform_data *get_keypad_data(void)
{
	return &keypad_data;
}
static struct gpio_keys_button gpio_keys_buttons[] = {
	{
		.code = KEY_VOLUMEDOWN,
		.gpio = PM8921_GPIO_PM_TO_SYS(20),
		.active_low = 1,
		.desc = "volume down",
		.type = EV_KEY,
		.wakeup = 0,
		.debounce_interval = 10,
	},
	{
		.code = KEY_VOLUMEUP,
		.gpio = PM8921_GPIO_PM_TO_SYS(21),
		.active_low = 1,
		.desc = "volume up",
		.type = EV_KEY,
		.wakeup = 0,
		.debounce_interval = 10,
	},
};

static struct gpio_keys_platform_data gpio_keys_pdata = {
	.buttons = gpio_keys_buttons,
	.nbuttons = 2,
};

static struct platform_device gpio_keys_device = {
	.name = "gpio-keys",
	.id = 0,
	.dev = { .platform_data = &gpio_keys_pdata },
	.num_resources = 0,
	.resource = NULL,
};

#define GPIO_SW_SIM_DETECTION		36

static struct gpio_event_direct_entry gpio_sw_gpio_map[] = {
	{PM8921_GPIO_PM_TO_SYS(GPIO_SW_SIM_DETECTION), SW_JACK_PHYSICAL_INSERT},
};

static struct gpio_event_input_info gpio_sw_gpio_info = {
	.info.func = gpio_event_input_func,
	.info.no_suspend = 1,
	.flags = 0,
	.type = EV_SW,
	.keymap = gpio_sw_gpio_map,
	.keymap_size = ARRAY_SIZE(gpio_sw_gpio_map),
};

static struct gpio_event_info *pmic_keypad_info[] = {
	&gpio_sw_gpio_info.info,
};

struct gpio_event_platform_data pmic_keypad_data = {
	.name       = "sim-detection",
	.info       = pmic_keypad_info,
	.info_count = ARRAY_SIZE(pmic_keypad_info),
};

static struct platform_device pmic_keypad_device = {
	.name	= GPIO_EVENT_DEV_NAME,
	.id	= 1,
	.dev	= {.platform_data = &pmic_keypad_data},
};

static int __init input_devices_init(void)
{
	platform_device_register(&gpio_keys_device);
#ifdef CONFIG_GPIO_SLIDER
	platform_device_register(&slider_device);
#endif
	platform_device_register(&pmic_keypad_device);
	return 0;
}

static void __exit input_devices_exit(void)
{
#ifdef CONFIG_GPIO_SLIDER
	platform_device_unregister(&slider_device);
#endif
	platform_device_unregister(&pmic_keypad_device);
}

module_init(input_devices_init);
module_exit(input_devices_exit);

/* Section: Leds */
#if defined(CONFIG_LEDS_AS3676)

struct as3676_platform_data as3676_platform_data = {
	.step_up_vtuning = 18,	/* 0 .. 31 uA on DCDC_FB */
	.audio_speed_down = 1,	/* 0..3 resp. 0, 200, 400, 800ms */
	.audio_speed_up = 4,	/* 0..7 resp. 0, 50, 100, 150,
					200,250,400, 800ms */
	.audio_agc_ctrl = 1,	/* 0 .. 7: 0==no AGC, 7 very aggressive*/
	.audio_gain = 7,	/* 0..7: -12, -6,  0, 6
					12, 18, 24, 30 dB */
	.audio_source = 2,	/* 0..3: 0=curr33, 1=DCDC_FB
					2=GPIO1,  3=GPIO2 */
	.leds[0] = {
		.name = "led_1-lcd",
		.on_charge_pump = 0,
		.max_current_uA = 20000,
		.startup_current_uA = 20000,
	},
	.leds[1] = {
		.name = "led_2-lcd",
		.on_charge_pump = 0,
		.max_current_uA = 20000,
		.startup_current_uA = 20000,
	},
	.leds[2] = {
		.name = "led_3-not-connected",
		.on_charge_pump = 0,
		.max_current_uA = 0,
	},
	.leds[3] = {
		.name = "led_4-key_bl",
		.on_charge_pump = 1,
		.max_current_uA = 38250,
	},
	.leds[4] = {
		.name = "led_5-not-connected",
		.on_charge_pump = 1,
		.max_current_uA = 0,
	},
	.leds[5] = {
		.name = "led_6-not-connected",
		.on_charge_pump = 1,
		.max_current_uA = 0,
	},
	.leds[6] = {
		.name = "led_7-rgb1-red",
		.on_charge_pump = 1,
		.max_current_uA = 5100,
	},
	.leds[7] = {
		.name = "led_8-rgb2-green",
		.on_charge_pump = 1,
		.max_current_uA = 5100,
	},
	.leds[8] = {
		.name = "led_9-rgb3-blue",
		.on_charge_pump = 1,
		.max_current_uA = 5100,
	},
	.leds[9] = {
		.name = "led_10-not-connected",
		.on_charge_pump = 1,
		.max_current_uA = 0,
	},
	.leds[10] = {
		.name = "led_11-not-connected",
		.on_charge_pump = 1,
		.max_current_uA = 0,
	},
	.leds[11] = {
		.name = "led_12-not-connected",
		.on_charge_pump = 1,
		.max_current_uA = 0,
	},
	.leds[12] = {
		.name = "led_13-not-connected",
		.on_charge_pump = 1,
		.max_current_uA = 0,
	},
};

#endif

/* Section: PMIC GPIO */
static struct pm8xxx_gpio_init pm8921_gpios[] __initdata = {

	/* KYPD_SNS(GPIO_1-5) and KYPD_DRV(GPIO_9,10) is set by PM8XXX keypad
	   driver. */

	/* SPKR_LEFT_EN    (GPIO_18),
	   SPKR_RIGHT_EN   (GPIO_19) and
	   WCD9310_RESET_N (GPIO_34) is set by Audio driver */

	/* UIM1_RST_CONN (GPIO_27),
	   UIM1_CLK_MSM  (GPIO_29) and
	   UIM1_CLK_CONN (GPIO_30) is set by UIM driver in AMSS */

	/* CLK_MSM_FWD (GPIO_39) is set by QCT code */

	/* NC */
	PM8XXX_GPIO_DISABLE(6),
	PM8XXX_GPIO_DISABLE(7),
	PM8XXX_GPIO_DISABLE(8),
	PM8XXX_GPIO_DISABLE(11),
	PM8XXX_GPIO_DISABLE(12),
	PM8XXX_GPIO_DISABLE(13),
	PM8XXX_GPIO_DISABLE(14),
	PM8XXX_GPIO_DISABLE(15),
	PM8XXX_GPIO_DISABLE(16),
	PM8XXX_GPIO_DISABLE(23),
	PM8XXX_GPIO_DISABLE(24),
	PM8XXX_GPIO_DISABLE(25),
	PM8XXX_GPIO_DISABLE(28),
	PM8XXX_GPIO_DISABLE(35),
	PM8XXX_GPIO_DISABLE(37),
	PM8XXX_GPIO_DISABLE(41),
	PM8XXX_GPIO_DISABLE(44),

	PM8XXX_GPIO_OUTPUT(17,	0),			/* IRDA_PWRDWN */
	PM8XXX_GPIO_INPUT(20,	PM_GPIO_PULL_UP_30),	/* VOLUME_DOWN_KEY */
	PM8XXX_GPIO_INPUT(21,	PM_GPIO_PULL_UP_30),	/* VOLUME_UP_KEY */
	PM8XXX_GPIO_OUTPUT(22,	0),			/* RF_ID_EN */
	PM8XXX_GPIO_INPUT(26,	PM_GPIO_PULL_NO),	/* SD_CARD_DET_N */
#if defined(CONFIG_SEMC_FELICA_SUPPORT) && !defined(CONFIG_NFC_PN544)
	/* FELICA_LOCK */
	PM8XXX_GPIO_INIT(31, PM_GPIO_DIR_IN, PM_GPIO_OUT_BUF_CMOS, 0, \
				PM_GPIO_PULL_NO, PM_GPIO_VIN_L17, \
				PM_GPIO_STRENGTH_NO, \
				PM_GPIO_FUNC_NORMAL, 0, 0),
	/* FELICA_FF */
	PM8XXX_GPIO_INIT(32, PM_GPIO_DIR_OUT, PM_GPIO_OUT_BUF_CMOS, 0, \
				PM_GPIO_PULL_NO, PM_GPIO_VIN_L17, \
				PM_GPIO_STRENGTH_LOW, \
				PM_GPIO_FUNC_NORMAL, 0, 0),
	/* FELICA_PON */
	PM8XXX_GPIO_INIT(33, PM_GPIO_DIR_OUT, PM_GPIO_OUT_BUF_CMOS, 0, \
				PM_GPIO_PULL_NO, PM_GPIO_VIN_S4, \
				PM_GPIO_STRENGTH_LOW, \
				PM_GPIO_FUNC_NORMAL, 0, 0),
#elif !defined(CONFIG_SEMC_FELICA_SUPPORT) && defined(CONFIG_NFC_PN544)
	PM8XXX_GPIO_DISABLE(31),
	PM8XXX_GPIO_DISABLE(32),
	/* NFC_EN */
	PM8XXX_GPIO_INIT(33, PM_GPIO_DIR_OUT, PM_GPIO_OUT_BUF_OPEN_DRAIN, 0, \
				PM_GPIO_PULL_NO, PM_GPIO_VIN_VPH, \
				PM_GPIO_STRENGTH_LOW, \
				PM_GPIO_FUNC_NORMAL, 0, 0),
#else
	PM8XXX_GPIO_DISABLE(31),
	PM8XXX_GPIO_DISABLE(32),
	PM8XXX_GPIO_DISABLE(33),
#endif
	PM8XXX_GPIO_INPUT(36,	PM_GPIO_PULL_NO),	/* SIM_DET_N */
	PM8XXX_GPIO_INPUT(38,	PM_GPIO_PULL_NO),	/* PLUG_DET */
	/* EXT_REG_EN1 */
	PM8XXX_GPIO_INIT(40, PM_GPIO_DIR_OUT, PM_GPIO_OUT_BUF_CMOS, 1, \
				PM_GPIO_PULL_NO, PM_GPIO_VIN_S4, \
				PM_GPIO_STRENGTH_LOW, \
				PM_GPIO_FUNC_NORMAL, 0, 0),
	/* OTG_OVP_CNTL */
	PM8XXX_GPIO_INIT(42, PM_GPIO_DIR_OUT, PM_GPIO_OUT_BUF_CMOS, 1, \
				PM_GPIO_PULL_NO, PM_GPIO_VIN_VPH, \
				PM_GPIO_STRENGTH_LOW, \
				PM_GPIO_FUNC_NORMAL, 0, 0),

	/* MLCD_RST_N (GPIO_43) is set by LCD driver */
};

/* Initial PM8921 MPP configurations */
static struct pm8xxx_mpp_init pm8921_mpps[] __initdata = {
	/* External 5V regulator enable; shared by HDMI and USB_OTG switches. */
	PM8XXX_MPP_INIT(7, D_INPUT, PM8921_MPP_DIG_LEVEL_VPH, DIN_TO_INT),
	PM8XXX_MPP_INIT(PM8XXX_AMUX_MPP_8, A_INPUT, PM8XXX_MPP_AIN_AMUX_CH8,
								DOUT_CTRL_LOW),
};

void __init pm8921_gpio_mpp_init(void)
{
	int i, rc;

	for (i = 0; i < ARRAY_SIZE(pm8921_gpios); i++) {
		rc = pm8xxx_gpio_config(pm8921_gpios[i].gpio,
					&pm8921_gpios[i].config);
		if (rc) {
			pr_err("%s: pm8xxx_gpio_config: rc=%d\n", __func__, rc);
			break;
		}
	}

	for (i = 0; i < ARRAY_SIZE(pm8921_mpps); i++) {
		rc = pm8xxx_mpp_config(pm8921_mpps[i].mpp,
					&pm8921_mpps[i].config);
		if (rc) {
			pr_err("%s: pm8xxx_mpp_config: rc=%d\n", __func__, rc);
			break;
		}
	}
}

/* Section: Regulators */
#define VREG_CONSUMERS(_id) \
	static struct regulator_consumer_supply vreg_consumers_##_id[]

/*
 * Consumer specific regulator names:
 *			 regulator name		consumer dev_name
 */
VREG_CONSUMERS(L1) = {
	REGULATOR_SUPPLY("8921_l1",		NULL),
};
VREG_CONSUMERS(L2) = {
	REGULATOR_SUPPLY("8921_l2",		NULL),
	REGULATOR_SUPPLY("dsi_vdda",		"mipi_dsi.1"),
	REGULATOR_SUPPLY("mipi_csi_vdd",	"msm_camera_imx074.0"),
	REGULATOR_SUPPLY("mipi_csi_vdd",	"msm_camera_ov2720.0"),
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	REGULATOR_SUPPLY("mipi_csi_vdd", "msm_camera_semc_sensor_main.0"),
	REGULATOR_SUPPLY("mipi_csi_vdd", "msm_camera_semc_sensor_sub.0"),
#endif
};
VREG_CONSUMERS(L3) = {
	REGULATOR_SUPPLY("8921_l3",		NULL),
	REGULATOR_SUPPLY("HSUSB_3p3",		"msm_otg"),
};
VREG_CONSUMERS(L4) = {
	REGULATOR_SUPPLY("8921_l4",		NULL),
	REGULATOR_SUPPLY("HSUSB_1p8",		"msm_otg"),
	REGULATOR_SUPPLY("iris_vddxo",		"wcnss_wlan.0"),
};
VREG_CONSUMERS(L5) = {
	REGULATOR_SUPPLY("8921_l5",		NULL),
	REGULATOR_SUPPLY("sdc_vdd",		"msm_sdcc.1"),
};
VREG_CONSUMERS(L6) = {
	REGULATOR_SUPPLY("8921_l6",		NULL),
	REGULATOR_SUPPLY("sdc_vdd",		"msm_sdcc.3"),
};
VREG_CONSUMERS(L7) = {
	REGULATOR_SUPPLY("8921_l7",		NULL),
	REGULATOR_SUPPLY("sdc_vddp",		"msm_sdcc.3"),
};
VREG_CONSUMERS(L8) = {
	REGULATOR_SUPPLY("8921_l8",		NULL),
	REGULATOR_SUPPLY("dsi_vci",		"mipi_dsi.1"),
};
VREG_CONSUMERS(L9) = {
	REGULATOR_SUPPLY("8921_l9",		NULL),
};
VREG_CONSUMERS(L10) = {
	REGULATOR_SUPPLY("8921_l10",		NULL),
	REGULATOR_SUPPLY("iris_vddpa",		"wcnss_wlan.0"),

};
VREG_CONSUMERS(L11) = {
	REGULATOR_SUPPLY("8921_l11",		NULL),
	REGULATOR_SUPPLY("cam_vana",		"msm_camera_imx074.0"),
	REGULATOR_SUPPLY("cam_vana",		"msm_camera_ov2720.0"),
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	REGULATOR_SUPPLY("cam_vana", "msm_camera_semc_sensor_main.0"),
	REGULATOR_SUPPLY("cam_vana", "msm_camera_semc_sensor_sub.0"),
#endif
};
VREG_CONSUMERS(L12) = {
	REGULATOR_SUPPLY("8921_l12",		NULL),
	REGULATOR_SUPPLY("cam_vdig",		"msm_camera_imx074.0"),
	REGULATOR_SUPPLY("cam_vdig",		"msm_camera_ov2720.0"),
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	REGULATOR_SUPPLY("cam_vdig", "msm_camera_semc_sensor_main.0"),
#endif
};
VREG_CONSUMERS(L14) = {
	REGULATOR_SUPPLY("8921_l14",		NULL),
	REGULATOR_SUPPLY("pa_therm",		"pm8xxx-adc"),
};
VREG_CONSUMERS(L15) = {
	REGULATOR_SUPPLY("8921_l15",		NULL),
};
VREG_CONSUMERS(L16) = {
	REGULATOR_SUPPLY("8921_l16",		NULL),
	REGULATOR_SUPPLY("cam_vaf",		"msm_camera_imx074.0"),
	REGULATOR_SUPPLY("cam_vaf",		"msm_camera_ov2720.0"),
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	REGULATOR_SUPPLY("cam_vaf", "msm_camera_semc_sensor_main.0"),
#endif
};
VREG_CONSUMERS(L17) = {
	REGULATOR_SUPPLY("8921_l17",		NULL),
};
VREG_CONSUMERS(L18) = {
	REGULATOR_SUPPLY("8921_l18",		NULL),
};
VREG_CONSUMERS(L21) = {
	REGULATOR_SUPPLY("8921_l21",		NULL),
};
VREG_CONSUMERS(L22) = {
	REGULATOR_SUPPLY("8921_l22",		NULL),
};
VREG_CONSUMERS(L23) = {
	REGULATOR_SUPPLY("8921_l23",		NULL),
	REGULATOR_SUPPLY("hdmi_avdd",		"hdmi_msm.0"),
};
VREG_CONSUMERS(L24) = {
	REGULATOR_SUPPLY("8921_l24",		NULL),
	REGULATOR_SUPPLY("riva_vddmx",		"wcnss_wlan.0"),
};
VREG_CONSUMERS(L25) = {
	REGULATOR_SUPPLY("8921_l25",		NULL),
	REGULATOR_SUPPLY("VDDD_CDC_D",		"tabla-slim"),
	REGULATOR_SUPPLY("CDC_VDDA_A_1P2V",	"tabla-slim"),
	REGULATOR_SUPPLY("VDDD_CDC_D",		"tabla2x-slim"),
	REGULATOR_SUPPLY("CDC_VDDA_A_1P2V",	"tabla2x-slim"),
};
VREG_CONSUMERS(L26) = {
	REGULATOR_SUPPLY("8921_l26",		NULL),
	REGULATOR_SUPPLY("core_vdd",		"pil_qdsp6v4.0"),
};
VREG_CONSUMERS(L27) = {
	REGULATOR_SUPPLY("8921_l27",		NULL),
	REGULATOR_SUPPLY("core_vdd",		"pil_qdsp6v4.2"),
};
VREG_CONSUMERS(L28) = {
	REGULATOR_SUPPLY("8921_l28",		NULL),
	REGULATOR_SUPPLY("core_vdd",		"pil_qdsp6v4.1"),
};
VREG_CONSUMERS(L29) = {
	REGULATOR_SUPPLY("8921_l29",		NULL),
	REGULATOR_SUPPLY("dsi_vddio",		"mipi_dsi.1"),
};
VREG_CONSUMERS(S1) = {
	REGULATOR_SUPPLY("8921_s1",		NULL),
};
VREG_CONSUMERS(S2) = {
	REGULATOR_SUPPLY("8921_s2",		NULL),
	REGULATOR_SUPPLY("iris_vddrfa",		"wcnss_wlan.0"),

};
VREG_CONSUMERS(S3) = {
	REGULATOR_SUPPLY("8921_s3",		NULL),
	REGULATOR_SUPPLY("HSUSB_VDDCX",		"msm_otg"),
	REGULATOR_SUPPLY("riva_vddcx",		"wcnss_wlan.0"),
};
VREG_CONSUMERS(S4) = {
	REGULATOR_SUPPLY("8921_s4",		NULL),
	REGULATOR_SUPPLY("sdc_vccq",		"msm_sdcc.1"),
	REGULATOR_SUPPLY("riva_vddpx",		"wcnss_wlan.0"),
	REGULATOR_SUPPLY("hdmi_vcc",		"hdmi_msm.0"),
	REGULATOR_SUPPLY("VDDIO_CDC",		"tabla-slim"),
	REGULATOR_SUPPLY("CDC_VDD_CP",		"tabla-slim"),
	REGULATOR_SUPPLY("CDC_VDDA_TX",		"tabla-slim"),
	REGULATOR_SUPPLY("CDC_VDDA_RX",		"tabla-slim"),
	REGULATOR_SUPPLY("VDDIO_CDC",		"tabla2x-slim"),
	REGULATOR_SUPPLY("CDC_VDD_CP",		"tabla2x-slim"),
	REGULATOR_SUPPLY("CDC_VDDA_TX",		"tabla2x-slim"),
	REGULATOR_SUPPLY("CDC_VDDA_RX",		"tabla2x-slim"),
	REGULATOR_SUPPLY("EXT_HUB_VDDIO",	"msm_hsic_host"),
};
VREG_CONSUMERS(S5) = {
	REGULATOR_SUPPLY("8921_s5",		NULL),
	REGULATOR_SUPPLY("krait0",		NULL),
};
VREG_CONSUMERS(S6) = {
	REGULATOR_SUPPLY("8921_s6",		NULL),
	REGULATOR_SUPPLY("krait1",		NULL),
};
VREG_CONSUMERS(S7) = {
	REGULATOR_SUPPLY("8921_s7",		NULL),
};
VREG_CONSUMERS(S8) = {
	REGULATOR_SUPPLY("8921_s8",		NULL),
};
VREG_CONSUMERS(LVS1) = {
	REGULATOR_SUPPLY("8921_lvs1",		NULL),
	REGULATOR_SUPPLY("sdc_vdd",		"msm_sdcc.4"),
	REGULATOR_SUPPLY("iris_vddio",		"wcnss_wlan.0"),
};
VREG_CONSUMERS(LVS2) = {
	REGULATOR_SUPPLY("8921_lvs2",		NULL),
	REGULATOR_SUPPLY("iris_vdddig",		"wcnss_wlan.0"),
};
VREG_CONSUMERS(LVS3) = {
	REGULATOR_SUPPLY("8921_lvs3",		NULL),
};
VREG_CONSUMERS(LVS4) = {
	REGULATOR_SUPPLY("8921_lvs4",		NULL),
};
VREG_CONSUMERS(LVS5) = {
	REGULATOR_SUPPLY("8921_lvs5",		NULL),
	REGULATOR_SUPPLY("cam_vio",		"msm_camera_imx074.0"),
	REGULATOR_SUPPLY("cam_vio",		"msm_camera_ov2720.0"),
#if defined(CONFIG_SEMC_CAM_MAIN_V4L2) || defined(CONFIG_SEMC_CAM_SUB_V4L2)
	REGULATOR_SUPPLY("cam_vio", "msm_camera_semc_sensor_main.0"),
	REGULATOR_SUPPLY("cam_vio", "msm_camera_semc_sensor_sub.0"),
#endif
};
VREG_CONSUMERS(LVS6) = {
	REGULATOR_SUPPLY("8921_lvs6",		NULL),
};
VREG_CONSUMERS(LVS7) = {
	REGULATOR_SUPPLY("8921_lvs7",		NULL),
};

#define PM8XXX_VREG_INIT(_id, _name, _min_uV, _max_uV, _modes, _ops, \
			 _apply_uV, _pull_down, _always_on, _supply_regulator, \
			 _system_uA, _enable_time, _reg_id) \
	{ \
		.init_data = { \
			.constraints = { \
				.valid_modes_mask	= _modes, \
				.valid_ops_mask		= _ops, \
				.min_uV			= _min_uV, \
				.max_uV			= _max_uV, \
				.input_uV		= _max_uV, \
				.apply_uV		= _apply_uV, \
				.always_on		= _always_on, \
				.name			= _name, \
			}, \
			.num_consumer_supplies	= \
					ARRAY_SIZE(vreg_consumers_##_id), \
			.consumer_supplies	= vreg_consumers_##_id, \
			.supply_regulator	= _supply_regulator, \
		}, \
		.id			= _reg_id, \
		.pull_down_enable	= _pull_down, \
		.system_uA		= _system_uA, \
		.enable_time		= _enable_time, \
	}

#define PM8XXX_LDO(_id, _name, _always_on, _pull_down, _min_uV, _max_uV, \
		_enable_time, _supply_regulator, _system_uA, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, _min_uV, _max_uV, REGULATOR_MODE_NORMAL \
		| REGULATOR_MODE_IDLE, REGULATOR_CHANGE_VOLTAGE | \
		REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_MODE | \
		REGULATOR_CHANGE_DRMS, 0, _pull_down, _always_on, \
		_supply_regulator, _system_uA, _enable_time, _reg_id)

#define PM8XXX_NLDO1200(_id, _name, _always_on, _pull_down, _min_uV, \
		_max_uV, _enable_time, _supply_regulator, _system_uA, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, _min_uV, _max_uV, REGULATOR_MODE_NORMAL \
		| REGULATOR_MODE_IDLE, REGULATOR_CHANGE_VOLTAGE | \
		REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_MODE | \
		REGULATOR_CHANGE_DRMS, 0, _pull_down, _always_on, \
		_supply_regulator, _system_uA, _enable_time, _reg_id)

#define PM8XXX_SMPS(_id, _name, _always_on, _pull_down, _min_uV, _max_uV, \
		_enable_time, _supply_regulator, _system_uA, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, _min_uV, _max_uV, REGULATOR_MODE_NORMAL \
		| REGULATOR_MODE_IDLE, REGULATOR_CHANGE_VOLTAGE | \
		REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_MODE | \
		REGULATOR_CHANGE_DRMS, 0, _pull_down, _always_on, \
		_supply_regulator, _system_uA, _enable_time, _reg_id)

#define PM8XXX_FTSMPS(_id, _name, _always_on, _pull_down, _min_uV, _max_uV, \
		_enable_time, _supply_regulator, _system_uA, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, _min_uV, _max_uV, REGULATOR_MODE_NORMAL, \
		REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS \
		| REGULATOR_CHANGE_MODE, 0, _pull_down, _always_on, \
		_supply_regulator, _system_uA, _enable_time, _reg_id)

#define PM8XXX_VS(_id, _name, _always_on, _pull_down, _enable_time, \
		_supply_regulator, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, 0, 0, 0, REGULATOR_CHANGE_STATUS, 0, \
		_pull_down, _always_on, _supply_regulator, 0, _enable_time, \
		_reg_id)

#define PM8XXX_VS300(_id, _name, _always_on, _pull_down, _enable_time, \
		_supply_regulator, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, 0, 0, 0, REGULATOR_CHANGE_STATUS, 0, \
		_pull_down, _always_on, _supply_regulator, 0, _enable_time, \
		_reg_id)

#define PM8XXX_NCP(_id, _name, _always_on, _min_uV, _max_uV, _enable_time, \
		_supply_regulator, _reg_id) \
	PM8XXX_VREG_INIT(_id, _name, _min_uV, _max_uV, 0, \
		REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS, 0, 0, \
		_always_on, _supply_regulator, 0, _enable_time, _reg_id)

/* Pin control initialization */
#define PM8XXX_PC(_id, _name, _always_on, _pin_fn, _pin_ctrl, \
		  _supply_regulator, _reg_id) \
	{ \
		.init_data = { \
			.constraints = { \
				.valid_ops_mask	= REGULATOR_CHANGE_STATUS, \
				.always_on	= _always_on, \
				.name		= _name, \
			}, \
			.num_consumer_supplies	= \
					ARRAY_SIZE(vreg_consumers_##_id##_PC), \
			.consumer_supplies	= vreg_consumers_##_id##_PC, \
			.supply_regulator  = _supply_regulator, \
		}, \
		.id		= _reg_id, \
		.pin_fn		= PM8XXX_VREG_PIN_FN_##_pin_fn, \
		.pin_ctrl	= _pin_ctrl, \
	}

#define GPIO_VREG(_id, _reg_name, _gpio_label, _gpio, _supply_regulator) \
	[GPIO_VREG_ID_##_id] = { \
		.init_data = { \
			.constraints = { \
				.valid_ops_mask	= REGULATOR_CHANGE_STATUS, \
			}, \
			.num_consumer_supplies	= \
					ARRAY_SIZE(vreg_consumers_##_id), \
			.consumer_supplies	= vreg_consumers_##_id, \
			.supply_regulator	= _supply_regulator, \
		}, \
		.regulator_name = _reg_name, \
		.gpio_label	= _gpio_label, \
		.gpio		= _gpio, \
	}

#define SAW_VREG_INIT(_id, _name, _min_uV, _max_uV) \
	{ \
		.constraints = { \
			.name		= _name, \
			.valid_ops_mask	= REGULATOR_CHANGE_VOLTAGE, \
			.min_uV		= _min_uV, \
			.max_uV		= _max_uV, \
		}, \
		.num_consumer_supplies	= ARRAY_SIZE(vreg_consumers_##_id), \
		.consumer_supplies	= vreg_consumers_##_id, \
	}

#define RPM_INIT(_id, _min_uV, _max_uV, _modes, _ops, _apply_uV, _default_uV, \
		 _peak_uA, _avg_uA, _pull_down, _pin_ctrl, _freq, _pin_fn, \
		 _force_mode, _power_mode, _state, _sleep_selectable, \
		 _always_on, _supply_regulator, _system_uA) \
	{ \
		.init_data = { \
			.constraints = { \
				.valid_modes_mask	= _modes, \
				.valid_ops_mask		= _ops, \
				.min_uV			= _min_uV, \
				.max_uV			= _max_uV, \
				.input_uV		= _min_uV, \
				.apply_uV		= _apply_uV, \
				.always_on		= _always_on, \
			}, \
			.num_consumer_supplies	= \
					ARRAY_SIZE(vreg_consumers_##_id), \
			.consumer_supplies	= vreg_consumers_##_id, \
			.supply_regulator	= _supply_regulator, \
		}, \
		.id			= RPM_VREG_ID_PM8921_##_id, \
		.default_uV		= _default_uV, \
		.peak_uA		= _peak_uA, \
		.avg_uA			= _avg_uA, \
		.pull_down_enable	= _pull_down, \
		.pin_ctrl		= _pin_ctrl, \
		.freq			= RPM_VREG_FREQ_##_freq, \
		.pin_fn			= _pin_fn, \
		.force_mode		= _force_mode, \
		.power_mode		= _power_mode, \
		.state			= _state, \
		.sleep_selectable	= _sleep_selectable, \
		.system_uA		= _system_uA, \
	}

#define RPM_LDO(_id, _always_on, _pd, _sleep_selectable, _min_uV, _max_uV, \
		_supply_regulator, _system_uA, _init_peak_uA) \
	RPM_INIT(_id, _min_uV, _max_uV, REGULATOR_MODE_NORMAL \
		 | REGULATOR_MODE_IDLE, REGULATOR_CHANGE_VOLTAGE \
		 | REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_MODE \
		 | REGULATOR_CHANGE_DRMS, 0, _max_uV, _init_peak_uA, 0, _pd, \
		 RPM_VREG_PIN_CTRL_NONE, NONE, RPM_VREG_PIN_FN_8960_NONE, \
		 RPM_VREG_FORCE_MODE_8960_NONE, RPM_VREG_POWER_MODE_8960_PWM, \
		 RPM_VREG_STATE_OFF, _sleep_selectable, _always_on, \
		 _supply_regulator, _system_uA)

#define RPM_SMPS(_id, _always_on, _pd, _sleep_selectable, _min_uV, _max_uV, \
		 _supply_regulator, _system_uA, _freq) \
	RPM_INIT(_id, _min_uV, _max_uV, REGULATOR_MODE_NORMAL \
		 | REGULATOR_MODE_IDLE, REGULATOR_CHANGE_VOLTAGE \
		 | REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_MODE \
		 | REGULATOR_CHANGE_DRMS, 0, _max_uV, _system_uA, 0, _pd, \
		 RPM_VREG_PIN_CTRL_NONE, _freq, RPM_VREG_PIN_FN_8960_NONE, \
		 RPM_VREG_FORCE_MODE_8960_NONE, RPM_VREG_POWER_MODE_8960_PWM, \
		 RPM_VREG_STATE_OFF, _sleep_selectable, _always_on, \
		 _supply_regulator, _system_uA)

#define RPM_VS(_id, _always_on, _pd, _sleep_selectable, _supply_regulator) \
	RPM_INIT(_id, 0, 0, 0, REGULATOR_CHANGE_STATUS, 0, 0, 1000, 1000, _pd, \
		 RPM_VREG_PIN_CTRL_NONE, NONE, RPM_VREG_PIN_FN_8960_NONE, \
		 RPM_VREG_FORCE_MODE_8960_NONE, RPM_VREG_POWER_MODE_8960_PWM, \
		 RPM_VREG_STATE_OFF, _sleep_selectable, _always_on, \
		 _supply_regulator, 0)

#define RPM_NCP(_id, _always_on, _sleep_selectable, _min_uV, _max_uV, \
		_supply_regulator, _freq) \
	RPM_INIT(_id, _min_uV, _max_uV, 0, REGULATOR_CHANGE_VOLTAGE \
		 | REGULATOR_CHANGE_STATUS, 0, _max_uV, 1000, 1000, 0, \
		 RPM_VREG_PIN_CTRL_NONE, _freq, RPM_VREG_PIN_FN_8960_NONE, \
		 RPM_VREG_FORCE_MODE_8960_NONE, RPM_VREG_POWER_MODE_8960_PWM, \
		 RPM_VREG_STATE_OFF, _sleep_selectable, _always_on, \
		 _supply_regulator, 0)

/* Pin control initialization */
#define RPM_PC_INIT(_id, _always_on, _pin_fn, _pin_ctrl, _supply_regulator) \
	{ \
		.init_data = { \
			.constraints = { \
				.valid_ops_mask	= REGULATOR_CHANGE_STATUS, \
				.always_on	= _always_on, \
			}, \
			.num_consumer_supplies	= \
					ARRAY_SIZE(vreg_consumers_##_id##_PC), \
			.consumer_supplies	= vreg_consumers_##_id##_PC, \
			.supply_regulator	= _supply_regulator, \
		}, \
		.id	  = RPM_VREG_ID_PM8921_##_id##_PC, \
		.pin_fn	  = RPM_VREG_PIN_FN_8960_##_pin_fn, \
		.pin_ctrl = _pin_ctrl, \
	}

/* SAW regulator constraints */
struct regulator_init_data msm_saw_regulator_pdata_s5 =
	/*	      ID  vreg_name	       min_uV   max_uV */
	SAW_VREG_INIT(S5, "8921_s5",	       850000, 1300000);
struct regulator_init_data msm_saw_regulator_pdata_s6 =
	SAW_VREG_INIT(S6, "8921_s6",	       850000, 1300000);

/* PM8921 regulator constraints */
struct pm8xxx_regulator_platform_data
msm_pm8921_regulator_pdata[] __devinitdata = {
	/*
	 *			  ID a_on pd min_uV   max_uV   en_t  supply
	 *	system_uA
	 */
	PM8XXX_NLDO1200(L26, "8921_l26", 0, 1, 1050000, 1050000, 200, "8921_s7",
		0, 1),
	PM8XXX_NLDO1200(L27, "8921_l27", 0, 1, 1050000, 1050000, 200, "8921_s7",
		0, 2),
	PM8XXX_NLDO1200(L28, "8921_l28", 0, 1, 1050000, 1050000, 200, "8921_s7",
		0, 3),
	PM8XXX_NLDO1200(L29, "8921_l29", 0, 1, 1700000, 2200000, 200, "8921_s8",
		0, 4),
};

static struct rpm_regulator_init_data
msm_rpm_regulator_init_data[] __devinitdata = {
	/*	 ID    a_on pd ss min_uV   max_uV  supply sys_uA freq */
	RPM_SMPS(S1,	 1, 1, 0, 1225000, 1225000, NULL, 100000, 3p20),
	RPM_SMPS(S2,	 0, 1, 0, 1300000, 1300000, NULL, 0,	  1p60),
	RPM_SMPS(S3,	 0, 1, 1,  500000, 1150000, NULL, 100000, 4p80),
	RPM_SMPS(S4,	 1, 1, 0, 1800000, 1800000, NULL, 100000, 1p60),
	RPM_SMPS(S7,	 0, 1, 0, 1150000, 1150000, NULL, 100000, 3p20),
	RPM_SMPS(S8,	 1, 1, 1, 2100000, 2100000, NULL, 100000, 1p60),

	/*	ID     a_on pd ss min_uV   max_uV  supply  sys_uA init_ip */
	RPM_LDO(L1,	 1, 1, 0, 1050000, 1050000, "8921_s4", 0, 10000),
	RPM_LDO(L2,	 0, 1, 0, 1100000, 1450000, "8921_s4", 0, 0),
	RPM_LDO(L3,	 0, 1, 0, 3075000, 3075000, NULL,      0, 0),
	RPM_LDO(L4,	 1, 1, 0, 1800000, 1800000, NULL,      10000, 10000),
	RPM_LDO(L5,	 0, 1, 0, 2950000, 2950000, NULL,      0, 0),
	RPM_LDO(L6,	 1, 1, 0, 2950000, 2950000, NULL,      0, 0),
	RPM_LDO(L7,	 1, 1, 0, 1850000, 2950000, NULL,      10000, 10000),
	RPM_LDO(L8,	 0, 1, 0, 2600000, 3000000, NULL,      0, 0),
	RPM_LDO(L9,	 0, 1, 0, 2850000, 2850000, NULL,      0, 0),
	RPM_LDO(L10,	 0, 1, 0, 3000000, 3000000, NULL,      0, 0),
	RPM_LDO(L11,	 0, 1, 0, 2600000, 3000000, NULL,      0, 0),
	RPM_LDO(L12,	 0, 1, 0, 1200000, 1200000, "8921_s4", 0, 0),
	RPM_LDO(L14,	 0, 1, 0, 1800000, 1800000, NULL,      0, 0),
	RPM_LDO(L15,	 0, 1, 0, 1800000, 2950000, NULL,      0, 0),
	RPM_LDO(L16,	 0, 1, 0, 2600000, 3000000, NULL,      0, 0),
	RPM_LDO(L17,	 0, 1, 0, 1800000, 3000000, NULL,      0, 0),
	RPM_LDO(L18,	 0, 1, 0, 1200000, 1200000, "8921_s4", 0, 0),
	RPM_LDO(L21,	 0, 1, 0, 1900000, 1900000, "8921_s8", 0, 0),
	RPM_LDO(L22,	 0, 1, 0, 2750000, 2750000, NULL,      0, 0),
	RPM_LDO(L23,	 1, 1, 1, 1800000, 1800000, "8921_s8", 10000, 10000),
	RPM_LDO(L24,	 0, 1, 1,  750000, 1150000, "8921_s1", 10000, 10000),
	RPM_LDO(L25,	 1, 1, 0, 1225000, 1225000, "8921_s1", 10000, 10000),

	/*	ID     a_on pd ss		    supply */
	RPM_VS(LVS1,	 0, 1, 0,		    "8921_s4"),
	RPM_VS(LVS2,	 0, 1, 0,		    "8921_s1"),
	RPM_VS(LVS3,	 0, 1, 0,		    "8921_s4"),
	RPM_VS(LVS4,	 0, 1, 0,		    "8921_s4"),
	RPM_VS(LVS5,	 0, 1, 0,		    "8921_s4"),
	RPM_VS(LVS6,	 0, 1, 0,		    "8921_s4"),
	RPM_VS(LVS7,	 0, 1, 0,		    "8921_s4"),
};

int msm_pm8921_regulator_pdata_len __devinitdata =
	ARRAY_SIZE(msm_pm8921_regulator_pdata);

struct rpm_regulator_platform_data msm_rpm_regulator_pdata __devinitdata = {
	.init_data = msm_rpm_regulator_init_data,
	.num_regulators = ARRAY_SIZE(msm_rpm_regulator_init_data),
	.version		= RPM_VREG_VERSION_8960,
	.vreg_id_vdd_mem	= RPM_VREG_ID_PM8921_L24,
	.vreg_id_vdd_dig	= RPM_VREG_ID_PM8921_S3,
};

/* Section: MSM GPIO */
static struct gpiomux_setting unused_gpio = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting gpio_2ma_no_pull_low = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting gpio_2ma_no_pull_in = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting gpio_2ma_pull_up_in = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting gpio_2ma_pull_down_in = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting cdc_mclk = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting slimbus = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

static struct gpiomux_setting gsbi3 = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi4 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi10 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi12 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting cam_mclk0 = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting cam_mclk1 = {
	.func = GPIOMUX_FUNC_2,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting irda_uart_tx = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting irda_uart_rx = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting felica_uart = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE
};

static struct gpiomux_setting tsif = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE
};

static struct gpiomux_setting hdmi_suspend_1_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting hdmi_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting hdmi_suspend_2_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_active_2_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting debug_uart_tx = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_NONE
};

static struct gpiomux_setting debug_uart_rx = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP
};

static struct msm_gpiomux_config semc_blue_all_cfgs[] __initdata = {
	{ /* NC */
		.gpio = 0,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* MCAM_RST_N */
		.gpio = 1,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* FLASH_DR_RST_N */
		.gpio = 2,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* FLASH_TRG */
		.gpio = 3,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_pull_down_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* CAM_MCLK1 */
		.gpio = 4,
		.settings = {
			[GPIOMUX_ACTIVE] = &cam_mclk1,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* CAM_MCLK0 */
		.gpio = 5,
		.settings = {
			[GPIOMUX_ACTIVE] = &cam_mclk0,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* NC */
		.gpio = 6,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 7,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 8,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 9,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* ACCEL_INT */
		.gpio = 10,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* TP_INT */
		.gpio = 11,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* HW_ID0 */
		.gpio = 12,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* HW_ID1 */
		.gpio = 13,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* HW_ID2 */
		.gpio = 14,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* HW_ID3 */
		.gpio = 15,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* I2C_DATA_TP */
		.gpio = 16,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi3,
			[GPIOMUX_SUSPENDED] = &gsbi3,
		},
	},
	{ /* I2C_CLK_TP */
		.gpio = 17,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi3,
			[GPIOMUX_SUSPENDED] = &gsbi3,
		},
	},
	{ /* CHAT_CAM_RST_N */
		.gpio = 18,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
#if defined(CONFIG_SEMC_FELICA_SUPPORT) && !defined(CONFIG_NFC_PN544)
	{ /* FELICIA_RFS */
		.gpio = 19,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
#elif !defined(CONFIG_SEMC_FELICA_SUPPORT) && defined(CONFIG_NFC_PN544)
	{ /* NFC_DWLD_EN */
		.gpio = 19,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
#else
	{ /* NC */
		.gpio = 19,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
#endif
	{ /* I2C_DATA_CAM */
		.gpio = 20,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi4,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* I2C_CLK_CAM */
		.gpio = 21,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi4,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* NC (ETM_O_TRCTL) */
		.gpio = 22,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRCLK) */
		.gpio = 23,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA0) */
		.gpio = 24,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA1) */
		.gpio = 25,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},

	/* NOT CONFIGURED: 26-31 */

	{ /* Reserved (USB_UICC_D_P) */
		.gpio = 32,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* Reserved (USB_UICC_D_M) */
		.gpio = 33,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	/* UART_TX_DFMS */
	{
		.gpio      = 34,
		.settings = {
			[GPIOMUX_ACTIVE] = &debug_uart_tx,
			[GPIOMUX_SUSPENDED] = &debug_uart_tx,
		},
	},
	/* UART_RX_DTMS */
	{
		.gpio      = 35,
		.settings = {
			[GPIOMUX_ACTIVE] = &debug_uart_rx,
			[GPIOMUX_SUSPENDED] = &debug_uart_rx,
		},
	},
	{ /* LCD_DCDC_EN */
		.gpio = 36,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* OTG_OVRCUR_DET_N */
		.gpio = 37,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* NC (ETM_O_TRDATA5) */
		.gpio = 38,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA4) */
		.gpio = 39,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA3) */
		.gpio = 40,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA2) */
		.gpio = 41,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* UART_TX_IRDA */
		.gpio = 42,
		.settings = {
			[GPIOMUX_ACTIVE] = &irda_uart_tx,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* UART_RX_IRDA */
		.gpio = 43,
		.settings = {
			[GPIOMUX_ACTIVE] = &irda_uart_rx,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* I2C_DATA_SENS */
		.gpio = 44,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi12,
			[GPIOMUX_SUSPENDED] = &gsbi12,
		},
	},
	{ /* I2C_CLK_SENS */
		.gpio = 45,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi12,
			[GPIOMUX_SUSPENDED] = &gsbi12,
		},
	},
	{ /* DEBUG_GPIO0 */
		.gpio = 46,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* DEBUG_GPIO1 */
		.gpio = 47,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* GYRO_FSYNC */
		.gpio = 48,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* PROX_INT_N */
		.gpio = 49,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_pull_up_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_up_in,
		},
	},
	{ /* NC (TKEY_RST_N) */
		.gpio = 50,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* USB_OTG_EN */
		.gpio = 51,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* NC (TKEY_INT_N) */
		.gpio = 52,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 53,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (LMU_HW_EN) */
		.gpio = 54,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},

	/* 55-57 not configured */

	{ /* NC (LMU_INT_N) */
		.gpio = 58,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* SLIMBUS1_MCLK */
		.gpio = 59,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cdc_mclk,
		},
	},
	{ /* SLIMBUS1_CLK */
		.gpio = 60,
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
	{ /* SLIMBUS1_DATA */
		.gpio = 61,
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},

	/* 62 not configured */

	{ /* Reserved (USB_UICC_EN) */
		.gpio = 63,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* MHL_RST_N */
		.gpio = 64,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* MHL_INT */
		.gpio = 65,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_pull_up_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_up_in,
		},
	},
	{ /* LCD_ID */
		.gpio = 66,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* NC (GYRO_DRDY) */
		.gpio = 67,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (MUTE_EN) */
		.gpio = 68,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* GYRO_INT_N */
		.gpio = 69,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* COMPASS_INT */
		.gpio = 70,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_down_in,
		},
	},
	{ /* UART_TX_FELICA */
		.gpio = 71,
		.settings = {
			[GPIOMUX_ACTIVE] = &felica_uart,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* UART_RX_FELICA */
		.gpio = 72,
		.settings = {
			[GPIOMUX_ACTIVE] = &felica_uart,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_up_in,
		},
	},
	{ /* I2C_DATA_PERI */
		.gpio = 73,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi10,
			[GPIOMUX_SUSPENDED] = &gsbi10,
		},
	},
	{ /* I2C_CLK_PERI */
		.gpio = 74,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi10,
			[GPIOMUX_SUSPENDED] = &gsbi10,
		},
	},
	{ /* TSIF1_CLK */
		.gpio = 75,
		.settings = {
			[GPIOMUX_ACTIVE] = &tsif,
			[GPIOMUX_SUSPENDED] = &tsif,
		},
	},
	{ /* TSIF1_EN */
		.gpio = 76,
		.settings = {
			[GPIOMUX_ACTIVE] = &tsif,
			[GPIOMUX_SUSPENDED] = &tsif,
		},
	},
	{ /* TSIF1_DATA */
		.gpio = 77,
		.settings = {
			[GPIOMUX_ACTIVE] = &tsif,
			[GPIOMUX_SUSPENDED] = &tsif,
		},
	},
	{ /* NC */
		.gpio = 78,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 79,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* TUNER_RST_EN */
		.gpio = 80,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* BATT_COVER_OPEN */
		.gpio = 81,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* TUNER_PWR_EN */
		.gpio = 82,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},

	/* 83 not configured */

	/* 84-88 configured in separate struct below */

	{ /* Reserved (CAM_AF_PWR_EN) */
		.gpio = 89,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 90,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 91,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* SLIDE_OPEN_DETECT */
		.gpio = 92,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* NC (VIB_DETECT/ETM_O_TRDATA11) */
		.gpio = 93,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 94,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* RF_ID_EXTENTION */
		.gpio = 95,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_low,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_low,
		},
	},
	{ /* NC */
		.gpio = 96,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA7) */
		.gpio = 97,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (ETM_O_TRDATA6) */
		.gpio = 98,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC (HDMI_CEC_OUT_MSM) */
		.gpio = 99,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* HDMI_DDC_CLK_MSM */
		.gpio = 100,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_1_cfg,
		},
	},
	{ /* HDMI_DDC_DATA_MSM */
		.gpio = 101,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_1_cfg,
		},
	},
	{ /* HDMI_HOTPLUG_DET_MSM */
		.gpio = 102,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_2_cfg,
		},
	},

	/* 103-105 not configured */

	{ /* NFC_IRQ/FELICA_INT */
		.gpio = 106,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_no_pull_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_no_pull_in,
		},
	},
	{ /* SW_SERVICE */
		.gpio = 107,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_2ma_pull_up_in,
			[GPIOMUX_SUSPENDED] = &gpio_2ma_pull_up_in,
		},
	},

	/* 108-149 not configured */

	{ /* NC */
		.gpio = 150,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
	{ /* NC */
		.gpio = 151,
		.settings = { [GPIOMUX_SUSPENDED] = &unused_gpio, },
	},
};


static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

struct msm_gpiomux_config wcnss_5wire_interface[] = {
	{
		.gpio = 84,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 85,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 86,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 87,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 88,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
};

const size_t wcnss_5wire_interface_size = ARRAY_SIZE(wcnss_5wire_interface);

void __init gpiomux_device_install(void)
{
	msm_gpiomux_install(semc_blue_all_cfgs,
			ARRAY_SIZE(semc_blue_all_cfgs));

	msm_gpiomux_install(wcnss_5wire_interface,
			ARRAY_SIZE(wcnss_5wire_interface));
}

