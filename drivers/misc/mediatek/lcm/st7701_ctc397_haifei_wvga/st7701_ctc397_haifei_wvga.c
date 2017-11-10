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
#define LCM_ID 0x7700
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
{0xFF,5,{0x77,0x01,0x00,0x00,0x10}},
{0xC0,2,{0x63,0x00}},

{0xC1,2,{0x0C,0x07}},

{0xC2,2,{0x31,0x08}},
{0xB0,16,{0x40,0x02,0x87,0x0E,0x15,0x0A,0x03,0x0A,0x0A,0x18,0x08,0x16,0x13,0x07,0x09,0x19}},

{0xB1,16,{0x40,0x01,0x86,0x0D,0x13,0x09,0x03,0x0A,0x09,0x1C,0x09,0x15,0x13,0x91,0x16,0x19}},
{0xFF,5,{0x77,0x01,0x00,0x00,0x11}},

{0xB0,1,{0x4D}},
{0xB1,1,{0x64}},
{0xB2,1,{0x07}},
{0xB3,1,{0x80}},

{0xB5,1,{0x47}},

{0xB7,1,{0x8A}},

{0xB8,1,{0x21}},

{0xC1,1,{0x78}},

{0xC2,1,{0x78}},

{0xD0,1,{0x88}},

{0xE0,4,{0x00,0x00,0x02}},

{0xE1,11,{0x06,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x20,0x20}},

{0xE2,11,{0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00}},

{0xE3,4,{0x00,0x00,0x33,0x33}},

{0xE4,2,{0x44,0x44}},

{0xE5,16,{0x09,0x31,0xBE,0xA0,0x0B,0x31,0xBE,0xA0,0x05,0x31,0xBE,0xA0,0x07,0x31,0xBE,0xA0}},

{0xE6,4,{0x00,0x00,0x33,0x33}},

{0xE7,2,{0x44,0x44}},

{0xE8,16,{0x08,0x31,0xBE,0xA0,0x0A,0x31,0xBE,0xA0,0x04,0x31,0xBE,0xA0,0x06,0x31,0xBE,0xA0}},

{0xEA,16,{0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00}},

{0xEB,7,{0x02,0x02,0x00,0x00,0x00,0x00,0x00}},

{0xEC,2,{0x02,0x02}},

{0xED,16,{0xF5,0x47,0x6F,0x0B,0x8F,0x9F,0xFF,0xFF,0xFF,0xFF,0xF9,0xF8,0xB0,0xF6,0x74,0x5F}},

{0xEF,12,{0x08,0x08,0x08,0x08,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x04}},

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

	params->dsi.vertical_sync_active = 3;
	params->dsi.vertical_backporch = 7;//10 16
	params->dsi.vertical_frontporch = 7;//8  20
    params->dsi.vertical_active_line= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active = 8; //8
	params->dsi.horizontal_backporch = 26; //50
	params->dsi.horizontal_frontporch = 26; //50
    params->dsi.horizontal_active_pixel= FRAME_WIDTH;

    params->dsi.PLL_CLOCK = 208;
    /*
    params->dsi.esd_check_enable = 0;
    params->dsi.customization_esd_check_enable = 0;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
    */
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

	push_table(lcm_deep_sleep_mode_in_setting,
		   sizeof(lcm_deep_sleep_mode_in_setting) /
		   sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
       MDELAY(50);
}

//unsigned int flicker=0x30;
static void lcm_resume(void)
{
	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int array[4];
	unsigned short device_id;
	unsigned char buffer[4];
			
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(20);	
	
    array[0]=0x00063902;
    array[1]=0x000177ff;
    array[2]=0x00001000;
    dsi_set_cmdq(array, 3, 1);
	MDELAY(10); 
	
	read_reg_v2(0xa1, buffer, 2);	
	device_id = (buffer[0]<<8) | (buffer[1]);
	
#if defined(BUILD_LK)
	printf("st7701_hsd397_zgd_wvga %s,line=%d, id = 0x%x, buffer[0]=0x%08x,buffer[1]=0x%08x\n",__func__,__LINE__, device_id, buffer[0],buffer[1]);
#else
	printk("st7701_hsd397_zgd_wvga %s,line=%d, id = 0x%x, buffer[0]=0x%08x,buffer[1]=0x%08x\n",__func__,__LINE__, device_id, buffer[0],buffer[1]);
#endif
  return (LCM_ID == device_id)?1:0;

}

LCM_DRIVER st7701_ctc397_haifei_wvga_lcm_drv = {
	.name = "st7701_ctc397_haifei_wvga",
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
