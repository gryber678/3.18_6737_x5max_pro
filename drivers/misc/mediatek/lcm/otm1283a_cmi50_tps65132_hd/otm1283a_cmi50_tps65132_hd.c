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
#if 1
#ifdef BUILD_LK
#define GPIO_LCD_PWR      GPIO_LCM_PWR
static void lcm_set_gpio_output(unsigned int GPIO, unsigned int output)
{
	mt_set_gpio_mode(GPIO, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO, (output > 0) ? GPIO_OUT_ONE : GPIO_OUT_ZERO);
}
#endif
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

static const struct i2c_device_id tps65132_id[] = {
  { TPS_I2C_ID_NAME, 0 },
  { }
};

#ifdef CONFIG_OF
static const struct of_device_id lcm_of_match[] = {
  { .compatible = "mediatek,I2C_LCD_BIAS", },
  {},
};

MODULE_DEVICE_TABLE(of, lcm_of_match)
#endif

  static struct i2c_driver tps65132_iic_driver = {
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
  char write_data[2]={0};
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
#if 0
#ifdef BUILD_LK

#ifdef GPIO_LCM_PWR
#define GPIO_LCD_PWR      GPIO_LCM_PWR
#else
#define GPIO_LCD_PWR      0xFFFFFFFF
#endif

static void lcm_set_gpio_output(unsigned int GPIO, unsigned int output)
{
	mt_set_gpio_mode(GPIO, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO, (output > 0) ? GPIO_OUT_ONE : GPIO_OUT_ZERO);
}
#else


/*static unsigned int GPIO_LCD_PWR_EN;*/
static struct pinctrl *lcmctrl;
static struct pinctrl_state *lcd_pwr_high;
static struct pinctrl_state *lcd_pwr_low;

static int lcm_get_gpio(struct device *dev)
{
	int ret = 0;

	lcmctrl = devm_pinctrl_get(dev);
	if (IS_ERR(lcmctrl)) {
		dev_err(dev, "Cannot find lcm pinctrl!");
		ret = PTR_ERR(lcmctrl);
	}
	/*lcm power pin lookup */
	lcd_pwr_high = pinctrl_lookup_state(lcmctrl, "lcm_pwr_high");
	if (IS_ERR(lcd_pwr_high)) {
		ret = PTR_ERR(lcd_pwr_high);
		pr_debug("%s : pinctrl err, lcd_pwr_high\n", __func__);
	}
	lcd_pwr_low = pinctrl_lookup_state(lcmctrl, "lcm_pwr_low");
	if (IS_ERR(lcd_pwr_low)) {
		ret = PTR_ERR(lcd_pwr_low);
		pr_debug("%s : pinctrl err, lcd_pwr_low\n", __func__);
	}
	return ret;
}

void lcm_set_gpio(int val)
{
	if (val == 0) {
		pinctrl_select_state(lcmctrl, lcd_pwr_low);
		pr_debug("LCM: lcm set power off\n");
	} else {
		pinctrl_select_state(lcmctrl, lcd_pwr_high);
		pr_debug("LCM: lcm set power on\n");
	}
}


static int lcm_probe(struct device *dev)
{
    printk("lcm probe\n");
	lcm_get_gpio(dev);

	return 0;
}

static const struct of_device_id lcm_of_ids[] = {
	{.compatible = "mediatek,mt6580-lcm",},
	{}
};

static struct platform_driver lcm_driver = {
	.driver = {
		   .name = "mtk_lcm",
		   .owner = THIS_MODULE,
		   .probe = lcm_probe,
#ifdef CONFIG_OF
		   .of_match_table = lcm_of_ids,
#endif
		   },
};

static int __init lcm_init(void)
{
	pr_notice("LCM: Register lcm driver\n");
	if (platform_driver_register(&lcm_driver)) {
		printk("LCM: failed to register disp driver\n");
		return -ENODEV;
	}

	return 0;
}

static void __exit lcm_exit(void)
{
	platform_driver_unregister(&lcm_driver);
	pr_notice("LCM: Unregister lcm driver done\n");
}
late_initcall(lcm_init);
module_exit(lcm_exit);
MODULE_AUTHOR("mediatek");
MODULE_DESCRIPTION("Display subsystem Driver");
MODULE_LICENSE("GPL");
#endif
#endif
/* --------------------------------------------------------------------------- */
/* Local Constants */
/* --------------------------------------------------------------------------- */

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID 0x91

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
#if 1
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
#endif
#if 0
static void lcm_init_power(void)
{

  unsigned char cmd = 0x0;
  unsigned char data = 0xFF;
  int ret=0;
  cmd=0x00;
  data=0x0e;
#ifdef BUILD_LK
	printf("[LK/LCM] lcm_init_power() enter\n");
	lcm_set_gpio_output(GPIO_LCD_PWR, GPIO_OUT_ONE);
	MDELAY(20);
#else
	printk("[Kernel/LCM] lcm_init_power() enter\n");
#endif
#if 1
#ifdef BUILD_LK
  ret=TPS65132_write_byte(cmd,data);
  if(ret) 	
    dprintf(0, "[LK]otm1906c----tps6132----cmd=%0x--i2c write error----\n",cmd); 	
  else
    dprintf(0, "[LK]otm1906c----tps6132----cmd=%0x--i2c write success----\n",cmd);			
#else
  ret=tps65132_write_bytes(cmd,data);
  if(ret<0)
    pr_err("[KERNEL]otm1906c----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
  else
    pr_err("[KERNEL]otm1906c----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
#endif
  cmd=0x01;
  data=0x0e;
#ifdef BUILD_LK
  ret=TPS65132_write_byte(cmd,data);
  if(ret) 	
    dprintf(0, "[LK]otm1906c----tps6132----cmd=%0x--i2c write error----\n",cmd); 	
  else
    dprintf(0, "[LK]otm1906c----tps6132----cmd=%0x--i2c write success----\n",cmd);	
#else
  ret=tps65132_write_bytes(cmd,data);
  if(ret<0)
    pr_err("[KERNEL]otm1906c----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
  else
    pr_err("[KERNEL]otm1906c----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
#endif
#endif
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
struct LCM_setting_table {
  unsigned cmd;
  unsigned char count;
  unsigned char para_list[64];
};
static struct LCM_setting_table lcm_initialization_setting[] = {
	{0x00, 1, {0x00}},
	{0xff, 3, {0x12,0x83,0x01}},

	{0x00, 1, {0x80}},
	{0xff, 2, {0x12,0x83}},

    {0x00, 1, {0x92}},
    {0xff, 2, {0x30,0x02}},//mipi 2 lane^M


	{0x00, 1, {0xb9}},
	{0xb0, 1, {0x51}},

	{0x00, 1, {0xc6}},
	{0xb0, 1, {0x03}},

	{0x00, 1, {0x80}},
	{0xc0, 9, {0x00,0x64,0x00,0x10,0x10,0x00,0x64,0x10,0x10}},

	{0x00, 1, {0x90}},
	{0xc0, 6, {0x00,0x5c,0x00,0x01,0x00,0x04}},

	{0x00, 1, {0xb3}},
	{0xc0, 2, {0x00,0x50}},

	{0x00, 1, {0x81}},
	{0xc1, 1, {0x55}},

	{0x00, 1, {0xa0}},
	{0xc1, 1, {0x02}},

	{0x00, 1, {0x82}},
	{0xc4, 1, {0x02}},

	{0x00, 1, {0x8b}},
	{0xc4, 1, {0x40}},

	{0x00, 1, {0x90}},
	{0xc4, 1, {0x49}},

	{0x00, 1, {0xe0}},
	{0xc0, 1, {0xc8}},

	{0x00, 1, {0xa0}},
	{0xc4, 14, {0x05,0x10,0x04,0x02,0x05,0x15,0x11,0x05,0x10,0x07,0x02,0x05,0x15,0x11}},

	{0x00, 1, {0xb0}},
	{0xc4, 2, {0x00,0x00}},

	{0x00, 1, {0x90}},
	{0xc5, 4, {0x50,0xa6,0xd0,0x66}},

	{0x00, 1, {0xb0}},
	{0xc5, 2, {0x04,0x38}},

	{0x00, 1, {0xb4}},
	{0xc5, 1, {0xc0}},

	{0x00, 1, {0xb5}},
	{0xc5, 6, {0x0b,0x95,0xff,0x0b,0x95,0xff}},

	{0x00, 1, {0xbb}},
	{0xc5, 1, {0x80}},

	{0x00, 1, {0x90}},
	{0xf5, 4, {0x02,0x11,0x02,0x11}},

	{0x00, 1, {0x94}},
	{0xf5, 1, {0x02}},

	{0x00, 1, {0xb2}},
	{0xf5, 8, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xba}},
	{0xf5, 1, {0x03}},

	{0x00, 1, {0x80}},
	{0xcb, 11, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0x90}},
	{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xa0}},
	{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xb0}},
	{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xc0}},
	{0xcb, 15, {0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x04,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xd0}},
	{0xcb, 15, {0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04}},

	{0x00, 1, {0xe0}},
	{0xcb, 14, {0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00}},

	{0x00, 1, {0xf0}},
	{0xcb, 11, {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},

	{0x00, 1, {0x80}},
	{0xcc, 15, {0x29,0x2a,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x06,0x00,0x08,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0x90}},
	{0xcc, 15, {0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x29,0x2a,0x09,0x0b,0x0d,0x0f,0x11,0x13}},

	{0x00, 1, {0xa0}},
	{0xcc, 14, {0x05,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00}},

	{0x00, 1, {0xb0}},
	{0xcc, 15, {0x29,0x2a,0x13,0x11,0x0f,0x0d,0x0b,0x09,0x01,0x00,0x07,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xc0}},
	{0xcc, 15, {0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x29,0x2a,0x14,0x12,0x10,0x0e,0x0c,0x0a}},

	{0x00, 1, {0xd0}},
	{0xcc, 14, {0x02,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00}},

	{0x00, 1, {0x80}},
	{0xce, 12, {0x89,0x05,0x10,0x88,0x05,0x10,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0x90}},
	{0xce, 14, {0x54,0xfd,0x10,0x54,0xfe,0x10,0x55,0x01,0x10,0x55,0x02,0x10,0x00,0x00}},

	{0x00, 1, {0xa0}},
	{0xce, 14, {0x58,0x07,0x04,0xfd,0x00,0x10,0x00,0x58,0x06,0x04,0xfe,0x00,0x10,0x00}},

	{0x00, 1, {0xb0}},
	{0xce, 14, {0x58,0x05,0x04,0xff,0x00,0x10,0x00,0x58,0x04,0x05,0x00,0x00,0x10,0x00}},

	{0x00, 1, {0xc0}},
	{0xce, 14, {0x58,0x03,0x05,0x01,0x00,0x10,0x00,0x58,0x02,0x05,0x02,0x00,0x10,0x00}},

	{0x00, 1, {0xd0}},
	{0xce, 14, {0x58,0x01,0x05,0x03,0x00,0x10,0x00,0x58,0x00,0x05,0x04,0x00,0x10,0x00}},

	{0x00, 1, {0x80}},
	{0xcf, 14, {0x50,0x00,0x05,0x05,0x00,0x10,0x00,0x50,0x01,0x05,0x06,0x00,0x10,0x00}},

	{0x00, 1, {0x90}},
	{0xcf, 14, {0x50,0x02,0x05,0x07,0x00,0x10,0x00,0x50,0x03,0x05,0x08,0x00,0x10,0x00}},

	{0x00, 1, {0xa0}},
	{0xcf, 14, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xb0}},
	{0xcf, 14, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	{0x00, 1, {0xc0}},
	{0xcf, 11, {0x39,0x39,0x20,0x20,0x00,0x00,0x01,0x01,0x20,0x00,0x00}},

	{0x00, 1, {0x00}},
	{0xd8, 2, {0xbe,0xbe}},

	{0x00, 1, {0x94}},
	{0xc5, 2, {0x66,0x66}},

	{0x00, 1, {0x00}},
	{0xd9, 1, {0x80}},

	{0x00, 1, {0x00}},
	{0xe1, 16, {0x02,0x08,0x14,0x0d,0x06,0x10,0x0b,0x0a,0x03,0x07,0x0d,0x09,0x10,0x14,0x0e,0x08}},

	{0x00, 1, {0x00}},
	{0xe2, 16, {0x02,0x08,0x14,0x0c,0x07,0x10,0x0c,0x0a,0x03,0x07,0x0d,0x09,0x10,0x14,0x0e,0x08}},

	{0x00, 1, {0x00}},
	{0xec, 33, {0x40,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x03}},

	{0x00, 1, {0x00}},
	{0xed, 33, {0x40,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x44,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x44,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x44,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x04}},

	{0x00, 1, {0x00}},
	{0xee, 33, {0x40,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x04}},

	{0x00, 1, {0x80}},
	{0xc4, 1, {0x30}},

	{0x00, 1, {0x00}},
	{0xff, 3, {0xff,0xff,0xff}},

	{0x35, 1 , {0x00}},//TE
	{0x11, 1 , {0x00}},
	{REGFLAG_DELAY, 120, {}},

	{0x29, 1 , {0x00}},
	{REGFLAG_DELAY, 20, {}},
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
	params->dbi.te_mode = LCM_DBI_TE_MODE_VSYNC_ONLY;
	params->dbi.te_edge_polarity = LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode = CMD_MODE;
#else
	//params->dsi.mode   = SYNC_EVENT_VDO_MODE;
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 720 * 3;

		params->dsi.vertical_sync_active				= 0x3;// 3    3
		params->dsi.vertical_backporch					= 0x0E;// 20   14
		params->dsi.vertical_frontporch					= 0x10; // 1  12 16
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 0x04;// 50  2
		params->dsi.horizontal_backporch				= 0x22 ;//40
		params->dsi.horizontal_frontporch				= 0x18 ;//30
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

	// Bit rate calculation
	//1 Every lane speed
    params->dsi.PLL_CLOCK =200; //180
    params->dsi.esd_check_enable = 1;  
    params->dsi.customization_esd_check_enable      = 0;
    params->dsi.noncont_clock = 1;
    params->dsi.noncont_clock_period = 2;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
    params->dsi.lcm_esd_check_table[0].count        = 1;  
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
}

extern void lcm_set_enp_bias(bool Val);
static void lcm_init(void)
{
  unsigned char cmd = 0x0;
  unsigned char data = 0xFF;
  int ret=0;

#ifndef BUILD_LK
  //TODO, modify to fix build error
  //lcm_set_enp_bias(1);
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
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting,
		   sizeof(lcm_deep_sleep_mode_in_setting) /
		   sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);
	MDELAY(1);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(50);
#ifndef BUILD_LK
  //TODO, modify to fix build error
    //lcm_set_enp_bias(0);
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
	unsigned int id0, id1, id2, id3, id4;
	unsigned char buffer[5];
	unsigned int array[5];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(50);

	// Set Maximum return byte = 1
	array[0] = 0x00053700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1, buffer, 5);
	id0 = buffer[0];
	id1 = buffer[1];
	id2 = buffer[2];
	id3 = buffer[3];
	id4 = buffer[4];

#if defined(BUILD_LK)
	printf("%s, Module ID = {%x, %x, %x, %x, %x} \n", __func__, id0,
	       id1, id2, id3, id4);
#else
	printk("%s, Module ID = {%x, %x, %x, %x,%x} \n", __func__, id0,
	       id1, id2, id3, id4);
#endif

	return ( 0x1283 == ((id2 << 8) | id3)) ? 1 : 0;
}
LCM_DRIVER otm1283a_cmi50_tps65132_hd_lcm_drv = {
	.name = "otm1283a_cmi50_tps65132_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
  .compare_id     = lcm_compare_id,
//	.init_power = lcm_init_power,
//	.resume_power = lcm_resume_power,
//	.suspend_power = lcm_suspend_power,
};
