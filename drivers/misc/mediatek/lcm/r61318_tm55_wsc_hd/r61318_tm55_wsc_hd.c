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
#else

#include <linux/proc_fs.h> //proc file use
#endif



// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA   // END OF REGISTERS MARKER


#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif
//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#if defined(BUILD_LK)
#define MDELAY(n)   mdelay(n)  // (lcm_util.mdelay(n*20000))
#else
extern void msleep(unsigned int msecs);
#define MDELAY(n)   msleep(n)  // (lcm_util.mdelay(n*20000))
#endif
#define R61318_LCM_ID       (0x1318)

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
  unsigned cmd;
  unsigned char count;
  unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
{0xB0,1,{0x00}},
{0xE3,1,{0x01}},
{0xB6,1,{0x36}},//4L 32,3L 36
{0xC0,6,{0x23,0xB2,0x16,0x10,0x02,0x7F}},
{0xC1,7,{0x0A,0x8A,0x0A,0x8A,0x00,0x00,0x00}},
{0xC3,1,{0x30}},
{0xC4,3,{0xC3,0xDD,0x80}},
{0xC5,3,{0x0B,0x10,0x13}},
{0xC6,1,{0x29}},
{0xC8,28,{0x31,0x82,0x00,0xA4,0x98,0x83,0x92,0x5A,0xAC,0xB9,0x07,0xA3,0x9C,0x95,0xD2,0x3A,0x65,0x84,0xCF,0x35,0xB6,0x54,0x42,0xC7,0x14,0x02,0x44,0x18}},

{0xCA,30,{0x04,0x10,0x15,0x1B,0x23,0x27,0x2A,0x28,0x1B,0x1B,0x19,0x15,0x12,0x10,0x06,0x04,0x11,0x17,0x1C,0x23,0x28,0x2A,0x27,0x1C,0x1B,0x1A,0x16,0x13,0x10,0x06}},

{0xD0,3,{0x05,0x0E,0x43}},
{0xD1,1,{0x03}},

{0xD2,2,{0x81,0x3F}},

{0xE5,1,{0x02}},
{0xD3,2,{0x33,0x33}},
{0xD4,1,{0x52}},//VCOM
{0xD5,2,{0x30,0x30}},
{0xD6,1,{0x01}},

{0x11, 0,{0x00}},
{REGFLAG_DELAY, 120, {}},         
{0x29, 0,{0x00}},
{REGFLAG_DELAY, 20, {}},
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
  params->dbi.te_mode = LCM_DBI_TE_MODE_DISABLED;
  params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;


  params->dsi.mode = SYNC_EVENT_VDO_MODE;//BURST_VDO_MODE;

  // DSI
  /* Command mode setting */
  params->dsi.LANE_NUM				= LCM_THREE_LANE;

  //The following defined the fomat for data coming from LCD engine.
  params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
  params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
  params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
  params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;

  params->dsi.packet_size=256;
  // Video mode setting
  params->dsi.intermediat_buffer_num = 2;

  params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

  params->dsi.vertical_sync_active				= 2;//4
  params->dsi.vertical_backporch					= 19; //12
  params->dsi.vertical_frontporch					= 17; //20
  params->dsi.vertical_active_line				= FRAME_HEIGHT;

  params->dsi.horizontal_sync_active			    = 10;//20;
  params->dsi.horizontal_backporch				= 80;//46;
  params->dsi.horizontal_frontporch				= 80;//46;
  params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

  params->dsi.PLL_CLOCK = 270; //212 dpi clock customization: should config 

	params->dsi.ssc_disable = 1;

	params->dsi.noncont_clock = 1;
	params->dsi.noncont_clock_period = 2;

	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
	params->dsi.lcm_esd_check_table[0].count        = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;	
}

