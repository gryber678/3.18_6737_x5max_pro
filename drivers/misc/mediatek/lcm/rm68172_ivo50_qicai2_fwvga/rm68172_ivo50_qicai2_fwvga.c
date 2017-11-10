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
#else
#include <linux/string.h>
#include <mt-plat/mt_gpio.h>
//#include <mach/mt_pm_ldo.h>
#include <mt-plat/upmu_common.h>
#endif


#include "lcm_drv.h"

#if defined(BUILD_LK)
#else

#include <linux/proc_fs.h> //proc file use
#endif



// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (854)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0xFA    //0xFFF ??   // END OF REGISTERS MARKER
#define LCM_RM68172_ID      (0x8172)

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

static struct LCM_setting_table lcm_compare_id_setting[] = { 
  // Display off sequence
  {0xF0, 5,{0x55,0xAA,0x52,0x08,0x01}},
  {REGFLAG_DELAY, 10, {}},

    // Sleep Mode On
//On{0xC3, 1, {0xFF}},
  {REGFLAG_END_OF_TABLE, 0x00, {}} 
};

//static unsigned char temp_v = 0x12 ;
/*HSD*/
static struct LCM_setting_table lcm_initialization_setting[] = {
    {0xF0, 5, {0x55, 0xAA, 0x52, 0x08, 0x02}},
    {0xF6, 2, {0x60, 0x40}},
    {0xFE, 4, {0x01, 0x80, 0x09, 0x09}},
    {0xF0, 5, {0x55, 0xAA, 0x52, 0x08, 0x01}},
    {0xB0, 1, {0x07}},
    {0xB1, 1, {0x07}},
    {0xB5, 1, {0x08}},
    {0xB6, 1, {0x44}},
    {0xB7, 1, {0x34}},
    {0xB8, 1, {0x24}},
    {0xB9, 1, {0x34}},
    {0xBA, 1, {0x14}},
    {0xBC, 3, {0x00, 0x78, 0x13}},
    {0xBD, 3, {0x00, 0x78, 0x13}},
    {0xBE, 2, {0x00, 0x15}},
    {0xD1, 52, {0x00, 0x00, 0x00, 0x17, 0x00, 0x3F, 0x00, 0x5E, 0x00, 0x7A, 0x00, 0xAA, 0x00, 0xD1, 0x01, 0x10, 0x01, 0x41, 0x01, 0x8A, 0x01, 0xC2, 0x02, 0x13, 0x02, 0x54, 0x02, 0x56, 0x02, 0x92, 0x02, 0xD8, 0x03, 0x07, 0x03, 0x46, 0x03, 0x70, 0x03, 0xA4, 0x03, 0xC1, 0x03, 0xE0, 0x03, 0xEC, 0x03, 0xF5, 0x03, 0xFA, 0x03, 0xFF}},
    {0xD2, 52, {0x00, 0x00, 0x00, 0x17, 0x00, 0x3F, 0x00, 0x5E, 0x00, 0x7A, 0x00, 0xAA, 0x00, 0xD1, 0x01, 0x10, 0x01, 0x41, 0x01, 0x8A, 0x01, 0xC2, 0x02, 0x13, 0x02, 0x54, 0x02, 0x56, 0x02, 0x92, 0x02, 0xD8, 0x03, 0x07, 0x03, 0x46, 0x03, 0x70, 0x03, 0xA4, 0x03, 0xC1, 0x03, 0xE0, 0x03, 0xEC, 0x03, 0xF5, 0x03, 0xFA, 0x03, 0xFF}},
    {0xD3, 52, {0x00, 0x00, 0x00, 0x17, 0x00, 0x3F, 0x00, 0x5E, 0x00, 0x7A, 0x00, 0xAA, 0x00, 0xD1, 0x01, 0x10, 0x01, 0x41, 0x01, 0x8A, 0x01, 0xC2, 0x02, 0x13, 0x02, 0x54, 0x02, 0x56, 0x02, 0x92, 0x02, 0xD8, 0x03, 0x07, 0x03, 0x46, 0x03, 0x70, 0x03, 0xA4, 0x03, 0xC1, 0x03, 0xE0, 0x03, 0xEC, 0x03, 0xF5, 0x03, 0xFA, 0x03, 0xFF}},
    {0xD4, 52, {0x00, 0x00, 0x00, 0x17, 0x00, 0x3F, 0x00, 0x5E, 0x00, 0x7A, 0x00, 0xAA, 0x00, 0xD1, 0x01, 0x10, 0x01, 0x41, 0x01, 0x8A, 0x01, 0xC2, 0x02, 0x13, 0x02, 0x54, 0x02, 0x56, 0x02, 0x92, 0x02, 0xD8, 0x03, 0x07, 0x03, 0x46, 0x03, 0x70, 0x03, 0xA4, 0x03, 0xC1, 0x03, 0xE0, 0x03, 0xEC, 0x03, 0xF5, 0x03, 0xFA, 0x03, 0xFF}},
    {0xD5, 52, {0x00, 0x00, 0x00, 0x17, 0x00, 0x3F, 0x00, 0x5E, 0x00, 0x7A, 0x00, 0xAA, 0x00, 0xD1, 0x01, 0x10, 0x01, 0x41, 0x01, 0x8A, 0x01, 0xC2, 0x02, 0x13, 0x02, 0x54, 0x02, 0x56, 0x02, 0x92, 0x02, 0xD8, 0x03, 0x07, 0x03, 0x46, 0x03, 0x70, 0x03, 0xA4, 0x03, 0xC1, 0x03, 0xE0, 0x03, 0xEC, 0x03, 0xF5, 0x03, 0xFA, 0x03, 0xFF}},
    {0xD6, 52, {0x00, 0x00, 0x00, 0x17, 0x00, 0x3F, 0x00, 0x5E, 0x00, 0x7A, 0x00, 0xAA, 0x00, 0xD1, 0x01, 0x10, 0x01, 0x41, 0x01, 0x8A, 0x01, 0xC2, 0x02, 0x13, 0x02, 0x54, 0x02, 0x56, 0x02, 0x92, 0x02, 0xD8, 0x03, 0x07, 0x03, 0x46, 0x03, 0x70, 0x03, 0xA4, 0x03, 0xC1, 0x03, 0xE0, 0x03, 0xEC, 0x03, 0xF5, 0x03, 0xFA, 0x03, 0xFF}},
    {0xF0, 5, {0x55, 0xAA, 0x52, 0x08, 0x03}},
    {0xB0, 7, {0x05, 0x17, 0xF9, 0x53, 0x53, 0x00, 0x30}},
    {0xB1, 7, {0x05, 0x17, 0xFB, 0x55, 0x53, 0x00, 0x30}},
    {0xB2, 9, {0xFB, 0xFC, 0xFD, 0xFE, 0xF0, 0x53, 0x00, 0xC5, 0x08}},
    {0xB3, 6, {0x5B, 0x00, 0xFB, 0x5A, 0x5A, 0x0C}},
    {0xB4, 9, {0xFF, 0x00, 0x01, 0x02, 0xC0, 0x40, 0x05, 0x08, 0x53}},
    {0xB5, 11, {0x00, 0x00, 0xFF, 0x83, 0x5F, 0x5E, 0x50, 0x50, 0x33, 0x33, 0x55}},
    {0xB6, 7, {0xBC, 0x00, 0x00, 0x00, 0x2A, 0x80, 0x00}},
    {0xB7, 8, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0xB8, 3, {0x11, 0x60, 0x00}},
    {0xB9, 1, {0x90}},
    {0xBA, 16, {0x44, 0x44, 0x08, 0xAC, 0xE2, 0x64, 0x44, 0x44, 0x44, 0x44, 0x47, 0x3F, 0xDB, 0x91, 0x54, 0x44}},
    {0xBB, 16, {0x44, 0x43, 0x79, 0xFD, 0xB5, 0x14, 0x44, 0x44, 0x44, 0x44, 0x40, 0x4A, 0xCE, 0x86, 0x24, 0x44}},
    {0xBC, 4, {0xE0, 0x1F, 0xF8, 0x07}},
    {0xBD, 4, {0xE0, 0x1F, 0xF8, 0x07}},
    {0xF0, 5, {0x55, 0xAA, 0x52, 0x08, 0x00}},
    {0xB0, 2, {0x00, 0x10}},
    {0xB4, 1, {0x10}},
    {0xB5, 1, {0x6B}},
    {0xBC, 1, {0x00}},
    {0x35, 1, {0x00}},
    {0x11, 0,{}},
	{REGFLAG_DELAY, 150, {}},
    {0x29, 0,{}},

