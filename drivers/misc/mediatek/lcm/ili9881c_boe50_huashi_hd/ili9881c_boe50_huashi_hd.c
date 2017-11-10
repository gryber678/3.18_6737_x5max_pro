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

/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2008
 *
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
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#else
//#include <mt-plat/upmu_common.h>
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

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)



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
static struct LCM_setting_table lcm_initialization_setting[] = {


    {0xff, 3,{0x98,0x81,0x03}},
    //GIP_1
    {0x01, 1,{0x00}},
    {0x02, 1,{0x00}},
    {0x03, 1,{0x73}},
    {0x04, 1,{0x00}},
    {0x05, 1,{0x00}},
    {0x06, 1,{0x0c}},
    {0x07, 1,{0x00}},
    {0x08, 1,{0x00}},
    {0x09, 1,{0x01}},
    {0x0a, 1,{0x01}},
    {0x0b, 1,{0x01}},
    {0x0c, 1,{0x01}},
    {0x0d, 1,{0x01}},
    {0x0e, 1,{0x01}},
    {0x0f, 1,{0x00}},
    {0x10, 1,{0x00}},
    {0x11, 1,{0x00}},
    {0x12, 1,{0x00}},
    {0x13, 1,{0x00}},
    {0x14, 1,{0x00}},
    {0x15, 1,{0x00}},
    {0x16, 1,{0x00}},
    {0x17, 1,{0x00}},
    {0x18, 1,{0x00}},
    {0x19, 1,{0x00}},
    {0x1a, 1,{0x00}},
    {0x1b, 1,{0x00}},
    {0x1c, 1,{0x00}},
    {0x1d, 1,{0x00}},
    {0x1e, 1,{0x44}},
    {0x1f, 1,{0xc0}},
    {0x20, 1,{0x0a}},
    {0x21, 1,{0x03}},
    {0x22, 1,{0x0a}},
    {0x23, 1,{0x00}},
    {0x24, 1,{0x8c}},
    {0x25, 1,{0x8c}},
    {0x26, 1,{0x00}},
    {0x27, 1,{0x00}},
    {0x28, 1,{0x3b}},
    {0x29, 1,{0x03}},
    {0x2a, 1,{0x00}},
    {0x2b, 1,{0x00}},
    {0x2c, 1,{0x00}},
    {0x2d, 1,{0x00}},
    {0x2e, 1,{0x00}},
    {0x2f, 1,{0x00}},
    {0x30, 1,{0x00}},
    {0x31, 1,{0x00}},
    {0x32, 1,{0x00}},
    {0x33, 1,{0x00}},
    {0x34, 1,{0x00}},
    {0x35, 1,{0x00}},
    {0x36, 1,{0x00}},
    {0x37, 1,{0x00}},
    {0x38, 1,{0x00}},
    {0x39, 1,{0x00}},
    {0x3a, 1,{0x00}},
    {0x3b, 1,{0x00}},
    {0x3c, 1,{0x00}},
    {0x3d, 1,{0x00}},
    {0x3e, 1,{0x00}},
    {0x3f, 1,{0x00}},
    {0x40, 1,{0x00}},
    {0x41, 1,{0x88}},
    {0x42, 1,{0x00}},
    {0x43, 1,{0x00}},
    {0x44, 1,{0x00}},


    //GIP_2             
    {0x50, 1,{0x01}},
    {0x51, 1,{0x23}},
    {0x52, 1,{0x45}},
    {0x53, 1,{0x67}},
    {0x54, 1,{0x89}},
    {0x55, 1,{0xab}},
    {0x56, 1,{0x01}},
    {0x57, 1,{0x23}},
    {0x58, 1,{0x45}},
    {0x59, 1,{0x67}},
    {0x5a, 1,{0x89}},
    {0x5b, 1,{0xab}},
    {0x5c, 1,{0xcd}},
    {0x5d, 1,{0xef}},