static void lcm_init(void)
{
/*  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);//1
  SET_RESET_PIN(1);
  MDELAY(120);//60

  push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
*/
	unsigned int data_array[16];

    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);

	
	data_array[0] = 0x00022902;
	data_array[1] = 0x000000b0;
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00022902;
	data_array[1] = 0x000001E3;	
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00022902;
	data_array[1] = 0x000036B6;	
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00072902;
	data_array[1] = 0x16B223C0; 
	data_array[2] = 0x007F0210;
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0] = 0x00082902;
	data_array[1] = 0x0A8A0AC1; 
	data_array[2] = 0x0000008A;
	dsi_set_cmdq(data_array, 3, 1); 

	data_array[0] = 0x00022902;
	data_array[1] = 0x000030C3;//30
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00042902; 	 
	data_array[1] = 0x80DDC3C4; 
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00042902; 	 
	data_array[1] = 0x13100BC5; 
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00022902; 	 
	data_array[1] = 0x000029C6; 
	dsi_set_cmdq(data_array, 2, 1);	
	
	data_array[0] = 0x001D2902;
	data_array[1] = 0x008231c8; 
	data_array[2] = 0x928398A4; 
	data_array[3] = 0x07B9AC5A; 
	data_array[4] = 0xD2959CA3; 
	data_array[5] = 0xCF84653A;
	data_array[6] = 0x4254B635; 
	data_array[7] = 0x440214C7; 
	data_array[8] = 0x00000018; 
	dsi_set_cmdq(data_array, 9, 1); 
	
	data_array[0] = 0x001F2902;
	data_array[1] = 0x151004CA;
	data_array[2] = 0x2A27231B;
	data_array[3] = 0x191B1B28;
	data_array[4] = 0x06101215;
	data_array[5] = 0x1C171104;
	data_array[6] = 0x272A2823;
	data_array[7] = 0x161A1B1C;
	data_array[8] = 0x00061013;
	dsi_set_cmdq(data_array, 9, 1);

	data_array[0] = 0x00022902; 	 
	data_array[1] = 0x000000CD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00042902; 	 
	data_array[1] = 0x430E05D0;
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00032902; 	 
	data_array[1] = 0x003F81D2;
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00032902; 	 
	data_array[1] = 0x003333D3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00022902;
	data_array[1] = 0x000003E5; 
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00022902;
	data_array[1] = 0x00004AD4;//vcom 
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00032902; 	 
	data_array[1] = 0x003030D5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00022902;
	data_array[1] = 0x000001D6; 
	dsi_set_cmdq(data_array, 2, 1);
	
	MDELAY(10);
	
	data_array[0]=0x00110500;
	dsi_set_cmdq(&data_array,1, 1);
	
	MDELAY(150);
	
	data_array[0]=0x00290500;
	dsi_set_cmdq(&data_array,1, 1);    

	MDELAY(60);
}

static void lcm_suspend(void)
{
  /*SET_RESET_PIN(0);
  MDELAY(10);*/
	unsigned int data_array[16];


	data_array[0]=0x00280500; 
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(30);

	data_array[0] = 0x00100500; 
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);

	data_array[0] = 0x00022902;
	data_array[1] = 0x000000b0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00022902;
	data_array[1] = 0x000001b1;
	dsi_set_cmdq(data_array, 2, 1);	
	MDELAY(30);

}

static void lcm_resume(void)
{
  lcm_init();
}


static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0;
	unsigned char buffer[5]; 
	unsigned int array[16];   
	unsigned char id_high = 0;
	unsigned char id_low = 0;
        int i=0;
	
//	LCM_DEBUG("R61318-=-LCM-20140718: %s, %d", __func__, __LINE__);
	
	SET_RESET_PIN(1);
	MDELAY(25);
	SET_RESET_PIN(0);
	MDELAY(25);
	SET_RESET_PIN(1);
	MDELAY(50);


	array[0] = 0x00022902;                          
	array[1] = 0x000000b0; 
	dsi_set_cmdq(array, 2, 1);//01 22 13 18 XX


	array[0] = 0x00053700;// read id return two byte,version and id

	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xbf, buffer, 5);

	id = buffer[2]<<8 | buffer[3]; //we only need ID

    #ifdef BUILD_LK
//		LCM_DEBUG("%s, R61318-=-LCM-LK: id= x%08x,id0 = 0x%08x,id1 = 0x%08x,id2 = 0x%08x,id3 = 0x%08x,id4 = 0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
    #else
//		LCM_DEBUG("%s, R61318-=-LCM-KERNEL: id= x%08x,id0 = 0x%08x,id1 = 0x%08x,id2 = 0x%08x,id3 = 0x%08x,id4 = 0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
    #endif


	return (R61318_LCM_ID == id)?1:0;
}



// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER r61318_tm55_wsc_hd_lcm_drv =
{
  .name			= "r61318_tm55_wsc_hd",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params     = lcm_get_params,
  .init           = lcm_init,
  .suspend        = lcm_suspend,
  .resume         = lcm_resume,
  .compare_id     = lcm_compare_id,
};
