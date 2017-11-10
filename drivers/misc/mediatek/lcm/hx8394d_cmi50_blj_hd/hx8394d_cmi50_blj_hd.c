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
#if defined(BUILD_LK)
#define LCM_DEBUG  printf
#define LCM_FUNC_TRACE() printf("huyl [uboot] %s\n",__func__)
#else
#define LCM_DEBUG  printk
#define LCM_FUNC_TRACE() printk("huyl [kernel] %s\n",__func__)
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)
#define LCM_ID                      								(0x0d)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER

#ifndef FALSE
#define FALSE 0
#endif
#define TRUE 1
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

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

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

static void lcm_get_params(LCM_PARAMS * params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type = LCM_TYPE_DSI;

	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	// enable tearing-free
	params->dbi.te_mode = LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity = LCM_POLARITY_RISING;

	params->dsi.mode = SYNC_EVENT_VDO_MODE;//SYNC_PULSE_VDO_MODE;	//BURST_VDO_MODE;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 720 * 3;
	params->dsi.vertical_sync_active				= 4;//2//
	//params->dsi.vertical_frontporch					= 20;//6
	params->dsi.vertical_backporch					= 12;//8
	params->dsi.vertical_frontporch					= 15;//6
	params->dsi.vertical_active_line				= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active				= 20;//86 20 2
	params->dsi.horizontal_backporch				= 30;//55 50 42
	params->dsi.horizontal_frontporch				= 40;//55	50 44
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

    params->dsi.PLL_CLOCK = 175;
	/*
	params->dsi.cont_clock =1; 
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.noncont_clock = 1;
	params->dsi.noncont_clock_period = 2;   // Unit : frames
	params->dsi.lcm_esd_check_table[0].cmd            =0x0A;
	params->dsi.lcm_esd_check_table[0].count          =1; 
	params->dsi.lcm_esd_check_table[0].para_list[0]   =0x9c;
	*/
}

