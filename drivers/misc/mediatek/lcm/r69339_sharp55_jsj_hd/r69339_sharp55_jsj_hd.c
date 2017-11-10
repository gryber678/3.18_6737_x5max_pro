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

#define REGFLAG_DELAY           	(0XFEF)
#define REGFLAG_END_OF_TABLE    	(0xFFF)	// END OF REGISTERS MARKER

#define R69339_LCM_ID 		        (0x9339)

// ---------------------------------------------------------------------------
// Local Variables
// 

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v) (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// 
// Local Functions
// 

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

static void lcm_init_power(void)
{
  SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(100);
}

static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
  SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(100);
}



static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
    {0x01, 1, {0x00}},
    {REGFLAG_DELAY, 100, {}},
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},

	// Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count,
		unsigned char force_update)
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


// 
// LCM Driver Implementations
// 

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

  params->dsi.mode = BURST_VDO_MODE;


	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;
	// Highly depends on LCD driver capability.

	params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage


	// Video mode setting
	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
    params->dsi.word_count = 720 * 3;

    params->dsi.vertical_sync_active				= 2;
    params->dsi.vertical_backporch					= 11;
    params->dsi.vertical_frontporch					= 14;
    params->dsi.vertical_active_line                = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active              = 8;
    params->dsi.horizontal_backporch				= 80;
    params->dsi.horizontal_frontporch				= 80;
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
	/***********************/
  params->dsi.PLL_CLOCK = 221;
#ifdef VANZO_LCM_ESD_CHECK_SUPPORT
    params->dsi.noncont_clock = TRUE;
	params->dsi.noncont_clock_period =1;
  /***************************/
    params->dsi.esd_check_enable = 1;
    params->dsi.customization_esd_check_enable = 1;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
#endif
}

static void lcm_init(void)
{

	unsigned int data_array[16];

	SET_RESET_PIN(1);
    MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(120);



	data_array[0] = 0x00022902; 
	data_array[1] = 0x000004b0; 
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0]=0x00022902;
	data_array[1]=0x000001D6;
	dsi_set_cmdq(data_array,2, 1);



	data_array[0] = 0x00062902; 
    data_array[1] = 0x00001cb3; 
	data_array[2] = 0x00000000; 
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00032902; 
    data_array[1] = 0x00000CB4; 
	dsi_set_cmdq(data_array, 2, 1);

    data_array[0] = 0x00032902; 
    data_array[1] = 0x00B309B6; 
    dsi_set_cmdq(data_array, 2, 1);	

	data_array[0] = 0x00252902; 
	data_array[1] = 0x00A28CC1; 
	data_array[2] = 0xBC91C3D7; 
	data_array[3] = 0x260507CB; 
	data_array[4] = 0x00BABA06; 
	data_array[5] = 0x00000000; 
	data_array[6] = 0x00000001; 
	data_array[7] = 0x40306200; 
	data_array[8] = 0x00040FA5; 
	data_array[9] = 0x00000000; 
	data_array[10] = 0x00000000; 
	dsi_set_cmdq(data_array, 11, 1);


	data_array[0] = 0x00082902; 
	data_array[1] = 0x00F500C2; 
	data_array[2] = 0x00000D0C; 
	dsi_set_cmdq(data_array, 3, 1);

    data_array[0] = 0x000C2902; 
    data_array[1] = 0x0F0070C4; 
    data_array[2] = 0x01000000; 
    data_array[3] = 0x01000001;
    dsi_set_cmdq(data_array, 4, 1);

    data_array[0] = 0x00062902; 
    data_array[1] = 0x7A7AB3C6; 
    data_array[2] = 0x00000000; 
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0] = 0x00272902; 
    data_array[1] = 0x1C130CC7; 
    data_array[2] = 0x4B413325; 
    data_array[3] = 0x5246405B; 
    data_array[4] = 0x736C665C; 
    data_array[5] = 0x00000000; 
    data_array[6] = 0x20170F04; 
    data_array[7] = 0x5B4A3F30; 
    data_array[8] = 0x60544841; 
    data_array[9] = 0x00737068; 
    data_array[10] = 0x00000000; 
    dsi_set_cmdq(data_array, 11, 1);

    data_array[0] = 0x000E2902;
    data_array[1] = 0x353300CB;
    data_array[2] = 0x00000CEB;
    data_array[3] = 0x00000000;
    data_array[4] = 0x00000000;
    dsi_set_cmdq(data_array, 5, 1);

    data_array[0] = 0x00022902;
    data_array[1] = 0x000003CC;
    dsi_set_cmdq(data_array, 2, 1);

    data_array[0] = 0x000B2902; 
    data_array[1] = 0x5E4311D0; 
    data_array[2] = 0x19194C9E; 
    data_array[3] = 0x00AA080C;
    dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x001A2902; 
	data_array[1] = 0x80001CD1; 
	data_array[2] = 0x08080804; 
	data_array[3] = 0x2057C000; 
	data_array[4] = 0x00D0021E; 
	data_array[5] = 0x08080480; 
	data_array[6] = 0x1E000008; 
	data_array[7] = 0x00000003;  
	dsi_set_cmdq(data_array, 8, 1);


	data_array[0] = 0x00192902; 
	data_array[1] = 0xBB330BD3; 
	data_array[2] = 0x3333B3BB; 
	data_array[3] = 0x93888033; 
	data_array[4] = 0x334ACA47; 
	data_array[5] = 0x3DF2F733; 
	data_array[6] = 0x000000BC; 
	data_array[7] = 0x00000000;  
	dsi_set_cmdq(data_array, 8, 1);

	data_array[0] = 0x000D2902; 
	data_array[1] = 0x00001ED5; 
	data_array[2] = 0x90019001; 
	data_array[3] = 0x00000001; 
	data_array[4] = 0x00000000; 
	dsi_set_cmdq(data_array, 5, 1);

    data_array[0] = 0x00290500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(100);

	data_array[0] = 0x00110500; // Sleep Out
	dsi_set_cmdq(data_array, 1, 1);
    MDELAY(200);
}

