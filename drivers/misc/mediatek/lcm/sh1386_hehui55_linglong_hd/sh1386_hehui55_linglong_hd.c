#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <string.h>
#else
#include "disp_dts_gpio.h"
#endif

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
#ifndef BUILD_LK
extern  long disp_dts_gpio_select_state(DTS_GPIO_STATE s);
#endif

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY          		(0XFEE)
#define REGFLAG_END_OF_TABLE      	(0xFFFF)	// END OF REGISTERS MARKER

#ifndef FALSE
#define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = { 0 };

#define SET_RESET_PIN(v)    		(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 					(lcm_util.udelay(n))
#define MDELAY(n) 					(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


struct LCM_setting_table {
  unsigned cmd;
  unsigned char count;
  unsigned char para_list[64];
};

static struct LCM_setting_table lcm_backlight_level_setting_F0[1]={0xF0,{0x55,0xAA,0x52,0x08,0x02}};
static struct LCM_setting_table lcm_backlight_level_setting_C0[1]={0xC0,20,{0x03,0x00,0x06,0x07,0x08,0x09,0x00,0x00,0x00,0x00,0x02,0x00,0x0A,0x0B,0x0C,0x0D,0x00,0x00,0x00,0x00}};
static struct LCM_setting_table lcm_backlight_level_setting_C1[1]={0xc1,{0x00,0x00}};
unsigned char goasetflag=0;

static void lcm_init_power(void)
{
#if defined(GPIO_LCM_LED_EN)
  mt_set_gpio_mode(GPIO_LCM_LED_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_LCM_LED_EN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_LCM_LED_EN, GPIO_OUT_ONE);
#endif 
}


static void lcm_suspend_power(void)
{
#if defined(GPIO_LCM_LED_EN)
  mt_set_gpio_mode(GPIO_LCM_LED_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_LCM_LED_EN, GPIO_DIR_OUT);mt_set_gpio_out(GPIO_LCM_LED_EN, GPIO_OUT_ZERO);
#endif
}

static void lcm_resume_power(void)
{
#if defined(GPIO_LCM_LED_EN)
  mt_set_gpio_mode(GPIO_LCM_LED_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_LCM_LED_EN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_LCM_LED_EN, GPIO_OUT_ONE);
#endif
}
static struct LCM_setting_table lcm_initialization_setting[] = {   
 	{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
	{0xB0,4,{0x0F,0x0F,0x1E,0x14}},
	{0xB2,1,{0x00}},
	{0xB6,1,{0x03}},	
#ifndef BUILD_LK
	{0xC0,20,{0x03,0x00,0x06,0x07,0x08,0x09,0x00,0x00,
				 0x00,0x00,0x02,0x00,0x0A,0x0B,0x0C,0x0D,
				 0x00,0x00,0x00,0x00}},
#else
	{0xC0,20,{0x03,0x00,0x06,0x07,0x08,0x09,0x00,0x00,
	          0x00,0x00,0x02,0x00,0x0A,0x0B,0x0C,0x0D,
	          0x00,0x00,0x00,0x00}},
#endif
	{0xC1,16,{0x08,0x24,0x24,0x01,0x18,0x24,0x9f,0x85,0x08,0x24,
				 0x24,0x01,0x18,0x24,0x95,0x85}},
	{0xC2,24,{0x03,0x05,0x1B,0x24,0x13,0x31,0x01,0x05,
				 0x1B,0x24,0x13,0x31,0x03,0x05,0x1B,0x38,
				 0x00,0x11,0x02,0x05,0x1B,0x38,0x00,0x11}}, 
	{0xC3,24,{0x02,0x05,0x1B,0x24,0x13,0x11,0x03,0x05,
				 0x1B,0x24,0x13,0x11,0x03,0x05,0x1B,0x38,
				 0x00,0x11,0x02,0x05,0x1B,0x38,0x00,0x11}},		  
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}}, 
	{0xB5,1,{0x1E}}, 
	{0xB6,1,{0x2D}}, 
	{0xB7,1,{0x04}}, 
	{0xB8,1,{0x05}}, 
	{0xB9,1,{0x04}}, 
	{0xBA,1,{0x14}}, 
	{0xBB,1,{0x2f}}, 
	{0xBE,1,{0x12}}, 
	{0xC2,3,{0x00,0x35,0x07}}, 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}}, 
	{0xC9,1,{0x13}}, 
	{0xD4,3,{0x02,0x04,0x2C}}, 
	{0xE1,35,{0x00,0x91,0xAE,0xCB,0xe6,0x54,0xff,0x1e,0x33,0x43,
				 0x55,0x4f,0x66,0x78,0x8b,0x55,0x9d,0xac,0xc0,0xcf,
				 0x55,0xe0,0xe8,0xf2,0xfb,0xaa,0x03,0x0d,0x15,0x1f,
				 0xaa,0x27,0x2c,0x31,0x34}}, 
	{0xE2,35,{0x00,0xad,0xc6,0xe4,0xfd,0x55,0x11,0x2a,0x3b,0x49,
				 0x55,0x54,0x6b,0x7c,0x8f,0x55,0xa1,0xaf,0xc3,0xd1,
				 0x55,0xe2,0xea,0xf3,0xfc,0xaa,0x04,0x0e,0x15,0x20,
				 0xaa,0x28,0x2d,0x32,0x35}}, 
	{0xE3,35,{0x55,0x05,0x1E,0x37,0x4b,0x55,0x5a,0x64,0x72,0x7f,
				 0x55,0x8b,0xa3,0xb8,0xd1,0xa5,0xe4,0xf6,0x0e,0x23,
				 0xaa,0x39,0x42,0x4f,0x59,0xaa,0x64,0x70,0x7a,0x86,
				 0xaa,0x90,0x96,0x9c,0x9f}},
	{0x8F,6,{0x5A,0x96,0x3C,0xC3,0xA5,0x69}},
	{0x89,1,{0x00}},
	{0x8C,3,{0x55,0x49,0x53}},
	{0x9A,1,{0x5A}},
	{0xFF,4,{0xA5,0x5A,0x13,0x86}},
	{0xFE,2,{0x01,0x54}},
	{0x35,1,{0x00}},
	{0x11,0x01,{0x00}},
        
	{REGFLAG_DELAY, 150, {}},
	{0x29,0x01,{0x00}},
	{REGFLAG_DELAY, 50, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

//static unsigned int var = 0x70;
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
	    					
	    	#if 0
	    	case 0xd9://  vcom 
                table[i].para_list[0] = var ;
                dsi_set_cmdq_V2(0xd9, table[i].count, table[i].para_list, force_update);
                var = var + 1;
                break;
            #endif 
                	
            default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
			// MDELAY(2);
       	}
    }
	
}


