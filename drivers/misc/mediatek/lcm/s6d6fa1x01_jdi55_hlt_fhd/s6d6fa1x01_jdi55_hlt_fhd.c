/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

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

#if 1
#ifdef BUILD_LK
#define LCD_DEBUG(fmt)  dprintf(CRITICAL,fmt)
#else
#define LCD_DEBUG(fmt)  printk(fmt)
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
#define TPS_I2C_BUSNUM  1//I2C_I2C_LCD_BIAS_CHANNEL//for I2C channel 0
#define TPS_ADDR 0x3E
static struct i2c_board_info __initdata tps65132_board_info = {I2C_BOARD_INFO(TPS_I2C_ID_NAME, TPS_ADDR)};
static struct i2c_client *tps65132_i2c_client = NULL;
static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tps65132_remove(struct i2c_client *client);
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
    printk( "tps65132_iic_probe\n");
    printk("TPS: info==>name=%s addr=0x%x\n",client->name,client->addr);
    tps65132_i2c_client  = client;
    return 0;
}
static int tps65132_remove(struct i2c_client *client)
{
    printk( "tps65132_remove\n");
    tps65132_i2c_client = NULL;
    i2c_unregister_device(client);
    return 0;
}
static int tps65132_write_bytes(unsigned char addr, unsigned char value)
{
    int ret = 0;
    struct i2c_client *client = tps65132_i2c_client;
    char write_data[2]={0};

    if (NULL == client)
        return -1;

    write_data[0]= addr;
    write_data[1] = value;
    ret=i2c_master_send(client, write_data, 2);
    if(ret<0)
        printk("tps65132 write data fail !!\n");	
    return ret ;
}
static int __init tps65132_iic_init(void)
{
    printk( "tps65132_iic_init\n");
    i2c_register_board_info(TPS_I2C_BUSNUM, &tps65132_board_info, 1);
    printk( "tps65132_iic_init2\n");
    if(i2c_add_driver(&tps65132_iic_driver))
    {
        printk( "add tps65132 error\n");
        return -1;
    }
    printk( "tps65132_iic_init success\n");	
    return 0;
}
static void __exit tps65132_iic_exit(void)
{
    printk( "tps65132_iic_exit\n");
    i2c_del_driver(&tps65132_iic_driver);
}
module_init(tps65132_iic_init);
module_exit(tps65132_iic_exit);
MODULE_AUTHOR("Xiaokuan Shi");
MODULE_DESCRIPTION("MTK TPS65132 I2C Driver");
MODULE_LICENSE("GPL");
#endif
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (1080)
#define FRAME_HEIGHT (1920)

#define LCM_OTM1285_ID			(0x1285)

#define GPIO_CTP_EN_PIN   (GPIO21 | 0x80000000)

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

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

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
    TPS65132_i2c.addr = (TPS65132_SLAVE_ADDR_WRITE >> 1);
    TPS65132_i2c.mode = ST_MODE;
    TPS65132_i2c.speed = 100;
    len = 2;
    ret_code = i2c_write(&TPS65132_i2c, write_data, len);
    return ret_code;
}

#endif
#endif
#define GPIO_65132_EN (GPIO94 | 0x80000000)
#if 1
static void lcm_init_power(void)
{
    unsigned char cmd = 0x0;
    unsigned char data = 0xFF;
    int ret=0;
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);
    cmd=0x00;
    data=0x0e;
    mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ONE);
#ifdef BUILD_LK
    ret=TPS65132_write_byte(cmd,data);
    if(ret) 	
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write error----\n",cmd); 	
    else
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write success----\n",cmd);			
#else
    ret=tps65132_write_bytes(cmd,data);
    if(ret<0)
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
    else
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
#endif
    MDELAY(1);
    cmd=0x01;
    data=0x0e;
#ifdef BUILD_LK
    ret=TPS65132_write_byte(cmd,data);
    if(ret) 	
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write error----\n",cmd); 	
    else
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write success----\n",cmd);	
#else
    ret=tps65132_write_bytes(cmd,data);
    if(ret<0)
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
    else
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
#endif
    MDELAY(1);
