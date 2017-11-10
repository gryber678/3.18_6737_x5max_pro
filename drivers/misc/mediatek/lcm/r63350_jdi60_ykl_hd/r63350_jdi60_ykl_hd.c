#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#include <asm-generic/gpio.h>

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#endif
#endif
#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#else
#include <mt-plat/mt_gpio.h>
#include <mach/gpio_const.h>
#endif
#include "lcm_drv.h"
/*#include "ddp_irq.h"*/


#ifdef BUILD_LK
#define GPIO_LCD_PWR      GPIO_LCM_PWR
static void lcm_set_gpio_output(unsigned int GPIO, unsigned int output)
{
    mt_set_gpio_mode(GPIO, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO, (output > 0) ? GPIO_OUT_ONE : GPIO_OUT_ZERO);
}
#endif

#ifdef BUILD_LK
#define LCD_DEBUG(fmt)  dprintf(CRITICAL,fmt)
#else
#define LCD_DEBUG(fmt)  pr_err(fmt)
#endif


#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#if 1
#define TPS_I2C_ID_NAME "I2C_LCD_BIAS"
/*
#define TPS_I2C_BUSNUM  1//I2C_I2C_LCD_BIAS_CHANNEL//for I2C channel 0
#define TPS_ADDR 0x3E
static struct i2c_board_info __initdata tps65132_board_info = {I2C_BOARD_INFO(TPS_I2C_ID_NAME, TPS_ADDR)};
*/
static struct i2c_client *tps65132_i2c_client = NULL;
static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tps65132_remove(struct i2c_client *client);
/*
   struct tps65132_dev    {
   struct i2c_client   *client;
   };
 */

static const struct i2c_device_id tps65132_id[] =
{
    { TPS_I2C_ID_NAME, 0 },
    { }
};

#ifdef CONFIG_OF
static const struct of_device_id lcm_of_match[] =
{
    { .compatible = "mediatek,I2C_LCD_BIAS", },
    {},
};

MODULE_DEVICE_TABLE(of, lcm_of_match)
#endif

static struct i2c_driver tps65132_iic_driver =
{
    .id_table   = tps65132_id,
    .probe      = tps65132_probe,
    .remove     = tps65132_remove,
    .driver     = {
        .owner	= THIS_MODULE,
        .name   = TPS_I2C_ID_NAME,
#ifdef CONFIG_OF
        .of_match_table = lcm_of_match,
#endif
    },
};

static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    pr_err("tps65132_iic_probe\n");
    pr_err("TPS: info==>name=%s addr=0x%x\n",client->name,client->addr);
    tps65132_i2c_client  = client;
    return 0;
}

static int tps65132_remove(struct i2c_client *client)
{
    pr_err( "tps65132_remove\n");
    tps65132_i2c_client = NULL;
    i2c_unregister_device(client);
    return 0;
}

static int tps65132_write_bytes(unsigned char addr, unsigned char value)
{
    int ret = 0;
    struct i2c_client *client = tps65132_i2c_client;
    char write_data[2]= {0};
    write_data[0]= addr;
    write_data[1] = value;
    ret=i2c_master_send(client, write_data, 2);
    if(ret<0)
        pr_err("tps65132 write data fail !!\n");
    return ret ;
}

static int __init tps65132_iic_init(void)
{
    pr_err( "tps65132_iic_init\n");
    //i2c_register_board_info(TPS_I2C_BUSNUM, &tps65132_board_info, 1);
    //pr_err( "tps65132_iic_init2\n");
    if(i2c_add_driver(&tps65132_iic_driver))
    {
        pr_err( "add tps65132 error\n");
        return -1;
    }
    pr_err( "tps65132_iic_init success\n");
    return 0;
}

static void __exit tps65132_iic_exit(void)
{
    pr_err( "tps65132_iic_exit\n");
    i2c_del_driver(&tps65132_iic_driver);
}
module_init(tps65132_iic_init);
module_exit(tps65132_iic_exit);
MODULE_AUTHOR("Xiaokuan Shi");
MODULE_DESCRIPTION("MTK TPS65132 I2C Driver");
MODULE_LICENSE("GPL");
//TPS65132_i2c.id = I2C_CHANNEL_0; //I2C_I2C_LCD_BIAS_CHANNEL;//I2C1;
#endif
#endif
/* --------------------------------------------------------------------------- */
/* Local Constants */
/* --------------------------------------------------------------------------- */

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)
#define LCM_ID_R63350                                     (0x3350)
#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0


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

