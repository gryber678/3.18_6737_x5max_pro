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


static void lcm_register(void)
{

	unsigned int data_array[40];
	//////////////////////////////////////////////lrzadd0821////for1222

	//B9
	data_array[0] = 0x00043902;                          
	data_array[1] = 0x9483ffB9; 
	dsi_set_cmdq(&data_array, 2, 1); 
	MDELAY(1);

	//BA
	data_array[0] = 0x00113902;
	data_array[1] = 0x008213Ba;
	data_array[2] = 0x1000c516;
	data_array[3] = 0x03240fff;
	data_array[4] = 0x20252421;
	data_array[5] = 0x00000008;
	dsi_set_cmdq(&data_array, 6, 1);
	MDELAY(1);
	//B1
	data_array[0] = 0x00123902;                          
	data_array[1] = 0x040001b1; 
	data_array[2] = 0xf11203c3;
	data_array[3] = 0x3f3f2c24;
	data_array[4] = 0xe6000257;
	data_array[5] = 0x0000a6E2;
	dsi_set_cmdq(&data_array, 6, 1); 
	MDELAY(1);
	//B2 
	data_array[0] = 0x00073902;                          
	data_array[1] = 0x0ec800B2; 
	data_array[2] = 0x00330030; 
	dsi_set_cmdq(&data_array, 3, 1); 
	MDELAY(1);
	//B4
	data_array[0]=0x00203902;                          
	data_array[1] = 0x322000B4; 
	data_array[2] = 0x15320810; 
	data_array[3] = 0x0810220f;                          
	data_array[4] = 0x0a560313; 
	data_array[5] = 0x02560317; 
	data_array[6] = 0x06026161; 
	data_array[7] = 0x0a5f0644; 
	data_array[8] = 0x0005706b; 
	dsi_set_cmdq(&data_array, 9, 1);
	MDELAY(1);

data_array[0]=0x00023902;
data_array[1]=0x00002eb6;
dsi_set_cmdq(&data_array,2, 1);
MDELAY(1);

	//D5
	data_array[0] = 0x003f3902;                          
	data_array[1] = 0x000000D5;
	data_array[2] = 0x01000A00;
	data_array[3] = 0x11000000;
	data_array[4] = 0x23456700;
	data_array[5] = 0x88880101;
	data_array[6] = 0x88888888;
	data_array[7] = 0x88888888;
	data_array[8] = 0x88bbaa88;
	data_array[9] = 0x88888845;
	data_array[10] = 0x88888888;
	data_array[11] = 0x88888888;
	data_array[12] = 0x88888888;
	data_array[13] = 0x88888888;
	data_array[14] = 0x001f1088;
	data_array[15] = 0x00000000;
	data_array[16] = 0x00000000;
	dsi_set_cmdq(&data_array, 17, 1); 
	MDELAY(1);
	//CC
data_array[0]=0x00023902;
data_array[1]=0x000009cc;
dsi_set_cmdq(&data_array,2, 1);
MDELAY(1);

data_array[0]=0x00053902;
data_array[1]=0x100206bf;
data_array[2]=0x00000004;
dsi_set_cmdq(&data_array,3, 1);
MDELAY(1);

	//C7
	data_array[0] = 0x00053902;                          
	data_array[1] = 0x001000c7; 
	data_array[2] = 0x00000010; 
	dsi_set_cmdq(&data_array, 3, 1);
	MDELAY(1);


	//E0
	data_array[0] = 0x002B3902;                          
	data_array[1] = 0x030000E0; 
	data_array[2] = 0x123f2e27;                          
	data_array[3] = 0x0f0b0634;
	data_array[4] = 0x14131412;                          
	data_array[5] = 0x01001711;
	data_array[6] = 0x3f2d2704;                          
	data_array[7] = 0x0b053411;
	data_array[8] = 0x12151210;                          
	data_array[9] = 0x09191013;
	data_array[10]= 0x09120716;
	data_array[11]= 0x00120617;
	dsi_set_cmdq(&data_array, 12, 1);
	MDELAY(1);

	//C0

	data_array[0] = 0x00033902;                          
	data_array[1] = 0x00170cc0; 
	dsi_set_cmdq(&data_array, 2, 1); 
	MDELAY(1);

	//C6-SetECO
	data_array[0] = 0x00033902;                          
	data_array[1] = 0x000808c6; 
	dsi_set_cmdq(&data_array, 2, 1); 
	MDELAY(1);
	//D4

data_array[0]=0x00023902;
data_array[1]=0x000032d4;
dsi_set_cmdq(&data_array,2, 1);
MDELAY(1);

	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(200);//200

	data_array[0] = 0x00290500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);

}

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
	params->dsi.vertical_sync_active				= 4;//2//
	//params->dsi.vertical_backporch					= 5;//8
	//params->dsi.vertical_frontporch					= 20;//6
	params->dsi.vertical_backporch					= 13;//8
	params->dsi.vertical_frontporch					= 15;//6
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 
	params->dsi.horizontal_sync_active				= 36;//86 20
	params->dsi.horizontal_backporch				= 90;//55 50
	params->dsi.horizontal_frontporch				= 90;//55	50
	params->dsi.horizontal_active_pixel			= FRAME_WIDTH;

	params->dsi.PLL_CLOCK =195;//190;200; 221;//250;
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
	MDELAY(180);	///===>180ms
	//	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);


	lcm_register();
}                                       

static unsigned int lcm_compare_id(void);
static void lcm_suspend(void)
{
	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	int data_array[16];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(150);

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

	//	lcm_compare_id();
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

#if defined(BUILD_LK)
	printf("%s, hx8394a id = 0x%08x\n", __func__, id);
#else
	printk("%s, hx8394a id = 0x%08x\n", __func__, id);
#endif


	return (LCM_ID_HX8394 == id)?1:0;
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8394a01_ctc60_shenchao_hd_lcm_drv =
{
	.name           = "hx8394a01_ctc60_shenchao_hd",
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
