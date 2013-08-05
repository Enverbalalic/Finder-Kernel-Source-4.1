/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
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

#include "msm_sensor.h"
#define SENSOR_NAME "imx111"
#define PLATFORM_DRIVER_NAME "msm_camera_imx111"
#define imx111_obj imx111_##obj

DEFINE_MUTEX(imx111_mut);
static struct msm_sensor_ctrl_t imx111_s_ctrl;

static struct msm_camera_i2c_reg_conf imx111_start_settings[] = {
	{0x0100, 0x01},
};

static struct msm_camera_i2c_reg_conf imx111_stop_settings[] = {
	{0x0100, 0x00},
};

static struct msm_camera_i2c_reg_conf imx111_groupon_settings[] = {
	{0x0104, 0x01},
};

static struct msm_camera_i2c_reg_conf imx111_groupoff_settings[] = {
	{0x0104, 0x00},
};

struct msm_camera_i2c_reg_conf imx111_video_setting[] = {
	//PLL Setting
	{0x0305,  0x02},
	{0x0307,  0x38},
	{0x30A4,  0x02},
	{0x303C,  0x4B},
			  
	//Mode Setting
	{0x0340,  0x04},
	{0x0341,  0xE2},
	{0x0342,  0x0D},
	{0x0343,  0xD0},
	{0x0344,  0x02},
	{0x0345,  0x54},
	{0x0346,  0x02},
	{0x0347,  0xB0},
	{0x0348,  0x0A},
	{0x0349,  0x8B},
	{0x034A,  0x07},
	{0x034B,  0x4F},
	{0x034C,  0x08},
	{0x034D,  0x38},
	{0x034E,  0x04},
	{0x034F,  0xA0},
	{0x0381,  0x01},
	{0x0383,  0x01},
	{0x0385,  0x01},
	{0x0387,  0x01},
	{0x3048,  0x00},
	{0x309B,  0x20},
	{0x30A1,  0x08},
	{0x30B2,  0x07},
	{0x30D5,  0x00},
	{0x30D6,  0x85},
	{0x30D7,  0x2A},
	{0x30DE,  0x00},
	{0x3301,  0x00},
	{0x3318,  0x62},

	{0x0101, 0x0003} //horizontal mirror & vertical flip

};

static struct msm_camera_i2c_reg_conf imx111_prev_settings[] = {
	//PLL Setting
	{0x0305,  0x02},
	{0x0307,  0x38},
	{0x30A4,  0x02},
	{0x303C,  0x4B},
			  
	//Mode Setting
	{0x0340,  0x04},
	{0x0341,  0xE2},
	{0x0342,  0x0D},
	{0x0343,  0xD0},
	{0x0344,  0x00},
	{0x0345,  0x08},
	{0x0346,  0x00},
	{0x0347,  0x30},
	{0x0348,  0x0C},
	{0x0349,  0xD7},
	{0x034A,  0x09},
	{0x034B,  0xCF},
	{0x034C,  0x06},
	{0x034D,  0x68},
	{0x034E,  0x04},
	{0x034F,  0xD0},
	{0x0381,  0x01},
	{0x0383,  0x03},
	{0x0385,  0x01},
	{0x0387,  0x03},
	{0x3048,  0x01},
	{0x309B,  0x28},
	{0x30A1,  0x09},
	{0x30B2,  0x05},
	{0x30D5,  0x09},
	{0x30D6,  0x01},
	{0x30D7,  0x01},
	{0x30DE,  0x02},
	{0x3301,  0x00},
	{0x3318,  0x72},

	{0x0101, 0x0003} //horizontal mirror & vertical flip
};

static struct msm_camera_i2c_reg_conf imx111_snap_settings[] = {
	//PLL Setting
	{0x0305,  0x02},
	{0x0307,  0x38},
	{0x30A4,  0x02},
	{0x303C,  0x4B},
			  
	//Mode Setting
	{0x0340,  0x09},
	{0x0341,  0xC6},
	{0x0342,  0x0D},
	{0x0343,  0xD0},
	{0x0344,  0x00},
	{0x0345,  0x08},
	{0x0346,  0x00},
	{0x0347,  0x30},
	{0x0348,  0x0C},
	{0x0349,  0xD7},
	{0x034A,  0x09},
	{0x034B,  0xCF},
	{0x034C,  0x0C},
	{0x034D,  0xD0},
	{0x034E,  0x09},
	{0x034F,  0xA0},
	{0x0381,  0x01},
	{0x0383,  0x01},
	{0x0385,  0x01},
	{0x0387,  0x01},
	{0x3048,  0x00},
	{0x309B,  0x20},
	{0x30A1,  0x08},
	{0x30B2,  0x07},
	{0x30D5,  0x00},
	{0x30D6,  0x85},
	{0x30D7,  0x2A},
	{0x30DE,  0x00},
	{0x3301,  0x00},
	{0x3318,  0x62},