/* --------------------------------------------------------------------------- */
/* Local Functions */
/* --------------------------------------------------------------------------- */
#ifdef BUILD_LK

#define TPS65132_SLAVE_ADDR_WRITE  0x7C
static struct mt_i2c_t TPS65132_i2c;

static int TPS65132_write_byte(kal_uint8 addr, kal_uint8 value)
{
    kal_uint32 ret_code = I2C_OK;
    kal_uint8 write_data[2];
    kal_uint16 len;

    write_data[0]= addr;
    write_data[1] = value;

    TPS65132_i2c.id = 1;//I2C_I2C_LCD_BIAS_CHANNEL;//I2C2;
    /* Since i2c will left shift 1 bit, we need to set FAN5405 I2C address to >>1 */
    TPS65132_i2c.addr = (TPS65132_SLAVE_ADDR_WRITE >> 1);
    TPS65132_i2c.mode = ST_MODE;
    TPS65132_i2c.speed = 100;
    len = 2;
    ret_code = i2c_write(&TPS65132_i2c, write_data, len);

    return ret_code;
}

#endif

/*
static void lcm_suspend_power(void)
{
#ifdef BUILD_LK
	printf("[LK/LCM] lcm_suspend_power() enter\n");
	lcm_set_gpio_output(GPIO_LCD_PWR, GPIO_OUT_ZERO);
	MDELAY(20);

#else
	printk("[Kernel/LCM] lcm_suspend_power() enter\n");
	lcm_set_gpio(0);
	MDELAY(20);
#endif
}

static void lcm_resume_power(void)
{
#ifdef BUILD_LK
	printf("[LK/LCM] lcm_resume_power() enter\n");
	lcm_set_gpio_output(GPIO_LCD_PWR, GPIO_OUT_ONE);
	MDELAY(20);

#else
	printk("[Kernel/LCM] lcm_resume_power() enter\n");
	lcm_set_gpio(1);
	MDELAY(20);
#endif
}
*/
struct LCM_setting_table
{
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

/*
static struct LCM_setting_table lcm_initialization_setting[] =
{
    {0xB0,1,{0x04}},
    {0xD6,1,{0x01}},
    {0xB3,6,{0x14,0x00,0x00,0x00,0x00,0x00}},
    {0xB4,2,{0x0C,0x00}},
    {0xB6,3,{0x49,0xBB,0x00}},
    {0xC1,35,{0x04,0x66,0x00,0xFF,0x93,0x2F,0x0A,0xF8,0xFF,0xFF,0x7F,0xD4,0x62,0x6C,0xA9,0xF3,0xFF,0xFF,0x1F,0x94,0x1F,0xFE,0xF8,0x00,0x00,0x00,0x00,0x00,0x62,0x00,0x02,0x22,0x00,0x01,0x11}},
    {0xC2,8, {0x31,0xF5,0x80,0x08,0x08,0x00,0x00,0x08}},
    {0xC4,11,{0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03}},
    {0xC6,22,{0xC8,0x01,0xB4,0x01,0x9C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x2C,0x09,0xC8}},
    {0xC7,30,{0x0A,0x20,0x29,0x33,0x41,0x4F,0x5A,0x69,0x4E,0x55,0x60,0x6B,0x72,0x77,0x7D,0x0A,0x1F,0x28,0x33,0x41,0x4F,0x59,0x69,0x4D,0x55,0x60,0x6B,0x71,0x77,0x7D}},
    {0xCB,15,{0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00}},
    {0xCC,1, {0x08}},
    {0xD0,10,{0x11,0x00,0x00,0x55,0xCC,0x40,0x19,0x19,0x09,0x00}},
    {0xD1,4, {0x00,0x48,0x16,0x0F}},
    {0xD3,26,{0x1B,0x33,0xBB,0xBB,0xB3,0x33,0x33,0x33,0x33,0x00,0x01,0x00,0x00,0xD8,0xA0,0x0B,0x3F,0x3F,0x33,0x33,0x72,0x12,0x8A,0x57,0x3D,0xBC}},
    {0xD5,7, {0x06,0x00,0x00,0x01,0x19,0x01,0x19}},
    {0x29,0, {0x00}},
    {REGFLAG_DELAY, 30, {}},
    {0x11, 0,{0x00}},
    {REGFLAG_DELAY, 150, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] =
{
    // Display off sequence
    {0x28, 1, {0x00}},
    {REGFLAG_DELAY, 10, {}},
    // Sleep Mode On
    {0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/
/*static struct LCM_setting_table lcm_deep_sleep_mode_out_setting[] = {
	// Display off sequence
	{0x11, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	// Sleep Mode On
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/
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
/* --------------------------------------------------------------------------- */
/* LCM Driver Implementations */
/* --------------------------------------------------------------------------- */

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
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
    params->dsi.mode   = SYNC_EVENT_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM                = LCM_THREE_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