    //GIP_3             
    {0x5e, 1,{0x11}},
    {0x5f, 1,{0x0c}},
    {0x60, 1,{0x0d}},
    {0x61, 1,{0x0e}},
    {0x62, 1,{0x0f}},
    {0x63, 1,{0x06}},
    {0x64, 1,{0x07}},
    {0x65, 1,{0x02}},
    {0x66, 1,{0x02}},
    {0x67, 1,{0x02}},
    {0x68, 1,{0x02}},
    {0x69, 1,{0x02}},
    {0x6a, 1,{0x02}},
    {0x6b, 1,{0x02}},
    {0x6c, 1,{0x02}},
    {0x6d, 1,{0x02}},
    {0x6e, 1,{0x02}},
    {0x6f, 1,{0x02}},
    {0x70, 1,{0x02}},
    {0x71, 1,{0x02}},
    {0x72, 1,{0x02}},
    {0x73, 1,{0x01}},
    {0x74, 1,{0x00}},
    {0x75, 1,{0x0c}},
    {0x76, 1,{0x0d}},
    {0x77, 1,{0x0e}},
    {0x78, 1,{0x0f}},
    {0x79, 1,{0x06}},
    {0x7a, 1,{0x07}},
    {0x7b, 1,{0x02}},
    {0x7c, 1,{0x02}},
    {0x7d, 1,{0x02}},
    {0x7e, 1,{0x02}},
    {0x7f, 1,{0x02}},
    {0x80, 1,{0x02}},
    {0x81, 1,{0x02}},
    {0x82, 1,{0x02}},
    {0x83, 1,{0x02}},
    {0x84, 1,{0x02}},
    {0x85, 1,{0x02}},
    {0x86, 1,{0x02}},
    {0x87, 1,{0x02}},
    {0x88, 1,{0x02}},
    {0x89, 1,{0x01}},
    {0x8a, 1,{0x00}},

    //CMD_Page4         
    {0xff, 3,{0x98,0x81,0x04}},
    {0x6c, 1,{0x15}},
    //{0x00, 1,{0x00}},
    {0x6e, 1,{0x1a}},
    {0x6f, 1,{0xa5}},
    {0x3a, 1,{0x24}},
    {0x8d, 1,{0x1f}},
    {0x87, 1,{0xba}},
    {0x26, 1,{0x76}},
    {0xb2, 1,{0xd1}},
    {0xff, 3,{0x98,0x81,0x01}},
    {0x22, 1,{0x0a}},

    {0x31, 1,{0x00}},
    {0x40, 1,{0x33}},
    {0x42, 1,{0x33}},


    //CMD_Page1
    {0x50, 1,{0xa6}},
    {0x51, 1,{0xa6}},
    {0x53, 1,{0x52}},
    {0x55, 1,{0x55}},
    {0x60, 1,{0x06}},
    {0x61, 1,{0x01}},
    {0x62, 1,{0x19}},
    {0x63, 1,{0x10}},

    {0xa0, 1,{0x08}},
    {0xa1, 1,{0x0c}},
    {0xa2, 1,{0x17}},
    {0xa3, 1,{0x11}},
    {0xa4, 1,{0x12}},
    {0xa5, 1,{0x23}},
    {0xa6, 1,{0x18}},
    {0xa7, 1,{0x19}},
    {0xa8, 1,{0x62}},
    {0xa9, 1,{0x1a}},
    {0xaa, 1,{0x26}},
    {0xab, 1,{0x55}},
    {0xac, 1,{0x17}},
    {0xad, 1,{0x17}},
    {0xae, 1,{0x4b}},
    {0xaf, 1,{0x22}},
    {0xb0, 1,{0x2b}},
    {0xb1, 1,{0x3e}},
    {0xb2, 1,{0x69}},
    {0xb3, 1,{0x39}},

