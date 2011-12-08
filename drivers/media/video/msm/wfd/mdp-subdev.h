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
*
*/

#include <linux/videodev2.h>
#include <media/v4l2-subdev.h>

#define MDP_MAGIC_IOCTL 'M'

struct mdp_buf_info {
	void *inst;
	struct vb2_buffer *b;
	u32 fd;
	u32 offset;
};

struct mdp_prop {
	void *inst;
	u32 height;
	u32 width;
};
#define MDP_Q_BUFFER  _IOW(MDP_MAGIC_IOCTL, 1, struct mdp_buf_info *)
#define MDP_DQ_BUFFER  _IOR(MDP_MAGIC_IOCTL, 2, struct mdp_out_buf *)
#define MDP_PREPARE_BUF  _IOW(MDP_MAGIC_IOCTL, 3, struct  mdp_buf_info *)
#define MDP_OPEN  _IOR(MDP_MAGIC_IOCTL, 4, void **)
#define MDP_SET_PROP  _IOW(MDP_MAGIC_IOCTL, 5, struct mdp_prop *)
#define MDP_RELEASE_BUF  _IOW(MDP_MAGIC_IOCTL, 6, struct mdp_buf_info *)
#define MDP_CLOSE  _IOR(MDP_MAGIC_IOCTL, 7, void *)
#define MDP_START  _IOR(MDP_MAGIC_IOCTL, 8, void *)
#define MDP_STOP  _IOR(MDP_MAGIC_IOCTL, 9, void *)
extern int mdp_init(struct v4l2_subdev *sd, u32 val);
extern long mdp_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg);