#ifdef BUILD_LK
struct LCM_setting_table lk_backlight_initialization_setting[] = {   
  {0xF0,5,{0x55,0xAA,0x52,0x08,0x00}}, 
  {REGFLAG_DELAY,20, {}},
  {0xC0,4,{0x03,0x00,0x06,0x07}},
  {REGFLAG_DELAY,20, {}},
  {0xF0,5,{0x55,0xAA,0x52,0x08,0x02}}, 
  {REGFLAG_END_OF_TABLE,0x00,{}}
};
int _lk_setbacklight(int level)
{

  printf("ttttt----_lk_setbacklight out\n");
  push_table(lk_backlight_initialization_setting,sizeof(lk_backlight_initialization_setting) /sizeof(struct LCM_setting_table), 1);

}
#endif
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
/*
  params->lcm_if = LCM_INTERFACE_DSI_DUAL;
  params->lcm_cmd_if = LCM_INTERFACE_DSI0;
*/  
  // enable tearing-free
  params->dbi.te_mode = LCM_DBI_TE_MODE_DISABLED; //LCM_DBI_TE_MODE_VSYNC_ONLY;
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

  params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

  params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
  params->dsi.word_count = 720 * 3;
  params->dsi.packet_size = 512;
  params->dsi.vertical_sync_active				= 4;// 0x3;// 3    2
  params->dsi.vertical_backporch					= 30;// 0x0E;// 20   1
  params->dsi.vertical_frontporch					= 30;// 0x10; // 1  12
  params->dsi.vertical_active_line				= FRAME_HEIGHT;

  params->dsi.horizontal_sync_active			= 4;// 0x04;// 50  2
  params->dsi.horizontal_backporch				= 30;// 0x22 ;
  params->dsi.horizontal_frontporch				= 20;// 0x18 ;
  params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

  params->dsi.PLL_CLOCK =180;
 // params->dsi.ssc_disable = 1;  // ssc disable control (1: disable, 0: enable, default: 0)
  params->dsi.cont_clock=1;

}

static void lcm_init(void)
{
    unsigned int data_array[16];
    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);

    MDELAY(20);

