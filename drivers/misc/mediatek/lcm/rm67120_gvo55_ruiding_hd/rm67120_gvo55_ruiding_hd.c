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
#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h> 
#include <platform/mt_pmic.h>
#include <string.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mt-plat/mt_gpio.h>
#include <mach/gpio_const.h>
#endif

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

#define FRAME_WIDTH                                         (720)
#define FRAME_HEIGHT                                        (1280)
#define LCM_ID                       						        (0x0080)

#define GPIO_LCD_BIAS_ENP_PIN GPIO55 | 0x80000000//GPIO_LCD_BIAS_ENP_PIN


#define LCM_DSI_CMD_MODE                                    0

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

#define SET_RESET_PIN(v)                                    (lcm_util.set_reset_pin((v)))

#define UDELAY(n)                                           (lcm_util.udelay(n))
#define MDELAY(n)                                           (lcm_util.mdelay(n))

#define REGFLAG_DELAY             							0XAA
#define REGFLAG_END_OF_TABLE      							0x100   // END OF REGISTERS MARKER

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                                           lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
  unsigned cmd;
  unsigned char count;
  unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
  {0xf0,5,{0x55,0xaa,0x52,0x08,0x00}},

  {0xc0,8,{0xc7,0x00,0x00,0x00,0x1e,0x10,0x60,0xe5}},

  {0xc1,8,{0xc0,0x01,0x00,0x00,0x1d,0x00,0xf0,0xc8}},

  {0xc2,8,{0xc0,0x02,0x00,0x00,0x1d,0x2a,0xa0,0x9f}},

  {0xc3,8,{0xc0,0x02,0x00,0x00,0x1e,0x2a,0xa0,0x9f}},

  {0xc4,8,{0xc0,0x02,0x00,0x00,0x1d,0x10,0x80,0xb8}},

  {0xc5,8,{0xc0,0x02,0x00,0x00,0x1e,0x10,0xa0,0xb8}},

  {0xc6,8,{0xc7,0x00,0x02,0x00,0x1e,0x10,0xa0,0xec}},

  {0xc7,8,{0xc7,0x00,0x00,0x00,0x1f,0x10,0x60,0xe5}},

  {0xc8,1,{0xff}},

  {0xb0,5,{0x00,0x08,0x0c,0x14,0x14}},

  {0xba,1,{0x20}},

  {0xbb,7,{0x55,0x55,0x55,0x55,0x55,0x55,0x55}},

  {0xf0,5,{0x55,0xaa,0x52,0x08,0x02}},

  {0xe1,1,{0x00}},

  {0xca,1,{0x04}},

  {0xe2,1,{0x0a}},

  {0xe3,1,{0x00}},

  {0xe7,1,{0x00}},

  {0xed,8,{0x48,0x00,0xe0,0x13,0x08,0x00,0x92,0x08}},

  {0xfd,6,{0x00,0x08,0x1c,0x00,0x00,0x01}},

  {0xc3,11,{0x11,0x24,0x04,0x0a,0x01,0x04,0x00,0x1c,0x10,0xf0,0x00}},

  {0xea,5,{0x7f,0x20,0x00,0x00,0x00}},

  {0xf0,5,{0x55,0xaa,0x52,0x08,0x01}},

  {0xb0,3,{0x01,0x01,0x01}},

  {0xb1,3,{0x05,0x05,0x05}},

  {0xb2,3,{0xd0,0xd0,0xd0}},

  {0xb4,3,{0x37,0x37,0x37}},

  {0xb5,3,{0x05,0x05,0x05}},

  {0xb6,3,{0x54,0x54,0x54}},

  {0xb7,3,{0x24,0x24,0x24}},

  {0xb8,3,{0x24,0x24,0x24}},

  {0xb9,3,{0x14,0x14,0x14}},

  {0xba,3,{0x14,0x14,0x14}},

  {0xbc,3,{0x00,0xf8,0xb2}},

  {0xbe,3,{0x23,0x00,0x90}},

  {0xca,1,{0x80}},

  {0xcb,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

  {0xcc,12,{0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19}},

  {0xf0,5,{0x55,0xaa,0x52,0x08,0x03}},

  {0xf1,6,{0x10,0x00,0x00,0x00,0x01,0x30}},

  {0xf6,1,{0x0a}},

  {0xf0,5,{0x55,0xaa,0x52,0x08,0x05}},

  {0xc0,7,{0x06,0x02,0x02,0x22,0x00,0x00,0x01}},

  {0x35,1,{0x00}},
  {0x36, 1, {0x02}},
  //{0xF0,	5,	{0x55,0xAA,0x52,0x08,0x01}}, 
  // Setting ending by predefined flag

  // Sleep Out
  {0x11, 1, {0x00}},
  {REGFLAG_DELAY, 120,  {0}},      
  // Display ON
  //{0x2C, 1, {0x00}},
  //{0x13, 1, {0x00}},
  {0x29, 1, {0x00}},
  {REGFLAG_DELAY, 30,  {0}},
  {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
  // Display off sequence
  {0x28, 0, {0x00}},
  {REGFLAG_DELAY, 50, {}},

  // Sleep Mode On
  {0x10, 0, {0x00}},
  {REGFLAG_DELAY, 120, {}},

  {REGFLAG_END_OF_TABLE, 0x00, {}}
};

// Display off sequence

static struct LCM_setting_table lcm_backlight_level_setting[] = {
  {0x51, 1, {0xFF}},
  {REGFLAG_END_OF_TABLE, 0x00, {}}
};


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
  params->type   = LCM_TYPE_DSI;
  params->width  = FRAME_WIDTH;
  params->height = FRAME_HEIGHT;

  // enable tearing-free
  params->dbi.te_mode             = LCM_DBI_TE_MODE_DISABLED;
  params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;



  params->dsi.mode   = SYNC_PULSE_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;


  // DSI
  /* Command mode setting */
  params->dsi.LANE_NUM                = LCM_FOUR_LANE;
  //The following defined the fomat for data coming from LCD engine.
  params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
  params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
  params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
  params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
  // Highly depends on LCD driver capability.
  // Not support in MT6573
  // Video mode setting       
    params->dsi.intermediat_buffer_num = 2;
	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	
	params->dsi.word_count=FRAME_WIDTH*3;	//DSI CMD mode need set these two bellow params, different to 6577
	//params->dsi.vertical_active_line=FRAME_HEIGHT;
	params->dsi.vertical_sync_active				= 10;
	params->dsi.vertical_backporch					= 20;
	params->dsi.vertical_frontporch				        = 20;
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active				= 8;
	params->dsi.horizontal_backporch				= 20;
	params->dsi.horizontal_frontporch				= 20;
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
  /* Bit rate calculation

     params->dsi.PLL_CLOCK=220;//////////////////////////////////////

     params->dsi.noncont_clock=1;
     params->dsi.noncont_clock_period=1;

     params->dsi.esd_check_enable = 1;
     params->dsi.customization_esd_check_enable = 1;
     params->dsi.lcm_esd_check_table[0].cmd = 0x0a;
     params->dsi.lcm_esd_check_table[0].count = 1;
     params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c; 
   */
	params->dsi.PLL_CLOCK=220;
}
static void lcm_init_power(void)
{
  mt_set_gpio_mode(GPIO_LCD_BIAS_ENP_PIN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_LCD_BIAS_ENP_PIN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
}
static void lcm_resume_power(void)
{
  mt_set_gpio_mode(GPIO_LCD_BIAS_ENP_PIN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_LCD_BIAS_ENP_PIN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
}
static void lcm_suspend_power(void)
{
  mt_set_gpio_mode(GPIO_LCD_BIAS_ENP_PIN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_LCD_BIAS_ENP_PIN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
}
static void lcm_init(void)
{

  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);




  SET_RESET_PIN(1);

  MDELAY(60);
  push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{

  push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1); 

  //    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
  //   lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);

  //    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ZERO);  //shm

  MDELAY(120);  


}


static void lcm_resume(void)
{
  lcm_init();
  //push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}















static unsigned int lcm_compare_id(void)
{
		int array[4];
		char buffer[5];
		char id_high=0;
		char id_low=0;
		int id1=0;


		SET_RESET_PIN(1);
		MDELAY(10);
		SET_RESET_PIN(0);
		MDELAY(10);
		SET_RESET_PIN(1);
		MDELAY(200);
		
		array[0] = 0x00023700;
		dsi_set_cmdq(array, 1, 1);

		read_reg_v2(0x04, buffer, 2);
		id_high = buffer[0];
		id_low = buffer[1];
		id1 = (id_high<<8) | id_low;

		#if defined(BUILD_LK)
		printf("hct_rm67120_dsi_vdo_hd_gvo %s id1 = 0x%04x(0x%04x,0x%04x)\n", __func__, id1, id_high, id_low);
		#else
		printk("hct_rm67120_dsi_vdo_hd_gvo %s id1 = 0x%04x(0x%04x,0x%04x)\n", __func__, id1, id_high, id_low);
		#endif
		return (LCM_ID == id1)?1:0;

}

static void lcm_setbacklight(unsigned int level)
{
  //	printk("HCT_lcm_setbacklight level = %d\n", level);

  unsigned int default_level = 50;
  unsigned int mapped_level = 0;

  //for LGE backlight IC mapping table
  if(level > 255)
    level = 255;

  if(level >0)
    mapped_level = default_level+(level)*(255-default_level)/(255);
  else
    mapped_level=0;

  // Refresh value of backlight level.
  lcm_backlight_level_setting[0].para_list[0] = mapped_level;

  push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
  /*	if(level<50)
        {
        level=50;
        }
        lcm_backlight_level_setting[0].para_list[0] = level;
        push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1); 	
   */
}
// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER rm67120_gvo55_ruiding_hd_lcm_drv =
{
  .name            = "rm67120_gvo55_ruiding_hd",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params     = lcm_get_params,
  .init           = lcm_init,
  .suspend        = lcm_suspend,
  .resume         = lcm_resume,   
  .compare_id    = lcm_compare_id,    
  .init_power        = lcm_init_power,
  .resume_power = lcm_resume_power,
  .set_backlight    = lcm_setbacklight,
  .suspend_power = lcm_suspend_power,
};


