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

#if defined(BUILD_LK)
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#endif

#if !defined(BUILD_LK)
#include <linux/string.h>
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

#define FRAME_WIDTH  			(720)
#define FRAME_HEIGHT 			(1280)
#define LCM_ID_NT35521			(0x21)

#define REGFLAG_DELAY          		(0XFEE)
#define REGFLAG_END_OF_TABLE      	(0xFFF)	// END OF REGISTERS MARKER

#ifndef FALSE
#define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = { 0 };

#define SET_RESET_PIN(v)    		(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 					(lcm_util.udelay(n))
#define MDELAY(n) 					(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


struct LCM_setting_table {
	unsigned cmd;
	unsigned char count;
	unsigned char para_list[64];
};
struct LCM_setting_table2 {
	unsigned cmd;
	unsigned char count;
	unsigned char para_list[64];
};
static void lcm_init_power(void)
{
}


static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
}
static struct LCM_setting_table lcm_initialization_setting[] = {

//NT35521S-CMO5.5-RIXIN-20151202
	{0xFF,4,{0xAA,0x55,0xA5,0x80}}, 
	 
	{0x6F,2,{0x11,0x00}}, 
	{0xF7,2,{0x20,0x00}}, 
	 
	{0x6F,1,{0x06}}, 
	{0xF7,1,{0xA0}}, 
	{0x6F,1,{0x19}}, 
	{0xF7,1,{0x12}}, 
	 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}}, 
	{0xC8,1,{0x80}}, 
	 
	{0xB1,2,{0x6C,0x21}}, 	 
	{0xB6,1,{0x0F}}, 
	{0x6F,1,{0x02}}, 
	{0xB8,1,{0x08}}, 
	 
	{0xBB,2,{0x74,0x44}}, 
	 
	{0xBC,2,{0x00,0x00}}, 
	 
	{0xBD,5,{0x02,0xB0,0x1e,0x1e,0x00}},     
	 
	 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}}, 
	 
	{0xB0,2,{0x05,0x05}}, 
	{0xB1,2,{0x05,0x05}}, 
	 
	{0xBC,2,{0x70,0x00}}, 
	{0xBD,2,{0x70,0x00}}, 
	 
	{0xCA,1,{0x00}}, 
	 
	{0xC0,1,{0x04}}, 
	 
	{0xBE,1,{0x62}}, //46    
	 
	{0xB9,2,{0x46,0x46}}, 
	{0xB3,2,{0x2D,0x2D}}, 
	 
	{0xBA,2,{0x25,0x25}}, 
	{0xB4,2,{0x19,0x19}}, 
	 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}}, 
	 
	{0xEE,1,{0x02}}, 
	 
	{0xB0,16,{0x00,0x6E,0x00,0x82,0x00,0xA6,0x00,0xBD,0x00,0xCA,0x00,0xE9,0x01,0x05,0x01,0x31}}, 
	{0xB1,16,{0x01,0x53,0x01,0x8A,0x01,0xB2,0x01,0xF7,0x02,0x31,0x02,0x33,0x02,0x67,0x02,0x9C}}, 
	{0xB2,16,{0x02,0xBC,0x02,0xEC,0x03,0x0B,0x03,0x32,0x03,0x4B,0x03,0x6E,0x03,0x84,0x03,0xA0}}, 
	{0xB3,4,{0x03,0xBF,0x03,0xD3}}, 
	 
	{0xBC,16,{0x00,0x15,0x00,0x26,0x00,0x45,0x00,0x63,0x00,0x83,0x00,0xAF,0x00,0xD1,0x01,0x02}}, 
	{0xBD,16,{0x01,0x2C,0x01,0x70,0x01,0xA0,0x01,0xEE,0x02,0x27,0x02,0x29,0x02,0x5C,0x02,0x9B}}, 
	{0xBE,16,{0x02,0xC0,0x02,0xF4,0x03,0x16,0x03,0x40,0x03,0x5B,0x03,0x80,0x03,0x96,0x03,0xAF}}, 
	{0xBF,4,{0x03,0xC5,0x03,0xD3}}, 
	 
	 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x06}}, 
	 
	{0xB0,2,{0x29,0x2A}}, 
	{0xB1,2,{0x10,0x12}}, 
	{0xB2,2,{0x14,0x16}}, 
	{0xB3,2,{0x18,0x1A}}, 
	{0xB4,2,{0x08,0x0A}}, 
	{0xB5,2,{0x2E,0x2E}}, 
	{0xB6,2,{0x2E,0x2E}}, 
	{0xB7,2,{0x2E,0x2E}}, 
	{0xB8,2,{0x2E,0x00}}, 
	{0xB9,2,{0x2E,0x2E}}, 
	 
	{0xBA,2,{0x2E,0x2E}}, 
	{0xBB,2,{0x01,0x2E}}, 
	{0xBC,2,{0x2E,0x2E}}, 
	{0xBD,2,{0x2E,0x2E}}, 
	{0xBE,2,{0x2E,0x2E}}, 
	{0xBF,2,{0x0B,0x09}}, 
	{0xC0,2,{0x1B,0x19}}, 
	{0xC1,2,{0x17,0x15}}, 
	{0xC2,2,{0x13,0x11}}, 
	{0xC3,2,{0x2A,0x29}}, 
	{0xE5,2,{0x2E,0x2E}}, 
	 
	{0xC4,2,{0x29,0x2A}}, 
	{0xC5,2,{0x1B,0x19}}, 
	{0xC6,2,{0x17,0x15}}, 
	{0xC7,2,{0x13,0x11}}, 
	{0xC8,2,{0x01,0x0B}}, 
	{0xC9,2,{0x2E,0x2E}}, 
	{0xCA,2,{0x2E,0x2E}}, 
	{0xCB,2,{0x2E,0x2E}}, 
	{0xCC,2,{0x2E,0x09}}, 
	{0xCD,2,{0x2E,0x2E}}, 
	 
	{0xCE,2,{0x2E,0x2E}}, 
	{0xCF,2,{0x08,0x2E}}, 
	{0xD0,2,{0x2E,0x2E}}, 
	{0xD1,2,{0x2E,0x2E}}, 
	{0xD2,2,{0x2E,0x2E}}, 
	{0xD3,2,{0x0A,0x00}}, 
	{0xD4,2,{0x10,0x12}}, 
	{0xD5,2,{0x14,0x16}}, 
	{0xD6,2,{0x18,0x1A}}, 
	{0xD7,2,{0x2A,0x29}}, 
	 
	{0xE6,2,{0x2E,0x2E}}, 
	     
	{0xD8,5,{0x00,0x00,0x00,0x00,0x00}}, 
	{0xD9,5,{0x00,0x00,0x00,0x00,0x00}}, 
	     
	{0xE7,1,{0x00}}, 
	 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}}, 
	 
	{0xB0,2,{0x00,0x00}}, 
	{0xB1,2,{0x00,0x00}}, 
	{0xB2,5,{0x05,0x00,0x00,0x00,0x00}}, 
	{0xB6,5,{0x05,0x00,0x00,0x00,0x00}}, 
	{0xB7,5,{0x05,0x00,0x00,0x00,0x00}}, 
	{0xBA,5,{0x57,0x00,0x00,0x00,0x00}}, 
	{0xBB,5,{0x57,0x00,0x00,0x00,0x00}}, 
	{0xC0,4,{0x00,0x00,0x00,0x00}}, 
	{0xC1,4,{0x00,0x00,0x00,0x00}}, 
	{0xC4,1,{0x60}}, 
	{0xC5,1,{0x40}}, 
	 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}}, 
	 
	{0xBD,5,{0x03,0x01,0x03,0x03,0x03}}, 
	{0xB0,2,{0x17,0x06}}, 
	{0xB1,2,{0x17,0x06}}, 
	{0xB2,2,{0x17,0x06}}, 
	{0xB3,2,{0x17,0x06}}, 
	{0xB4,2,{0x17,0x06}}, 
	{0xB5,2,{0x17,0x06}}, 
	{0xB8,1,{0x00}}, 
	{0xB9,1,{0x00}}, 
	{0xBA,1,{0x00}}, 
	{0xBB,1,{0x02}}, 
	{0xBC,1,{0x00}}, 
	 
	{0xC0,1,{0x07}}, 
	{0xC4,1,{0x80}}, 
	{0xC5,1,{0xA4}}, 
	{0xC8,2,{0x05,0x30}}, 
	{0xC9,2,{0x01,0x31}}, 
	{0xCC,3,{0x00,0x00,0x3C}}, 
	{0xCD,3,{0x00,0x00,0x3C}}, 
	 
	{0xD1,5,{0x00,0x05,0x09,0x07,0x10}}, 
	{0xD2,5,{0x00,0x05,0x0E,0x07,0x10}}, 
	 
	{0xE5,1,{0x06}}, 
	{0xE6,1,{0x06}}, 
	{0xE7,1,{0x06}}, 
	{0xE8,1,{0x06}}, 
	{0xE9,1,{0x06}}, 
	{0xEA,1,{0x06}}, 
	 
	{0xED,1,{0x30}}, 
	 
	{0x6F,1,{0x11}}, 
	     
	{0xF3,1,{0x01}},

	

		//**********************************************
		 {0xFF,4,{0xAA,0x55,0xA5,0x80}},//3 ///line NT35521 
		 {0x6F,1,{0x07}},
	        {0xF7,1,{0x50}},
	        
	        {0xFF,4,{0xAA,0x55,0x25,0x01}},//3 //line NT35521S
	        {0x6F,1,{0x16}},
	        {0xF7,1,{0x10}},
		//*************************************************
		{0x62,1,{0x01}}, 
		{0x11,	1,	{0x00}},
		{REGFLAG_DELAY, 150, {}},
		{0x29,	1,	{0x00}},
		{REGFLAG_DELAY, 20, {}},
		{REGFLAG_END_OF_TABLE, 0x00, {}}, 

};