#ifdef BUILD_LK
    mt_set_gpio_mode(GPIO95,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO95,GPIO_DIR_OUT);
    if(mt_set_gpio_out(GPIO95,GPIO_OUT_ONE)!=RSUCCESS){
        mt_set_gpio_out(GPIO95,GPIO_OUT_ONE);
    }
    mt_set_gpio_mode(GPIO94,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO94,GPIO_DIR_OUT);
    if(mt_set_gpio_out(GPIO94,GPIO_OUT_ONE)!=RSUCCESS){
        mt_set_gpio_out(GPIO94,GPIO_OUT_ONE);
    }
#else
    disp_dts_gpio_select_state(5);
    //disp_dts_gpio_select_state(7);
#endif
    push_table(lcm_initialization_setting,sizeof(lcm_initialization_setting) /sizeof(struct LCM_setting_table), 1);
}
static void lcm_setbacklight(unsigned int level)
{
#ifdef BUILD_LK
    _lk_setbacklight(128);
#else
    if(level<50) level=50;
    if(level>255) level=255;
    level = level*(1023-50)/255+50;
    lcm_backlight_level_setting_C1[0].para_list[0]=level>>8;
    lcm_backlight_level_setting_C1[0].para_list[1]=level&0xff;
    dsi_set_cmdq_V2(0xC1, 2,lcm_backlight_level_setting_C1[0].para_list, 1);// backlight adjust
#endif
#ifndef BUILD_LK
    printk(KERN_EMERG"ttttttttttttt  backlight level: %d ---%x ,%x\n",level,lcm_backlight_level_setting_C1[0].para_list[0],lcm_backlight_level_setting_C1[0].para_list[1]);
#endif
}
static void lcm_suspend(void)
{
  unsigned int data_array[16];
    data_array[0] = 0x00280500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(10);
    data_array[0] = 0x00100500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(20);
#ifdef BUILD_LK
    mt_set_gpio_mode(GPIO94,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO94,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO94,GPIO_OUT_ZERO);

    //mt_set_gpio_mode(GPIO95,GPIO_MODE_GPIO);
    //mt_set_gpio_dir(GPIO95,GPIO_DIR_OUT);
    //mt_set_gpio_out(GPIO95,GPIO_OUT_ZERO);
#else
	disp_dts_gpio_select_state(4);
	//disp_dts_gpio_select_state(6);
    printk(KERN_EMERG"-----------qingzhan suspend: \n");
#endif
	SET_RESET_PIN(0);
}

static unsigned int lcm_compare_id(void);
static void lcm_resume(void)
{
#ifndef BUILD_LK
    printk("kls resume\n");
#endif
    /*
  unsigned int data_array[16];
    data_array[0] = 0x00110500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(10);
    data_array[0] = 0x00290500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(120);
*/
	lcm_init();
	goasetflag=0xeb;
}




static unsigned int lcm_compare_id(void)
{
  /* unsigned int id0, id1, id2, id3, id4; */
  unsigned char buffer[5];
  unsigned int array[5];

  int id = 0;
  SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(10);

  // Set Maximum return byte = 1
  array[0] = 0x00053700;
  dsi_set_cmdq(array, 1, 1);

  read_reg_v2(0xA1, buffer, 5);
  /* id_adc = adc_read_vol(); */
  /* id0 = buffer[0]; */
  /* id1 = buffer[1]; */
  /* id2 = buffer[2]; */
  /* id3 = buffer[3]+id_adc; */
  /* id4 = buffer[4]; */

  //id = (buffer[2] << 8) | buffer[3] + adc_read_vol();
  id = (buffer[2] << 8) | buffer[3];

#if defined(BUILD_LK)
  printf("%s, ID = %x, {%x, %x, %x, %x, %x} \n", __func__,
      id, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
#else
  printk("%s, ID = %x, {%x, %x, %x, %x, %x} \n", __func__,
      id, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
#endif

  return 1;
}

LCM_DRIVER sh1386_hehui55_linglong_hd_lcm_drv= {
  .name = "sh1386_hehui55_linglong_hd",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params     = lcm_get_params,
  .init           = lcm_init,
  .suspend        = lcm_suspend,
  .resume         = lcm_resume,
  .compare_id     = lcm_compare_id,
  .set_backlight  = lcm_setbacklight,
  .init_power        = lcm_init_power,
  .resume_power = lcm_resume_power,
  .suspend_power = lcm_suspend_power,

};