#endif
}
#endif
static void lcm_resume_power(void)
{
#if 1
    unsigned char cmd = 0x0;
    unsigned char data = 0xFF;
    int ret=0;
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);
    cmd=0x00;
    data=0x0e;
    mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ONE);
#ifdef BUILD_LK
    ret=TPS65132_write_byte(cmd,data);
    if(ret) 	
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write error----\n",cmd); 	
    else
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write success----\n",cmd);			
#else
    ret=tps65132_write_bytes(cmd,data);
    if(ret<0)
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
    else
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
#endif
    MDELAY(1);
    cmd=0x01;
    data=0x0e;
#ifdef BUILD_LK
    ret=TPS65132_write_byte(cmd,data);
    if(ret) 	
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write error----\n",cmd); 	
    else
        dprintf(0, "[LK]nt35598----tps6132----cmd=%0x--i2c write success----\n",cmd);	
#else
    ret=tps65132_write_bytes(cmd,data);
    if(ret<0)
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
    else
        printk("[KERNEL]nt35598----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
#endif
    MDELAY(1);
#endif
}

static void lcm_suspend_power(void)
{
    mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ZERO);
}

struct LCM_setting_table {
    unsigned int cmd;
    unsigned char count;
    unsigned char para_list[64];
};
static struct LCM_setting_table lcm_initialization_setting[] = 
{
#if 1
  /*  {0xF0,  2, {0x5A,0x5A}},
    {0xF1,  2, {0x5A,0x5A}},
    {0xFC,  2, {0x5A,0x5A}},
    {0xF5,  7, {0x10,0x18,0x00,0xD1,0xA7,0x11,0x08}},
    {0xB2, 16, {0x00,0x00,0x00,0x10,0x55,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
    {0xB3,  6, {0x10,0xF0,0x00,0xBB,0x04,0x08}},     
    {0xB6,  6, {0x29,0x10,0x2C,0x64,0x64,0x01}}, 
    {0xB7, 14, {0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x09,0x03,0xF5,0x16,0x0F,0x00,0x00}},
    {0xB8, 27, {0x41,0x00,0x03,0x03,0x03,0x24,0x11,0x00,0x83,0xF4,0xBB,0x00,0x03,0x21,0x24,0x00,0xAB,0xA9,0x0A,0xB4,0x70,0xFF,0xFF,0x00,0x1D,0x00,0x00}},
    {0xB9, 27, {0x20,0x00,0x03,0x03,0x03,0x00,0x11,0x00,0x02,0xF4,0xBB,0x00,0x03,0x00,0x00,0x11,0x5A,0x58,0x15,0xA5,0x20,0xFF,0xFF,0x00,0x39,0x00,0x00}},
    {0xBA, 27, {0x41,0x00,0x03,0x03,0x03,0x24,0x11,0x00,0x83,0xF4,0xBB,0x00,0x03,0x21,0x24,0x00,0xAB,0xA9,0x0A,0xB4,0x70,0xFF,0xFF,0x00,0x1D,0x00,0x00}},
    {0xBB, 27, {0x41,0x00,0x03,0x03,0x03,0x24,0x11,0x00,0x83,0xF4,0xBB,0x00,0x03,0x21,0x24,0x00,0xAB,0xA9,0x0A,0xB4,0x70,0xFF,0xFF,0x00,0x1D,0x00,0x00}},
    {0xBC, 23, {0x20,0x00,0x3C,0x19,0x31,0x02,0x07,0x00,0x00,0x2D,0x08,0x0F,0x0E,0x07,0x1E,0x04,0x02,0x2A,0x00,0x00,0x02,0x00,0x9E}},
    {0xBD, 20, {0x01,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x05,0x01,0x00,0x00,0x00,0x40,0x00,0x05,0x2D,0x05,0x05,0x05}},
    {0xBE, 43, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x0B,0x0B,0x0B,0x07,0x0A,0x06,0x02,0x01,0x00,0x09,0x0B,0x0E,0x0D,0x0C,0x11,0x10,0x0F,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x05,0x02,0x01,0x00,0x08,0x12,0x0E,0x0D,0x0C,0x11,0x10,0x0F}},
    {0xBF, 14, {0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x0C,0xAC,0x67,0xCC,0xCC,0x0C}},
    {0xD7,  3, {0xA1,0x75,0x20}},
    {0xE3,  1, {0x22}},
    {0xE8,  7, {0x00,0x03,0x1A,0x0E,0x02,0x06,0xD0}},
    {0xF2,  6, {0x46,0x43,0x13,0x33,0xC1,0x18}},
    {0xF4, 17, {0x00,0x6E,0x19,0x10,0x0E,0xC9,0x02,0x55,0x35,0x54,0xAA,0x55,0x05,0x04,0x44,0x48,0x30}},
    {0xF7,  4, {0x00,0x00,0x3F,0xFF}},
    {0xFA, 82, {0x05,0x54,0x1C,0x1E,0x23,0x27,0x29,0x2B,0x2C,0x2A,0x39,0x3D,0x1F,0x20,0x20,0x29,0x28,0x23,0x26,0x21,0x1F,0x1F,0x08,0x07,0x0B,0x12,0x0B,0x05,0x54,0x1C,0x1E,0x23,0x27,0x29,0x2B,0x2C,0x2A,0x39,0x3D,0x1F,0x20,0x20,0x29,0x28,0x23,0x26,0x21,0x1F,0x1F,0x08,0x07,0x0B,0x12,0x0B,0x05,0x54,0x1C,0x1E,0x23,0x27,0x29,0x2B,0x2C,0x2A,0x39,0x3D,0x1F,0x20,0x20,0x29,0x28,0x23,0x26,0x21,0x1F,0x1F,0x08,0x07,0x0B,0x12,0x0B,0x00}},
    {0xFB, 81, {0x05,0x54,0x1C,0x1E,0x23,0x27,0x29,0x2B,0x2C,0x2A,0x39,0x3D,0x1F,0x20,0x20,0x29,0x28,0x23,0x26,0x21,0x1F,0x1F,0x08,0x07,0x0B,0x12,0x0B,0x05,0x54,0x1C,0x1E,0x23,0x27,0x29,0x2B,0x2C,0x2A,0x39,0x3D,0x1F,0x20,0x20,0x29,0x28,0x23,0x26,0x21,0x1F,0x1F,0x08,0x07,0x0B,0x12,0x0B,0x05,0x54,0x1C,0x1E,0x23,0x27,0x29,0x2B,0x2C,0x2A,0x39,0x3D,0x1F,0x20,0x20,0x29,0x28,0x23,0x26,0x21,0x1F,0x1F,0x08,0x07,0x0B,0x12,0x0B}},
    {0xF0, 2, {0xA5,0xA5}},
    {0xF1, 2, {0xA5,0xA5}},
    {0xFC, 2, {0xA5,0xA5}},

    {0x11,0,{0x00}},
    {REGFLAG_DELAY, 120, {}}, 
    {0x29,0,{0x00}},
    {REGFLAG_DELAY, 10, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
#else*/
    {0xF0, 2 ,{0x5A, 0x5A}},
    {0xF1, 2 ,{0x5A, 0x5A}},
    {0xFC, 2 ,{0x5A, 0x5A}},
    {0xF5, 7,{0x10, 0x18, 0x00, 0xD1, 0xA7, 0x11, 0x08}},
    {0xB2, 16,{0x00, 0x00, 0x00, 0x10, 0x55, 0x0B, 0x00, 0x00, 0x00, 0x00, 
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0xB3, 6,{0x10, 0xF0, 0x00, 0xBB, 0x04, 0x08}},
    {0xB6, 6,{0x29, 0x10, 0x2C, 0x64, 0x64, 0x01}},
    {0xB7, 14,{0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x09, 0x03, 0xF5, 
               0x16, 0x0F, 0x00, 0x00}},
    {0xB8, 27,{ 0x41, 0x00, 0x03,0x03, 0x03, 0x24, 0x11,0x00, 0x83, 0xF4,
                0xBB,0x00, 0x03, 0x21, 0x24,0x00, 0xAB, 0xA9, 0x0A,0xB4, 
                0x70, 0xFF, 0xFF,0x00, 0x1D, 0x00, 0x00}},
    {0xB9, 27 ,{0x20, 0x00, 0x03,0x03, 0x03, 0x00, 0x11,0x00, 0x02, 0xF4, 
               0xBB,0x00, 0x03, 0x00, 0x00,0x11, 0x5A, 0x58, 0x15,0xA5, 
               0x20, 0xFF, 0xFF,0x00, 0x39, 0x00, 0x00}},
    {0xBA, 27 ,{0x41, 0x00, 0x03,0x03, 0x03, 0x24, 0x11,0x00, 0x83, 0xF4, 
               0xBB,0x00, 0x03, 0x21, 0x24,0x00, 0xAB, 0xA9, 0x0A,0xB4, 
               0x70, 0xFF, 0xFF,0x00, 0x1D, 0x00, 0x00}},
    {0xBB, 27 ,{0x41,0x00,0x03,0x03,0x03,0x24,0x11,0x00,0x83,0xF4,0xBB,
               0x00,0x03,0x21,0x24,0x00,0xAB,0xA9,0x0A,0xB4,0x70,0xFF,
               0xFF,0x00,0x1D,0x00,0x00}},  
    {0xBC, 23 ,{0x20, 0x00, 0x3C,0x19, 0x31, 0x02, 0x07,0x00, 0x00,
                0x2D, 0x08,0x0F, 0x0E, 0x07, 0x1E,0x04, 0x02, 0x2A, 
                0x00,0x00, 0x02, 0x00, 0x9E}},  
    {0xBD, 20 ,{0x01, 0x07, 0x07,0x07, 0x00, 0x00, 0x00,0x00, 0x05, 
                0x01, 0x00,0x00, 0x00, 0x40, 0x00,0x05, 0x2D, 0x05, 
                0x05,0x00}},
    {0xBE, 43 ,{0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x0B, 0x0B,
                0x0B, 0x0B,0x07, 0x0A, 0x06, 0x02,0x01, 0x00, 0x09,
                0x0B,0x0E, 0x0D, 0x0C, 0x11,0x10, 0x0F, 0x0B, 0x0B,
                0x0B, 0x0B, 0x0B, 0x0B,0x05, 0x02, 0x01, 0x00,0x08, 
                0x12, 0x0E, 0x0D,0x0C, 0x11, 0x10, 0x0F}},  
    {0xBF, 14 ,{0x00, 0x00, 0x00,0x00, 0xCC, 0xCC, 0xCC,0xCC, 0x0C, 
                0xAC, 0x67,0xCC, 0xCC, 0x0C}},  
    {0xD7, 3, {0xA1, 0x75, 0x20}},  
    {0xE3, 1, {0x22}},  
    {0xE8, 7,{0x00, 0x03, 0x1A,0x0E, 0x02, 0x06, 0xD0}},
    {0xF2, 6,{0x46, 0x43, 0x13,0x33, 0xC1, 0x18}},
    {0xF4, 27 ,{0x20, 0x20, 0x70,0x20, 0x14, 0x20, 0x14,0x06, 0x10, 
                0x1C, 0x00,0x6E, 0x19, 0x10, 0x0E,0xC9, 0x02, 0x55,
                0x35,0x54, 0xAA, 0x55, 0x05,0x04, 0x44, 0x48, 0x30}},   
    {0xF7, 4,{0x00, 0x00, 0x3F, 0xFF}},

    {0XFA,82,{0x05, 0x54, 0x0E,0x0E, 0x12, 0x17, 0x18,0x1C, 0x1E, 0x1E, 
              0x2D,0x34, 0x16, 0x19, 0x19,0x24, 0x22, 0x1F, 0x23,0x22, 
              0x1F, 0x1F, 0x0C,0x0D, 0x14, 0x1D, 0x0E,0x05, 0x54, 0x10,
              0x11,0x15, 0x1A, 0x1C, 0x1F,0x20, 0x20, 0x2F, 0x35,0x18,
              0x1A, 0x1B, 0x26,0x24, 0x21, 0x25, 0x23,0x21, 0x21, 0x0D, 
              0x0E,0x15, 0x1D, 0x0F, 0x05,0x54, 0x0C, 0x0B, 0x0F,0x14, 
              0x16, 0x1A, 0x1C,0x1C, 0x2C, 0x33, 0x16,0x18, 0x18, 0x23, 
              0x21,0x1E, 0x22, 0x20, 0x1E,0x1E, 0x0B, 0x0C, 0x13,0x1C, 
              0x0E, 0x00}},
    {0XFB,81,{0x05, 0x54, 0x0E,0x0E, 0x12, 0x17, 0x18,0x1C, 0x1E, 0x1E, 
              0x2D,0x34, 0x16, 0x19, 0x19,0x24, 0x22, 0x1F, 0x23,0x22, 
              0x1F, 0x1F, 0x0C,0x0D, 0x14, 0x1D, 0x0E,0x05, 0x54, 0x10, 
              0x11,0x15, 0x1A, 0x1C, 0x1F,0x20, 0x20, 0x2F, 0x35,0x18, 
              0x1A, 0x1B, 0x26,0x24, 0x21, 0x25, 0x23,0x21, 0x21, 0x0D, 
              0x0E,0x15, 0x1D, 0x0F, 0x05,0x54, 0x0C, 0x0B, 0x0F,0x14, 
              0x16, 0x1A, 0x1C,0x1C, 0x2C, 0x33, 0x16,0x18, 0x18, 0x23, 
              0x21,0x1E, 0x22, 0x20, 0x1E,0x1E, 0x0B, 0x0C, 0x13,0x1C, 
              0x0E}},
    {0xF0, 2 ,{0x5A, 0x5A}},
    {0xF1, 2 ,{0x5A, 0x5A}},
    {0xFC, 2 ,{0x5A, 0x5A}},

    {0x11, 0 ,{0x00}},
    {REGFLAG_DELAY, 120, {}},

    {0x29, 0 ,{0x00}},
    {REGFLAG_DELAY, 50, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
#endif
};
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
    {0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
    {0x29, 0, {0x00}},
    {REGFLAG_DELAY, 10, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
    // Display off sequence
    {0x28, 0, {0x00}},
    {REGFLAG_DELAY, 20, {}},

    // Sleep Mode On
    {0x10, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_compare_id_setting[] = {
    // Display off sequence
    {0xF0,	5,	{0x55, 0xaa, 0x52,0x08,0x00}},
    {REGFLAG_DELAY, 10, {}},

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
                //MDELAY(2);
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

    params->dsi.mode   = SYNC_PULSE_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;

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
    params->dsi.word_count = 1080 * 3;

    params->dsi.vertical_sync_active				= 0x4;// 3    2
    params->dsi.vertical_backporch					= 0x15;// 20   1
    params->dsi.vertical_frontporch					= 0x16; // 1  12
    params->dsi.vertical_active_line				= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active				= 0x8;// 50  2
    params->dsi.horizontal_backporch				= 0x50 ;
    params->dsi.horizontal_frontporch				= 0x50 ;
    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

    // Bit rate calculation
    //1 Every lane speed
    params->dsi.PLL_CLOCK = 480;
    params->dsi.ssc_disable = 1;  // ssc disable control (1: disable, 0: enable, default: 0)
    params->dsi.esd_check_enable            = 0;
    params->dsi.customization_esd_check_enable  = 0;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
}

static void lcm_init(void)
{
    mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
    MDELAY(10);

    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(5);
    SET_RESET_PIN(1);
    MDELAY(120);

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);	
}

static void lcm_suspend(void)
{
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(5);
    SET_RESET_PIN(1);
    MDELAY(120);
    push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}

//static unsigned int VcomH = 0x40;
//static unsigned int lcm_compare_id(void);
static void lcm_resume(void)
{
    lcm_init();
    //lcm_compare_id();
    push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
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

    return (LCM_OTM1285_ID == ((id2 << 8) | id3)) ? 1 : 0;
}




LCM_DRIVER s6d6fa1x01_jdi55_hlt_fhd_lcm_drv =
{
    .name			= "s6d6fa1x01_jdi55_hlt_fhd",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    //.compare_id     = lcm_compare_id,
    .init_power     = lcm_init_power,
    .resume_power   = lcm_resume_power,
    .suspend_power  = lcm_suspend_power,
};