static void lcm_suspend(void)
{

	unsigned int data_array[16];

	data_array[0]=0x00280500; 
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(30);

	data_array[0] = 0x00100500; 
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);//delay more for 3 frames time  17*3=54ms

	data_array[0] = 0x00B02300;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01B12300;//Deep standby
	dsi_set_cmdq(data_array, 1, 1);
    	MDELAY(30);

	SET_RESET_PIN(0);
	MDELAY(50);
}


static unsigned int lcm_compare_id(void);
static void lcm_resume(void)
{
/*
	push_table(lcm_sleep_out_setting,
			sizeof(lcm_sleep_out_setting) /
			sizeof(struct LCM_setting_table), 1);
	*/
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
	
	
	SET_RESET_PIN(1);
	MDELAY(25);
	SET_RESET_PIN(0);
	MDELAY(25);
	SET_RESET_PIN(1);
    MDELAY(150);


	array[0] = 0x00022902;                          
	array[1] = 0x000000b0; 
	dsi_set_cmdq(array, 2, 1);


	array[0] = 0x00053700;// read id return two byte,version and id

	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xbf, buffer, 5);

	id = buffer[2]<<8 | buffer[3]; //we only need ID

    #ifdef BUILD_LK
		printf("%s, R69339-=-LCM-LK: id= x%08x,id0 = 0x%08x,id1 = 0x%08x,id2 = 0x%08x,id3 = 0x%08x,id4 = 0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
    #else
		printk("%s, R69339-=-LCM-KERNEL: id= x%08x,id0 = 0x%08x,id1 = 0x%08x,id2 = 0x%08x,id3 = 0x%08x,id4 = 0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
    #endif


	return (R69339_LCM_ID == id)?1:0;
}

LCM_DRIVER r69339_sharp55_jsj_hd_lcm_drv =
{
	.name = "r69339_sharp55_jsj_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
	.compare_id = lcm_compare_id,
    .init_power     = lcm_init_power,
    .resume_power   = lcm_resume_power,
    .suspend_power  = lcm_suspend_power,
#if (LCM_DSI_CMD_MODE)
	.update = lcm_update,
#endif //wqtao
};