	{0x0101, 0x0003} //horizontal mirror & vertical flip
};

static struct msm_camera_i2c_reg_conf imx111_recommend_settings[] = {
	//Global Setting
	{0x3080,  0x50},
	{0x3087,  0x53},
	{0x309D,  0x94},
	{0x30C6,  0x00},
	{0x30C7,  0x00},
	{0x3115,  0x0B},
	{0x3118,  0x30},
	{0x311D,  0x25},
	{0x3121,  0x0A},
	{0x3212,  0xF2},
	{0x3213,  0x0F},
	{0x3215,  0x0F},
	{0x3217,  0x0B},
	{0x3219,  0x0B},
	{0x321B,  0x0D},
	{0x321D,  0x0D},
	{0x32AA,  0x11},

	//Black level Setting
	{0x3032,  0x40}
};

static struct v4l2_subdev_info imx111_subdev_info[] = {
	{
	.code   = V4L2_MBUS_FMT_SBGGR10_1X10,
	.colorspace = V4L2_COLORSPACE_JPEG,
	.fmt    = 1,
	.order    = 0,
	},
	/* more can be supported, to be added later */
};

static struct msm_camera_i2c_conf_array imx111_init_conf[] = {
	{&imx111_recommend_settings[0],
	ARRAY_SIZE(imx111_recommend_settings), 0, MSM_CAMERA_I2C_BYTE_DATA}
};