static void lcm_init(void)
{
	unsigned int data_array[16];
    //int vcom=0x50;
	
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(180);	///===>180ms

	data_array[0] = 0x00043902;
	data_array[1] = 0x9483FFB9;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x008373BA;  
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x12126CB1;
	data_array[2] = 0xF1110426;
	data_array[3] = 0x23547A81;	// 3A --> 7A
	data_array[4] = 0x58D2C080;
	dsi_set_cmdq(data_array, 5, 1);
	MDELAY(1);

	data_array[0] = 0x000C3902;
	data_array[1] = 0x0E6400B2;
	data_array[2] = 0x081C220D;
	data_array[3] = 0x004D1C08;
	dsi_set_cmdq(data_array, 4, 1);
	MDELAY(1);

	data_array[0] = 0x000D3902;
	data_array[1] = 0x51FF00B4;
	data_array[2] = 0x035A595A;
	data_array[3] = 0x2070015A;
	data_array[4] = 0x00000070;
	dsi_set_cmdq(data_array, 5, 1);
	MDELAY(1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000007BC;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	//data_array[0] = 0x00043902;		//HX5186 MODE
	//data_array[1] = 0x010E41BF;
	//dsi_set_cmdq(data_array, 2, 1);
	//MDELAY(1);

	data_array[0] = 0x001F3902;
	data_array[1] = 0x000F00D3;
	data_array[2] = 0x00100740;
	data_array[3] = 0x00081008;
	data_array[4] = 0x0E155408;
	data_array[5] = 0x15020E05;
	data_array[6] = 0x47060506;
	data_array[7] = 0x4B0A0A44;
	data_array[8] = 0x00070710;
	dsi_set_cmdq(data_array, 9, 1);
	MDELAY(1);

	data_array[0] = 0x002D3902;
	data_array[1] = 0x1B1A1AD5;
	data_array[2] = 0x0201001B;
	data_array[3] = 0x06050403;
	data_array[4] = 0x0A090807;
	data_array[5] = 0x1825240B;
	data_array[6] = 0x18272618;
	data_array[7] = 0x18181818;
	data_array[8] = 0x18181818;
	data_array[9] = 0x18181818;
	data_array[10] = 0x20181818;
	data_array[11] = 0x18181821;
	data_array[12] = 0x00000018;
	dsi_set_cmdq(data_array, 13, 1);
	MDELAY(1);

	data_array[0] = 0x002D3902;
	data_array[1] = 0x1B1A1AD6;
	data_array[2] = 0x090A0B1B;
	data_array[3] = 0x05060708;
	data_array[4] = 0x01020304;
	data_array[5] = 0x58202100;
	data_array[6] = 0x18262758;
	data_array[7] = 0x18181818;
	data_array[8] = 0x18181818;
	data_array[9] = 0x18181818;
	data_array[10] = 0x25181818;
	data_array[11] = 0x18181824;
	data_array[12] = 0x00000018;
	dsi_set_cmdq(data_array, 13, 1);
	MDELAY(1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000BDC6;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000002BD;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	data_array[0] = 0x000D3902;
	data_array[1] = 0xEEFFFFD8;
	data_array[2] = 0xFFA0FBEB;
	data_array[3] = 0xFBEBEEFF;
	data_array[4] = 0x000000A0;
	dsi_set_cmdq(data_array, 5, 1);
	MDELAY(1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000009CC;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001430C0;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x40C000C7;
	data_array[2] = 0x000000C0;
	dsi_set_cmdq(data_array, 3, 1);
	MDELAY(1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x006f6fB6;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(1);


	data_array[0] = 0x002B3902;
	data_array[1]=0x1A1500E0;
	data_array[2]=0x263F3832;
	data_array[3]=0x0D0B0740;
	data_array[4]=0x13110E17;
	data_array[5]=0x13081311;
	data_array[6]=0x15001815;
	data_array[7]=0x3F38321A;
	data_array[8]=0x0B074026;
	data_array[9]=0x110E170D;
	data_array[10]=0x08131113;
	data_array[11]=0x00181513;
	dsi_set_cmdq(data_array, 12, 1);
	MDELAY(1);


	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(150);

	data_array[0] = 0x00290500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);
}

static void lcm_suspend(void)
{
	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	unsigned int data_array[16];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(150);

	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(20);

	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(120);
}


static void lcm_resume(void)
{
#ifndef BUILD_LK
	unsigned int data_array[16];
	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
#endif
}

static unsigned int lcm_compare_id(void)
{
 unsigned int id=0;
  unsigned char buffer[2];
  unsigned int array[16];  

  SET_RESET_PIN(1);
  MDELAY(1);
  SET_RESET_PIN(0);
  MDELAY(1);
  SET_RESET_PIN(1);
  MDELAY(120);//Must over 6 ms

  array[0]=0x00043902;
  array[1]=0x9483FFB9;// page enable
  dsi_set_cmdq(array, 2, 1);
  MDELAY(10);

  array[0]=0x00033902; 
  array[1]=0x008373BA;// page enable //9341
  //array[2]=0x1800A416; 
  dsi_set_cmdq(array, 2, 1); 
  MDELAY(10); 

  array[0] = 0x00023700;// return byte number
  dsi_set_cmdq(array, 1, 1);
  MDELAY(10);

  read_reg_v2(0xdc, buffer, 2);
  id = buffer[0]; 

#ifdef BUILD_LK
  printf("[HX8394D]%s,  id = 0x%x\n", __func__, id);
#else
  printk("[HX8394D]%s,  id = 0x%x\n", __func__, id);
#endif

  return (LCM_ID == id)?1:0;

}

static unsigned int lcm_esd_check(void)
{
	int temp0=0,temp1=0,temp2=0,temp3=0;

#ifndef BUILD_LK
	char  buffer[3];
	int   array[4];

	//printk("xcz enter lcm_esd_check---\n");

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0a, buffer, 1);
        temp0 = buffer[0];
        read_reg_v2(0x09, buffer, 3);
        temp1 = buffer[0];//0x80
        temp2 = buffer[1];//0x73

	//printk("xcz enter lcm_esd_check---, temp0 = %x, temp1 = %x, temp2 = %x,\n", temp0, temp1, temp2);
	if((0x1c == temp0)&&(0x80 == temp1)&&(0x73 == temp2))
	{
		//printk("%s %d\n FALSE", __func__, __LINE__);
		return FALSE;
	}
	else
	{		 
		return TRUE;
	}
#endif
}

static unsigned int lcm_esd_recover(void)
{
#ifndef BUILD_LK
	//printk("xcz enter lcm_esd_recover---\n");
#endif

	lcm_init();	
	return TRUE;
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8394d_cmi50_blj_hd_lcm_drv= {
	.name = "hx8394d_cmi50_blj_hd",
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
