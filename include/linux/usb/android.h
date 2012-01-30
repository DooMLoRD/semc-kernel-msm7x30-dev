/*
 * Platform data for Android USB
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef	__LINUX_USB_ANDROID_H
#define	__LINUX_USB_ANDROID_H

#include <linux/if_ether.h>

struct android_usb_platform_data {
	int (*update_pid_and_serial_num)(uint32_t, const char *);
	/* USB device descriptor fields */
	__u16 vendor_id;

	__u16 version;
	/* Fields for composition switch support */
	struct usb_composition *compositions;
	int num_compositions;

	char *product_name;
	char *manufacturer_name;
	char *serial_number;

	/* number of LUNS for mass storage function */
	int nluns;
	int self_powered;
	struct platform_device *usb_mass_storage_device;
};

/* EUI-64 identifier format for Device Identification VPD page */
struct eui64_id {
	u8 ieee_company_id[3];
	u8 vendor_specific_ext_field[5];
};

/* Platform data for "usb_mass_storage" driver. */
struct usb_mass_storage_platform_data {
	/* Contains values for the SC_INQUIRY SCSI command. */
	char *vendor;
	char *product;
	int release;

	/* number of LUNS */
	int nluns;

	/* Information for CD-ROM */
	char *cdrom_vendor;
	char *cdrom_product;
	int cdrom_release;

	/* number of CD-ROM LUNS */
	int cdrom_nluns;

	char *serial_number;
	struct eui64_id eui64_id;
};

/* Platform data for USB ethernet driver. */
struct usb_ether_platform_data {
	u8	ethaddr[ETH_ALEN];
	u32	vendorID;
	const char *vendorDescr;
};

#endif	/* __LINUX_USB_ANDROID_H */
