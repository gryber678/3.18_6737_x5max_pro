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

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(800)
#define LCM_ID 0x77
#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER


#define LCM_DSI_CMD_MODE									0


// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))
#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
#define SET_GPIO_OUT(n, v)  (lcm_util.set_gpio_out((n), (v)))
#define read_reg_v2(cmd, buffer, buffer_size)  lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()

static unsigned int lcm_compare_id(void);

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static void lcm_init_power(void)
{
#ifdef GPIO_LCM_LED_EN
    mt_set_gpio_mode(GPIO_LCM_LED_EN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_LCM_LED_EN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_LCM_LED_EN, GPIO_OUT_ONE);
#endif
}

static void lcm_suspend_power(void)
{
#ifdef GPIO_LCM_LED_EN
    mt_set_gpio_mode(GPIO_LCM_LED_EN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_LCM_LED_EN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_LCM_LED_EN, GPIO_OUT_ZERO);
#endif
}

static void lcm_resume_power(void)
{
#ifdef GPIO_LCM_LED_EN
    mt_set_gpio_mode(GPIO_LCM_LED_EN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_LCM_LED_EN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_LCM_LED_EN, GPIO_OUT_ONE);
#endif
}


//#if 0
static struct LCM_setting_table lcm_compare_id_setting[] = {
	{0xFF, 3, {0xFF, 0x98, 0x16}},
	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_initialization_setting[] = {
{0xff, 5,{0x77,0x01,0x00,0x00,0x10}},
{0xc0, 2,{0x63,0x00}},
{0xc1, 2,{0x0a,0x02}},
{0xc2, 2,{0x31,0x08}},
{0xb0, 16,{0x40,0x05,0x12,0x13,0x19,0x0c,0x11,0x0a,0x0a,0x23,0x09,0x13,0x0e,0x12,0x16,0x19}},
{0xb1, 16,{0x40,0x05,0xd1,0x13,0x18,0x0b,0x13,0x09,0x09,0x2a,0x09,0x17,0x15,0x9e,0x22,0x19}},
{0xff, 5,{0x77,0x01,0x00,0x00,0x11}},
{0xb0, 1,{0x55}},
{0xb1, 1,{0x5a}},
{0xb2, 1,{0x07}},
{0xb3, 1,{0x80}},
{0xb5, 1,{0x47}},
{0xb7, 1,{0x8a}},
{0xb8, 1,{0x21}},
{0xc1, 1,{0x78}},
{0xc2, 1,{0x78}},
{0xd0, 1,{0x88}},
 {REGFLAG_DELAY, 100, {}},
{0xe0, 3,{0x00,0x00,0x02}},
{0xe1, 11,{0x04,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x20,0x20}},
{0xe2, 13,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xe3, 4,{0x00,0x00,0x33,0x00}},
{0xe4, 2,{0x22,0x00}},
{0xe5, 16,{0x04,0x34,0xaa,0xaa,0x06,0x34,0xaa,0xaa,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xe6, 4,{0x00,0x00,0x33,0x00}},
{0xe7, 2,{0x22,0x00}},
{0xe8, 16,{0x05,0x34,0xaa,0xaa,0x07,0x34,0xaa,0xaa,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xeb, 7,{0x02,0x00,0x40,0x40,0x00,0x00,0x00}},
{0xed, 16,{0xfa,0x45,0x0b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb0,0x54,0xaf}},
{0xff, 5,{0x77,0x01,0x00,0x00,0x00}},
 {REGFLAG_DELAY, 20, {}},
{0x11, 0,{}},
{REGFLAG_DELAY, 120, {}},
{0x29,1,{0x00}},
{REGFLAG_DELAY, 20, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},
    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 10, {}},
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

        unsigned int cmd;
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
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM= LCM_TWO_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

    // Highly depends on LCD driver capability.
    // Not support in MT6573
    params->dsi.packet_size=256;

    // Video mode setting
	params->dsi.intermediat_buffer_num = 2;

    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 480 * 3;

	params->dsi.vertical_sync_active = 2;
	params->dsi.vertical_backporch   = 18;//10 16
	params->dsi.vertical_frontporch  = 20;//8  20
  params->dsi.vertical_active_line = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active = 10; //8
	params->dsi.horizontal_backporch   = 60; //50
	params->dsi.horizontal_frontporch  = 60; //50
  params->dsi.horizontal_active_pixel= FRAME_WIDTH;

  params->dsi.PLL_CLOCK = 200;
 params->dsi.ssc_disable = 1;  // ssc disable control (1: disable, 0: enable, default: 0)
    
    params->dsi.esd_check_enable = 1;
    params->dsi.customization_esd_check_enable = 1;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
    
}

static void lcm_init(void)
{
    SET_RESET_PIN(1);
	MDELAY(1);
    SET_RESET_PIN(0);
	MDELAY(10);
    SET_RESET_PIN(1);
	MDELAY(120);

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
	
	push_table(lcm_deep_sleep_mode_in_setting,	sizeof(lcm_deep_sleep_mode_in_setting)/sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(50);
}

static void lcm_resume(void)
{

	lcm_init();

	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int array[1];
	unsigned short device_id;
	unsigned char buffer[4];
			
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(20);	
	
	array[0] = 0x00043700;
	dsi_set_cmdq(array, 1, 1);
	MDELAY(10); 
	
	read_reg_v2(0xd3, buffer, 2);	
	device_id = buffer[1];
	
#if defined(BUILD_LK)
	printf("st7701_boe397_tn_wvga %s,line=%d, id = 0x%x, buffer[0]=0x%08x,buffer[1]=0x%08x\n",__func__,__LINE__, device_id, buffer[0],buffer[1]);
#else
	printk("st7701_boe397_tn_wvga %s,line=%d, id = 0x%x, buffer[0]=0x%08x,buffer[1]=0x%08x\n",__func__,__LINE__, device_id, buffer[0],buffer[1]);
#endif
  return (LCM_ID == device_id)?1:0;

}

LCM_DRIVER st7701_boe397_tn_wvga_lcm_drv = {
	.name = "st7701_boe397_tn_wvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
	.compare_id = lcm_compare_id,
    .init_power        = lcm_init_power,
    .resume_power = lcm_resume_power,
    .suspend_power = lcm_suspend_power,
};