    params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

    params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;

    params->dsi.vertical_sync_active                = 2;
    params->dsi.vertical_backporch                    = 6;//4;//16;///8 13 17
    params->dsi.vertical_frontporch                    = 8;//16;///8
    params->dsi.vertical_active_line                = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active                = 10;//8;
    params->dsi.horizontal_backporch                = 80;//40//60;
    params->dsi.horizontal_frontporch                = 80;//60;//60;//140;
    params->dsi.horizontal_blanking_pixel              = 60;
    params->dsi.horizontal_active_pixel                = FRAME_WIDTH;

    // Bit rate calculation
    //1 Every lane speed

    params->dsi.PLL_CLOCK=280;//230
    params->dsi.esd_check_enable = 0;
    params->dsi.customization_esd_check_enable = 0;
}

extern void lcm_set_enp_bias(bool Val);

static void init_lcm_registers(void)
{
    unsigned int data_array[16];

    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(30);
    SET_RESET_PIN(1);
    MDELAY(150);

    data_array[0]=0x00022902;
    data_array[1]=0x000004B0;//B0
    dsi_set_cmdq(data_array,2, 1);

    data_array[0]=0x00022902;
    data_array[1]=0x000001D6;//D6
    dsi_set_cmdq(data_array,2, 1);

    data_array[0] = 0x00072902;
    data_array[1] = 0x000014B3;//B3
    data_array[2] = 0x00000000;
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0] = 0x00032902;
    data_array[1] = 0x000008B4;//B4 0C_4L 08_3L
    dsi_set_cmdq(data_array, 2, 1);

    data_array[0] = 0x00042902;
    data_array[1] = 0x00BB49B6;//B6
    dsi_set_cmdq(data_array, 2, 1);

    data_array[0] = 0x00242902;
    data_array[1] = 0x006604C1;//C1
    data_array[2] = 0x0A2F93FF;
    data_array[3] = 0x7FFFFFF8;
    data_array[4] = 0xA96C62D4;
    data_array[5] = 0x1FFFFFF3;
    data_array[6] = 0xF8FE1F94;
    data_array[7] = 0x00000000;
    data_array[8] = 0x02006200;
    data_array[9] = 0x11010022;
    dsi_set_cmdq(data_array, 10, 1);

    data_array[0] = 0x00092902;
    data_array[1] = 0x80F531C2;//C2
    data_array[2] = 0x00000808;
    data_array[3] = 0x00000008;
    dsi_set_cmdq(data_array, 4, 1);

    data_array[0] = 0x000C2902;
    data_array[1] = 0x000070C4;//C4
    data_array[2] = 0x00000000;
    data_array[3] = 0x03010000;
    dsi_set_cmdq(data_array, 4, 1);

    data_array[0] = 0x00162902;
    data_array[1] = 0xB401C8C6;//C6
    data_array[2] = 0x00009C01;
    data_array[3] = 0x00000000;
    data_array[4] = 0x00000000;
    data_array[5] = 0x2C050000;
    data_array[6] = 0x0000C809;
    dsi_set_cmdq(data_array, 7, 1);

    data_array[0] = 0x001F2902;
    data_array[1] = 0x29200AC7;//C7
    data_array[2] = 0x5A4F4133;
    data_array[3] = 0x60554E69;
    data_array[4] = 0x7D77726B;
    data_array[5] = 0x33281F0A;
    data_array[6] = 0x69594F41;
    data_array[7] = 0x6B60554D;
    data_array[8] = 0x007D7771;
    dsi_set_cmdq(data_array, 9, 1);

    data_array[0] = 0x00102902;
    data_array[1] = 0xFFFFFFCB;//CB
    data_array[2] = 0x000000FF;
    data_array[3] = 0x00000000;
    data_array[4] = 0x0000E000;
    dsi_set_cmdq(data_array, 5, 1);

    data_array[0]=0x00022902;
    data_array[1]=0x000008CC;//CC
    dsi_set_cmdq(data_array,2, 1);

    data_array[0] = 0x000B2902;
    data_array[1] = 0x000011D0;//D0
    data_array[2] = 0x1940CC55;
    data_array[3] = 0x00000919;
    dsi_set_cmdq(data_array, 4, 1);

    data_array[0] = 0x00052902;
    data_array[1] = 0x164800D1;//D1
    data_array[2] = 0x0000000F;
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0] = 0x001B2902;
    data_array[1] = 0xBB331BD3;//D3
    data_array[2] = 0x3333B3BB;
    data_array[3] = 0x01003333;
    data_array[4] = 0xA0D80000;
    data_array[5] = 0x333F3F0B;
    data_array[6] = 0x8A127233;
    data_array[7] = 0x00BC3D57;
    dsi_set_cmdq(data_array, 8, 1);

    data_array[0] = 0x00082902;
    data_array[1] = 0x000006D5;//D5 VCOM
    data_array[2] = 0x19011901;
    dsi_set_cmdq(data_array, 3, 1);
    MDELAY(10);

    data_array[0] = 0x00290500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(30);

    data_array[0] = 0x00110500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(150);
}

