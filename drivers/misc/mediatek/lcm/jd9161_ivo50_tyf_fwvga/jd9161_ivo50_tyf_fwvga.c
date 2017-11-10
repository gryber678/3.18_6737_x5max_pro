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
#else
    #include <linux/string.h>
    #include <linux/kernel.h>
    #if defined(BUILD_LK)
        #include <asm/arch/mt_gpio.h>
    #else
       // #include <mach/mt_gpio.h>
    #endif
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


static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};
#if 0
static struct LCM_setting_table lcm_compare_id_setting[] = {
	{0xBF, 3, {0x91, 0x61, 0xF2}},
	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
#endif
//#if 0
static struct LCM_setting_table lcm_compare_id_setting[] = {
	{0xBF, 3, {0x91, 0x61, 0xF2}},
	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
//#endif

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


static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
// Sleep Out
	{0x11, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
// Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},
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



/*

static unsigned int var = 0x40;
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
	    					
	    					case 0xb3://  vcom 
                table[i].para_list[1] = var ;
                dsi_set_cmdq_V2(0xb3, table[i].count, table[i].para_list, force_update);
                var = var + 2;
                break;
                
                	
            default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
			// MDELAY(2);
       	}
    }
	
}

*/




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

	params->dsi.vertical_sync_active = 4;//6
	params->dsi.vertical_backporch = 16;//10 16 //6
	params->dsi.vertical_frontporch = 20;//8  20 6
	params->dsi.vertical_active_line = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active = 8;//10 //8
	params->dsi.horizontal_backporch = 80; //50
	params->dsi.horizontal_frontporch = 10; //60 //50
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;

	// Bit rate calculation
        //params->dsi.pll_div1=1;             // div1=0,1,2,3;div1_real=1,2,4,4
        //params->dsi.pll_div2=0;             // div2=0,1,2,3;div2_real=1,2,4,4
        //params->dsi.fbk_div =14;   //18
        params->dsi.esd_check_enable = 1;
        params->dsi.customization_esd_check_enable = 1;
        params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
        params->dsi.lcm_esd_check_table[0].count        = 1;
        params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
        params->dsi.PLL_CLOCK = 220;//185; //198
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);
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
	MDELAY(50);
	SET_RESET_PIN(1);
       MDELAY(120);

}

//static unsigned int flicker = 0x35;
static void lcm_resume(void)
{
	lcm_init();
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
	array[1]=0xf26191Bf;
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
LCM_DRIVER jd9161_ivo50_tyf_fwvga_lcm_drv =
{
    .name			= "jd9161_ivo50_tyf_fwvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
    //.init_power        = lcm_init_power,
    //.resume_power = lcm_resume_power,
    //.suspend_power = lcm_suspend_power,
};

