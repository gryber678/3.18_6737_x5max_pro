/**************************************************************************
*  AW2028_LED.c
*
*  Create Date :
*
*  Modify Date :
*
*  Create by   : AWINIC Technology CO., LTD
*
*  Version     : 0.9, 2019/09/18
**************************************************************************/
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>
#include <linux/gameport.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/leds.h>
#include <linux/wakelock.h>

#define AW2028_I2C_NAME		"AW2028_LED"
#define AW2028_I2C_BUS		2
#define AW2028_I2C_ADDR		0x65

#define CONFIG_OF
static ssize_t AW2028_get_reg(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t AW2028_set_reg(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t AW2028_set_open(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t AW2028_get_open(struct device* cd, struct device_attribute *attr, char* buf);
static ssize_t AW2028_set_onoff(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t AW2028_get_onoff(struct device* cd, struct device_attribute *attr, char* buf);
static ssize_t AW2028_set_blink(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t AW2028_get_blink(struct device* cd, struct device_attribute *attr, char* buf);
static ssize_t AW2028_set_blinkmcolor(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t AW2028_get_blinkmcolor(struct device* cd, struct device_attribute *attr, char* buf);
static ssize_t AW2028_set_audiomode(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t AW2028_get_audiomode(struct device* cd, struct device_attribute *attr, char* buf);

static DEVICE_ATTR(reg, 0664, AW2028_get_reg,  AW2028_set_reg);
static DEVICE_ATTR(open, 0664, AW2028_get_open,  AW2028_set_open);
static DEVICE_ATTR(onoff, 0664, AW2028_get_onoff, AW2028_set_onoff);
static DEVICE_ATTR(blink, 0664, AW2028_get_blink, AW2028_set_blink);
static DEVICE_ATTR(blinkmcolor, 0664, AW2028_get_blinkmcolor, AW2028_set_blinkmcolor);
static DEVICE_ATTR(audiomode, 0664, AW2028_get_audiomode, AW2028_set_audiomode);

struct i2c_client *AW2028_i2c_client;

struct pinctrl *aw8090ctrl = NULL;
struct pinctrl_state *aw8090_shdn_high = NULL;
struct pinctrl_state *aw8090_shdn_low = NULL;
static struct class* breathlight_class = NULL;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO Control
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int AW8090_pinctrl_init(struct platform_device *pdev)
{
	int ret = 0;
	aw8090ctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(aw8090ctrl)) {
		dev_err(&pdev->dev, "Cannot find aw8090 pinctrl!");
		ret = PTR_ERR(aw8090ctrl);
		printk("%s devm_pinctrl_get fail!\n", __func__);
	}

	aw8090_shdn_high = pinctrl_lookup_state(aw8090ctrl, "aw8090_shdn_high");
	if (IS_ERR(aw8090_shdn_high)) {
		ret = PTR_ERR(aw8090_shdn_high);
		printk("%s : pinctrl err, aw8090_shdn_high\n", __func__);
	}

	aw8090_shdn_low = pinctrl_lookup_state(aw8090ctrl, "aw8090_shdn_low");
	if (IS_ERR(aw8090_shdn_low)) {
		ret = PTR_ERR(aw8090_shdn_low);
		printk("%s : pinctrl err, aw8090_shdn_low\n", __func__);
	}

	return ret;
}

static void AW8090_pa_pwron(void)
{
    printk("%s enter\n", __func__);
	pinctrl_select_state(aw8090ctrl, aw8090_shdn_low);
    msleep(5);
    pinctrl_select_state(aw8090ctrl, aw8090_shdn_high);
    msleep(10);
    printk("%s out\n", __func__);
}

static void AW8090_pa_pwroff(void)
{
	pinctrl_select_state(aw8090ctrl, aw8090_shdn_low);
	msleep(5);
}
//////////////////////////////////////////////////////
// i2c write and read
//////////////////////////////////////////////////////
unsigned char I2C_write_reg(unsigned char addr, unsigned char reg_data)
{
	char ret;
	u8 wdbuf[512] = {0};

	struct i2c_msg msgs[] = {
		{
			.addr	= AW2028_i2c_client->addr,
			.flags	= 0,
			.len	= 2,
			.buf	= wdbuf,
		},
	};

	if(NULL == AW2028_i2c_client)
	{
		pr_err("AW2028_i2c_client is NULL\n");
		return -1;
	}

	wdbuf[0] = addr;
	wdbuf[1] = reg_data;

	ret = i2c_transfer(AW2028_i2c_client->adapter, msgs, 1);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);

    return ret;
}

unsigned char I2C_read_reg(unsigned char addr)
{
	unsigned char ret;
	u8 rdbuf[512] = {0};

	struct i2c_msg msgs[] = {
		{
			.addr	= AW2028_i2c_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rdbuf,
		},
		{
			.addr	= AW2028_i2c_client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= rdbuf,
		},
	};

	if(NULL == AW2028_i2c_client)
	{
		pr_err("AW2028_i2c_client is NULL\n");
		return -1;
	}

	rdbuf[0] = addr;

	ret = i2c_transfer(AW2028_i2c_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);

    return rdbuf[0];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AW2028 LED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ms2timer(unsigned int time)
{
	unsigned char i, ret;
	unsigned int ref[16] = {4, 128, 256, 384, 512, 762, 1024, 1524, 2048, 2560, 3072, 4096, 5120, 6144, 7168, 8192};

	for(i=0; i<15; i++)
	{
		if(time <= ref[0])
		{
			return 0;
		}
		else if(time > ref[15])
		{
			return 15;
		}
		else if((time>ref[i]) && (time<=ref[i+1]))
		{
			if((time-ref[i]) <= (ref[i+1]-time))
			{
				return i;
			}
			else
			{
				return (i+1);
			}
		}
	}
}


unsigned char AW2028_LED_ON(unsigned char r, unsigned char g, unsigned char b)
{
	I2C_write_reg(0x00, 0x55);		// software reset

	I2C_write_reg(0x01, 0x01);		// GCR
	I2C_write_reg(0x03, 0x01);		// IMAX
	I2C_write_reg(0x04, 0x00);		// LCFG1
	I2C_write_reg(0x05, 0x00);		// LCFG2
	I2C_write_reg(0x06, 0x00);		// LCFG3
	I2C_write_reg(0x07, 0x07);		// LEDEN

	I2C_write_reg(0x10, r);		// ILED1
	I2C_write_reg(0x11, g);		// ILED2
	I2C_write_reg(0x12, b);		// ILED3
	I2C_write_reg(0x1C, 0xFF);		// PWM1
	I2C_write_reg(0x1D, 0xFF);		// PWM2
	I2C_write_reg(0x1E, 0xFF);		// PWM3

	return 0;
}

unsigned char AW2028_LED_OFF(void)
{
	I2C_write_reg(0x00, 0x55);		// software reset
	return 0;
}

unsigned char AW2028_LED_Blink(unsigned char r, unsigned char g, unsigned char b, unsigned int trise_ms, unsigned int ton_ms, unsigned int tfall_ms, unsigned int toff_ms)
{
	unsigned char trise, ton, tfall, toff;

	trise = ms2timer(trise_ms);
	ton   = ms2timer(ton_ms);
	tfall = ms2timer(tfall_ms);
	toff  = ms2timer(toff_ms);

I2C_write_reg(0x00, 0x55);  //reset register
I2C_write_reg(0x01, 0x01);  //enable chip
I2C_write_reg(0x03, 0x01);  //current 18mA
I2C_write_reg(0x04, 0x01);
I2C_write_reg(0x05, 0x01);
I2C_write_reg(0x06, 0x01);
I2C_write_reg(0x07, 0x07);  //enable 3 LEDs
I2C_write_reg(0x08, 0x08);
I2C_write_reg(0x10, 0xFF);  //R color#1
I2C_write_reg(0x11, 0x00); //G
I2C_write_reg(0x12, 0x00); //B
I2C_write_reg(0x13, 0x00);  //R color#2
I2C_write_reg(0x14, 0x00); //G
I2C_write_reg(0x15, 0xFF); //B
I2C_write_reg(0x16, 0x00);  //R color#3
I2C_write_reg(0x17, 0xFF); //G
I2C_write_reg(0x18, 0x00); //B
I2C_write_reg(0x19, 0xFF);  //R color#4
I2C_write_reg(0x1a, 0xFF); //G
I2C_write_reg(0x1b, 0xFF); //B
I2C_write_reg(0x1C, 0xFF);
I2C_write_reg(0x30, 0x64);  //risetime:1.04s on time 0.51S
I2C_write_reg(0x31, 0x64);  // fall time:1.04s off time 0.51s
I2C_write_reg(0x32, 0x00);  // delay time 0x00
I2C_write_reg(0x33, 0x07);  //run color#123 display forever
I2C_write_reg(0x09, 0x07);  //start breath
	return 0;
}

unsigned char AW2028_LED_Blink_mcolor(unsigned char r, unsigned char g, unsigned char b, unsigned int trise_ms, unsigned int ton_ms, unsigned int tfall_ms, unsigned int toff_ms)
{

if(r !=0){
	unsigned char trise, ton, tfall, toff;

	trise = ms2timer(trise_ms);
	ton   = ms2timer(ton_ms);
	tfall = ms2timer(tfall_ms);
	toff  = ms2timer(toff_ms);

	I2C_write_reg(0x00, 0x55);		// software reset

	I2C_write_reg(0x01, 0x01);		// GCR
	I2C_write_reg(0x03, 0x02);		// IMAX
	I2C_write_reg(0x04, 0x01);		// LCFG1
	I2C_write_reg(0x05, 0x01);		// LCFG2
	I2C_write_reg(0x06, 0x01);		// LCFG3
	I2C_write_reg(0x07, 0x07);		// LEDEN
	I2C_write_reg(0x08, 0x08);		// LEDCTR

	I2C_write_reg(0x10, r);		// ILED1
	I2C_write_reg(0x11, g);		// ILED2
	I2C_write_reg(0x12, b);		// ILED3

	I2C_write_reg(0x13, g);		// ILED1
	I2C_write_reg(0x14, b);		// ILED2
	I2C_write_reg(0x15, r);		// ILED3

	I2C_write_reg(0x16, b);		// ILED1
	I2C_write_reg(0x17, r);		// ILED2
	I2C_write_reg(0x18, g);		// ILED3

	I2C_write_reg(0x1C, 0xFF);		// PWM1
	I2C_write_reg(0x1D, 0xFF);		// PWM2
	I2C_write_reg(0x1E, 0xFF);		// PWM3

	I2C_write_reg(0x30, (trise<<4)|ton);		// PAT_T1		Trise & Ton
	I2C_write_reg(0x31, (tfall<<4)|toff);		// PAT_T2		Tfall & Toff
	I2C_write_reg(0x32, 0x00);		// PAT_T3				Tdelay
	I2C_write_reg(0x33, 0x07);		// PAT_T4 	  PAT_CTR & Color
	I2C_write_reg(0x34, 0x00);		// PAT_T5		    Timer

	I2C_write_reg(0x09, 0x07);		// PAT_RIN
	return 0;
}
else if(g !=0)
{
	unsigned char trise, ton, tfall, toff;

	trise = ms2timer(trise_ms);
	ton   = ms2timer(ton_ms);
	tfall = ms2timer(tfall_ms);
	toff  = ms2timer(toff_ms);

	I2C_write_reg(0x00, 0x55);		// software reset

	I2C_write_reg(0x01, 0x01);		// GCR
	I2C_write_reg(0x03, 0x02);		// IMAX
	I2C_write_reg(0x04, 0x01);		// LCFG1
	I2C_write_reg(0x05, 0x01);		// LCFG2
	I2C_write_reg(0x06, 0x01);		// LCFG3
	I2C_write_reg(0x07, 0x07);		// LEDEN
	I2C_write_reg(0x08, 0x08);		// LEDCTR
	
	I2C_write_reg(0x10, r);		// ILED1
	I2C_write_reg(0x11, g);		// ILED2
	I2C_write_reg(0x12, b);		// ILED3

	I2C_write_reg(0x13, r);		// ILED1
	I2C_write_reg(0x14, b);		// ILED2
	I2C_write_reg(0x15, g);		// ILED3

	I2C_write_reg(0x16, g);		// ILED1
	I2C_write_reg(0x17, r);		// ILED2
	I2C_write_reg(0x18, b);		// ILED3

	I2C_write_reg(0x1C, 0xFF);		// PWM1
	I2C_write_reg(0x1D, 0xFF);		// PWM2
	I2C_write_reg(0x1E, 0xFF);		// PWM3

	I2C_write_reg(0x30, (trise<<4)|ton);		// PAT_T1		Trise & Ton
	I2C_write_reg(0x31, (tfall<<4)|toff);		// PAT_T2		Tfall & Toff
	I2C_write_reg(0x32, 0x00);		// PAT_T3				Tdelay
	I2C_write_reg(0x33, 0x07);		// PAT_T4 	  PAT_CTR & Color
	I2C_write_reg(0x34, 0x00);		// PAT_T5		    Timer

	I2C_write_reg(0x09, 0x07);		// PAT_RIN
	return 0;
	
};

}

unsigned char AW2028_Audio_Corss_Zero(void)
{
	I2C_write_reg(0x00, 0x55);		// software reset

	I2C_write_reg(0x01, 0x01);		// GCR
	I2C_write_reg(0x03, 0x01);		// IMAX
	I2C_write_reg(0x07, 0x07);		// LEDEN
	I2C_write_reg(0x10, 0xFF);		// ILED1
	I2C_write_reg(0x11, 0xFF);		// ILED2
	I2C_write_reg(0x12, 0xFF);		// ILED3
	I2C_write_reg(0x1C, 0xFF);		// PWM1
	I2C_write_reg(0x1D, 0xFF);		// PWM2
	I2C_write_reg(0x1E, 0xFF);		// PWM3

	I2C_write_reg(0x40, 0x11);		// AUDIO_CTR
	I2C_write_reg(0x41, 0x07);		// AUDIO_LEDEN
	I2C_write_reg(0x42, 0x00);		// AUDIO_FLT
	I2C_write_reg(0x43, 0x1A);		// AGC_GAIN
	I2C_write_reg(0x44, 0x1F);		// GAIN_MAX
	I2C_write_reg(0x45, 0x3D);		// AGC_CFG
	I2C_write_reg(0x46, 0x20);		// ATTH 0x14
	I2C_write_reg(0x47, 0x10);		// RLTH 0x0A
	I2C_write_reg(0x48, 0x00);		// NOISE
	I2C_write_reg(0x49, 0x02);		// TIMER
	I2C_write_reg(0x40, 0x13);		// AUDIO_CTR

	return 0;
}

unsigned char AW2028_Audio_Timer(void)
{
	I2C_write_reg(0x00, 0x55);		// software reset

	I2C_write_reg(0x01, 0x01);		// GCR
	I2C_write_reg(0x03, 0x01);		// IMAX
	I2C_write_reg(0x07, 0x07);		// LEDEN
	I2C_write_reg(0x10, 0xFF);		// ILED1
	I2C_write_reg(0x11, 0xFF);		// ILED2
	I2C_write_reg(0x12, 0xFF);		// ILED3
	I2C_write_reg(0x1C, 0xFF);		// PWM1
	I2C_write_reg(0x1D, 0xFF);		// PWM2
	I2C_write_reg(0x1E, 0xFF);		// PWM3

	I2C_write_reg(0x40, 0x11);		// AUDIO_CTR
	I2C_write_reg(0x41, 0x07);		// AUDIO_LEDEN
	I2C_write_reg(0x42, 0x00);		// AUDIO_FLT
	I2C_write_reg(0x43, 0x1A);		// AGC_GAIN
	I2C_write_reg(0x44, 0x1F);		// GAIN_MAX
	I2C_write_reg(0x45, 0x3D);		// AGC_CFG
	I2C_write_reg(0x46, 0x14);		// ATTH
	I2C_write_reg(0x47, 0x0A);		// RLTH
	I2C_write_reg(0x48, 0x00);		// NOISE
	I2C_write_reg(0x49, 0x00);		// TIMER
	I2C_write_reg(0x40, 0x0B);		// AUDIO_CTR

	return 0;
}


unsigned char AW2028_Audio(unsigned char mode)
{
	if(mode > 5)
	{
		mode = 0;
	}
	I2C_write_reg(0x00, 0x55);		// software reset

	I2C_write_reg(0x01, 0x01);		// GCR
	I2C_write_reg(0x03, 0x01);		// IMAX
	I2C_write_reg(0x07, 0x07);		// LEDEN
	I2C_write_reg(0x10, 0xFF);		// ILED1
	I2C_write_reg(0x11, 0xFF);		// ILED2
	I2C_write_reg(0x12, 0xFF);		// ILED3
	I2C_write_reg(0x1C, 0xFF);		// PWM1
	I2C_write_reg(0x1D, 0xFF);		// PWM2
	I2C_write_reg(0x1E, 0xFF);		// PWM3

	I2C_write_reg(0x40, (mode<<3)|0x01);		// AUDIO_CTR
	I2C_write_reg(0x41, 0x07);		// AUDIO_LEDEN
	I2C_write_reg(0x42, 0x00);		// AUDIO_FLT
	I2C_write_reg(0x43, 0x1A);		// AGC_GAIN
	I2C_write_reg(0x44, 0x1F);		// GAIN_MAX
	I2C_write_reg(0x45, 0x3D);		// AGC_CFG
	I2C_write_reg(0x46, 0x14);		// ATTH
	I2C_write_reg(0x47, 0x0A);		// RLTH
	I2C_write_reg(0x48, 0x00);		// NOISE
	I2C_write_reg(0x49, 0x00);		// TIMER
	I2C_write_reg(0x40, (mode<<3)|0x03);		// AUDIO_CTR

	return 0;
}

unsigned char AW2028_Audio_OFF(void)
{
	I2C_write_reg(0x00, 0x55);		// software reset
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ssize_t AW2028_get_reg(struct device* cd,struct device_attribute *attr, char* buf)
{
	unsigned char reg_val;
	unsigned char i;
	ssize_t len = 0;
	for(i=0;i<0x4B;i++)
	{
		reg_val = I2C_read_reg(i);
		len += snprintf(buf+len, PAGE_SIZE-len, "reg%2X = 0x%2X, ", i,reg_val);
	}

	return len;
}

static ssize_t AW2028_set_reg(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
	unsigned int databuf[2];
	if(2 == sscanf(buf,"%x %x",&databuf[0], &databuf[1]))
	{
		I2C_write_reg((unsigned char)databuf[0],databuf[1]);
	}
	return len;
}

static ssize_t AW2028_get_open(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_OFF(void)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 0 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_ON(r, g, b)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 1  r   g   b  > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 1 255 255 255 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_Blink(r, g, b, trise, ton, tfall, tfall)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 2  r   g   b  trise ton tfall toff > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 2 255 255 255 1000    0 1000  1000 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_Audio(mode)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 3 mode > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 3   1  > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");

	return len;
}

static ssize_t AW2028_set_open(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
	unsigned int databuf[16];
	sscanf(buf,"%d",&databuf[0]);
	if(databuf[0] == 0) {		// OFF
		AW2028_LED_OFF();
	} else if(databuf[0] == 1) {	//ON
		sscanf(&buf[1], "%d %d %d", &databuf[1], &databuf[2], &databuf[3]);
		AW2028_LED_ON(databuf[1], databuf[2], databuf[3]);
	} else if(databuf[0] == 2) {	//Blink
		sscanf(&buf[1], "%d %d %d %d %d %d %d", &databuf[1], &databuf[2], &databuf[3], &databuf[4], &databuf[5], &databuf[6], &databuf[7]);
		AW2028_LED_Blink(databuf[1], databuf[2], databuf[3], databuf[4], databuf[5], databuf[6], databuf[7]);
	} else if(databuf[0] == 3) {	//Audio
		sscanf(&buf[1], "%d", &databuf[1]);
		AW8090_pa_pwron();
		AW2028_Audio(databuf[1]);
	} else if(databuf[0] == 4) {	// Audio OFF
		AW2028_Audio_OFF();
		AW8090_pa_pwroff();
	}
	return len;
}

static ssize_t AW2028_get_onoff(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_OFF(void)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 0 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_ON(r, g, b)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 1  r   g   b  > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 1 255 255 255 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	return len;
}

static ssize_t AW2028_set_onoff(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
	unsigned int databuf[16];
	sscanf(buf,"%d %d %d %d",&databuf[0], &databuf[1], &databuf[2], &databuf[3]);  //onoff r g b
	if(0 == databuf[0]) //OFF
	{
		AW2028_LED_OFF();
	}
	else
	{
		AW2028_LED_ON(databuf[1], databuf[2], databuf[3]);
	}
	return len;
}

static ssize_t AW2028_get_blink(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_Blink(r, g, b, trise, ton, tfall, tfall)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 2  r   g   b  trise ton tfall toff > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 2 255 255 255 1000    0 1000  1000 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	return len;
}

static ssize_t AW2028_set_blink(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
	unsigned int databuf[16];
	sscanf(buf, "%d %d %d %d %d %d %d %d", &databuf[0], &databuf[1], &databuf[2], &databuf[3], &databuf[4], &databuf[5], &databuf[6], &databuf[7]); //onoff r g b rise on fall off
	if( 0 == databuf[0])
	{
		AW2028_LED_OFF();
	}
	else{
		AW2028_LED_Blink(databuf[1], databuf[2], databuf[3], databuf[4], databuf[5], databuf[6], databuf[7]);
	}
	return len;
}

static ssize_t AW2028_get_blinkmcolor(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_Blink_MCOLOR(r, g, b, trise, ton, tfall, tfall)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 2  r   g   b  trise ton tfall toff > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 2 255 255 255 1000    0 1000  1000 > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	return len;
}

static ssize_t AW2028_set_blinkmcolor(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
	unsigned int databuf[16];
	sscanf(buf, "%d %d %d %d %d %d %d %d", &databuf[0], &databuf[1], &databuf[2], &databuf[3], &databuf[4], &databuf[5], &databuf[6], &databuf[7]); //onoff r g b rise on fall off
	if( 0 == databuf[0])
	{
		AW2028_LED_OFF();
	}
	else{
		AW2028_LED_Blink_mcolor(databuf[1], databuf[2], databuf[3], databuf[4], databuf[5], databuf[6], databuf[7]);
	}
	return len;
}

static ssize_t AW2028_get_audiomode(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "AW2028_LED_Audio(mode)\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 3 mode > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 3   1  > open\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "\n");
	return len;
}

static ssize_t AW2028_set_audiomode(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
	unsigned int databuf[16];
	sscanf(buf,"%d %d",&databuf[0], &databuf[1]); //onoff
	if(databuf[0] == 0) //onff
	{	// Audio OFF
		AW2028_Audio_OFF();
		AW8090_pa_pwroff();
	}
	else
	{	//Audio
		AW8090_pa_pwron();
		AW2028_Audio(databuf[1]);
	}
	return len;
}

static int AW2028_create_sysfs(void)
{
	int err = -1;
	dev_t dev_num = 0;
	int breathlight_major = 0;
	int breathlight_minor = 0;
	struct device* breathlight_dev = NULL;
	err = alloc_chrdev_region(&dev_num, 88, 1, "breathlight");
	breathlight_major = MAJOR(dev_num);
	breathlight_minor = MINOR(dev_num);
	breathlight_class = class_create(THIS_MODULE, "breathlight");
	breathlight_dev = device_create(breathlight_class, NULL, dev_num, "%s", "breathlight");
	err = device_create_file(breathlight_dev,&dev_attr_reg);
	err = device_create_file(breathlight_dev,&dev_attr_open);
	err = device_create_file(breathlight_dev,&dev_attr_onoff);
	err = device_create_file(breathlight_dev,&dev_attr_blink);
	err = device_create_file(breathlight_dev,&dev_attr_blinkmcolor);
	err = device_create_file(breathlight_dev,&dev_attr_audiomode);
	return err;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int AW2028_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned char reg_value;
	unsigned char cnt = 5;
//	struct led_classdev *led_cdev;
	int err = 0;
	printk("%s start\n", __func__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	AW2028_i2c_client = client;

	while(cnt>0)
	{
		reg_value = I2C_read_reg(0x00);
		printk("AW2028 CHIPID=0x%2x\n", reg_value);
		if(reg_value == 0xB1)
		{
			break;
		}
		msleep(10);
		cnt--;
	}
	if(!cnt)
	{
		err = -ENODEV;
		goto exit_create_singlethread;
	}

	AW2028_create_sysfs();

	return 0;

exit_create_singlethread:
	AW2028_i2c_client = NULL;
exit_check_functionality_failed:
	return err;
}

static int AW2028_i2c_remove(struct i2c_client *client)
{
//	AW2028_i2c_client = NULL;
	i2c_set_clientdata(client, NULL);
	return 0;
}


static const struct i2c_device_id AW2028_i2c_id[] = {
	{ AW2028_I2C_NAME, 0 },
	{ }
};

#ifdef CONFIG_OF
static const struct of_device_id extled_of_match[] = {
	{.compatible = "mediatek,aw2028_led"},
	{},
};
#endif

static struct i2c_driver AW2028_i2c_driver = {
    .driver = {
            //.owner  = THIS_MODULE,
            .name   = AW2028_I2C_NAME,
#ifdef CONFIG_OF
				.of_match_table = extled_of_match,
#endif
    },

    .probe          = AW2028_i2c_probe,
    .remove         = AW2028_i2c_remove,
    .id_table       = AW2028_i2c_id,

};



static int AW2028_led_remove(struct platform_device *pdev)
{
	printk("AW2028 remove\n");
	i2c_del_driver(&AW2028_i2c_driver);
	return 0;
}

static int AW2028_led_probe(struct platform_device *pdev)
{
	printk("%s start!\n", __func__);
	int ret = 0;

	/* pinctrl init */
	ret = AW8090_pinctrl_init(pdev);
	printk("aw8090  ret=%d\n",ret);
	ret = i2c_add_driver(&AW2028_i2c_driver);
	if (ret != 0) {
		printk("[%s] failed to register AW2028 i2c driver.\n", __func__);
		return ret;
	} else {
		printk("[%s] Success to register AW2028 i2c driver.\n", __func__);
	}

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id aw2028plt_of_match[] = {
	{.compatible = "mediatek,aw2028_led"},
	{},
};
#endif

static struct platform_driver AW2028_led_driver = {
	.probe	 = AW2028_led_probe,
	.remove	 = AW2028_led_remove,
	.driver = {
		.name  = "aw2028_led",
	#ifdef CONFIG_OF
		.of_match_table = aw2028plt_of_match,
	#endif
	}
};

static int __init AW2028_LED_init(void) {
	int ret;
	printk("%s start\n", __func__);

	ret = platform_driver_register(&AW2028_led_driver);
	if (ret) {
		printk("****[%s] Unable to register driver (%d)\n", __func__, ret);
		return ret;
	}
	return 0;
}

static void __exit AW2028_LED_exit(void) {
	printk("%s exit\n", __func__);
	platform_driver_unregister(&AW2028_led_driver);
}

module_init(AW2028_LED_init);
module_exit(AW2028_LED_exit);

MODULE_AUTHOR("<liweilei@awinic.com.cn>");
MODULE_DESCRIPTION("AWINIC AW2028 LED Driver");
MODULE_LICENSE("GPL");