static void lcm_init(void)
{
    unsigned char cmd = 0x0;
    unsigned char data = 0xFF;
    int ret=0;

#ifndef BUILD_LK
    lcm_set_enp_bias(1);
#endif
    cmd=0x00;
    data=0x0e;
#ifdef BUILD_LK
    ret=TPS65132_write_byte(cmd,data);
#else
    ret=tps65132_write_bytes(cmd,data);
#endif
    cmd=0x01;
    data=0x0e;
#ifdef BUILD_LK
    ret=TPS65132_write_byte(cmd,data);
#else
    ret=tps65132_write_bytes(cmd,data);
#endif

    SET_RESET_PIN(1);
    MDELAY(1);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);

    //push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
    init_lcm_registers();
}

static void lcm_suspend(void)
{
    /*
    push_table(lcm_deep_sleep_mode_in_setting,
               sizeof(lcm_deep_sleep_mode_in_setting) /
               sizeof(struct LCM_setting_table), 1);
    */
    unsigned int data_array[16];

    data_array[0] = 0x00280500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(30);

    data_array[0] = 0x00100500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(120);

    SET_RESET_PIN(0);
    MDELAY(10);

#ifndef BUILD_LK
    lcm_set_enp_bias(0);
    MDELAY(10);
#endif

}

static void lcm_resume(void)
{
    lcm_init();

    /*	push_table(lcm_deep_sleep_mode_out_setting,
    		   sizeof(lcm_deep_sleep_mode_out_setting) /
    		   sizeof(struct LCM_setting_table), 1);
    */

}


static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;
    unsigned char buffer[4];
    unsigned int array[16];

    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(1);

    SET_RESET_PIN(1);
    MDELAY(20);
    array[0] = 0x00013700;// read id return two byte,version and id
    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0x3A, buffer, 2);

    array[0] = 0x00022902;
    array[1] = 0x000000b0;
    dsi_set_cmdq(array, 2, 1);

    array[0] = 0x00043700;// read id return two byte,version and id
    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0xbf, buffer, 4);
    id = buffer[2]<<8 | buffer[3]; //we only need ID
#ifdef BUILD_LK
    dprintf(0, "%s, LK r63417 debug: r63417 id = 0x%08x\n", __func__, id);
#else
    pr_debug("%s, kernel r63417 horse debug: r63417 id = 0x%08x\n", __func__, id);
#endif
    if(id == LCM_ID_R63350)
        return 1;
    else
        return 0;
}

LCM_DRIVER r63350_jdi60_ykl_hd_lcm_drv =
{
    .name = "r63350_jdi60_ykl_hd",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params = lcm_get_params,
    .init = lcm_init,
    .suspend = lcm_suspend,
    .resume = lcm_resume,
    .compare_id	= lcm_compare_id,
//	.init_power = lcm_init_power,
//	.resume_power = lcm_resume_power,
//	.suspend_power = lcm_suspend_power,
};
