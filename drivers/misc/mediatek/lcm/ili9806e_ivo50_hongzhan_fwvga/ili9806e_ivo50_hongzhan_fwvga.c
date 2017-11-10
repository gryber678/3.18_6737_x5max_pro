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

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (854)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0xFA    //0xFFF ??   // END OF REGISTERS MARKER

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


//static unsigned char temp_v = 0x12 ;
/*HSD*/
static struct LCM_setting_table lcm_initialization_setting[] = {

      /*
    Note :

    Data ID will depends on the following rule.

    count of parameters > 1	=> Data ID = 0x39
    count of parameters = 1	=> Data ID = 0x15
    count of parameters = 0	=> Data ID = 0x05

    Structure Format :

    {DCS command, count of parameters, {parameter list}}
    {REGFLAG_DELAY, milliseconds of time, {}},

    ...

    Setting ending by predefined flag

    {REGFLAG_END_OF_TABLE, 0x00, {}} // haha
       */
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x01}},      //Change to Page 1
     
{0x08,1,{0x18}},
    {0x21,1,{0x01}},                  //DE = 1 Active
    {0x30,1,{0x01}},                  //480 X 854
{0x31,1,{0x00}},

{0x50,1,{0x65}},

{0x51,1,{0x65}}, 
    {0x60,1,{0x07}},                  //SDTI
    {0x61,1,{0x00}},                 //CRTI
{0x62,1,{0x07}}, 
    {0x63,1,{0x00}},                 //PCTI

{0x40,1,{0x10}}, 
{0x41,1,{0x44}}, 
{0x42,1,{0x00}}, 
{0x43,1,{0x8A}}, 
{0x44,1,{0x86}}, 

{0x46,1,{0x55}}, 
{0x52,1,{0x00}}, 
{0x53,1,{0x49}}, 
{0xFF,5,{0xFF,0x98,0x06,0x04,0x01}},
    {0xA0,1,{0x00}},  // Gamma 0 	//255
         
{0xA1,1,{0x08}},
                                            
{0xA2,1,{0x13}},
                                            
{0xA3,1,{0x11}},
                                         
    {0xA4,1,{0x0A}},  // Gamma 24	//231
                                            
{0xA5,1,{0x18}},
                                            
{0xA6,1,{0x04}},
                                            
{0xA7,1,{0x05}},
                                            
{0xA8,1,{0x07}},
                                            
{0xA9,1,{0x0C}},
                                            
{0xAA,1,{0x0C}},
                                            
{0xAB,1,{0x06}},
                                            
{0xAC,1,{0x0D}},
                                            
{0xAD,1,{0x29}},
                                            
{0xAE,1,{0x24}},
                                            
    {0xAF,1,{0x00}},  // Gamma 255	//0
                                            


    {0xC0,1,{0x00}},  // Gamma 0 	
                                            
{0xC1,1,{0x08}},
                                            
{0xC2,1,{0x13}},
         
{0xC3,1,{0x11}},
                                            
{0xC4,1,{0x0B}},
                                            
{0xC5,1,{0x17}},
                                            
{0xC6,1,{0x0E}},
                                            
{0xC7,1,{0x0A}},
                                            
{0xC8,1,{0x02}},
                                            
{0xC9,1,{0x07}},
                                            
{0xCA,1,{0x02}},
                                            
{0xCB,1,{0x05}},
                                         
{0xCC,1,{0x0C}},
                                            
{0xCD,1,{0x28}},
                                            
{0xCE,1,{0x24}},
                                            
    {0xCF,1,{0x00}},  // Gamma 255	
                                            

    //****************************************************************************//
    //****************************** Page 6 Command ******************************//
    //****************************************************************************//
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},
    {0x00,1,{0x21}},
{0x01,1,{0x06}},
{0x02,1,{0xA0}},
{0x03,1,{0x02}},
    {0x04,1,{0x01}},
    {0x05,1,{0x01}},
    {0x06,1,{0x80}},
{0x07,1,{0x04}},
{0x08,1,{0x00}},
    {0x09,1,{0x80}},
    {0x0A,1,{0x00}},
    {0x0B,1,{0x00}},
{0x0C,1,{0x33}},
{0x0D,1,{0x33}},
{0x0E,1,{0x09}},
    {0x0F,1,{0x00}},
{0x10,1,{0xFF}},
{0x11,1,{0xF0}},
{0x12,1,{0x00}},
    {0x13,1,{0x00}},
    {0x14,1,{0x00}},
    {0x15,1,{0xC0}},
    {0x16,1,{0x08}},
    {0x17,1,{0x00}},
    {0x18,1,{0x00}},
    {0x19,1,{0x00}},
    {0x1A,1,{0x00}},
    {0x1B,1,{0x00}},
    {0x1C,1,{0x00}},
    {0x1D,1,{0x00}},
    {0x20,1,{0x01}},
    {0x21,1,{0x23}},
    {0x22,1,{0x45}},
    {0x23,1,{0x67}},
    {0x24,1,{0x01}},
    {0x25,1,{0x23}},
    {0x26,1,{0x45}},
    {0x27,1,{0x67}},
{0x30,1,{0x12}},
{0x31,1,{0x22}},
{0x32,1,{0x22}},
{0x33,1,{0x22}},
{0x34,1,{0x87}},
{0x35,1,{0x96}},  
{0x36,1,{0xAA}},
{0x37,1,{0xDB}},
{0x38,1,{0xCC}},
{0x39,1,{0xBD}},
{0x3A,1,{0x78}},
{0x3B,1,{0x69}},
    {0x3C,1,{0x22}},
    {0x3D,1,{0x22}},
    {0x3E,1,{0x22}},
    {0x3F,1,{0x22}},
    {0x40,1,{0x22}},
    {0x52,1,{0x10}},
    {0x53,1,{0x10}},

