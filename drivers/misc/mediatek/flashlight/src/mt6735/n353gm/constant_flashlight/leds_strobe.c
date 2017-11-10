#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/time.h>
#include "kd_flashlight.h"
#include <asm/io.h>
#include <asm/uaccess.h>
#include "kd_camera_typedef.h"
/* Vanzo:wuzhiyong on: Mon, 16 Mar 2015 14:16:57 +0800
 * sub_flashlight driver
 */ 
#include <mt-plat/upmu_common.h>
#include <mach/upmu_hw.h>
#include <mt-plat/mt_pwm.h>
//End of Vanzo:wuzhiyong
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/leds.h>



/******************************************************************************
 * Debug configuration
******************************************************************************/
/* availible parameter */
/* ANDROID_LOG_ASSERT */
/* ANDROID_LOG_ERROR */
/* ANDROID_LOG_WARNING */
/* ANDROID_LOG_INFO */
/* ANDROID_LOG_DEBUG */
/* ANDROID_LOG_VERBOSE */

#define TAG_NAME "[leds_strobe.c]"
#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
#define PK_DBG_FUNC(fmt, arg...)    pr_err(TAG_NAME "%s: " fmt, __func__ , ##arg)

/*#define DEBUG_LEDS_STROBE*/
#ifdef DEBUG_LEDS_STROBE
#define PK_DBG PK_DBG_FUNC
#else
#define PK_DBG(a, ...)
#endif

/******************************************************************************
 * local variables
******************************************************************************/

static DEFINE_SPINLOCK(g_strobeSMPLock);	/* cotta-- SMP proection */


static u32 strobe_Res;
static u32 strobe_Timeus;
static BOOL g_strobe_On;

static int g_duty = -1;
static int g_timeOutTimeMs;

static DEFINE_MUTEX(g_strobeSem);




static struct work_struct workTimeOut;

/* #define FLASH_GPIO_ENF GPIO12 */
/* #define FLASH_GPIO_ENT GPIO13 */


/*****************************************************************************
Functions
*****************************************************************************/
static void work_timeOutFunc(struct work_struct *data);


//struct platform_device *flashlight_plt_dev = NULL;

struct pinctrl *flashctrl = NULL;
struct pinctrl_state *mode_gpio_h = NULL;
struct pinctrl_state *mode_gpio_l = NULL;
struct pinctrl_state *en_gpio_h = NULL;
struct pinctrl_state *en_gpio_l = NULL;

int flashlight_gpio_init(struct platform_device *pdev)
{
  int ret = 0;

  printk(KERN_ERR "%s line %d \n",__func__,__LINE__);
  flashctrl = devm_pinctrl_get(&pdev->dev);
  if (IS_ERR(flashctrl)) {
    dev_err(&pdev->dev, "Cannot find flash pinctrl!");
    printk(KERN_ERR"--------Cannot find flash pinctrl -----------\n");
    ret = PTR_ERR(flashctrl);
  }
  /*Cam0 Power/Rst Ping initialization */
  mode_gpio_h= pinctrl_lookup_state(flashctrl, "state_mode_output1");
  if (IS_ERR(mode_gpio_h)) {
    ret = PTR_ERR(mode_gpio_h);
    pr_err("%s : pinctrl err, mode_gpio_h\n", __func__);
  }

  mode_gpio_l = pinctrl_lookup_state(flashctrl, "state_mode_output0");
  if (IS_ERR(mode_gpio_l)) {
    ret = PTR_ERR(mode_gpio_l);
    pr_err("%s : pinctrl err, mode_gpio_l\n", __func__);
  }


  en_gpio_h = pinctrl_lookup_state(flashctrl, "state_en_output1");
  if (IS_ERR(en_gpio_h)) {
    ret = PTR_ERR(en_gpio_h);
    pr_err("%s : pinctrl err, en_gpio_h\n", __func__);
  }

  en_gpio_l = pinctrl_lookup_state(flashctrl, "state_en_output0");
  if (IS_ERR(en_gpio_l)) {
    ret = PTR_ERR(en_gpio_l);
    pr_err("%s : pinctrl err, en_gpio_l\n", __func__);
  }
  
  printk(KERN_ERR "%s line %d \n",__func__,__LINE__);
  return ret;
}
struct flash_setting
{
    u8 nled_mode; //0, off; 1, on; 2, blink;
    bool use_mode; //0 as flashlight; 1 as touch;
};