/*
   static struct LCM_setting_table lcm_sleep_out_setting[] = {
// Sleep Out
{0x11, 0, {0x00}},
{REGFLAG_DELAY, 120, {}},

// Display ON
{0x29, 0, {0x00}},
{REGFLAG_DELAY, 20, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
// Display off sequence
{0x28, 0, {0x00}},
{REGFLAG_DELAY, 30, {}},
// Sleep Mode On
{0x10, 0, {0x00}},
{REGFLAG_DELAY, 150, {}},

{REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/
static void push_table(struct LCM_setting_table *table, unsigned int count,
		unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {

		unsigned cmd;
		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY:
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V2(cmd, table[i].count,
					table[i].para_list, force_update);
		}

	}

}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS * util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
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

	params->dsi.mode = SYNC_PULSE_VDO_MODE;	//BURST_VDO_MODE;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_THREE_LANE;//LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 720 * 3;

	params->dsi.vertical_sync_active				= 0x3;// 3    2
	params->dsi.vertical_backporch					= 0x0E;// 20   1
	params->dsi.vertical_frontporch					= 0x10; // 1  12
	params->dsi.vertical_active_line				= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active				= 0x04;// 50  2
	params->dsi.horizontal_backporch				= 0x22 ;
	params->dsi.horizontal_frontporch				= 0x18 ;
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

	params->dsi.PLL_CLOCK =260;
	params->dsi.cont_clock =1; 
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.noncont_clock = 1;
	params->dsi.noncont_clock_period = 2;   // Unit : frames
	params->dsi.lcm_esd_check_table[0].cmd            =0x0A;
	params->dsi.lcm_esd_check_table[0].count          =1; 
	params->dsi.lcm_esd_check_table[0].para_list[0]   =0x9c;
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting,
			sizeof(lcm_initialization_setting) /
			sizeof(struct LCM_setting_table), 1);
}
static struct LCM_setting_table2 lcm_initialization_setting2[] = {

	{0x00,1,{0x00}}, 
	{0xff,3,{0x12,0x85,0x01}}, 
	{0x00,1,{0x80}}, 
	{0xff,2,{0x12,0x85}}, 
	{0x00,1,{0xB3}}, 
	{0xC0,1,{0x22}},/// 33colum 44 1+2dot 55
	{0x00,1,{0x00}}, //add {0xfb,1,{0x01}},
	{0x00,1,{0x00}},
	{0xff,3,{0xFF,0xFF,0xFF}}, 
	{0x22, 0, {0x00}}, 

	{REGFLAG_DELAY, 50, {}}
};

static void lcm_suspend(void)
{
	unsigned int data_array[16];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting2,
			sizeof(lcm_initialization_setting2) /
			sizeof(struct LCM_setting_table2), 1);

	data_array[0] = 0x00280500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);

	data_array[0] = 0x00100500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(150);
}

static void lcm_resume(void)
{

	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) /	sizeof(struct LCM_setting_table), 1);

}

extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);
static int adc_read_vol(void)
{
		int adc[1];
		int data[4] ={0,0,0,0};
		int sum = 0;
		int adc_vol=0;
		int num = 0;

		for(num=0;num<10;num++)
		{
		IMM_GetOneChannelValue(12, data, adc);
		sum+=(data[0]*100+data[1]);
		}
		adc_vol = sum/10;

#if defined(BUILD_LK)
printf("sunxing adc_vol is %d\n",adc_vol);
#else
printk("sunxing adc_vol is %d\n",adc_vol);
#endif

    return (adc_vol<30) ? 0 : 1;
	//return 0;
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id=0;
	unsigned char buffer[3];
	unsigned int array[16];
	unsigned int data_array[16];
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);

	SET_RESET_PIN(1);
	MDELAY(120);

	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000108;
	dsi_set_cmdq(data_array, 3, 1);

	array[0] = 0x00033700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xC5, buffer, 3);
	id = buffer[1]+adc_read_vol(); //we only need ID
#ifdef BUILD_LK
	printf("%s, LK nt35521 debug: nt35521 id = 0x%08x buffer[0]=0x%08x,buffer[1]=0x%08x,buffer[2]=0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2]);
#else
	printk("%s, LK nt35521 debug: nt35521 id = 0x%08x buffer[0]=0x%08x,buffer[1]=0x%08x,buffer[2]=0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2]);
#endif

		return (LCM_ID_NT35521 == id) ? 1 : 0;
}


LCM_DRIVER nt35521_cmi55_rixin_hd_lcm_drv= {
	.name = "nt35521_cmi55_rixin_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
	.init_power        = lcm_init_power,
	.resume_power = lcm_resume_power,
	.suspend_power = lcm_suspend_power,

};