	{REGFLAG_DELAY, 25, {}},

	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
  {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},
    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

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
            #if 0
                if (cmd != 0xFF && cmd != 0x2C && cmd != 0x3C) {
                    //#if defined(BUILD_UBOOT)
                    //	printf("[DISP] - uboot - REG_R(0x%x) = 0x%x. \n", cmd, table[i].para_list[0]);
                    //#endif
                    while(read_reg(cmd) != table[i].para_list[0]);
                }
            #endif
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
    params->dsi.LANE_NUM				= LCM_TWO_LANE;

    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;

    params->dsi.packet_size=256;
    // Video mode setting
    params->dsi.intermediat_buffer_num = 2;

    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

    params->dsi.vertical_sync_active				= 4;//4
    params->dsi.vertical_backporch					= 16; //18
    params->dsi.vertical_frontporch					= 20; //18
    params->dsi.vertical_active_line				= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active			    = 8;//8;
    params->dsi.horizontal_backporch				= 80;//46;
    params->dsi.horizontal_frontporch				= 10;//46;
  //  params->dsi.horizontal_blanking_pixel				= 0;
    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

    // Bit rate calculation
	// Bit rate calculation
	//params->dsi.pll_div1=26;//26;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
	//	params->dsi.pll_div2=1; 		       // div2=0~15: fout=fvo/(2*div2)

