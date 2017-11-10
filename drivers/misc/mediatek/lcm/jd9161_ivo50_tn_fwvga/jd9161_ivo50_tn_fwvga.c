/****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008

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

*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.

*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).

*****************************************************************************/
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

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

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID 0x91

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

static struct LCM_setting_table
{
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
static struct LCM_setting_table lcm_compare_id_setting[] =
{
    {0xFF, 3, {0xFF, 0x98, 0x16}},
    {REGFLAG_DELAY, 10, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
//#endif

static struct LCM_setting_table lcm_initialization_setting[] =
{

{0xbf, 3,{0x91,0x61,0xf2}},
{0xb3, 2,{0x00,0x8e}},
{0xb4, 2,{0x00,0x8e}},
{0xb8, 6,{0x00,0x9f,0x01,0x00,0x9f,0x01}},
{0xba, 3,{0x3e,0x23,0x04}},
{0xc3, 1,{0x02}},
{0xc4, 2,{0x30,0x6a}},
{0xc7, 9,{0x00,0x01,0x31,0x0a,0x6a,0x2a,0x13,0xa5,0xa5}},
{0xc8, 38,{0x7f,0x7b,0x73,0x68,0x5a,0x40,0x38,0x1c,0x34,0x33,0x35,0x58,0x4c,0x5d,0x56,0x5d,0x58,0x51,0x48,0x7f,0x7b,0x73,0x68,0x5a,0x40,0x38,0x1c,0x34,0x33,0x35,0x58,0x4c,0x5d,0x56,0x5d,0x58,0x51,0x48}},
{0xd4, 16,{0x1f,0x1f,0x1f,0x03,0x01,0x05,0x07,0x09,0x0b,0x11,0x13,0x1f,0x1f,0x1f,0x1f,0x1f}},
{0xd5, 16,{0x1f,0x1f,0x1f,0x02,0x00,0x04,0x06,0x08,0x0a,0x10,0x12,0x1f,0x1f,0x1f,0x1f,0x1f}},
{0xd6, 16,{0x1f,0x1f,0x1f,0x10,0x12,0x04,0x0a,0x08,0x06,0x02,0x00,0x1f,0x1f,0x1f,0x1f,0x1f}},
{0xd7, 16,{0x1f,0x1f,0x1f,0x11,0x13,0x05,0x0b,0x09,0x07,0x03,0x01,0x1f,0x1f,0x1f,0x1f,0x1f}},
{0xd8, 20,{0x20,0x00,0x00,0x30,0x03,0x30,0x01,0x02,0x30,0x01,0x02,0x06,0x70,0x73,0x5d,0x72,0x06,0x38,0x70,0x08}},
{0xd9, 19,{0x00,0x0a,0x0a,0x88,0x00,0x00,0x06,0x7b,0x00,0x80,0x00,0x3b,0x33,0x1f,0x00,0x00,0x00,0x06,0x70}},
{0x35, 0,{0x00}},
{0xbe, 1,{0x01}},
{0xc1, 1,{0x10}},
{0xcc, 10,{0x34,0x20,0x38,0x60,0x11,0x91,0x00,0x40,0x00,0x00}},
{0xbe, 1,{0x00}},
//{0x11, 0,{}},
//{0x29, 0,{}},

    {0x11,1,{0x00}},                // Sleep-Out
    {REGFLAG_DELAY, 120, {}},
    {0x29,1,{0x00}},                // Display On
    {REGFLAG_DELAY, 20, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_set_window[] =
{
    {0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
    {0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] =
{
// Sleep Out
    {0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},
// Display ON
    {0x29, 1, {0x00}},
    {REGFLAG_DELAY, 10, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] =
{
    {0xE3, 5,{0x03,0x03,0x03,0x03,0xC0}},
    // Display off sequence
    {0x28, 1, {0x00}},
    {REGFLAG_DELAY, 20, {}},
    // Sleep Mode On
    {0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_backlight_level_setting[] =
{
    {0x51, 1, {0xFF}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {

        unsigned int cmd;
        cmd = table[i].cmd;

        switch (cmd)
        {

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

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode = CMD_MODE;
#else
    //params->dsi.mode   = SYNC_EVENT_VDO_MODE;
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

    // Highly depends on LCD driver capability.
    // Not support in MT6573
    params->dsi.packet_size = 256;

    // Video mode setting
    params->dsi.intermediat_buffer_num = 2;

    params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
    params->dsi.word_count = 480 * 3;

	params->dsi.vertical_sync_active = 4;
	params->dsi.vertical_backporch = 6;//10 16
	params->dsi.vertical_frontporch = 6;//8  20
    params->dsi.vertical_active_line = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active = 10; //8
	params->dsi.horizontal_backporch = 10; //50
	params->dsi.horizontal_frontporch = 10; //50
    params->dsi.horizontal_active_pixel = FRAME_WIDTH;

    // Bit rate calculation
    //params->dsi.pll_div1=1;             // div1=0,1,2,3;div1_real=1,2,4,4
    //params->dsi.pll_div2=0;             // div2=0,1,2,3;div2_real=1,2,4,4
    //params->dsi.fbk_div =14;   //18
    params->dsi.PLL_CLOCK = 208;
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
//static unsigned int flicker = 0x35;
static void lcm_resume(void)
{
    lcm_init();
}


static unsigned int lcm_compare_id(void)
{
    unsigned int id=0;
    unsigned char buffer[2];
    unsigned int array[16];

    //return 1;

    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(20);//Must over 6 ms

    array[0]=0x00043902;
    array[1]=0xf36192Bf;
    dsi_set_cmdq(&array, 2, 1);
    MDELAY(10);

    array[0] = 0x00023700;// return byte number
    dsi_set_cmdq(&array, 1, 1);
    MDELAY(10);

    read_reg_v2(0xda, buffer, 2);
    id = buffer[0];

#if defined(BUILD_LK)
    printf("%s, id = 0x%08x\n", __func__, id);
#else
    printk("%s, id = 0x%08x\n", __func__, id);
#endif
    return (LCM_ID == id)?1:0;

}


// Get LCM Driver Hooks
//
LCM_DRIVER jd9161_ivo50_tn_fwvga_lcm_drv =
{
    .name			= "jd9161_ivo50_tn_fwvga",
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