{0x54,1,{0x13}},  
    //****************************** Page 7 Command ******************************//
    //****************************************************************************//
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x07}},      //Change to Page 7
    {0x17,1,{0x22}},      //VGL_REG ON  
    {0x02,1,{0x77}},   
{0x06,1,{0x13}}, 
    {0xE1,1,{0x79}},   
{0xB3,1,{0x10}}, 
    {0x26,1,{0xB2}},
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},      //Change to Page 0

    {0x11, 0,{0x00}},

    {REGFLAG_DELAY, 120, {}},         

    {0x29, 0,{0x00}},

    {REGFLAG_DELAY, 20, {}},
      // Setting ending by predefined flag
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
/*
   static struct LCM_setting_table lcm_sleep_out_setting[] = {
// Sleep Out
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},

// Display ON
{0x29, 1, {0x00}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};
 */
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
  {0xE3, 5,{0x03,0x03,0x03,0x03,0xC0}},
  // Display off sequence
  {0x28, 1, {0x00}},
  {REGFLAG_DELAY, 20, {}},
  // Sleep Mode On
  {0x10, 1, {0x00}},
  {REGFLAG_DELAY, 120, {}},
  {REGFLAG_END_OF_TABLE, 0x00, {}}
};
/*
   static struct LCM_setting_table lcm_backlight_level_setting[] = {
   {0x51, 1, {0xFF}},
   {REGFLAG_END_OF_TABLE, 0x00, {}}
   };
 */

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
  params->dsi.vertical_backporch					= 12; //18
  params->dsi.vertical_frontporch					= 20; //18
  params->dsi.vertical_active_line				= FRAME_HEIGHT;

  params->dsi.horizontal_sync_active			    = 20;//8;
  params->dsi.horizontal_backporch				= 70;//46;
  params->dsi.horizontal_frontporch				= 70;//46;
  //  params->dsi.horizontal_blanking_pixel				= 0;
  params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

  // Bit rate calculation
  // Bit rate calculation
  //	params->dsi.pll_div1=26;//26;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
  //	params->dsi.pll_div2=1; 		       // div2=0~15: fout=fvo/(2*div2)

  //params->dsi.pll_div1=26;//26;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
  //params->dsi.pll_div2=1; 		             // div2=0~15: fout=fvo/(2*div2)


  params->dsi.PLL_CLOCK = 212; //dpi clock customization: should config 
  //params->dpi.ssc_disable = 1;
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

}

static void lcm_resume(void)
{
  /* lcm_compare_id(); */
  lcm_init();
}

static unsigned int lcm_compare_id(void)
{
    int array[4];
    char buffer[1];
    char id_high=0;
    char id_midd=0;
    char id_low=0;
    int id=0;

    //Do reset here
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(25);       
    SET_RESET_PIN(1);
    MDELAY(50);      
   
    array[0]=0x00063902;
    array[1]=0x0698ffff;
    array[2]=0x00000104;
    dsi_set_cmdq(array, 3, 1);
    MDELAY(10);

    array[0]=0x00023700;
    dsi_set_cmdq(array, 1, 1);
    //read_reg_v2(0x04, buffer, 3);//if read 0x04,should get 0x008000,that is both OK.

    read_reg_v2(0x00, buffer,1);
    id_high = buffer[0]; ///////////////////////0x98

    read_reg_v2(0x01, buffer,1);
    id_midd = buffer[0]; ///////////////////////0x06

    read_reg_v2(0x02, buffer,1);
    id_low = buffer[0]; ////////////////////////0x04

    id = (id_high << 16) | (id_midd << 8) | id_low;
    
    //Lcd_Log("wqcat %s, 0x00=0x%x,0x01=0x%x,0x02=0x%x,id=0x%x\n", __func__, id_high,id_midd,id_low,id);
    return (0x980604 == id)?1:0;
    //return (get_lcd_id()==1)?1:0;
	//return 0;
}
/*
   static unsigned int lcm_esd_check(void) //hehe
   {

#ifndef BUILD_LK
//unsigned int id = 0 ;
unsigned char buffer[3];
//unsigned int data_array[6];
//unsigned int chip_id = 0;
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
 */
// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER ili9806e_ivo50_hongzhan_fwvga_lcm_drv =
{
  .name			= "ili9806e_ivo50_hongzhan_fwvga",
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