    {0xc0, 1,{0x08}},
    {0xc1, 1,{0x19}},
    {0xc2, 1,{0x22}},
    {0xc3, 1,{0x0b}},
    {0xc4, 1,{0x0e}},
    {0xc5, 1,{0x20}},
    {0xc6, 1,{0x14}},
    {0xc7, 1,{0x18}},
    {0xc8, 1,{0x6b}},
    {0xc9, 1,{0x1c}},
    {0xca, 1,{0x2a}},
    {0xcb, 1,{0x60}},
    {0xcc, 1,{0x1b}},
    {0xcd, 1,{0x1a}},
    {0xce, 1,{0x4f}},
    {0xcf, 1,{0x24}},
    {0xd0, 1,{0x28}},
    {0xd1, 1,{0x48}},
    {0xd2, 1,{0x63}},
    {0xd3, 1,{0x39}},

    //CMD_Page0
    {0xff, 3,{0x98,0x81,0x00}},

    {0x35, 1, {0x00}},	//TE on                
    {0x11,1,{0x00}},        // Sleep-Out
    {REGFLAG_DELAY, 120, {}},
    {0x29,1, {0x00}},       // Display On
    {REGFLAG_DELAY, 20, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}

};
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    // Display off sequence
    {0x28, 1, {0x00}},
    {REGFLAG_DELAY, 20, {}},
    {0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Sleep Mode On
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
/*static struct LCM_setting_table lcm_deep_sleep_mode_out_setting[] = {
// Display off sequence
{0xF0,	5,	{0x55, 0xaa, 0x52,0x08,0x00}},
{REGFLAG_DELAY, 10, {}},

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

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode = CMD_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;
#endif

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

    params->dsi.vertical_sync_active                = 4;// 3    2
    params->dsi.vertical_backporch                  = 20;// 20   1
    params->dsi.vertical_frontporch                 = 10; // 1  12
    params->dsi.vertical_active_line                = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active              = 20;// 50  2
    params->dsi.horizontal_backporch                = 100;//90
    params->dsi.horizontal_frontporch               = 100;//90
    params->dsi.horizontal_active_pixel             = FRAME_WIDTH;

    params->dsi.ssc_disable                         = 1;
    params->dsi.ssc_range                         = 4;
    params->dsi.HS_TRAIL                             = 15;
    params->dsi.esd_check_enable = 1;
    params->dsi.customization_esd_check_enable = 0;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
    params->dsi.PLL_CLOCK =225;
}

static void lcm_init(void)
{
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
    push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);  
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);
}

extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);
static int adc_read_vol(void)
{
    int adc[1];
    int data[4] ={0,0,0,0};
    int sum = 0;
    int adc_vol=0;
    int num = 0;

    for(num=0;num<10;num++)
    {
        IMM_GetOneChannelValue(12, data, adc);
        sum+=(data[0]*100+data[1]);
    }
    adc_vol = sum/10;

#if defined(BUILD_LK)
    printf("wujie  adc_vol is %d\n",adc_vol);
#else
    printk("wujie  adc_vol is %d\n",adc_vol);
#endif

    return (adc_vol>60) ? 0 : 1;
}
static unsigned int lcm_compare_id(void);
static void lcm_resume(void)
{
    lcm_init();
 //   lcm_compare_id();
}

static unsigned int lcm_compare_id(void)
{

    int array[4];
    char buffer[3];
    char id_high=0;
    char id_low=0;
    int id=0;

    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(100);

    //{0x39, 0xFF, 5, { 0xFF,0x98,0x06,0x04,0x01}}, // Change to Page 1 CMD
    array[0] = 0x00043902;
    array[1] = 0x018198FF;
    dsi_set_cmdq(array, 2, 1);

    array[0] = 0x00013700;
    dsi_set_cmdq(array, 1, 1);
    read_reg_v2(0x00, &buffer[0], 1);  //0x98

    id = buffer[0];

    id = id + + adc_read_vol();
    return (0x99 == id)?1:0;

}

LCM_DRIVER ili9881c_boe50_huashi_hd_lcm_drv =
{
    .name	    = "ili9881c_boe50_huashi_hd",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    .init_power     = lcm_init_power,
    .resume_power   = lcm_resume_power,
    .suspend_power  = lcm_suspend_power,
    //.esd_check = lcm_esd_check,
    //.esd_recover = lcm_esd_recover,
};