    //params->dsi.pll_div1=26;//26;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
   // params->dsi.pll_div2=1; 		             // div2=0~15: fout=fvo/(2*div2)


params->dsi.PLL_CLOCK = 220; //dpi clock customization: should config 
}

static unsigned int lcm_compare_id(void);
static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(1);
    SET_RESET_PIN(0);
    MDELAY(10);//1
    SET_RESET_PIN(1);
    MDELAY(120);//60

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
    push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(60);
}

static void lcm_resume(void)
{
    /* lcm_compare_id(); */
	lcm_init();
}

static unsigned int lcm_compare_id(void)
{
  unsigned int id; 
  unsigned char buffer[5];
  unsigned int array[5];

  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(100);

  push_table(lcm_compare_id_setting,sizeof(lcm_compare_id_setting) /sizeof(struct LCM_setting_table), 1); 

  
  array[0] = 0x00023700;// read id return two byte,version and id
  dsi_set_cmdq(array, 1, 1); 

  read_reg_v2(0xc5, buffer, 2); 
  id = (buffer[1]<<8)|buffer[0]; //we only need ID
#if defined(BUILD_LK)
printf("%s, [rm68172]  buffer[0] = [0x%d] buffer[2] = [0x%d] ID = [0x%d]\n",__func__, buffer[0], buffer[1], id);
#else
printk("%s, [rm68172]  buffer[0] = [0x%d] buffer[2] = [0x%d] ID = [0x%d]\n",__func__, buffer[0], buffer[1], id);
#endif

  return ((LCM_RM68172_ID == id)? 1 : 0);
}

static unsigned int lcm_esd_check(void) //hehe
{

#ifndef BUILD_LK
    unsigned int id = 0 ;
    unsigned char buffer[3];
    unsigned int data_array[6];
    unsigned int chip_id = 0;
     //return 1 ;
	 //printk("[fl10801] %s\n", __func__);
     //data_array[0] = 0x00013700;// read id return two byte,version and id
     //dsi_set_cmdq(data_array, 1, 1);

     read_reg_v2(0x0a, buffer, 1);

	 if(0x9c == buffer[0])
	 {

		 	  //printk("[fl10801->lcm_esd_check] esd check ok \n");
	 		return 0;
	 }
	 else
	 {
		    //printk("[fl10801->lcm_esd_check] esd check fail \n");
	 		return 1;
	 }



#endif
}

static unsigned int lcm_esd_recover(void)
{
//    unsigned char para = 0;

	lcm_init();
	//lcm_resume();

    return 1; //TRUE
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER rm68172_ivo50_qicai2_fwvga_lcm_drv =
{
    .name			= "rm68172_ivo50_qicai2_fwvga",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    //.set_backlight	= lcm_setbacklight,
    //.esd_check      = lcm_esd_check,
    //.esd_recover    = lcm_esd_recover,
    //.set_pwm        = lcm_setpwm,
    //.get_pwm        = lcm_getpwm,
    .compare_id    = lcm_compare_id,
};