int led_set_pwm(int pwm_num, struct flash_setting* led)
{
    struct pwm_spec_config pwm_setting;
    int time_index = 0;
    
    printk(KERN_ERR "%s line %d \n",__func__,__LINE__);    
    
    pwm_setting.pwm_no = pwm_num;
    pwm_setting.mode = PWM_MODE_OLD;
    pwm_setting.clk_src = PWM_CLK_OLD_MODE_32K;
    
    switch (led->nled_mode)
    {
        case 0 :
            pwm_setting.PWM_MODE_OLD_REGS.THRESH = 0;
            pwm_setting.clk_div = CLK_DIV1;
            pwm_setting.PWM_MODE_OLD_REGS.DATA_WIDTH = 100;
            break;
        
        case 1 :
            if(led->use_mode)
                pwm_setting.PWM_MODE_OLD_REGS.THRESH = 20;
            else
                pwm_setting.PWM_MODE_OLD_REGS.THRESH = 70;
            
            pwm_setting.clk_div = CLK_DIV1;
            pwm_setting.PWM_MODE_OLD_REGS.DATA_WIDTH = 100;
            break;
    }
    
    pwm_setting.PWM_MODE_OLD_REGS.IDLE_VALUE = 0;
    pwm_setting.PWM_MODE_OLD_REGS.GUARD_VALUE = 0;
    pwm_setting.PWM_MODE_OLD_REGS.GDURATION = 0;
    pwm_setting.PWM_MODE_OLD_REGS.WAVE_NUM = 0;
    pwm_set_spec_config(&pwm_setting);
    
    return 0;
}


int FL_Enable(void)
{
    struct flash_setting led_tmp_setting = {1, 0};
    printk(KERN_ERR "%s line %d \n",__func__,__LINE__);
    pinctrl_select_state(flashctrl,mode_gpio_h);
    
    led_set_pwm(3,&led_tmp_setting);
    
    if(g_duty != 1)
    {
        led_tmp_setting.nled_mode = 1;
        led_tmp_setting.use_mode = 1;
        led_set_pwm(3,&led_tmp_setting);
    }
    else
    {
        led_tmp_setting.nled_mode = 1;
        led_tmp_setting.use_mode = 0;
        led_set_pwm(3,&led_tmp_setting);
    }
    //sub_engpio_high();
    return 0;
}



int FL_Disable(void)
{
    struct flash_setting led_tmp_setting = {0, 0};
    printk(KERN_ERR "%s line %d \n",__func__,__LINE__);
    
    led_set_pwm(3,&led_tmp_setting);  
    pinctrl_select_state(flashctrl,mode_gpio_l);

    return 0;
}

int FL_dim_duty(kal_uint32 duty)
{
	PK_DBG(" FL_dim_duty line=%d\n", __LINE__);
	g_duty = duty;
	return 0;
}




int FL_Init(void)
{
    struct flash_setting led_tmp_setting = {0, 0};
    
    printk(KERN_ERR "%s line %d \n",__func__,__LINE__);    
    
    led_set_pwm(3,&led_tmp_setting);
    pinctrl_select_state(flashctrl,mode_gpio_l);

    INIT_WORK(&workTimeOut, work_timeOutFunc);    
    return 0;
}


int FL_Uninit(void)
{
	FL_Disable();
	return 0;
}

/*****************************************************************************
User interface
*****************************************************************************/

static void work_timeOutFunc(struct work_struct *data)
{
	FL_Disable();
	PK_DBG("ledTimeOut_callback\n");
}



enum hrtimer_restart ledTimeOutCallback(struct hrtimer *timer)
{
	schedule_work(&workTimeOut);
	return HRTIMER_NORESTART;
}

