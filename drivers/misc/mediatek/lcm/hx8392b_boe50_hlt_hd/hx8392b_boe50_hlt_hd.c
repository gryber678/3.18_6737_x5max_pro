/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2008
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mt-plat/mt_gpio.h>
//#include <mt-plat/upmu_common.h>
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)


#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE 
#define TRUE 1
#endif
//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
#define LCM_DSI_CMD_MODE									0
#define LCM_ID_HX8394 0x94
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util = {0};
#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) lcm_util.dsi_write_regs(addr, pdata, byte_nums)
/* #define read_reg lcm_util.dsi_read_reg() */
#define read_reg_v2(cmd, buffer, buffer_size) lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0xb9, 3,{0xff,0x83,0x92}},
{0x35, 1,{0x00}},
{0xb1, 21,{0x45,0x4b,0x4b,0x14,0x3b,0x31,0x55,0x11,0xed,0xd0,0x2b,0x4a,0x0d,0x43,0xf0,0xc0,0xc0,0xc0,0xc0,0x00,0x02}},
{0xb2, 18,{0x00,0x06,0xc8,0x12,0x12,0x62,0x1a,0x12,0x12,0x1a,0x1a,0x01,0x00,0x20,0x04,0x13,0x95,0x40}},
{0xb4, 40,{0x00,0xff,0x00,0x3a,0x04,0x39,0x04,0x39,0x00,0x00,0x03,0x04,0x00,0x0c,0x02,0x03,0x04,0x00,0x00,0x03,0x32,0x00,0x3a,0x04,0x39,0x04,0x39,0x00,0x00,0x03,0x04,0x00,0x0c,0x02,0x03,0x04,0x00,0x00,0x03,0x32}},
{0xb6, 2,{0x6e,0x6e}},
{0xe0, 42,{0x00,0x1d,0x23,0x39,0x3d,0x3f,0x31,0x4a,0x07,0x0d,0x0e,0x12,0x14,0x12,0x14,0x10,0x17,0x00,0x1d,0x23,0x39,0x3d,0x3f,0x31,0x4a,0x07,0x0d,0x0e,0x12,0x14,0x12,0x14,0x10,0x17,0x0b,0x17,0x06,0x11,0x0b,0x17,0x06,0x11}},
{0xba, 2,{0x43,0x03}},
{0xcc, 1,{0x08}},
{0xd3, 40,{0x00,0x00,0x01,0x00,0x00,0x00,0x28,0x08,0x32,0x00,0x13,0x00,0x13,0x32,0x13,0xc0,0x00,0x00,0x32,0x10,0x08,0x00,0x00,0x10,0x13,0x13,0x01,0x04,0x00,0x27,0x00,0x00,0x00,0x0a,0x00,0x01,0x12,0x05,0x15,0x00}},
{0xd5, 54,{0x01,0x01,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1e,0x1e,0x20,0x20,0x18,0x18,0x2f,0x2f,0x30,0x30,0x31,0x31,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xd6, 54,{0x00,0x00,0x01,0x01,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1e,0x1e,0x20,0x20,0x18,0x18,0x2f,0x2f,0x30,0x30,0x31,0x31,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xbd, 1,{0x00}},
{0xd8, 28,{0xf0,0x00,0x00,0x00,0x02,0xea,0x00,0xf0,0x00,0x00,0x00,0x02,0xea,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xbd, 1,{0x01}},
{0xd8, 42,{0xf0,0x00,0x00,0x00,0x03,0x3f,0x00,0xf0,0x00,0x00,0x00,0x03,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xbd, 1,{0x02}},
{0xd8, 14,{0xf0,0x00,0x00,0x00,0x03,0x3f,0x00,0xf0,0x00,0x00,0x00,0x03,0x3f,0x00}},
{0xbd, 1,{0x00}},
{0xc0, 2,{0x01,0x92}},
  {0x11,  0,  {0x00}},

  {REGFLAG_DELAY, 200, {0}}, 
  {0x29,  0,  {0x00}},

  {REGFLAG_DELAY, 10, {0}},
};
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_init_power(void)
{
}

static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
}

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	// enable tearing-free
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

	//params->dsi.mode   = BURST_VDO_MODE; 
	params->dsi.mode   = SYNC_EVENT_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

	params->dsi.word_count=720*3;
	params->dsi.vertical_sync_active				= 2;//2//
	//params->dsi.vertical_backporch					= 5;//8
	//params->dsi.vertical_frontporch					= 20;//6
	params->dsi.vertical_backporch					= 18;//8
	params->dsi.vertical_frontporch					= 20;//6
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 
	params->dsi.horizontal_sync_active				= 76;//86 20
	params->dsi.horizontal_backporch				= 77;//55 50
	params->dsi.horizontal_frontporch				= 76;//55	50
	params->dsi.horizontal_active_pixel			= FRAME_WIDTH;

    params->dsi.PLL_CLOCK = 208;
}
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++) {

        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {

            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;

            case REGFLAG_END_OF_TABLE :
                break;

            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
        }
    }

}


static void lcm_init(void)
{
	//unsigned int data_array[16];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(10);	///===>180ms
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);

}                                       

static void lcm_suspend(void)
{
	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	int data_array[16];

	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);

	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
}


static void lcm_resume(void)
{
#ifndef BUILD_LK
	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
#endif
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id,id0,id1;
	unsigned char buffer[5];
	int array[5];

	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(150);

	array[0]=0x00043902;
	array[1]=0x9483FFB9;// page enable
	dsi_set_cmdq(array, 2, 1);
	MDELAY(10);

	array[0]=0x00023902;
	array[1]=0x000013ba;
	dsi_set_cmdq(array, 2, 1);
	MDELAY(10);

	array[0] = 0x00023700;// return byte number
	dsi_set_cmdq(array, 1, 1);
	MDELAY(10);

	read_reg_v2(0xF4, buffer, 2);
	id0 = buffer[0]; 
	read_reg_v2(0xdc, buffer, 2);
	id1 = buffer[0];//1a 0d
	id = id0;
  
	return (LCM_ID_HX8394 == id)?1:0;
}
// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8392b_boe50_hlt_hd_lcm_drv =
{
	.name           = "hx8392b_boe50_hlt_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
	//.esd_check	= lcm_esd_check,
	//.esd_recover	= lcm_esd_recover,
	.init_power        = lcm_init_power,
	.resume_power = lcm_resume_power,
	.suspend_power = lcm_suspend_power,
};
