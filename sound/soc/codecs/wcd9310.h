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

#include <sound/soc.h>

#define TABLA_VERSION_1_0	0
#define TABLA_VERSION_1_1	1
#define TABLA_VERSION_2_0	2

#define TABLA_NUM_REGISTERS 0x400
#define TABLA_MAX_REGISTER (TABLA_NUM_REGISTERS-1)
#define TABLA_CACHE_SIZE TABLA_NUM_REGISTERS

#define TABLA_REG_VAL(reg, val)		{reg, 0, val}

extern const u8 tabla_reg_readable[TABLA_CACHE_SIZE];
extern const u8 tabla_reg_defaults[TABLA_CACHE_SIZE];

enum tabla_micbias_num {
	TABLA_MICBIAS1,
	TABLA_MICBIAS2,
	TABLA_MICBIAS3,
	TABLA_MICBIAS4,
};

enum tabla_pid_current {
	TABLA_PID_MIC_2P5_UA,
	TABLA_PID_MIC_5_UA,
	TABLA_PID_MIC_10_UA,
	TABLA_PID_MIC_20_UA,
};

struct tabla_mbhc_calibration {
	enum tabla_micbias_num bias;
	int tldoh;
	int bg_fast_settle;
	enum tabla_pid_current mic_current;
	int mic_pid;
	enum tabla_pid_current hph_current;
	int setup_plug_removal_delay;
	int shutdown_plug_removal;
};

struct tabla_reg_mask_val {
	u16	reg;
	u8	mask;
	u8	val;
};

extern int tabla_hs_detect(struct snd_soc_codec *codec,
	struct snd_soc_jack *headset_jack, struct snd_soc_jack *button_jack,
	struct tabla_mbhc_calibration *calibration);

struct anc_header {
	u32 reserved[3];
	u32 num_anc_slots;
};

extern int tabla_mclk_enable(struct snd_soc_codec *codec, int mclk_enable);
