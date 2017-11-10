/* drivers/input/touchscreen/goodix_tool.c
 *
 * 2010 - 2014 Goodix Technology.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be a reference
 * to you, when you are integrating the GOODiX's CTP IC into your system,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Version: 1.0
 * Revision Record:
 *      V1.0:  first release. 2014/09/28.
 *
 */

#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <generated/utsrelease.h>
#include "include/gt1x_tpd_common.h"

//static ssize_t gt1x_tool_read(struct file *filp, char __user *buffer, size_t count, loff_t *ppos);
//static ssize_t gt1x_tool_write(struct file *filp, const char *buffer, size_t count, loff_t *ppos);

//static s32 gt1x_tool_release(struct inode *inode, struct file *filp);
//static s32 gt1x_tool_open(struct inode *inode, struct file *file);

#pragma pack(1)
typedef struct {
	u8 wr;			/*write read flag£¬0:R  1:W  2:PID 3:*/
	u8 flag;		/*0:no need flag/int 1: need flag  2:need int*/
	u8 flag_addr[2];	/*flag address*/
	u8 flag_val;		/*flag val*/
	u8 flag_relation;	/*flag_val:flag 0:not equal 1:equal 2:> 3:<*/
	u16 circle;		/*polling cycle*/
	u8 times;		/*plling times*/
	u8 retry;		/*I2C retry times*/
	u16 delay;		/*delay before read or after write*/
	u16 data_len;		/*data length*/
	u8 addr_len;		/*address length*/
	u8 addr[2];		/*address*/
	u8 res[3];		/*reserved*/
	u8 *data;		/*data pointer*/
} st_cmd_head;
#pragma pack()
st_cmd_head cmd_head;

s32 DATA_LENGTH = 0;
s8 IC_TYPE[16] = "GT9XX";


#define DATA_LENGTH_UINT    512
#define CMD_HEAD_LENGTH     (sizeof(st_cmd_head) - sizeof(u8 *))

static char procname[20] = { 0 };

static struct proc_dir_entry *gt1x_tool_proc_entry;
static const struct file_operations gt1x_tool_fops = {
//	.read = gt1x_tool_read,
//	.write = gt1x_tool_write,
//	.open = gt1x_tool_open,
//	.release = gt1x_tool_release,
	.owner = THIS_MODULE,
};
static void set_tool_node_name(char *procname)
{
	int v0 = 0, v1 = 0, v2 = 0;
	int ret;

	ret = sscanf(UTS_RELEASE, "%d.%d.%d", &v0, &v1, &v2);
	sprintf(procname, "gmnode%02d%02d%02d", v0, v1, v2);
}

int gt1x_init_tool_node(void)
{
	memset(&cmd_head, 0, sizeof(cmd_head));
	cmd_head.wr = 1;	/*if the first operation is read, will return fail.*/
	cmd_head.data = kzalloc(DATA_LENGTH_UINT, GFP_KERNEL);
	if (NULL == cmd_head.data) {
		GTP_ERROR("Apply for memory failed.");
		return -1;
	}
	GTP_INFO("Applied memory size:%d.", DATA_LENGTH_UINT);
	DATA_LENGTH = DATA_LENGTH_UINT - GTP_ADDR_LENGTH;

	set_tool_node_name(procname);

	gt1x_tool_proc_entry = proc_create(procname, 0664, NULL, &gt1x_tool_fops);
	if (gt1x_tool_proc_entry == NULL) {
		GTP_ERROR("Couldn't create proc entry!");
		return -1;
	}
	GTP_INFO("Create proc entry success!");
	return 0;
}

void gt1x_deinit_tool_node(void)
{
	remove_proc_entry(procname, NULL);
	kfree(cmd_head.data);
	cmd_head.data = NULL;
}

static s32 tool_i2c_read(u8 *buf, u16 len)
{
	u16 addr = (buf[0] << 8) + buf[1];

	if (!gt1x_i2c_read(addr, &buf[2], len))
		return 1;
	return -1;
}

static s32 tool_i2c_write(u8 *buf, u16 len)
{
	u16 addr = (buf[0] << 8) + buf[1];

	if (!gt1x_i2c_write(addr, &buf[2], len - 2))
		return 1;
	return -1;
}

static u8 relation(u8 src, u8 dst, u8 rlt)
{
	u8 ret = 0;

	switch (rlt) {
	case 0:
		ret = (src != dst) ? true : false;
		break;

	case 1:
		ret = (src == dst) ? true : false;
		GTP_DEBUG("equal:src:0x%02x   dst:0x%02x   ret:%d.", src, dst, (s32) ret);
		break;

	case 2:
		ret = (src > dst) ? true : false;
		break;

	case 3:
		ret = (src < dst) ? true : false;
		break;

	case 4:
		ret = (src & dst) ? true : false;
		break;

	case 5:
		ret = (!(src | dst)) ? true : false;
		break;

	default:
		ret = false;
		break;
	}

	return ret;
}

/*******************************************************
Function:
    Comfirm function.
Input:
  None.
Output:
    Return write length.
********************************************************/
static u8 comfirm(void)
{
	s32 i = 0;
	u8 buf[32];

	memcpy(buf, cmd_head.flag_addr, cmd_head.addr_len);

	for (i = 0; i < cmd_head.times; i++) {
		if (tool_i2c_read(buf, 1) <= 0) {
			GTP_ERROR("Read flag data failed!");
			return -1;
		}

		if (true == relation(buf[GTP_ADDR_LENGTH], cmd_head.flag_val, cmd_head.flag_relation)) {
			GTP_DEBUG("value at flag addr:0x%02x.", buf[GTP_ADDR_LENGTH]);
			GTP_DEBUG("flag value:0x%02x.", cmd_head.flag_val);
			break;
		}

		msleep(cmd_head.circle);
	}

	if (i >= cmd_head.times) {
		GTP_ERROR("Didn't get the flag to continue!");
		return -1;
	}

	return 0;
}

/*******************************************************
Function:
    Goodix tool write function.
Input:
  standard proc write function param.
Output:
    Return write length.
********************************************************/



/*******************************************************
Function:
    Goodix tool read function.
Input:
  standard proc read function param.
Output:
    Return read length.
********************************************************/