static struct hrtimer g_timeOutTimer;
void timerInit(void)
{
	INIT_WORK(&workTimeOut, work_timeOutFunc);
	g_timeOutTimeMs = 1000;
	hrtimer_init(&g_timeOutTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	g_timeOutTimer.function = ledTimeOutCallback;
}



static int constant_flashlight_ioctl(unsigned int cmd, unsigned long arg)
{
	int i4RetValue = 0;
	int ior_shift;
	int iow_shift;
	int iowr_shift;

	ior_shift = cmd - (_IOR(FLASHLIGHT_MAGIC, 0, int));
	iow_shift = cmd - (_IOW(FLASHLIGHT_MAGIC, 0, int));
	iowr_shift = cmd - (_IOWR(FLASHLIGHT_MAGIC, 0, int));
/*	PK_DBG
	    ("LM3642 constant_flashlight_ioctl() line=%d ior_shift=%d, iow_shift=%d iowr_shift=%d arg=%d\n",
	     __LINE__, ior_shift, iow_shift, iowr_shift, (int)arg);
*/
	switch (cmd) {

	case FLASH_IOC_SET_TIME_OUT_TIME_MS:
		PK_DBG("FLASH_IOC_SET_TIME_OUT_TIME_MS: %d\n", (int)arg);
		g_timeOutTimeMs = arg;
		break;


	case FLASH_IOC_SET_DUTY:
		PK_DBG("FLASHLIGHT_DUTY: %d\n", (int)arg);
		FL_dim_duty(arg);
		break;


	case FLASH_IOC_SET_STEP:
		PK_DBG("FLASH_IOC_SET_STEP: %d\n", (int)arg);

		break;

	case FLASH_IOC_SET_ONOFF:
		PK_DBG("FLASHLIGHT_ONOFF: %d\n", (int)arg);
		if (arg == 1) {

			int s;
			int ms;

			if (g_timeOutTimeMs > 1000) {
				s = g_timeOutTimeMs / 1000;
				ms = g_timeOutTimeMs - s * 1000;
			} else {
				s = 0;
				ms = g_timeOutTimeMs;
			}

			if (g_timeOutTimeMs != 0) {
				ktime_t ktime;

				ktime = ktime_set(s, ms * 1000000);
				hrtimer_start(&g_timeOutTimer, ktime, HRTIMER_MODE_REL);
			}
			FL_Enable();
		} else {
			FL_Disable();
			hrtimer_cancel(&g_timeOutTimer);
		}
		break;
	default:
		PK_DBG(" No such command\n");
		i4RetValue = -EPERM;
		break;
	}
	return i4RetValue;
}




static int constant_flashlight_open(void *pArg)
{
	int i4RetValue = 0;

	PK_DBG("constant_flashlight_open line=%d\n", __LINE__);

	if (0 == strobe_Res) {
		FL_Init();
		timerInit();
	}
	PK_DBG("constant_flashlight_open line=%d\n", __LINE__);
	spin_lock_irq(&g_strobeSMPLock);


	if (strobe_Res) {
		PK_DBG(" busy!\n");
		i4RetValue = -EBUSY;
	} else {
		strobe_Res += 1;
	}


	spin_unlock_irq(&g_strobeSMPLock);
	PK_DBG("constant_flashlight_open line=%d\n", __LINE__);

	return i4RetValue;

}


static int constant_flashlight_release(void *pArg)
{
	PK_DBG(" constant_flashlight_release\n");

	if (strobe_Res) {
		spin_lock_irq(&g_strobeSMPLock);

		strobe_Res = 0;
		strobe_Timeus = 0;

		/* LED On Status */
		g_strobe_On = FALSE;

		spin_unlock_irq(&g_strobeSMPLock);

		FL_Uninit();
	}

	PK_DBG(" Done\n");

	return 0;

}


FLASHLIGHT_FUNCTION_STRUCT constantFlashlightFunc = {
	constant_flashlight_open,
	constant_flashlight_release,
	constant_flashlight_ioctl
};


MUINT32 constantFlashlightInit(PFLASHLIGHT_FUNCTION_STRUCT *pfFunc)
{
	if (pfFunc != NULL)
		*pfFunc = &constantFlashlightFunc;
	return 0;
}



/* LED flash control for high current capture mode*/
ssize_t strobe_VDIrq(void)
{

	return 0;
}
EXPORT_SYMBOL(strobe_VDIrq);
