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

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
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

#define FRAME_WIDTH                                         (480)
#define FRAME_HEIGHT                                        (800)
#define LCM_ILI9806_CPT_CLY_AUO_YKL_ID  (0x0604)

#define REGFLAG_DELAY                                       0XFFE
#define REGFLAG_END_OF_TABLE                                0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE                                    0

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)                                    (lcm_util.set_reset_pin((v)))
#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
#define SET_GPIO_OUT(n, v)  (lcm_util.set_gpio_out((n), (v)))
#define read_reg_v2(cmd, buffer, buffer_size)  lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

#define UDELAY(n)                                           (lcm_util.udelay(n))
#define MDELAY(n)                                           (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg                                            lcm_util.dsi_read_reg()

static void lcm_init_power(void)
{
}

static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
}

static struct LCM_setting_table
{
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};
#if 0
static struct LCM_setting_table lcm_compare_id_setting[] =
{
    {0xFF, 3, {0xFF, 0x98, 0x06}},
    {REGFLAG_DELAY, 10, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
#endif
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
//****************************************************************************//
//****************************** Page 1 Command ******************************//
//****************************************************************************//
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x01 }},    // Change to Page 1

    {0x08,1,{0x10}},                  // output SDA

    {0x21,1,{0x01 }},                 // DE = 1 Active

    {0x30,1,{0x02}},                  // 480 X 800

    {0x31,1,{0x02 }},                 // 2-dot Inversion

    {0x40,1,{0x14 }},                // BT

    {0x41,1,{0x44 }},                 // DVDDH DVDDL clamp

    {0x42,1,{0x01}},                  // VGH/VGL

    {0x43,1,{0x89 }},                 // VGH_CLAMP 0FF ;

    {0x44,1,{0x89}},                  // VGL_CLAMP OFF ;

    {0x45,1,{0x1b }},                 // VGL_REG  -11V

    {0x46,1,{0x44 }},

    {0x47,1,{0x44 }},

    {0x50,1,{0x85}},                    //Flicker

    {0x51,1,{0x85}},                    //Flicker

    {0x52,1,{0x00}},                    //Flicker

    {0x53,1,{0x64}},                    //Flicker

    {0x60,1,{0x07}},                  // SDTI

    {0x61,1,{0x06}},                 // CRTI

    {0x62,1,{0x06}},                  // EQTI

    {0x63,1,{0x04}},                 // PCTI

//++++++++++++++++++ Gamma Setting ++++++++++++++++++//
    {0xA0,1,{0x00}},   // Gamma 255

    {0xA1,1,{0x00}},   // Gamma 251

    {0xA2,1,{0x03}},   // Gamma 247

    {0xA3,1,{0x0e}},   // Gamma 239

    {0xA4,1,{0x08}},   // Gamma 231

    {0xA5,1,{0x1f}},   // Gamma 203

    {0xA6,1,{0x0f}},   // Gamma 175

    {0xA7,1,{0x0b}},   // Gamma 147

    {0xA8,1,{0x03}},   // Gamma 108

    {0xA9,1,{0x06}},   // Gamma 80

    {0xAA,1,{0x05}},   // Gamma 52

    {0xAB,1,{0x02}},   // Gamma 32

    {0xAC,1,{0x0e}},   // Gamma 16

    {0xAD,1,{0x25}},   // Gamma 8

    {0xAE,1,{0x1d}},   // Gamma 4

    {0xAF,1,{0x00}},   // Gamma 0

///==============Nagitive
    {0xC0,1,{0x00}},   // Gamma 255

    {0xC1,1,{0x04}},   // Gamma 251

    {0xC2,1,{0x0f}},   // Gamma 247

    {0xC3,1,{0x10}},   // Gamma 239

    {0xC4,1,{0x0b }},  // Gamma 231

    {0xC5,1,{0x1e}},   // Gamma 203

    {0xC6,1,{0x09 }},  // Gamma 175

    {0xC7,1,{0x0a}},   // Gamma 147

    {0xC8,1,{0x00 }},  // Gamma 108

    {0xC9,1,{0x0a}},   // Gamma 80

    {0xCA,1,{0x01}},   // Gamma 52

    {0xCB,1,{0x06}},   // Gamma 24

    {0xCC,1,{0x09}},   // Gamma 16

    {0xCD,1,{0x2a}},   // Gamma 8

    {0xCE,1,{0x28}},   // Gamma 4

    {0xCF,1,{0x00 }},  // Gamma 0




//+++++++++++++++++++++++++++++++++++++++++++++++++++//