static struct msm_camera_i2c_conf_array imx111_confs[] = {
	{&imx111_snap_settings[0],
	ARRAY_SIZE(imx111_snap_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx111_prev_settings[0],
	ARRAY_SIZE(imx111_prev_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx111_video_setting[0],
	ARRAY_SIZE(imx111_video_setting), 0, MSM_CAMERA_I2C_BYTE_DATA},
};

static struct msm_sensor_output_info_t imx111_dimensions[] = {
	{
	/* full size */
		.x_output = 0x0CD0, /* 3280 */
		.y_output = 0x09A0, /* 2464 */
		.line_length_pclk = 0x0DD0, /* 3536 */
		.frame_length_lines = 0x09C6, /* 2502 */
		.vt_pixel_clk = 135000000,
		.op_pixel_clk = 270000000,
		.binning_factor = 1,
	},
	{
	/* 30 fps 1/2 * 1/2 */
		.x_output = 0x0668, /* 1640 */
		.y_output = 0x04D0, /* 1232 */
		.line_length_pclk = 0x0DD0, /* 3536 */
		.frame_length_lines = 0x04E6, /* 1254 */
		.vt_pixel_clk = 135000000,
		.op_pixel_clk = 270000000,
		.binning_factor = 1,
	},
	{
	/* 30 Full HD  */
		.x_output = 0x0838, /* 2104 */
		.y_output = 0x04A0, /* 1184 */
		.line_length_pclk = 0x0DD0, /* 3536 */
		.frame_length_lines = 0x04E2, /* 1250 */
		.vt_pixel_clk = 135000000,
		.op_pixel_clk = 270000000,
		.binning_factor = 1,
	},
};

static struct msm_camera_csi_params imx111_csic_params = {
	.data_format = CSI_10BIT,
	.lane_cnt    = 2,
	.lane_assign = 0xe4,
	.dpcm_scheme = 0,
	.settle_cnt  = 0x14,
};

static struct msm_camera_csi_params *imx111_csic_params_array[] = {
	&imx111_csic_params,
	&imx111_csic_params,
	&imx111_csic_params,
};

static struct msm_camera_csid_vc_cfg imx111_cid_cfg[] = {
	{0, CSI_RAW10, CSI_DECODE_10BIT},
	{1, CSI_EMBED_DATA, CSI_DECODE_8BIT},
	{2, CSI_RESERVED_DATA_0, CSI_DECODE_8BIT},
};

static struct msm_camera_csi2_params imx111_csi_params = {
	.csid_params = {
		.lane_cnt = 2,
		.lut_params = {
			.num_cid = ARRAY_SIZE(imx111_cid_cfg),
			.vc_cfg = imx111_cid_cfg,
		},
	},
	.csiphy_params = {
		.lane_cnt = 2,
		.settle_cnt = 0x14,
	},
};

static struct msm_camera_csi2_params *imx111_csi_params_array[] = {
	&imx111_csi_params,
	&imx111_csi_params,
	&imx111_csi_params,
};

static struct msm_sensor_output_reg_addr_t imx111_reg_addr = {
	.x_output = 0x034C,
	.y_output = 0x034E,
	.line_length_pclk = 0x0342,
	.frame_length_lines = 0x0340,
};

static struct msm_sensor_id_info_t imx111_id_info = {
	.sensor_id_reg_addr = 0x0000,
	.sensor_id = 0x0111,
};

static struct msm_sensor_exp_gain_info_t imx111_exp_gain_info = {
	.coarse_int_time_addr = 0x0202,
	.global_gain_addr = 0x0204,
	.vert_offset = 5,
};

static const struct i2c_device_id imx111_i2c_id[] = {
	{SENSOR_NAME, (kernel_ulong_t)&imx111_s_ctrl},
	{ }
};

static struct i2c_driver imx111_i2c_driver = {
	.id_table = imx111_i2c_id,
	.probe  = msm_sensor_i2c_probe,
	.driver = {
		.name = SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client imx111_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};


static int __init imx111_sensor_init_module(void)
{
	return i2c_add_driver(&imx111_i2c_driver);
}

static struct v4l2_subdev_core_ops imx111_subdev_core_ops = {
	.ioctl = msm_sensor_subdev_ioctl,
	.s_power = msm_sensor_power,
};

static struct v4l2_subdev_video_ops imx111_subdev_video_ops = {
	.enum_mbus_fmt = msm_sensor_v4l2_enum_fmt,
};

static struct v4l2_subdev_ops imx111_subdev_ops = {
	.core = &imx111_subdev_core_ops,
	.video  = &imx111_subdev_video_ops,
};

static struct msm_sensor_fn_t imx111_func_tbl = {
	.sensor_start_stream = msm_sensor_start_stream,
	.sensor_stop_stream = msm_sensor_stop_stream,
	.sensor_group_hold_on = msm_sensor_group_hold_on,
	.sensor_group_hold_off = msm_sensor_group_hold_off,
	.sensor_set_fps = msm_sensor_set_fps,
	.sensor_write_exp_gain = msm_sensor_write_exp_gain1,
	.sensor_write_snapshot_exp_gain = msm_sensor_write_exp_gain1,
	.sensor_setting = msm_sensor_setting,
	.sensor_csi_setting = msm_sensor_setting1,
	.sensor_set_sensor_mode = msm_sensor_set_sensor_mode,
	.sensor_mode_init = msm_sensor_mode_init,
	.sensor_get_output_info = msm_sensor_get_output_info,
	.sensor_config = msm_sensor_config,
	.sensor_power_up = msm_sensor_power_up,
	.sensor_power_down = msm_sensor_power_down,
	.sensor_adjust_frame_lines = msm_sensor_adjust_frame_lines,
	.sensor_get_csi_params = msm_sensor_get_csi_params,
};

static struct msm_sensor_reg_t imx111_regs = {
	.default_data_type = MSM_CAMERA_I2C_BYTE_DATA,
	.start_stream_conf = imx111_start_settings,
	.start_stream_conf_size = ARRAY_SIZE(imx111_start_settings),
	.stop_stream_conf = imx111_stop_settings,
	.stop_stream_conf_size = ARRAY_SIZE(imx111_stop_settings),
	.group_hold_on_conf = imx111_groupon_settings,
	.group_hold_on_conf_size = ARRAY_SIZE(imx111_groupon_settings),
	.group_hold_off_conf = imx111_groupoff_settings,
	.group_hold_off_conf_size = ARRAY_SIZE(imx111_groupoff_settings),
	.init_settings = &imx111_init_conf[0],
	.init_size = ARRAY_SIZE(imx111_init_conf),
	.mode_settings = &imx111_confs[0],
	.output_settings = &imx111_dimensions[0],
	.num_conf = ARRAY_SIZE(imx111_confs),
};

static struct msm_sensor_ctrl_t imx111_s_ctrl = {
	.msm_sensor_reg = &imx111_regs,
	.sensor_i2c_client = &imx111_sensor_i2c_client,
	.sensor_i2c_addr = 0x20,
	.sensor_output_reg_addr = &imx111_reg_addr,
	.sensor_id_info = &imx111_id_info,
	.sensor_exp_gain_info = &imx111_exp_gain_info,
	.cam_mode = MSM_SENSOR_MODE_INVALID,
	.csic_params = &imx111_csic_params_array[0],
	.csi_params = &imx111_csi_params_array[0],
	.msm_sensor_mutex = &imx111_mut,
	.sensor_i2c_driver = &imx111_i2c_driver,
	.sensor_v4l2_subdev_info = imx111_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(imx111_subdev_info),
	.sensor_v4l2_subdev_ops = &imx111_subdev_ops,
	.func_tbl = &imx111_func_tbl,
	.clk_rate = MSM_SENSOR_MCLK_24HZ,
};

module_init(imx111_sensor_init_module);
MODULE_DESCRIPTION("SONY 8MP Bayer sensor driver");
MODULE_LICENSE("GPL v2");
