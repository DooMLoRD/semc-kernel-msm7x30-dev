/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/reboot.h>
#include <linux/workqueue.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <mach/irqs.h>
#include <mach/scm.h>
#include <mach/subsystem_restart.h>
#include <mach/subsystem_notif.h>
#include <mach/peripheral-loader.h>
#include "smd_private.h"
#include "ramdump.h"

#define MODULE_NAME			"wcnss_8960"

static void riva_smsm_cb_fn(struct work_struct *);
static DECLARE_WORK(riva_smsm_cb_work, riva_smsm_cb_fn);

static void riva_fatal_fn(struct work_struct *);
static DECLARE_WORK(riva_fatal_work, riva_fatal_fn);

static void *riva_ramdump_dev;
static int riva_crash;
static int ss_restart_inprogress;
static int enable_riva_ssr;

static void riva_smsm_cb_fn(struct work_struct *work)
{
	if (!enable_riva_ssr)
		panic(MODULE_NAME ": SMSM reset request received from Riva");
	else
		subsystem_restart("riva");
}

static void smsm_state_cb_hdlr(void *data, uint32_t old_state,
					uint32_t new_state)
{
	riva_crash = true;
	pr_err("%s: smsm state changed to smsm reset\n", MODULE_NAME);

	if (ss_restart_inprogress) {
		pr_err("%s: Ignoring smsm reset req, restart in progress\n",
						MODULE_NAME);
		return;
	}
	if (new_state & SMSM_RESET)
		schedule_work(&riva_smsm_cb_work);
}

static void riva_fatal_fn(struct work_struct *work)
{
	if (!ss_restart_inprogress)
		panic(MODULE_NAME ": Watchdog bite received from Riva");
}

/* SMSM reset Riva */
static void smsm_riva_reset(void)
{
	/* per SS reset request bit is not available now,
	 * all SS host modules are setting this bit
	 * This is still under discussion*/
	smsm_change_state(SMSM_APPS_STATE, SMSM_RESET, SMSM_RESET);
}

/* Subsystem handlers */
static int riva_shutdown(const struct subsys_data *subsys)
{
	pil_force_shutdown("wcnss");
	return 0;
}

static int riva_powerup(const struct subsys_data *subsys)
{
	pil_force_boot("wcnss");
	return 0;
}

/* RAM segments for Riva SS;
 * We don't specify the full 5MB allocated for Riva. Only 3MB is specified */
static struct ramdump_segment riva_segments[] = {{0x8f200000,
						0x8f500000 - 0x8f200000} };

static int riva_ramdump(int enable, const struct subsys_data *subsys)
{
	pr_debug("%s: enable[%d]\n", MODULE_NAME, enable);
	if (enable)
		return do_ramdump(riva_ramdump_dev,
				riva_segments,
				ARRAY_SIZE(riva_segments));
	else
		return 0;
}

/* Riva crash handler */
static void riva_crash_shutdown(const struct subsys_data *subsys)
{
	ss_restart_inprogress = true;

	pr_err("%s: crash shutdown : %d\n", MODULE_NAME, riva_crash);
	if (riva_crash != true)
		smsm_riva_reset();
}

static struct subsys_data riva_8960 = {
	.name = "riva",
	.shutdown = riva_shutdown,
	.powerup = riva_powerup,
	.ramdump = riva_ramdump,
	.crash_shutdown = riva_crash_shutdown
};

static int enable_riva_ssr_set(const char *val, struct kernel_param *kp)
{
	int ret;

	ret = param_set_int(val, kp);
	if (ret)
		return ret;

	if (enable_riva_ssr)
		pr_info(MODULE_NAME ": Subsystem restart activated for riva.\n");

	return 0;
}

module_param_call(enable_riva_ssr, enable_riva_ssr_set, param_get_int,
			&enable_riva_ssr, S_IRUGO | S_IWUSR);

static int __init riva_restart_init(void)
{
	return ssr_register_subsystem(&riva_8960);
}

static int __init riva_ssr_module_init(void)
{
	int ret;

	ret = smsm_state_cb_register(SMSM_WCNSS_STATE, SMSM_RESET,
					smsm_state_cb_hdlr, 0);
	if (ret < 0) {
		pr_err("%s: Unable to register smsm callback for Riva Reset!"
				" (%d)\n", MODULE_NAME, ret);
		goto out;
	}
	ret = riva_restart_init();
	if (ret < 0) {
		pr_err("%s: Unable to register with ssr. (%d)\n",
				MODULE_NAME, ret);
		goto out;
	}
	riva_ramdump_dev = create_ramdump_device("riva");
	if (!riva_ramdump_dev) {
		pr_err("%s: Unable to create ramdump device.\n",
				MODULE_NAME);
		ret = -ENOMEM;
		goto out;
	}
	pr_info("%s: module initialized\n", MODULE_NAME);
out:
	return ret;
}

static void __exit riva_ssr_module_exit(void)
{
	free_irq(RIVA_APSS_WDOG_BITE_RESET_RDY_IRQ, NULL);
}

module_init(riva_ssr_module_init);
module_exit(riva_ssr_module_exit);

MODULE_LICENSE("GPL v2");
