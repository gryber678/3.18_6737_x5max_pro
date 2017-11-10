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
#else
#include <linux/string.h>
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA   // END OF REGISTERS MARKER


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define R69339_LCM_ID       (0x9339)

#if defined(BUILD_UBOOT)
#define LCM_DEBUG(fmt, arg...) printf(fmt, ##arg)
#elif defined(BUILD_LK)
#define LCM_DEBUG(fmt, arg...) printf(fmt, ##arg)
#else
#define SENSORDB(fmt, arg...) xlog_printk(ANDROID_LOG_DEBUG, "[R69339]", fmt, ##arg)
#define LCM_DEBUG(fmt, arg...) printk(fmt, ##arg)
#endif

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};


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

    params->type   = LCM_TYPE_DSI;

    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

    // enable tearing-free
    params->dbi.te_mode				= LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

    params->dsi.mode   = SYNC_EVENT_VDO_MODE;//BURST_VDO_MODE;

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_FOUR_LANE;

    params->dsi.data_format.color_order 	= LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq		= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 		= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  	= LCM_DSI_FORMAT_RGB888;

    params->dsi.packet_size=256;
    //params->dsi.word_count=FRAME_WIDTH*3;	

    // Video mode setting		
    params->dsi.intermediat_buffer_num =  0; //2;

    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    params->dsi.word_count=720*3;

    params->dsi.vertical_sync_active			= 1;
    params->dsi.vertical_backporch				= 13;
    params->dsi.vertical_frontporch				= 11;	
    params->dsi.vertical_active_line				= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active			= 10; //2
    params->dsi.horizontal_backporch			= 40; //4
    params->dsi.horizontal_frontporch			= 100; //4
    params->dsi.horizontal_active_pixel			= FRAME_WIDTH;
    params->dsi.PLL_CLOCK = 300;
}


static void lcm_init(void)
{
    unsigned int data_array[16];

    LCM_DEBUG("R69339-=-LCM-20140718: %s, %d", __func__, __LINE__);
    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);

    /*data_array[0] = 0x00022902; 						  
    data_array[1] = 0x000000b0;
    dsi_set_cmdq(data_array, 2, 1);

    data_array[0] = 0x00022902; 						  
    data_array[1] = 0x000001D6;
    dsi_set_cmdq(data_array, 2, 1);

    data_array[0] = 0x00022902; 						  
    data_array[1] = 0x00000cb4;//4//4L_0C  3L_08  2L_04
    dsi_set_cmdq(data_array, 2, 1);		
    MDELAY(10);*/

    data_array[0] = 0x00290500; //set_display_on
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);

    data_array[0] = 0x00110500; // Sleep Out
    dsi_set_cmdq(data_array, 1, 1);	
    MDELAY(150);

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

static void lcm_resume(void)
{
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

    LCM_DEBUG("R69339-=-LCM-20140718: %s, %d", __func__, __LINE__);

    SET_RESET_PIN(1);
    MDELAY(25);
    SET_RESET_PIN(0);
    MDELAY(25);
    SET_RESET_PIN(1);
    MDELAY(50);


    array[0] = 0x00022902;                          
    array[1] = 0x000000b0; 
    dsi_set_cmdq(array, 2, 1);


    array[0] = 0x00053700;// read id return two byte,version and id

    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0xbf, buffer, 5);

    id = buffer[2]<<8 | buffer[3]; //we only need ID

#ifdef BUILD_LK
    LCM_DEBUG("%s, R69339-=-LCM-LK: id= x%08x,id0 = 0x%08x,id1 = 0x%08x,id2 = 0x%08x,id3 = 0x%08x,id4 = 0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
#else
    LCM_DEBUG("%s, R69339-=-LCM-KERNEL: id= x%08x,id0 = 0x%08x,id1 = 0x%08x,id2 = 0x%08x,id3 = 0x%08x,id4 = 0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
#endif


    return (R69339_LCM_ID == id)?1:0;
}



// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER r69339_hd720_dsi_vdo_lcm_drv = 
{
    .name			= "r69339_hd720_dsi_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    //.esd_check 	= lcm_esd_check,
    //.esd_recover 	= lcm_esd_recover	
};