//****************************************************************************//
//****************************** Page 6 Command ******************************//
//****************************************************************************//
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},      // Change to Page 6

    {0x00,1,{0xa0}},

    {0x01,1,{0x05}},

    {0x02,1,{0x00 }},

    {0x03,1,{0x00}},

    {0x04,1,{0x01}},

    {0x05,1,{0x01}},

    {0x06,1,{0x88 }},

    {0x07,1,{0x04}},

    {0x08,1,{0x01}},

    {0x09,1,{0x90}},

    {0x0A,1,{0x04}},

    {0x0B,1,{0x01}},

    {0x0C,1,{0x01}},

    {0x0D,1,{0x01}},

    {0x0E,1,{0x00}},

    {0x0F,1,{0x00}},

    {0x10,1,{0x55}},

    {0x11,1,{0x50}},

    {0x12,1,{0x01}},

    {0x13,1,{0x85}},

    {0x14,1,{0x85}},

    {0x15,1,{0xC0}},

    {0x16,1,{0x0b}},

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


    {0x30,1,{0x02}},

    {0x31,1,{0x22}},

    {0x32,1,{0x11}},

    {0x33,1,{0xaa}},

    {0x34,1,{0xbb}},

    {0x35,1,{0x66}},

    {0x36,1,{0x00}},

    {0x37,1,{0x22}},

    {0x38,1,{0x22}},

    {0x39,1,{0x22}},

    {0x3A,1,{0x22}},

    {0x3B,1,{0x22}},

    {0x3C,1,{0x22}},

    {0x3D,1,{0x22}},

    {0x3E,1,{0x22}},

    {0x3F,1,{0x22}},

    {0x40,1,{0x22}},

//{0x52,1,{0x10}},

    {0x53,1,{0x1a}},
//****************************************************************************//
//****************************** Page 7 Command ******************************//
//****************************************************************************//
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x07}},      // Change to Page 7

    {0x18,1,{0x1D}},   // VREG1 VREG2 output

    {0x17,1,{0x12}},   // VGL_REG ON

    {0x02,1,{0x77}},   //
    {0xE1,1,{0x79}},

    {0x06,1,{0x13}},
    {0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},      // Change to Page 0

    {0x11, 1, {0x00}},                 // Sleep-Out
    {REGFLAG_DELAY, 120, {0x00}},
    {0x29, 1, {0x00 }},                // Display On
    {REGFLAG_DELAY, 10, {0x00}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_set_window[] =
{
    {0x2A,    4,  {0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
    {0x2B,    4,  {0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] =
{
// Sleep Out
    {0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},
// Display ON
    {0x29, 1, {0x00}},
    {REGFLAG_DELAY, 150, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] =
{
    // Display off sequence
    {0x28, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},
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
    params->dsi.vertical_backporch = 16;//10 16
    params->dsi.vertical_frontporch = 20;//8  20
    params->dsi.vertical_active_line = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active			= 10;
    params->dsi.horizontal_backporch				= 80;
    params->dsi.horizontal_frontporch				= 80; //200
    //params->dsi.horizontal_blanking_pixel		= 60; //
    params->dsi.horizontal_active_pixel 		= FRAME_WIDTH;

    params->dsi.PLL_CLOCK = 221;
}

static void lcm_init(void)
{

    SET_RESET_PIN(1);
    MDELAY(1);
    SET_RESET_PIN(0);
    MDELAY(10);//Must > 10ms
    SET_RESET_PIN(1);
    MDELAY(120);//Must > 120ms

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
    push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
    //lcm_init();
    push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[16];

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
    data_array[3]= 0x00053902;
    data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
    data_array[5]= (y1_LSB);
    data_array[6]= 0x002c3909;

    dsi_set_cmdq(&data_array, 7, 0);

}

static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;
    unsigned int id_midd = 0;
    unsigned int id_low = 0;
    unsigned char buffer[5];
    unsigned char buffer1[5];
    unsigned int array[16];

    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);

    array[0] = 0x00063902;
    array[1] = 0x0698FFFF;
    array[2] = 0x00000104;
    dsi_set_cmdq(array, 3, 1);

    array[0] = 0x00023700;
    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0x01, buffer, 1);
    id_midd = buffer[1];
    read_reg_v2(0x02, buffer1, 1);
    id_low = buffer1[1];
    id = (id_midd << 8 | id_low); //we only need ID
#if defined(BUILD_LK)
    printf
    (" [ili9806e_boe397_xyl] %s, id = [0x%x],buffer[0] = [0x%x],buffer[1] = [0x%x] buffer[2] = [0x%x]  buffer[3] = [0x%x] buffer1[0] = [0x%x],buffer1[1] = [0x%x] buffer1[2] = [0x%x]  buffer1[3] = [0x%x]\n",
     __func__, id, buffer[0], buffer[1], buffer[2], buffer[3], buffer1[0], buffer1[1], buffer1[2], buffer1[3]);
#else
    printk
    (" [ili9806e_boe397_xyl] %s, id = [0x%x],buffer[0] = [0x%x],buffer[1] = [0x%x] buffer[2] = [0x%x]  buffer[3] = [0x%x] buffer1[0] = [0x%x],buffer1[1] = [0x%x] buffer1[2] = [0x%x]  buffer1[3] = [0x%x]\n",
     __func__, id, buffer[0], buffer[1], buffer[2], buffer[3], buffer1[0], buffer1[1], buffer1[2], buffer1[3]);
#endif

    return (LCM_ILI9806_CPT_CLY_AUO_YKL_ID == id) ? 1 : 0;
}

LCM_DRIVER ili9806e_boe397_jingtai_wvga_lcm_drv =
{
    .name			= "ili9806e_boe397_jingtai_wvga",
    .set_util_funcs = lcm_set_util_funcs,
    .compare_id    = lcm_compare_id,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .init_power        = lcm_init_power,
    .resume_power = lcm_resume_power,
    .suspend_power = lcm_suspend_power,

#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
};

