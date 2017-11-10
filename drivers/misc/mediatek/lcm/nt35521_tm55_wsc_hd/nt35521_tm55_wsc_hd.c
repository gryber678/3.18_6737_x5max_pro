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
#include <platform/mt_pmic.h>
#else
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

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#define LCM_ID			(0x21)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};
#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0x100   // END OF REGISTERS MARKER
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
{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
{0xff,6,{0x00,0x00,0x00,0x00,0x00,0x00}},
{0x6f,4,{0x00,0x00,0x00,0x00}},
{0xf7,2,{0x00,0x00}},
{0xbd,6,{0x01,0xa0,0x0c,0x08,0x01,0x00}},
{0x6f,2,{0x00,0x00}},
{0xb8,2,{0x01,0x02}},
{0xbb,4,{0x11,0x11,0x00,0x00}},
{0xbc,4,{0x00,0x00,0x00,0x00}},
{0xb6,2,{0x06,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xb0,4,{0x09,0x09,0x00,0x00}},
{0xb1,4,{0x09,0x09,0x00,0x00}},
{0xbc,4,{0x78,0x00,0x00,0x00}},
{0xbd,4,{0x78,0x00,0x00,0x00}},
{0xca,2,{0x00,0x00}},
{0xc0,2,{0x04,0x00}},
{0xb5,4,{0x03,0x03,0x00,0x00}},
{0xbe,2,{0x60,0x00}},
{0xb3,4,{0x28,0x28,0x00,0x00}},
{0xb4,4,{0x0f,0x0f,0x00,0x00}},
{0xb9,4,{0x34,0x34,0x00,0x00}},
{0xba,4,{0x17,0x60,0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},
{0xee,2,{0x01,0x00}},
{0xB0,16,{0x00,0x00,0x00,0x26,0x00,0x4E,0x00,0x6A,0x00,0x80,0x00,0xA4,0x00,0xBF,0x00,0xEA}},
{0xB1,16,{0x01,0x14,0x01,0x55,0x01,0x85,0x01,0xD5,0x02,0x15,0x02,0x17,0x02,0x57,0x02,0x97}},
{0xB2,16,{0x02,0xC6,0x03,0x01,0x03,0x27,0x03,0x56,0x03,0x76,0x03,0x9F,0x03,0xB8,0x03,0xD5}},
{0xB3,4,{0x03,0xF9,0x03,0xFF}},
{0x6f,2,{0x00,0x00}},
{0xf7,2,{0x00,0x00}},
{0x6f,2,{0x00,0x00}},
{0xf7,2,{0x00,0x00}},
{0x6f,2,{0x00,0x00}},
{0xf4,2,{0x00,0x00}},
{0x6f,2,{0x00,0x00}},
{0xf3,2,{0x00,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x06}},
{0xb0,4,{0x12,0x10,0x00,0x00}},
{0xb1,4,{0x18,0x16,0x00,0x00}},
{0xb2,4,{0x00,0x02,0x00,0x00}},
{0xb3,4,{0x31,0x31,0x00,0x00}},
{0xb4,4,{0x31,0x31,0x00,0x00}},
{0xb5,4,{0x31,0x31,0x00,0x00}},
{0xb6,4,{0x31,0x31,0x00,0x00}},
{0xb7,4,{0x31,0x31,0x00,0x00}},
{0xb8,4,{0x31,0x08,0x00,0x00}},
{0xb9,4,{0x2e,0x2d,0x00,0x00}},
{0xba,4,{0x2d,0x2e,0x00,0x00}},
{0xbb,4,{0x09,0x31,0x00,0x00}},
{0xbc,4,{0x31,0x31,0x00,0x00}},
{0xbd,4,{0x31,0x31,0x00,0x00}},
{0xbe,4,{0x31,0x31,0x00,0x00}},
{0xbf,4,{0x31,0x31,0x00,0x00}},
{0xc0,4,{0x31,0x31,0x00,0x00}},
{0xc1,4,{0x03,0x01,0x00,0x00}},
{0xc2,4,{0x17,0x19,0x00,0x00}},
{0xc3,4,{0x11,0x13,0x00,0x00}},
{0xe5,4,{0x31,0x31,0x00,0x00}},
{0xc4,4,{0x17,0x19,0x00,0x00}},
{0xc5,4,{0x11,0x13,0x00,0x00}},
{0xc6,4,{0x03,0x01,0x00,0x00}},
{0xc7,4,{0x31,0x31,0x00,0x00}},
{0xc8,4,{0x31,0x31,0x00,0x00}},
{0xc9,4,{0x31,0x31,0x00,0x00}},
{0xca,4,{0x31,0x31,0x00,0x00}},
{0xcb,4,{0x31,0x31,0x00,0x00}},
{0xcc,4,{0x31,0x09,0x00,0x00}},
{0xcd,4,{0x2d,0x2e,0x00,0x00}},
{0xce,4,{0x2e,0x2d,0x00,0x00}},
{0xcf,4,{0x08,0x31,0x00,0x00}},
{0xd0,4,{0x31,0x31,0x00,0x00}},
{0xd1,4,{0x31,0x31,0x00,0x00}},
{0xd2,4,{0x31,0x31,0x00,0x00}},
{0xd3,4,{0x31,0x31,0x00,0x00}},
{0xd4,4,{0x31,0x31,0x00,0x00}},
{0xd5,4,{0x00,0x02,0x00,0x00}},
{0xd6,4,{0x12,0x10,0x00,0x00}},
{0xd7,4,{0x18,0x16,0x00,0x00}},
{0xe6,4,{0x31,0x31,0x00,0x00}},
{0xd8,6,{0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd9,6,{0x00,0x00,0x00,0x00,0x00,0x00}},
{0xe7,2,{0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xed,2,{0x30,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xb1,4,{0x20,0x00,0x00,0x00}},
{0xb0,4,{0x20,0x00,0x00,0x00}},



{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
    //{0xf0,6,{0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xe5,2,{0x06,0x00}},
    //{0xf0,6,{0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xb0,4,{0x17,0x06,0x00,0x00}},
{0xb8,2,{0x00,0x00}},
{0xbd,6,{0x03,0x03,0x01,0x00,0x03,0x00}},
{0xb1,4,{0x17,0x06,0x00,0x00}},
{0xb9,4,{0x00,0x00,0x00,0x00}},
{0xb2,4,{0x17,0x06,0x00,0x00}},
{0xba,4,{0x00,0x00,0x00,0x00}},
{0xb3,4,{0x17,0x06,0x00,0x00}},
{0xbb,4,{0x00,0x00,0x00,0x00}},
{0xb4,4,{0x17,0x06,0x00,0x00}},
{0xb5,4,{0x17,0x06,0x00,0x00}},
{0xb6,4,{0x17,0x06,0x00,0x00}},
{0xb7,4,{0x17,0x06,0x00,0x00}},
{0xbc,4,{0x00,0x00,0x00,0x00}},
{0xe5,2,{0x06,0x00}},
{0xe6,2,{0x06,0x00}},
{0xe7,2,{0x06,0x00}},
{0xe8,2,{0x06,0x00}},
{0xe9,2,{0x06,0x00}},
{0xea,2,{0x06,0x00}},
{0xeb,2,{0x06,0x00}},
{0xec,2,{0x06,0x00}},
    //{0xf0,6,{0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xc0,2,{0x0b,0x00}},
{0xc1,2,{0x09,0x00}},
{0xc2,2,{0x0b,0x00}},
{0xc3,2,{0x09,0x00}},


{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
    //{0xf0,0x55,0xaa,0x52,0x08,0x03,0x00}},
{0xb2,6,{0x05,0x00,0x00,0x00,0x90,0x00}},
{0xb3,6,{0x05,0x00,0x00,0x00,0x90,0x00}},
{0xb4,6,{0x05,0x00,0x00,0x00,0x90,0x00}},
{0xb5,6,{0x05,0x00,0x00,0x00,0x90,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
    //{0xf0,0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xc4,2,{0x10,0x00}},
{0xc5,2,{0x10,0x00}},
{0xc6,2,{0x10,0x00}},
{0xc7,2,{0x10,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
    //{0xf0,0x55,0xaa,0x52,0x08,0x03,0x00}},
{0xb6,6,{0x05,0x00,0x00,0x00,0x90,0x00}},
{0xb7,6,{0x05,0x00,0x00,0x00,0x90,0x00}},
{0xb8,6,{0x05,0x00,0x00,0x00,0x90,0x00}},
{0xb9,6,{0x05,0x00,0x00,0x00,0x90,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
    //{0xf0,0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xc8,4,{0x08,0x20,0x00,0x00}},
{0xc9,4,{0x04,0x20,0x00,0x00}},
{0xca,4,{0x07,0x00,0x00,0x00}},
{0xcb,4,{0x00,0x00,0x00,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
    //{0xf0,0x55,0xaa,0x52,0x08,0x03,0x00}},
{0xba,6,{0x53,0x01,0x00,0x01,0x00,0x00}},
{0xbb,6,{0x53,0x01,0x00,0x01,0x00,0x00}},
{0xbc,6,{0x53,0x01,0x00,0x01,0x00,0x00}},
{0xbd,6,{0x53,0x01,0x00,0x01,0x00,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
    //{0xf0,0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xd1,6,{0x00,0x05,0x00,0x07,0x10,0x00}},
{0xd2,6,{0x00,0x05,0x04,0x07,0x10,0x00}},
{0xd3,6,{0x00,0x00,0x0a,0x07,0x10,0x00}},
{0xd4,6,{0x00,0x00,0x0a,0x07,0x10,0x00}},
    //{0xf0,6,{0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xd0,8,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd5,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd6,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd7,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd8,6,{0x00,0x00,0x00,0x00,0x00,0x00}},
    //{0xf0,6,{0x55,0xaa,0x52,0x08,0x05,0x00}},
{0xcc,4,{0x00,0x00,0x01,0x00}},
{0xcd,4,{0x00,0x00,0x01,0x00}},
{0xce,4,{0x00,0x00,0x02,0x00}},
{0xcf,4,{0x00,0x00,0x02,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xc0,6,{0x00,0x34,0x00,0x00,0x00,0x00}},
{0xc1,6,{0x00,0x00,0x34,0x00,0x00,0x00}},
{0xc2,6,{0x00,0x00,0x34,0x00,0x00,0x00}},
{0xc3,6,{0x00,0x00,0x34,0x00,0x00,0x00}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xc4,2,{0x60,0x00}},
{0xc5,2,{0x40,0x00}},
{0xc6,2,{0x00,0x00}},
{0xc7,2,{0x00,0x00}},
{0x6f,2,{0x00,0x00}},
{0xf9,2,{0x00,0x00 }},

{0xff,4,{0xaa,0x55,0x25,0x01}},
{0x6f,1,{0x16}},
{0xf7,1,{0x10}},

    {0x11, 1 , {0x00}},
    {REGFLAG_DELAY, 120, {}},

    {0x29, 1 , {0x00}},
    {REGFLAG_DELAY, 300, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}},
};
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
	
};


static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},

    // Sleep Mode On
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 200, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	{0xF0,	5,	{0x55, 0xaa, 0x52,0x08,0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


//OTM
/*static unsigned int var = 0x58;
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
	    					
	    					case 0xd9://  vcom 
                table[i].para_list[0] = var ;
                dsi_set_cmdq_V2(0xd9, table[i].count, table[i].para_list, force_update);
                var = var + 2;
                break;
                
                	
            default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
			// MDELAY(2);
       	}
    }
	
}*/


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
				//MDELAY(2);
       	}
    }

}




// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type = LCM_TYPE_DSI;

	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	// enable tearing-free
	params->dbi.te_mode = LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity = LCM_POLARITY_RISING;

	params->dsi.mode   = SYNC_PULSE_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_THREE_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 720 * 3;

    params->dsi.vertical_sync_active				= 4;// 3    2
    params->dsi.vertical_backporch					=16;// 20   1
    params->dsi.vertical_frontporch					= 20; // 1  12
    params->dsi.vertical_active_line				= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active			=20;// 50  2
    params->dsi.horizontal_backporch				= 64 ;
    params->dsi.horizontal_frontporch				= 64 ;
    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

	// Bit rate calculation
	//1 Every lane speed
    //	params->dsi.pll_div1 = 0;	// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
    //	params->dsi.pll_div2 = 1;	// div2=0,1,2,3;div1_real=1,2,4,4
    //	params->dsi.fbk_div = 14;	//12;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
    params->dsi.PLL_CLOCK =280;

    params->dsi.cont_clock = 0;
    params->dsi.esd_check_enable = 1;
    params->dsi.noncont_clock = 1;
    params->dsi.customization_esd_check_enable = 1;
    params->dsi.clk_lp_per_line_enable =1 ;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
}

static void lcm_init(void)
{
/*#if defined(BUILD_LK)
	upmu_set_rg_vgp5_vosel(6);
	upmu_set_rg_vgp5_en(1);
#else
	hwPowerOn(MT65XX_POWER_LDO_VGP5, VOL_3000, "Lance_LCM");
#endif*/

	//MDELAY(5);

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static unsigned int lcm_compare_id(void);
//static unsigned int lcm_compare_id(void);
static void lcm_suspend(void)
{
    SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(20);
//#ifndef BUILD_LK
//printk("tengqi---lcmid %x\n",lcm_compare_id());
//#endif
	push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
//   lcm_compare_id();
/*#if defined(BUILD_LK)
	upmu_set_rg_vgp5_en(0);
#else
	hwPowerDown(MT65XX_POWER_LDO_VGP5, "Lance_LCM");
#endif*/
}

//static unsigned int VcomH = 0x40;
static void lcm_resume(void)
{
     // unsigned int data_array[16];
	lcm_init();
//	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
   /*
      data_array[0]= 0x00023902;
      data_array[1]= 0x00000000;
      dsi_set_cmdq(data_array, 2, 1);

      data_array[0]= 0x00023902;
      data_array[1]= (0x00<<24)|(0x00<<16)|(VcomH<<8)|0xD9;
      dsi_set_cmdq(data_array, 2, 1);
      VcomH = VcomH+2;
*/
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
    dsi_set_cmdq(&data_array, 3, 1); 

    array[0] = 0x00033700;// read id return two byte,version and id
    dsi_set_cmdq(array, 1, 1); 

    read_reg_v2(0xC5, buffer, 3); 
    id = buffer[1]; //we only need ID
    #ifdef BUILD_LK
    printf("%s, nt35521 id = 0x%08x buffer[0]=0x%08x,buffer[1]=0x%08x,buffer[2]=0x%08x\n",
           __func__, id,buffer[0],buffer[1],buffer[2]);
    #else
    printk("%s, nt35521 id = 0x%08x buffer[0]=0x%08x,buffer[1]=0x%08x,buffer[2]=0x%08x\n",
           __func__, id,buffer[0],buffer[1],buffer[2]);
    #endif

   if(id == LCM_ID)
        return 1;
    else
        return 0;

}

static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
	char  buffer[3];
	int   array[4];

	if(lcm_esd_test)
	{
		lcm_esd_test = FALSE;
		return TRUE;
	}

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x36, buffer, 1);
	if(buffer[0]==0x90)
	{
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
	lcm_init();
	lcm_resume();

	return TRUE;
}
LCM_DRIVER nt35521_tm55_wsc_hd_lcm_drv =
{
	.name = "nt35521_tm55_wsc_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
	.compare_id = lcm_compare_id,
    .init_power     = lcm_init_power,
    .resume_power   = lcm_resume_power,
    .suspend_power  = lcm_suspend_power,
};

