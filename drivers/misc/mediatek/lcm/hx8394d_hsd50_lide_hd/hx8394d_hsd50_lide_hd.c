#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
//#include <cust_adc.h>
#define MIN_VOLTAGE (100)
#define MAX_VOLTAGE (300)
#define LCM_ID (0x94)

#define FRAME_WIDTH  				(720)
#define FRAME_HEIGHT 				(1280)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE			0

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) 					(lcm_util.udelay(n))
#define MDELAY(n) 					(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg                                            lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       

#define AUXADC_LCM_VOLTAGE_CHANNEL                          12
extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);

#ifndef BUILD_LK
extern atomic_t ESDCheck_byCPU;
#endif

struct LCM_setting_table {
	unsigned cmd;
	unsigned char count;
	unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
	{0xB9,3,{0xFF,0x83,0x94}},
	{0xBA,2,{0x73,0x83}},
	{0xB1,15,{0x6A,0x15,0x15,0x13,0x04,0x11,0xF1,0x80,0xEC,0x55,0x23,0x80,0xC0,0xD2,0x58}},
	{0xB2,11,{0x00,0x64,0x10,0x07,0x12,0x1C,0x08,0x08,0x1C,0x4D,0x00}},
	{0xB4,12,{0x00,0xFF,0x03,0x5A,0x03,0x5A,0x03,0x5A,0x01,0x6A,0x01,0x6A}},
	{0xBC,1,{0x07}}, 
	{0xBF,3,{0x41,0x0E,0x01}},//HX5186
	{0xD2,1,{0x55}}, 
	{0xD3,30,{0x00,0x0F,0x00,0x40,0x1A,0x08,0x00,0x32,0x10,0x07,0x00,0x07,0x54,0x15,0x0F,0x05,0x04,0x02,0x12,0x10,0x05,0x07,0x33,0x33,0x0B,0x0B,0x37,0x10,0x07,0x07}},
	{0xD5,44,{0x19,0x19,0x18,0x18,0x1B,0x1B,0x1A,0x1A,0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,0x20,0x21,0x18,0x18,0x22,0x23,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
	{0xD6,44,{0x18,0x18,0x19,0x19,0x1B,0x1B,0x1A,0x1A,0x03,0x02,0x01,0x00,0x07,0x06,0x05,0x04,0x23,0x22,0x18,0x18,0x21,0x20,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
	{0xB6,2,{0x82,0x82}},
	{0xE0,42,{0x00,0x03,0x09,0x2D,0x33,0x3F,0x16,0x38,0x06,0x0A,0x0C,0x18,0x0D,0x11,0x13,0x11,0x11,0x06,0x10,0x12,0x16,0x00,0x03,0x09,0x2D,0x33,0x3F,0x16,0x38,0x06,0x0A,0x0C,0x18,0x0D,0x11,0x13,0x11,0x11,0x06,0x10,0x12,0x16}},
	{0xC0,2,{0x30,0x14}},
	{0xC7,4,{0x00,0xC0,0x01,0xC0}},
	{0xCC,1,{0x09}}, 
	{0xDF,1,{0x88}}, 

	{0X11, 0 ,{0x00}},
	{REGFLAG_DELAY, 120, {}},
	{0X29, 0 ,{0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28,1,{0x00}},
	{REGFLAG_DELAY, 50, {}},
	// Sleep Mode On
	{0x10,1,{0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

	for(i = 0; i < count; i++)
	{
		unsigned cmd;
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
	params->dbi.te_mode 		=LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity    = LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode = CMD_MODE;
#else
	params->dsi.mode   = SYNC_PULSE_VDO_MODE;//SYNC_EVENT_VDO_MODE;//SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;
#endif

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM		    = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	// Not support in MT6573
	params->dsi.packet_size = 230;

	// Video mode setting
	params->dsi.intermediat_buffer_num = 0;//2;

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 720 * 3;

	params->dsi.vertical_sync_active 				= 2;//4;4
	params->dsi.vertical_backporch 					= 16;//16;//16 12
	params->dsi.vertical_frontporch 				= 9;//20;//20 18 10
	params->dsi.vertical_active_line 				= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active				= 80;
	params->dsi.horizontal_backporch 				= 80;
	params->dsi.horizontal_frontporch 				= 20;
	params->dsi.horizontal_active_pixel 				= FRAME_WIDTH;

	// Bit rate calculation
	/*
	   params->dsi.pll_div1=1;             // div1=0,1,2,3;div1_real=1,2,4,4
	   params->dsi.pll_div2=0;             // div2=0,1,2,3;div2_real=1,2,4,4
	   params->dsi.fbk_div =15;   	    //10.11.12/48,13/55,15/62,17/70
	 */

	params->dsi.PLL_CLOCK=220;															

	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.cont_clock=0;// 1
	params->dsi.lcm_esd_check_table[0].cmd = 0x9;//09 ,45,d9
	params->dsi.lcm_esd_check_table[0].count = 3;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x80;
	params->dsi.lcm_esd_check_table[0].para_list[1] = 0x73;
	params->dsi.lcm_esd_check_table[0].para_list[2] = 0x4;//TE off //0x6 {0x35,1,{0x00}},te

	params->dsi.lcm_esd_check_table[1].cmd = 0xd9;
	params->dsi.lcm_esd_check_table[1].count = 1;
	params->dsi.lcm_esd_check_table[1].para_list[0] = 0x80;
	//params->dsi.lcm_esd_check_table[1].para_list[1] = 0x00;
	//params->dsi.lcm_esd_check_table[1].para_list[2] = 0x45;

	//params->dsi.lcm_esd_check_table[2].cmd = 0x45;
	//params->dsi.lcm_esd_check_table[2].count = 2;
	//params->dsi.lcm_esd_check_table[2].para_list[0] = 0x5;
	//params->dsi.lcm_esd_check_table[2].para_list[1] = 0x19;

	// A: XLLLB-253 daizhiyi 20160627 {
	params->physical_width = 75;
	params->physical_height = 133;
	// A: }
}

//HIMAX Erin 20161010
static void init_lcm_registers(void)
{

	         
			 unsigned int data_array[19];
		 
			data_array[0]=0x00043902;
data_array[1]=0x9483FFB9;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00033902;
data_array[1]=0x008373BA;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00103902;
data_array[1]=0x15156AB1;
data_array[2]=0xF1110413;
data_array[3]=0x2355EC80;
data_array[4]=0x58D2C080;
dsi_set_cmdq(&data_array,5, 1);

data_array[0]=0x000C3902;
data_array[1]=0x106400B2;
data_array[2]=0x081C1207;
data_array[3]=0x004D1C08;
dsi_set_cmdq(&data_array,4, 1);

data_array[0]=0x000D3902;
data_array[1]=0x03FF00B4;
data_array[2]=0x035A035A;
data_array[3]=0x016A015A;
data_array[4]=0x0000006A;
dsi_set_cmdq(&data_array,5, 1);

data_array[0]=0x00023902;
data_array[1]=0x000007BC;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00043902;
data_array[1]=0x010E41BF;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00023902;
data_array[1]=0x000055D2;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x001F3902;
data_array[1]=0x000F00D3;
data_array[2]=0x00081A40;
data_array[3]=0x00081032;
data_array[4]=0x0F155408;
data_array[5]=0x12020405;
data_array[6]=0x23070510;
data_array[7]=0x270C0C22;
data_array[8]=0x00070710;
dsi_set_cmdq(&data_array,9, 1);
MDELAY(5);

data_array[0]=0x002D3902;
data_array[1]=0x181919D5;
data_array[2]=0x1A1B1B18;
data_array[3]=0x0605041A;
data_array[4]=0x02010007;
data_array[5]=0x18212003;
data_array[6]=0x18232218;
data_array[7]=0x18181818;
data_array[8]=0x18181818;
data_array[9]=0x18181818;
data_array[10]=0x18181818;
data_array[11]=0x18181818;
data_array[12]=0x00000018;
dsi_set_cmdq(&data_array,13, 1);
MDELAY(5);

data_array[0]=0x002D3902;
data_array[1]=0x191818D6;
data_array[2]=0x1A1B1B19;
data_array[3]=0x0102031A;
data_array[4]=0x05060700;
data_array[5]=0x18222304;
data_array[6]=0x18202118;
data_array[7]=0x18181818;
data_array[8]=0x18181818;
data_array[9]=0x18181818;
data_array[10]=0x18181818;
data_array[11]=0x18181818;
data_array[12]=0x00000018;
dsi_set_cmdq(&data_array,13, 1);
MDELAY(5);

data_array[0]=0x00033902;
data_array[1]=0x008282B6;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x002B3902;
data_array[1]=0x090300E0;
data_array[2]=0x163F332D;
data_array[3]=0x0C0A0638;
data_array[4]=0x13110D18;
data_array[5]=0x10061111;
data_array[6]=0x03001612;
data_array[7]=0x3F332D09;
data_array[8]=0x0A063816;
data_array[9]=0x110D180C;
data_array[10]=0x06111113;
data_array[11]=0x00161210;
dsi_set_cmdq(&data_array,12, 1);
MDELAY(5);

data_array[0]=0x00033902;
data_array[1]=0x001430C0;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00053902;
data_array[1]=0x00C000C7;
data_array[2]=0x000000C0;
dsi_set_cmdq(&data_array,3, 1);

data_array[0]=0x00023902;
data_array[1]=0x000009CC;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00023902;
data_array[1]=0x000088DF;
dsi_set_cmdq(&data_array,2, 1);

data_array[0]=0x00110500;
dsi_set_cmdq(&data_array,1, 1);
MDELAY(120);

data_array[0]=0x00290500;
dsi_set_cmdq(&data_array,1, 1);
MDELAY(20);



}







static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(120);
	
	init_lcm_registers();

/*
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);

*/
}

static void lcm_suspend(void)
{

	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10); 
	SET_RESET_PIN(1);
	MDELAY(120);

	//mt_set_gpio_out(GPIO_LCM_PWR_EN,GPIO_OUT_ZERO);
	//mt_set_gpio_out(GPIO_LCM_PWR2_EN,GPIO_OUT_ZERO);
}


static void lcm_resume(void)
{

	lcm_init();
}

#if LCM_DSI_CMD_MODE
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
#endif


static unsigned int lcm_compare_id(void)
{

	unsigned int id = 0;

	unsigned char buffer[3];
	unsigned int data_array[16];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(50);

	data_array[0]=0x00043902;
	data_array[1]=0x9483FFB9;// page enable
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(10);

	data_array[0]=0x00023902;
	data_array[1]=0x000013ba;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(10);

	data_array[0] = 0x00023700;// return byte number
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);

	read_reg_v2(0xF4, buffer, 2);
	id = buffer[0];

#ifdef BUILD_LK
	printf("%s, LK  debug:id = 0x%08x\n", __func__, id);
#else
	printk("%s, kernel debug:id = 0x%08x\n", __func__, id);
#endif

	return (LCM_ID == id) ? 1 : 0;
}

#if 0
static unsigned int rgk_lcm_compare_id(void)
{
	int data[4] = {0,0,0,0};
	int lcm_vol = 0;

#ifdef AUXADC_LCM_VOLTAGE_CHANNEL
	int res = 0;
	int rawdata = 0;
	res = IMM_GetOneChannelValue(AUXADC_LCM_VOLTAGE_CHANNEL,data,&rawdata);
	if(res < 0)
	{ 
#ifdef BUILD_LK
		printf("[adc_uboot]: get data error\n");
#endif
		return 0;

	}
#endif

	lcm_vol = data[0]*1000+data[1]*10;

#ifdef BUILD_LK
	printf("[adc_uboot]: lcm_vol= %d\n",lcm_vol);
#else
	printk("[adc_kernel]: lcm_vol= 0x%x\n",lcm_vol);
#endif
	if (lcm_vol>=MIN_VOLTAGE &&lcm_vol <= MAX_VOLTAGE && lcm_compare_id())
	{
		return 1;
	}

	return 0;

}
#endif
static unsigned int lcm_ata_check(unsigned char *buffer)
{
#ifndef BUILD_LK
	unsigned int id=0;
	unsigned char buf[8];
	unsigned int array[16];  

	array[0]=0x00043902;
	array[1]=0x9483FFB9;// page enable
	dsi_set_cmdq(array, 2, 1);
	MDELAY(10);

	array[0]=0x00023902;
	array[1]=0x000013ba;
	dsi_set_cmdq(array, 2, 1);
	MDELAY(10);

	array[0] = 0x00023700;// return byte number
	dsi_set_cmdq(array, 1, 1);
	MDELAY(10);
	atomic_set(&ESDCheck_byCPU,1);
	read_reg_v2(0xF4, buf, 2);
	atomic_set(&ESDCheck_byCPU,0);
	id = buf[0];

#ifdef BUILD_LK
	printf("%s, LK  debug:id = 0x%08x\n", __func__, id);
#else
	printk("%s, kernel debug:id = 0x%08x\n", __func__, id);
#endif

	if(id == LCM_ID)
		return 1;
	else
		return 0;

#else
	return 0;
#endif
}

LCM_DRIVER hx8394d_hsd50_lide_hd_lcm_drv =
{
	.name           = "hx8394d_hsd50_lide_hd",
	.set_util_funcs    = lcm_set_util_funcs,
	.get_params        = lcm_get_params,
	.init              = lcm_init,
	.suspend           = lcm_suspend,
	.resume            = lcm_resume,
	.compare_id        = lcm_compare_id,
	//.esd_check       = lcm_esd_check,
	//.esd_recover     = lcm_esd_recover,
	.ata_check        = lcm_ata_check,
#if (LCM_DSI_CMD_MODE)
	.update = lcm_update,
#endif
};
