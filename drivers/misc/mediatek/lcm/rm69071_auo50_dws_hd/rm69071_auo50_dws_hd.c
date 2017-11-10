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

#define FRAME_WIDTH  			(720)
#define FRAME_HEIGHT 			(1280)



// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util;
static  int  VOL_2V9=20;
static int currentBrightness;

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define REGFLAG_DELAY             								0xFC
#define REGFLAG_UDELAY             								0xFB

#define REGFLAG_END_OF_TABLE      							    	0xFD  

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmd_by_cmdq_dual(handle,cmd,count,ppara,force_update)    lcm_util.dsi_set_cmdq_V23(handle,cmd,count,ppara,force_update);
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	(lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update))
#define dsi_set_cmdq(pdata, queue_size, force_update)		(lcm_util.dsi_set_cmdq(pdata, queue_size, force_update))
#define wrtie_cmd(cmd)										(lcm_util.dsi_write_cmd(cmd))
#define write_regs(addr, pdata, byte_nums)					(lcm_util.dsi_write_regs(addr, pdata, byte_nums))
#define read_reg(cmd)										(lcm_util.dsi_dcs_read_lcm_reg(cmd))
#define read_reg_v2(cmd, buffer, buffer_size)   			(lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size))

#define dsi_lcm_set_gpio_out(pin, out)						(lcm_util.set_gpio_out(pin, out))
#define dsi_lcm_set_gpio_mode(pin, mode)					(lcm_util.set_gpio_mode(pin, mode))
#define dsi_lcm_set_gpio_dir(pin, dir)						(lcm_util.set_gpio_dir(pin, dir))
#define dsi_lcm_set_gpio_pull_enable(pin, en)				(lcm_util.set_gpio_pull_enable(pin, en))



#define   LCM_DSI_CMD_MODE							(0)

struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {

	{0xFE, 1,  {0x07}},
	{0xA9, 1,  {0x6A}},	
	{0xFE, 1,  {0x08}},	
	{0x03, 1,  {0x40}},	
	{0x07, 1,  {0x1A}},	
	{0xFE, 1,  {0x00}},	
	{0x35, 1,  {0x01}},	
	{0x55, 1,  {0x00}},
	{0x51, 1,  {0x00}},	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_poweron_setting[] = {

	{0xFE, 1,  {0x07}},
	{0xA9, 1,  {0xFA}},
	//{REGFLAG_UDELAY, 16667, {}},
	{REGFLAG_DELAY, 17, {}},	
	{0xFE, 1,  {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};
static struct LCM_setting_table lcm_poweroff_setting[] = {

	{0xFE, 1,  {0x07}},
	{0xA9, 1,  {0x6A}},
	//{REGFLAG_UDELAY, 16667, {}},
	{REGFLAG_DELAY, 17, {}},
	{0xFE, 1,  {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};
static struct LCM_setting_table lcm_sleepout[] = {

	{0x11, 1,  {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};
static struct LCM_setting_table lcm_sleepin[] = {

	{0x10, 1,  {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};
static struct LCM_setting_table lcm_displayon[] = {

	{0x29, 1,  {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};
static struct LCM_setting_table lcm_displayoff[] = {

	{0x28, 1,  {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}	
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {
        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {

            	case REGFLAG_DELAY :
              		if(table[i].count <= 10)
                    		MDELAY(table[i].count);
                	else
                    		MDELAY(table[i].count);
                	break;
				
		case REGFLAG_UDELAY :
			UDELAY(table[i].count);
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

	params->dbi.te_mode = LCM_DBI_TE_MODE_VSYNC_ONLY;
	params->dbi.te_edge_polarity = LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode = CMD_MODE;
#else
	params->dsi.mode = SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;
#endif

	// DSI
	/* Command mode setting */
	//1 Three lane or Four lane
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	
	params->dsi.vertical_sync_active = 2;
	params->dsi.vertical_backporch = 6;
	params->dsi.vertical_frontporch = 16;
	params->dsi.vertical_active_line = FRAME_HEIGHT;


	params->dsi.horizontal_sync_active = 8;
	params->dsi.horizontal_backporch = 16;
	params->dsi.horizontal_frontporch = 32;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;
	//params->dsi.pll_select=1;     //0: MIPI_PLL; 1: LVDS_PLL
	// Bit rate calculation
	//1 Every lane speed
	params->dsi.PLL_CLOCK = 425;//?????????????200~220

}

static void lcm_init(void)
{	

/*
#ifdef BUILD_LK
	mt6331_upmu_set_rg_vgp1_en(1);
#else
	hwPowerOn(MT6331_POWER_LDO_VGP1, VOL_3000, "LCM_DRV");
#endif
*/
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);	
	MDELAY(50);	
	SET_RESET_PIN(1);
	MDELAY(120);	
	
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);	
	MDELAY(100);
	
	push_table(lcm_sleepout, sizeof(lcm_sleepout) / sizeof(struct LCM_setting_table), 1);	
	MDELAY(50);// time must equal to 50

	MDELAY(200);

	push_table(lcm_displayon, sizeof(lcm_displayon) / sizeof(struct LCM_setting_table), 1);	
       MDELAY(50);		
	
	MDELAY(100);		//20
	//set VENG to -2.9V
	
	push_table(lcm_poweron_setting, sizeof(lcm_poweron_setting) / sizeof(struct LCM_setting_table), 1);	

}

static void lcm_suspend(void)
{

	push_table(lcm_displayoff, sizeof(lcm_displayoff) / sizeof(struct LCM_setting_table), 1);	
	MDELAY(100);	
	

	push_table(lcm_poweroff_setting, sizeof(lcm_poweroff_setting) / sizeof(struct LCM_setting_table), 1);	
	MDELAY(100);	
	
	push_table(lcm_sleepin, sizeof(lcm_sleepin) / sizeof(struct LCM_setting_table), 1);	
	MDELAY(100);	


}


static void lcm_resume(void)
{
	int i;
	unsigned char tempBrightness;
	for(i=0;;i++)
	{	
		if(lcm_initialization_setting[i].cmd == 0x51)//brightness ctrl
		{	    
			tempBrightness = lcm_initialization_setting[i].para_list[0];
			lcm_initialization_setting[i].para_list[0] = currentBrightness;
			break;
		}
	}    
	lcm_init();
	for(i=0;;i++)
	{
		if(lcm_initialization_setting[i].cmd == 0x51)//brightness ctrl
		{	    
			lcm_initialization_setting[i].para_list[0] = tempBrightness;
			break;
		}    
	}
}

#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
		       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = (x0 & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = (x1 & 0xFF);
	unsigned char y0_MSB = ((y0 >> 8) & 0xFF);
	unsigned char y0_LSB = (y0 & 0xFF);
	unsigned char y1_MSB = ((y1 >> 8) & 0xFF);
	unsigned char y1_LSB = (y1 & 0xFF);

	unsigned int data_array[16];

	data_array[0] = 0x00053902;
	data_array[1] =
	    (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00053902;
	data_array[1] =
	    (y1_MSB << 24) | (y0_LSB << 16) | (y0_MSB << 8) | 0x2b;
	data_array[2] = (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);

}
#endif


static void lcm_setbacklight_cmdq(void* handle,unsigned int level)
{
#ifdef BUILD_LK
 	dprintf(0,"%s,lk RM69071 backlight: level = %d\n", __func__, level);
#else
 	printk("%s, kernel RM69071 backlight: level = %d\n", __func__, level);
#endif
 // Refresh value of backlight level.

	unsigned int cmd = 0x51;
	unsigned int count =1;
 	currentBrightness = level;
 	dsi_set_cmd_by_cmdq_dual(handle, cmd, count, &currentBrightness, 1);

} 


LCM_DRIVER rm69071_auo50_dws_hd_lcm_drv = {
	.name = "rm69071_auo50_dws_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
#if (LCM_DSI_CMD_MODE)
	.update = lcm_update,
#endif
//	.set_backlight_cmdq = lcm_setbacklight_cmdq,
};
