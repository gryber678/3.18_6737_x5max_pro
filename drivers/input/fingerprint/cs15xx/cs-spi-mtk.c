/*
   Copyright (c) 2015 by ChipSailing Technology.
   All rights reserved.

   ChipSailing SPI Sensor driver for Android MTK platform

Author:
Maintain:

 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>  
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/cdev.h>
#include <linux/spi/spi.h>
#include <linux/input.h>
//#include <linux/spi/spidev.h>
#include <linux/delay.h> //mdelay
#include <linux/uaccess.h>
#include <linux/memory.h>//memset
#include <linux/types.h>
#include <linux/of_irq.h>
#include <linux/rtpm_prio.h>
#include <linux/interrupt.h>
#include <linux/wakelock.h>
#include "cs-spi-mtk.h"


#define Driver_Version			"7.0.1"
//#define DRIVER_CLASS_FILE		"Chip_Sailing_file"
//#define DRIVER_NODE_FILE		"CS_ctrl"
#define DRIVER_NAME			"ChipSailing"
#define DATA_LENGTH			32	//spi read or write buffer length 
//static int IMAGE_NUM = 45;
#define IMG_LENGTH			9856	//25600
#define READ_LENGTH			10240
#define bufsiz				  4096
static struct timer_list	csFingerDetecTimer,csTimer;

static struct input_dev *cs15xx_inputdev = NULL;
static int  cs_work_func_finish;
static u64 kickNum,irqNum,irq_current_num;
static	u64 tempNum1,tempNum2;
static  u64 currentMs3,currentMs4,temp_currentMs,currentMs5;
u64   num1,num2;
static int csTimerOn = 0;   //0:csTimer is off        1:csTimer is on
int g_fingerup = 1;
int g_Identify_finish = 0;

struct cs_fp_data{
    spinlock_t		spi_lock;
    struct spi_device	*spi;
    struct list_head	device_entry;
    struct mutex		buf_lock;
    unsigned		users;
    dev_t 			devno;	// device number
    struct cdev 		cdev;	// character device
    struct class 		*class;	// class device
    struct device		*device;
    u8			*buffer;
    struct input_dev 	*input;
};



static DECLARE_WAIT_QUEUE_HEAD(waiter);
static DEFINE_MUTEX(device_list_lock);
static LIST_HEAD(device_list);
static struct cs_fp_data *spi_dev;
struct work_struct  work;
struct device_node *cs_finger_irq_node;//sunwei
static u8 tx[DATA_LENGTH];
static u8 rx[DATA_LENGTH];
static unsigned  int finger_status;  //finger_status=1：按下的状态     finger_status=0：未抬起		
//static u8 image[IMG_LENGTH];
//static u8 rx_buffer[READ_LENGTH];

unsigned int fingerprint_irq = 0;   
static struct pinctrl *pinctrl1;
static struct pinctrl_state *pins_default;
static struct pinctrl_state *eint_as_int, *eint_pulldown, *eint_pulldisable, *rst_output0, *rst_output1,*power_output0,*power_output1;
struct pinctrl *hct_finger_pinctrl;
struct pinctrl_state *hct_finger_power_on, *hct_finger_power_off,*hct_finger_reset_high,*hct_finger_reset_low,*hct_finger_spi0_mi_as_spi0_mi,*hct_finger_spi0_mi_as_gpio,
                     *hct_finger_spi0_mo_as_spi0_mo,*hct_finger_spi0_mo_as_gpio,*hct_finger_spi0_clk_as_spi0_clk,*hct_finger_spi0_clk_as_gpio,*hct_finger_spi0_cs_as_spi0_cs,*hct_finger_spi0_cs_as_gpio,
                     *hct_finger_int_as_int,*hct_finger_power18_on,*hct_finger_power18_off;


static bool IRQ_FLAG = false;
static int  KEY_MODE_FLAG;

static struct task_struct *cs_irq_thread = NULL;
//static struct workqueue_struct *mtk_tpd_wq;
static struct  fasync_struct *async;
//high_time and low_time Decision SCK frequency , cs1062 Using 5Ms
static struct mt_chip_conf spi_conf = {
    .setuptime = 7,//15,cs
    .holdtime = 7,//15, cs
    .high_time = 16,//6 sck
    .low_time =  17,//6  sck
    .cs_idletime = 3,//20,
    //.ulthgh_thrsh = 0,
    .cpol = 0,
    .cpha = 0,
    .rx_mlsb = 1,
    .tx_mlsb = 1,
    .tx_endian = 0,
    .rx_endian = 0,
    .com_mod = DMA_TRANSFER,
    //.com_mod = FIFO_TRANSFER,
    .pause = 0,
    .finish_intr = 1,
    .deassert = 0,
    .ulthigh = 0,
    .tckdly = 0,
};

extern long cs_fp_compat_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);
static const struct file_operations cs_fp_fops = {
    .owner =	THIS_MODULE,
    .write =	cs_fp_write,
    .read =		cs_fp_read,
    .unlocked_ioctl = cs_fp_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = cs_fp_compat_ioctl,
#endif
    .open =		cs_fp_open,
    .release =	cs_fp_release,
    .fasync  =	cs106x_fasync,
    //.llseek =	no_llseek,
};
/*
   static struct of_device_id cs_fp_match_table[] =
   {
   {.compatible = "mediatek,cs_finger",},
   {},
   };
 */


static int cs_fp_remove(struct spi_device *spi)
{


    /* make sure ops on existing fds can abort cleanly */
    spin_lock_irq(&spi_dev->spi_lock);
    spi_dev->spi = NULL;
    spin_unlock_irq(&spi_dev->spi_lock);

    //if(rx_buffer != NULL)
    //kfree(rx_buffer);

    /* prevent new opens */
    mutex_lock(&device_list_lock);
    //delete character device driver
    cdev_del(&(spi_dev->cdev));
    //unregister_chrdev(cs_fp_MAJOR, DRIVER_NAME );
    //unregister_chrdev_region(&(spi_dev->cdev.dev), 1);
    device_destroy(spi_dev->class, spi_dev->devno);
    class_destroy(spi_dev->class);

    mutex_unlock(&device_list_lock);

    return 0;
}

static struct spi_driver cs_fp_spi_driver = {
    .driver = {
        .name =	DRIVER_NAME,
        .bus = &spi_bus_type,
        .owner = THIS_MODULE,
        //.of_match_table = cs_fp_match_table,
    },
    .probe =	cs_fp_probe,
    .remove =	cs_fp_remove,
    //least suspend/resume
};




int BitCount(unsigned int n)
{
    unsigned int cnt = 0;

    while(n>0){
        if((n&1)==1)
            cnt++;
        n>>=1;
    }

    return cnt>7?1:0;
}

static int force_to_idle(void) {

    int ret = 0;
    int count = 0;

    switch ( cs_read_sfr(0x46,1) ){
        case 0x70:
            break;
        case 0x71:
            do {
                cs_write_sfr(0x46, 0x70);
                if (0x70 == cs_read_sfr(0x46, 1)) {
                    break;
                }
            } while (++count < 3);
            if (count == 3)
                ret = -1;

            count = 0;
            do {
                if (cs_read_sfr(0x50,1) & 0x01) {
                    cs_write_sfr(0x50, 0x01);
                    break;
                }
            } while (++count < 1);

            break;
        default:
            do {
                //wakeup();
                cs_write_sfr(0x46, 0x70);
                if (0x70 == cs_read_sfr(0x46,1)) {
                    break;
                }
            } while (++count < 3);
            if (count == 3)
                ret = -2;

            count = 0;
            do {
                if (cs_read_sfr(0x50, 1) & 0x01) {
                    cs_write_sfr(0x50, 0x01);
                    break;
                }
            } while (++count < 1);
            break;
    }

    return 0;
}

void forceMode(int mod)
{
    force_to_idle();
    //Setmode to normal

    switch(mod)
    {

        case 4:  //deep sleep
            cs_write_sfr(0x46, 0x76);break;
        case 3:  //normal
            cs_write_sfr(0x46, 0x71);break;
        case 2:  //sleep
            cs_write_sfr(0x46, 0x72);break;
        case 1:  //idle
            cs_write_sfr(0x46, 0x70);break;
        default:
            break;
    }


} 


static struct spi_board_info spi_board_chipsailing[] __initdata = {
    [0] = {
        .modalias= DRIVER_NAME,
        .bus_num = 0,
        .chip_select=0,
        .mode = SPI_MODE_0,
        .controller_data = &spi_conf,
    },
};


static int cs_fp_open(struct inode *inode, struct file *filp)
{	

    struct cs_fp_data	*spidev;
    int	status = -ENXIO;
    spidev = kmalloc(bufsiz, GFP_KERNEL);
    cs_debug("%s\n",__func__);
    mutex_lock(&device_list_lock);
    filp->private_data = spidev;
    list_for_each_entry(spidev, &device_list, device_entry) {
        if (spidev->devno == inode->i_rdev) {
            status = 0;
            break;
        }
    }
    if (status == 0) {
        spidev->users++;
        filp->private_data = spidev;
        nonseekable_open(inode, filp);
    }
    else


        mutex_unlock(&device_list_lock);

    return status;

}



// Register interrupt device
static int cs15xx_create_inputdev(void)
{
    cs15xx_inputdev = input_allocate_device();
    if (!cs15xx_inputdev)
    {
        printk("[ChipSailing]cs15xx_inputdev create faile!\n");
        return -ENOMEM;
    }

    __set_bit(EV_KEY,cs15xx_inputdev->evbit);
    __set_bit(KEY_KICK1S,cs15xx_inputdev->keybit);
    __set_bit(KEY_KICK2S,cs15xx_inputdev->keybit);
    __set_bit(KEY_LONGTOUCH,cs15xx_inputdev->keybit);
    __set_bit(KEY_POWER,cs15xx_inputdev->keybit);

    __set_bit(KEY_HOME,cs15xx_inputdev->keybit);
    __set_bit(KEY_BACK,cs15xx_inputdev->keybit);
    __set_bit(KEY_MENU,cs15xx_inputdev->keybit);
    __set_bit(KEY_F11,cs15xx_inputdev->keybit);  
    __set_bit(KEY_F10,cs15xx_inputdev->keybit);
    __set_bit(KEY_F6,cs15xx_inputdev->keybit); 
    __set_bit(KEY_F5,cs15xx_inputdev->keybit);   

    __set_bit(NAV_UP,cs15xx_inputdev->keybit);
    __set_bit(NAV_DOWN,cs15xx_inputdev->keybit);
    __set_bit(NAV_LEFT,cs15xx_inputdev->keybit);
    __set_bit(NAV_RIGHT,cs15xx_inputdev->keybit);


    cs15xx_inputdev->id.bustype = BUS_HOST;
    cs15xx_inputdev->name = "cs15xx_inputdev";
    cs15xx_inputdev->id.version = 1;


    if (input_register_device(cs15xx_inputdev))
    {
        printk("[ChipSailing]:register inputdev failed\n");
        input_free_device(cs15xx_inputdev);
        return -ENOMEM;
    }
    else
    {
        //			printk("[ChipSailing]:register inputdev success!\n");
        return 0;
    }
}



static int cs_fp_release(struct inode *inode, struct file *filp)
{


    struct cs_fp_data	*spidev;
    //	int	status = 0;
    cs_debug("%s\n",__func__);
    mutex_lock(&device_list_lock);
    spidev = filp->private_data;
    filp->private_data = NULL;

    // last close?
    spidev->users--;
    if (!spidev->users) {
        int	dofree;
        spin_lock_irq(&spidev->spi_lock);
        dofree = (spidev->spi == NULL);
        spin_unlock_irq(&spidev->spi_lock);

        if (dofree)
            kfree(spidev);
    }

    mutex_unlock(&device_list_lock);

    return 0;
}

/* read function for character device driver*/
    static ssize_t
cs_fp_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)


{
    struct cs_fp_data *spidev;
    ssize_t status = 0;
    if(count > bufsiz)
        return -EMSGSIZE;
    spidev = filp->private_data;
    //mutex_lock(&spi_dev->buf_lock);
    //status = spi_send_cmd_fifo(spidev,buf,rx,count);
    //spi = spi_dev_get(spi_dev->spi);
    //ret = copy_to_user(buf,image,count);
    if(spidev == NULL){
        status = -EFAULT;
        cs_error("ChipSailing : copy_to_user error ! \n");
    }

    cs_debug("%s,     \n",__func__);
    return 0;
}

/* write function for character device driver */
    static ssize_t
cs_fp_write(struct file *filp, const char __user *buf,size_t count, 
        loff_t *f_pos)

{
    int ret;
    unsigned char buffer[128] = {0};
    cs_debug("%s\n",__func__);

    //copy from userspace
    ret = copy_from_user(buffer,buf,count-1);
    if(ret < 0){
        cs_error( "%s, copy data from user space, failed\n", __func__);
        return -1;
    }

    if(strcmp(buffer,"info") == 0){
        cs_read_info();
    }
    if(strcmp(buffer,"reset") == 0){
        cs_fp_config();
    }
    if(strcmp(buffer,"test") == 0){
        printk("fingerprint_irq==%d \n",fingerprint_irq);
    }

    return -1;
}

static int cs_fp_message(struct spi_ioc_transfer *u_xfers, unsigned n_xfers)

{
    struct spi_message	msg;
    struct spi_transfer	*k_xfers;
    struct spi_transfer	*k_tmp;
    struct spi_ioc_transfer *u_tmp;
    unsigned		n, total;
    u8			*buf;
    int			status = -EFAULT;

    spi_message_init(&msg);
    k_xfers = kcalloc(n_xfers, sizeof(*k_tmp), GFP_KERNEL);
    if (k_xfers == NULL){
        cs_error("ChipSailing : kcalloc transfer xfers error ! \n");
        return -ENOMEM;
    }

    // Construct spi_message, copying any tx data to bounce buffer.
    // We walk the array of user-provided transfers, using each one
    //to initialize a kernel version of the same transfer.
    if(spi_dev->buffer == NULL){
        spi_dev->buffer = kmalloc(bufsiz, GFP_KERNEL);
        if(spi_dev->buffer == NULL)
            return -ENOMEM;
    }

    buf = spi_dev->buffer;
    total = 0;
    for (n = n_xfers, k_tmp = k_xfers, u_tmp = u_xfers; n>0;n--, k_tmp++, u_tmp++) {

        k_tmp->len = u_tmp->len;

        total += k_tmp->len;
        if (total > bufsiz) {
            status = -EMSGSIZE;
            cs_error("ChipSailing : total size too large ! %x\n",total);
            goto done;
        }

        if (u_tmp->rx_buf) {
            k_tmp->rx_buf = buf;
            if (!access_ok(VERIFY_WRITE, (u8 __user *)(uintptr_t) u_tmp->rx_buf,u_tmp->len)){

                cs_debug("ChipSailing : rx_buf tese access not ok ! \n");
                goto done;
            }
        }

        if (u_tmp->tx_buf) {
            k_tmp->tx_buf = buf;
            if (copy_from_user(buf, (const u8 __user *)(uintptr_t) u_tmp->tx_buf,u_tmp->len)){

                cs_error("ChipSailing : tx_buf copy_from_user error ! \n");
                goto done;
            }
        }


        buf += k_tmp->len;

        k_tmp->cs_change = !!u_tmp->cs_change;
        k_tmp->bits_per_word = u_tmp->bits_per_word;
        k_tmp->delay_usecs = u_tmp->delay_usecs;
        k_tmp->speed_hz = u_tmp->speed_hz;
#ifdef VERBOSE
        dev_dbg(&spi_dev->spi->dev,
                "  xfer len %zd %s%s%s%dbits %u usec %uHz\n",
                u_tmp->len,
                u_tmp->rx_buf ? "rx " : "",
                u_tmp->tx_buf ? "tx " : "",
                u_tmp->cs_change ? "cs " : "",
                u_tmp->bits_per_word ? : spi_dev->spi->bits_per_word,
                u_tmp->delay_usecs,
                u_tmp->speed_hz ? : spi_dev->spi->max_speed_hz);
#endif
        spi_message_add_tail(k_tmp, &msg);
    }

    status = spi_sync(spi_dev->spi, &msg);
    if (status < 0){
        cs_error("ChipSailing : spi sync error in %s  \n",__func__);
        goto done;
    }

    // copy any rx data out of bounce buffer
    buf = spi_dev->buffer;
    for (n = n_xfers, u_tmp = u_xfers; n; n--, u_tmp++) {
        if (u_tmp->rx_buf) {
            if (__copy_to_user((u8 __user *)(uintptr_t) u_tmp->rx_buf, buf,	u_tmp->len)) {

                status = -EFAULT;
                cs_error("ChipSailing : copy_to_user error in %s  \n",__func__);
                goto done;
            }
        }
        buf += u_tmp->len;
    }

    status = total;


done:	

    kfree(k_xfers);
    return status;
}

#ifdef CONFIG_COMPAT
long cs_fp_compat_ioctl(struct file *filp,unsigned int cmd,unsigned long arg){
    return cs_fp_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
}
#endif
static long cs_fp_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{	
    struct cs_fp_data	*spidev;
    struct spi_device	*spi;
    int err = 0;
    int retval = 0;
    int ret = 0;
    u32 tmp;
    unsigned		n_ioc;
    struct spi_ioc_transfer	*ioc;
    cs_debug("ChipSailing into ioctl \n");

    /* Check type and command number */
    if (_IOC_TYPE(cmd) != SPI_IOC_MAGIC)
        return -ENOTTY;

    /* Check access direction once here; don't repeat below.
     * IOC_DIR is from the user perspective, while access_ok is
     * from the kernel perspective; so they look reversed.
     */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    if (err == 0 && _IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    if (err){
        cs_error("ChipSailing : _IOC_DIR  error! \n");
        return -EFAULT;
    }
    spidev = filp->private_data;

    spi = spi_dev_get(spidev->spi);

    mutex_lock(&spidev->buf_lock);
    switch(cmd){
        /* read requests */
        case SPI_IOC_RD_MODE:

            retval = __put_user(spi->mode & SPI_MODE_MASK,
                    (__u8 __user *)arg);
            break;
        case SPI_IOC_RD_LSB_FIRST:
            retval = __put_user((spi->mode & SPI_LSB_FIRST) ?  1 : 0,
                    (__u8 __user *)arg);
            break;
        case SPI_IOC_RD_BITS_PER_WORD:
            retval = __put_user(spi->bits_per_word, (__u8 __user *)arg);
            break;
        case SPI_IOC_RD_MAX_SPEED_HZ:
            retval = __put_user(spi->max_speed_hz, (__u32 __user *)arg);
            break;

            /* write requests */
        case SPI_IOC_WR_MODE:
            retval = __get_user(tmp, (u8 __user *)arg);
            if (retval == 0) {
                u8	save = spi->mode;

                if (tmp & ~SPI_MODE_MASK) {
                    retval = -EINVAL;
                    break;
                }

                tmp |= spi->mode & ~SPI_MODE_MASK;
                spi->mode = (u8)tmp;
                retval = spi_setup(spi);
                if (retval < 0)
                    spi->mode = save;
                else
                    cs_debug("spi mode %02x\n", tmp);
            }
            break;
        case SPI_IOC_WR_LSB_FIRST:
            retval = __get_user(tmp, (__u8 __user *)arg);
            if (retval == 0) {
                u8	save = spi->mode;

                if (tmp)
                    spi->mode |= SPI_LSB_FIRST;
                else
                    spi->mode &= ~SPI_LSB_FIRST;
                retval = spi_setup(spi);
                if (retval < 0)
                    spi->mode = save;
                else
                    cs_debug("%csb first\n",
                            tmp ? 'l' : 'm');
            }
            break;
        case SPI_IOC_WR_BITS_PER_WORD:
            retval = __get_user(tmp, (__u8 __user *)arg);
            if (retval == 0) {
                u8	save = spi->bits_per_word;

                spi->bits_per_word = tmp;
                retval = spi_setup(spi);
                if (retval < 0)
                    spi->bits_per_word = save;
                else
                    cs_debug("%d bits per word\n", tmp);
            }
            break;
        case SPI_IOC_WR_MAX_SPEED_HZ:
            retval = __get_user(tmp, (__u32 __user *)arg);
            if (retval == 0) {
                u32	save = spi->max_speed_hz;

                spi->max_speed_hz = tmp;
                retval = spi_setup(spi);
                if (retval < 0)
                    spi->max_speed_hz = save;
                else
                    cs_debug("%d Hz (max)\n", tmp);
            }
            break;

        case SPI_IOC_SCAN_IMAGE:
            printk("[chipsailing] cmd====== scan image \n");

#ifdef CONFIG_COMPAT
        case 0xc0046b06:
#endif
            //mt_eint_mask(CS_EINT_NUMBER);

            disable_irq(fingerprint_irq);
            config_image_info();
            msleep(1);
            cs_read_image((u8 __user*)arg);


            //mt_eint_unmask(CS_EINT_NUMBER);
            enable_irq(fingerprint_irq);
            break;
        case SPI_IOC_SENSOR_RESET:
            printk("[chipsailing] cmd======reset \n");
            cs_fp_config();
            break;
        case SPI_IOC_KEYMODE_ON:
            g_Identify_finish = 1;
            printk("[chipsailing] cmd=SPI_IOC_KEYMODE_ON \n");
            break;
        case SPI_IOC_KEYMODE_OFF:
            g_Identify_finish = 0;
            printk("[chipsailing] cmd=SPI_IOC_KEYMODE_OFF \n");
            break;

        default:
            cs_debug("ChipSailing IOC into default \n");
            /* segmented and/or full-duplex I/O request */
            if (_IOC_NR(cmd) != _IOC_NR(SPI_IOC_MESSAGE(0)) || _IOC_DIR(cmd) != _IOC_WRITE) {

                cs_error("ChipSailing : IO_NR & IO_WRITE error ! \n");
                retval = -ENOTTY;
                break;
            }

            tmp = _IOC_SIZE(cmd);
            if ((tmp % sizeof(struct spi_ioc_transfer)) != 0) {
                cs_error("ChipSailing : tmp / sizeof(spi_ioc_transfer) error ! \n");
                retval = -EINVAL;
                break;
            }
            n_ioc = tmp / sizeof(struct spi_ioc_transfer);
            if (n_ioc == 0){
                cs_debug("ChipSailing : n_ioc == 0 ! \n");
                break;
            }
            /* copy into scratch area */
            ioc = kmalloc(tmp, GFP_KERNEL);
            if (!ioc) {
                cs_error("ChipSailing : kmalloc ioc error ! \n");
                retval = -ENOMEM;
                break;
            }
            if (__copy_from_user(ioc, (void __user *)arg, tmp)) {
                cs_error("ChipSailing : copy_from_user error ! \n");
                kfree(ioc);
                retval = -EFAULT;
                break;
            }

            /* translate to spi_message, execute */
            retval = cs_fp_message(ioc, n_ioc);
            kfree(ioc);
            break;
    }
    mutex_unlock(&spidev->buf_lock);
    spi_dev_put(spi);
    return ret;
}


// Select SPI work mode
static int mtspi_set_mode(int mode)
{
    struct mt_chip_conf* spi_par;
    //int ret;
    spi_par = &spi_conf;
    if (!spi_par)
    {
        return -1;
    }
    if (1 == mode)
    {
        spi_par->com_mod = DMA_TRANSFER;

    }
    else
    {
        spi_par->com_mod = FIFO_TRANSFER;

    }

    if(spi_setup(spi_dev->spi) < 0){
        cs_debug("ChipSailing : Failed to set mt_chip_confi ... \n");
    }
    return 0;
}

// SPI DMA mode
static int spi_send_cmd(struct cs_fp_data *spidev,u8 *tx,u8 *rx,u16 
        spilen)

{
    int ret=0;
    struct spi_message m;
    struct spi_transfer t = {
        .cs_change = 0,
        .delay_usecs = 0,
        //.speed_hz = CS_SPI_CLOCK_SPEED,
        .tx_buf = tx,
        .rx_buf = rx,
        .len = spilen,
        .tx_dma = 0,
        .rx_dma = 0,
        .bits_per_word = 8,
    };
    mtspi_set_mode(1);  
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);
    ret= spi_sync(spidev->spi,&m);
    return ret;
}

// SPI FIFO mode
static int spi_send_cmd_fifo(struct cs_fp_data *spidev,u8 *tx,u8 *rx,
        u16 spilen)
{
    int ret=0;
    struct spi_message m;
    struct spi_transfer t = {
        .cs_change = 0,
        .delay_usecs = 0,
        //.speed_hz = CS_SPI_CLOCK_SPEED,
        .tx_buf = tx,
        .rx_buf = rx,
        .len = spilen,
        .tx_dma = 0,
        .rx_dma = 0,
        .bits_per_word = 8,
    };
    mtspi_set_mode(0);
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);
    ret= spi_sync(spidev->spi,&m);
    return ret;
}

static void cs_write_sfr( u8 addr, u8 value )
{
    u8 send_buf[3]={0};

    send_buf[0] = 0xCC;
    send_buf[1] = addr;
    send_buf[2] = value;

    spi_send_cmd_fifo(spi_dev,send_buf,rx,3);
}

static u8 cs_read_sfr( u8 addr, u8 count )
{
    //	int ret;

    tx[0] = 0xDD;
    tx[1] = addr;

    spi_send_cmd_fifo(spi_dev,tx,rx,count+2);
    return rx[2];
}

static void cs_write_sram( u16 addr, u16 value )
{	
    //	u16 ret;

    tx[0] = 0xAA;
    tx[1] = (addr&0xFF00)>>8;	//high
    tx[2] = addr&0x00FF;		//low
    tx[3] = value&0x00FF;		//low
    tx[4] = (value&0xFF00)>>8;	//high

    spi_send_cmd_fifo(spi_dev,tx,rx,5);
}

static u16 cs_read_sram( u16 addr, u8 count )
{	
    //u16 ret;

    tx[0] = 0xBB;
    tx[1] = addr>>8;
    tx[2] = addr;

    spi_send_cmd_fifo(spi_dev,tx,rx,3+count+1);
    return (rx[5] & 0x00FF)<<8|rx[4];
}

/*static void cs_write_sfr_bit( u8 addr, u8 bit, bool flag )
  {
  u16 temp = 0x00;

  temp = cs_read_sfr(addr,1);

  if(flag)
  {
  temp = temp | (1<<bit);
  }
  else
  {
  temp = temp & (~(1<<bit));
  }
  cs_write_sfr(addr, temp);
  } */

static void cs_write_sram_bit( u16 addr, u16 bit, bool flag )
{
    u16 temp = 0x00;

    temp = cs_read_sram(addr,2);

    if(flag)
    {
        temp = temp | (1<<bit);
    }
    else
    {
        temp = temp & (~(1<<bit));
    }
    cs_write_sram(addr,temp);
}

//read IC image
static int cs_read_image(u8 *buffer){
    //int j,num=0;
    //read image data
    //u8 *image;
    int ret;
    u8 *rx_buffer;
    tx[0] = 0xBB;
    tx[1] = 0xFF;
    tx[2] = 0xFF;
    //image=kmalloc(IMG_LENGTH, GFP_KERNEL);
    rx_buffer=kmalloc(READ_LENGTH, GFP_KERNEL);


    spi_send_cmd(spi_dev,tx,rx_buffer,READ_LENGTH);
    //memcpy(image,rx_buffer+4,IMG_LENGTH);

    ret = copy_to_user((u8 __user *)buffer, rx_buffer+4, IMG_LENGTH);
    if(ret<0)
        cs_error("ChipSailing : read image , copy to user error \n");

    //	cs_debug("ChipSailing cs_read_image end\n");

    //kfree(image);
    kfree(rx_buffer);
    return 0;
}

//print some IC info
static u16 cs_read_info(){
    //unsigned char ret;
    u16 hwid,data;
    //	u8 temp;
    cs_debug("ChipSailing CS driver version : %s\n",Driver_Version);
    //read IC ID
    cs_read_sfr(0x3F,1);
    hwid = rx[2];
    hwid = hwid<<8;
    cs_read_sfr(0x3E,1);
    hwid = hwid | rx[2];
    cs_debug("ChipSailing read ID : 0x%x \n",hwid);

    //read ic mode
    cs_read_sfr(0x46, 1);
    cs_debug("ChipSailing read mode : %x \n",rx[2]);

    //read gain
    cs_read_sram(0xFC2E,2);
    data = rx[5];
    data = data<<8;
    data = data | rx[4];
    cs_debug("ChipSailing read gain : %x \n",data);
    return hwid;
}

static int cs_fp_reset(unsigned int delay_ms){
    cs_debug("[%s] start\n",__func__);
    pinctrl_select_state(hct_finger_pinctrl, hct_finger_reset_low);
    msleep(5);
    pinctrl_select_state(hct_finger_pinctrl, hct_finger_reset_high);
    if(delay_ms)
        msleep(delay_ms);
    return 0;
}


int cs_fp_config(void){	
    //reset IC
    //FUNC_ENTRY();
    cs_fp_reset(5);

    cs_write_sfr(0x0F, 0x01);
    cs_write_sfr(0x1C, 0x1D);
    cs_write_sfr(0x1F, 0x0A);
    cs_write_sfr(0x42, 0xB4);
    cs_write_sfr(0x60, 0x08);
    cs_write_sfr(0x63, 0x60);
    //cs_write_sfr(0x47, 0x60);//add 20160712
    //cs_write_sfr(0x13, 0x31);//add 20160712
    //cs_write_sram(0xFC1E, 0x0);//add 20160712

    /*****for 3.3V********/
    cs_write_sfr(0x22, 0x07);
    cs_write_sram(0xFC8C, 0x0001);
    cs_write_sram(0xFC90, 0x0001);
    /*********************/

    cs_write_sram(0xFC02, 0x0420);
    cs_write_sram(0xFC1A, 0x0C30);

    cs_write_sram(0xFC22, 0x085C);//cs_write_sram(0xFC22, 0x0848);
    cs_write_sram(0xFC2E, 0x00FB);//cs_write_sram(0xFC2E, 0x008F);	//cs_write_sram(0xFC2E, 0x00F6); 20160624
    cs_write_sram(0xFC06, 0x0039);
    cs_write_sram(0xFC08, 0x0008);//add  times    20160624
    cs_write_sram(0xFC0A, 0x0016);
    cs_write_sram(0xFC0C, 0x0022);

    cs_write_sram(0xFC12, 0x002A);
    cs_write_sram(0xFC14, 0x0035);
    cs_write_sram(0xFC16, 0x002B);
    cs_write_sram(0xFC18, 0x0039);
    cs_write_sram(0xFC28, 0x002E);
    cs_write_sram(0xFC2A, 0x0018);
    cs_write_sram(0xFC26, 0x282D);
    cs_write_sram(0xFC30, 0x0270);//cs_write_sram(0xFC30, 0x0260);  //cs_write_sram(0xFC30, 0x0300);	// 20160624
    cs_write_sram(0xFC82, 0x01FF);
    cs_write_sram(0xFC84, 0x0007);
    cs_write_sram(0xFC86, 0x0001);

    cs_write_sram_bit(0xFC80, 12,0);
    cs_write_sram_bit(0xFC88, 9, 0);
    cs_write_sram_bit(0xFC8A, 2, 0);
    cs_write_sram(0xFC8E, 0x3354);
    //FUNC_EXIT();
    printk("ChipSailing IC data load succeed !\n");
    return 0;
}


static int config_image_info(){		
    //open afe
    cs_write_sram(0xFC00, 0x0003);
    return 0;
}

//CS chip init
static int cs_init(void)
{	
    int status;
    status = cs_fp_config();
    if(status < 0){
        cs_error("ChipSailing %s : load cs chip data error !! ",__func__);
        return -1;
    }
    status = cs_read_info();
    if(status < 0){
        cs_error("ChipSailing %s : read cs chip info error !! ",__func__);
        return -1;
    }

    return 0;
}

/*static irqreturn_t cs_irq_handler(unsigned irq,struct irq_desc *desc)
  {
  cs_debug("[han_test][%s]:%d",__func__,__LINE__);
  IRQ_FLAG = 1;
  disable_irq_nosync(fingerprint_irq);
  wake_up_interruptible(&waiter);
  cs_debug("[han_test][%s]:%d",__func__,__LINE__);
  return 0;
  }*/
static irqreturn_t cs_irq_handler(int irq, void *dev_id)                                                     
{

    wake_up_interruptible(&waiter);
    IRQ_FLAG = 1;
    return 0;

}


u64  getCurrenMs(void)
{
    if((get_jiffies_64()*10)<0)
        return 0;
    else
        return (get_jiffies_64()*10);
}


void  cs_timer_func(unsigned long arg)
{

    printk("csTimer is ok\n");
    printk("receive data from timer: %ld\n",arg);
    num2=kickNum;
    printk("Num1=%lld Num2=%lld\n",num1,num2);

    tempNum2=irqNum;
    printk("ChipSailing cs_timer_func  tempNum2-tempNum1=%lld\n",(tempNum2-tempNum1));

    if((currentMs4-currentMs3)>=400)
    {
        input_report_key(cs15xx_inputdev,KEY_F10,1);
        input_sync(cs15xx_inputdev);
        input_report_key(cs15xx_inputdev,KEY_F10,0);
        input_sync(cs15xx_inputdev);

        input_report_key(cs15xx_inputdev,KEY_F5,1);
        input_sync(cs15xx_inputdev);
        input_report_key(cs15xx_inputdev,KEY_F5,0);
        input_sync(cs15xx_inputdev);
        printk("[ChipSailing] input_report_key  KEY_LONGTOUCH !\n");
    }


    //	}
else
{
    if(num2-num1==2)
    {
        input_report_key(cs15xx_inputdev,KEY_KICK2S,1);
        input_sync(cs15xx_inputdev);
        input_report_key(cs15xx_inputdev,KEY_KICK2S,0);
        input_sync(cs15xx_inputdev);
        printk("[ChipSailing] input_report_key KEY_KICK twice  !\n");
    }
    else if(num2-num1==1)
    {
        //input_report_key(cs15xx_inputdev,KEY_F10,1);
        // input_sync(cs15xx_inputdev);
        //input_report_key(cs15xx_inputdev,KEY_F10,0);
        // input_sync(cs15xx_inputdev);
        printk("[ChipSailing] input_report_key KEY_KICK once  !\n");
    }
    else
    {

        printk("[ChipSailing] not input_report_key \n");
    }
}
num1=kickNum;
csTimerOn=0;

}




/*
   static int 	_HOME_KEY= 0;
   static int 	_MENU_KEY= 1;
   static int 	_BACK_KEY= 2;
   static int 	_PWR_KEY= 3;
   static int 	_F11_KEY= 4;
   static int 	_F5_KEY = 5;
   static int 	_F6_KEY = 6;

 */
void cs_report_key(int key_type,int key_status)
{
    printk("[ChipSailing]  cs input_report_key key_type=%d \n",key_type);
    switch(key_type){
        case 0:
            input_report_key(cs15xx_inputdev,KEY_HOME,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing]  cs input_report_key KEY_HOME \n");
            break;

        case 1:
            input_report_key(cs15xx_inputdev,KEY_MENU,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing] cs input_report_key KEY_MENU \n");
            break;
        case 2:
            input_report_key(cs15xx_inputdev,KEY_BACK,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing] cs input_report_key KEY_BACK \n");
            break;
        case 3:
            input_report_key(cs15xx_inputdev,KEY_POWER,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing] cs input_report_key KEY_POWER \n");
            break;
        case 4:
            input_report_key(cs15xx_inputdev,KEY_F10,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing]cs input_report_key KEY_F10 \n");
            break;
        case 5:
            input_report_key(cs15xx_inputdev,KEY_F5,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing]cs input_report_key  KEY_F5\n");
            break;
        case 6:
            input_report_key(cs15xx_inputdev,KEY_F6,key_status);
            input_sync(cs15xx_inputdev);
            printk("[ChipSailing] cs input_report_key  KEY_F6\n");
            break;
        default: break;

    }
}



void csFingerDetecTimer_func(unsigned long arg)
{ 



    if(cs_work_func_finish==0){
        g_fingerup = 1;
        printk("[ChipSailing] getCurrenMs()-temp_currentMs = %lld !\n",(getCurrenMs()-temp_currentMs));
        if( (getCurrenMs()-temp_currentMs)>13)
        {
            printk("[ChipSailing] finger_status = %d !\n",finger_status);
            if( finger_status == 0)
            {



                currentMs4=getCurrenMs();
                printk(" ChipSailing currentMs4-currentMs3=%lld\n",(currentMs4-currentMs3));
                if(  (currentMs4-currentMs3)<400){
                    /*			  cs_report_key(_F11_KEY,1);
                                          cs_report_key(_F11_KEY,0);
                                          cs_report_key(_F5_KEY,1);
                                          cs_report_key(_F5_KEY,0);
                                          del_timer(&csTimer);
                     */		  	csTimerOn=0;

                }

                temp_currentMs=getCurrenMs();
                finger_status = 2;
            }

        }
    }
}

static int timer_init(void)
{
    printk("[timer]init timer START!\n");
    init_timer(&csTimer);

    csTimer.expires = jiffies+1;
    csTimer.data = 60;
    csTimer.function = cs_timer_func;
    printk("[timer]add timer success!\n");



    init_timer(&csFingerDetecTimer);
    printk("[timer]init csFingerDetecTimer finish!\n");
    csFingerDetecTimer.expires = jiffies+1;
    csFingerDetecTimer.data = 60;
    csFingerDetecTimer.function = csFingerDetecTimer_func;


    printk("[timer]add timer success!\n");
    return 0;
}

static void  timer_exit (void)

{
    del_timer(&csTimer);
    del_timer(&csFingerDetecTimer);
    printk("Hello module exit\n");
}   




static int cs_work_func(void *data)
{

    u8 mode = 0x00;
    u8 status = 0x00;
    u8 mode2 = 0x00;
    u8 status2 = 0x00;

    u8 reg48_val[2] = {0};
    u64 currentMs1,currentMs2;
    irqNum=0;

    csTimerOn=0;

    do{


        currentMs1=getCurrenMs();

        wait_event_interruptible(waiter, IRQ_FLAG != 0);
        cs_work_func_finish=1;  //cs_work_func
        currentMs2=getCurrenMs();


        disable_irq(fingerprint_irq);
        //	printk("[ChipSailing]Disable_irq now!\n");

        mode   = cs_read_sfr(0x46, 1);
        status = cs_read_sfr(0x50, 1);
        reg48_val[0]=cs_read_sfr(0x48,1);
        //  printk("fngr_present_posl 0x48 value=%x  \n",reg48_val[0]);

        /*	if(csTimerOn==0)
                {
                add_timer(&csTimer);
                mod_timer(&csTimer, jiffies + 180);
                csTimerOn=1;

                tempNum1=irqNum;

                }
         */



        if(status != 0x01)
        {
            if( (currentMs2-currentMs1)>100 )   //200ms   1 //20
            {

                ++kickNum;
                printk("[ChipSailing] kickNum =%lld  !\n",kickNum);

            }
            printk("[ChipSailing]Got an irq !\n");
            ++irqNum;
        }



        {
            del_timer(&csFingerDetecTimer);
            currentMs5=getCurrenMs();
            //printk("ChipSailing del csFingerDetecTimer time=%lld \n",(currentMs5-));

            add_timer(&csFingerDetecTimer);
            mod_timer(&csFingerDetecTimer, jiffies + 30);  //   1~10ms
            if( (status != 0x01) && (reg48_val[0]!=0)  )//if( (mode == 0x72) && (status != 0x01) && (reg48_val[0]!=0)  )
            {
                irq_current_num=irqNum;
                if( ((currentMs2 - currentMs1) >13) && (finger_status==2)  )
                {

                    currentMs3=getCurrenMs();

                    finger_status=1;
                }
                else
                {

                    finger_status=0;
                }
            }
        }

        if(mode == 0x71 && status == 0x04)
        {
            cs_write_sfr(0x46, 0x70);
            cs_write_sfr(0x50, 0x01);
            //printk(" normal 0x48 value=%x   BitCount=%d  \n",reg48_val[0],BitCount(reg48_val[0]));


            //	     printk("[ChipSailing]BitCount =%d \n",BitCount(reg48_val[0]));
            if( BitCount(reg48_val[0])==1 )
            {
                kill_fasync(&async, SIGIO, POLL_IN);

            }

            else
            {
                //force to normal
                forceMode(3);
            }

        }

        else if(mode == 0x72 && status == 0x08)
        {
            cs_write_sfr(0x46, 0x70);
            cs_write_sfr(0x50, 0x01);
            kill_fasync(&async, SIGIO, POLL_IN);
            printk("[ChipSailing]ChipSailing sleep kill_fasync ..... \n");
            if(g_fingerup ){
                if(g_Identify_finish){
                    input_report_key(cs15xx_inputdev,KEY_F10,1);
                    input_sync(cs15xx_inputdev);
                    input_report_key(cs15xx_inputdev,KEY_F10,0);
                    input_sync(cs15xx_inputdev);
                }
                g_fingerup = 0;
            }
        }
        else
        {
            cs_write_sfr(0x50, 0x01);
            printk("[ChipSailing]ChipSailing idle kill_fasync ..... \n");
        }


        mode2   = cs_read_sfr(0x46, 1);
        status2 = cs_read_sfr(0x50, 1);

        //	printk("[ChipSailing]cs_work_func before mode is 0x%x , status is 0x%x  \n",mode,status);
        printk("[ChipSailing]cs_work_func after  mode is 0x%x , status is 0x%x  \n\n\n",mode2,status2);
        //cs15xx_irq_active = 0;
        IRQ_FLAG = 0;
        cs_work_func_finish=0;
        enable_irq(fingerprint_irq);
        printk("[ChipSailing]enable_irq now!\n");

    }while(!kthread_should_stop());
    return 0;
}


static int cs106x_fasync(int fd, struct file *filp, int mode)
{
    int ret;
    ret = fasync_helper(fd, filp, mode, &async);
    cs_error("ChipSailing : fasync_helper .... \n");
    return ret;
}


static int cs_gpio_init(struct cs_fp_data * spi_dev ){
    int ret;

    if(spi_dev == NULL)
    {
        cs_error(" pdev = NULL !\n");
        return -1;
    }

    /*spi_dev->spi->dev.of_node=of_find_matching_node(spi_dev->spi->dev.of_node, cs_fp_match_table);
      if(!(spi_dev->spi->dev.of_node)){
      cs_error("of_find_compatible_node error\n");
      return -1;
      }
      cs_debug("of_find_compatible_node ok\n");
     */
    hct_finger_pinctrl = devm_pinctrl_get(&spi_dev->spi->dev);
    if (IS_ERR(hct_finger_pinctrl)) {
        ret = PTR_ERR(hct_finger_pinctrl);
        dev_err(&spi_dev->spi->dev, "hct_finger cannot find pinctrl\n");
        return ret;
    }

    //printk("[%s] hct_finger_pinctrl+++++++++++++++++\n",pdev->name);

    hct_finger_power_on = pinctrl_lookup_state(hct_finger_pinctrl, "finger_power_en1");
    if (IS_ERR(hct_finger_power_on)) {
        ret = PTR_ERR(hct_finger_power_on);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_power_on!\n");
        return ret;
    }
    hct_finger_power_off = pinctrl_lookup_state(hct_finger_pinctrl, "finger_power_en0");
    if (IS_ERR(hct_finger_power_off)) {
        ret = PTR_ERR(hct_finger_power_off);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_power_off!\n");
        return ret;
    }
    hct_finger_reset_high = pinctrl_lookup_state(hct_finger_pinctrl, "finger_reset_en1");
    if (IS_ERR(hct_finger_reset_high)) {
        ret = PTR_ERR(hct_finger_reset_high);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_reset_high!\n");
        return ret;
    }
    hct_finger_reset_low = pinctrl_lookup_state(hct_finger_pinctrl, "finger_reset_en0");
    if (IS_ERR(hct_finger_reset_low)) {
        ret = PTR_ERR(hct_finger_reset_low);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_reset_low!\n");
        return ret;
    }
    hct_finger_spi0_mi_as_spi0_mi = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_mi_as_spi0_mi");
    if (IS_ERR(hct_finger_spi0_mi_as_spi0_mi)) {
        ret = PTR_ERR(hct_finger_spi0_mi_as_spi0_mi);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_mi_as_spi0_mi!\n");
        return ret;
    }
    hct_finger_spi0_mi_as_gpio = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_mi_as_gpio");
    if (IS_ERR(hct_finger_spi0_mi_as_gpio)) {
        ret = PTR_ERR(hct_finger_spi0_mi_as_gpio);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_mi_as_gpio!\n");
        return ret;
    }
    hct_finger_spi0_mo_as_spi0_mo = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_mo_as_spi0_mo");
    if (IS_ERR(hct_finger_spi0_mo_as_spi0_mo)) {
        ret = PTR_ERR(hct_finger_spi0_mo_as_spi0_mo);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_mo_as_spi0_mo!\n");
        return ret;
    }
    hct_finger_spi0_mo_as_gpio = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_mo_as_gpio");
    if (IS_ERR(hct_finger_spi0_mo_as_gpio)) {
        ret = PTR_ERR(hct_finger_spi0_mo_as_gpio);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_mo_as_gpio!\n");
        return ret;
    }
    hct_finger_spi0_clk_as_spi0_clk = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_clk_as_spi0_clk");
    if (IS_ERR(hct_finger_spi0_clk_as_spi0_clk)) {
        ret = PTR_ERR(hct_finger_spi0_clk_as_spi0_clk);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_clk_as_spi0_clk!\n");
        return ret;
    }
    hct_finger_spi0_clk_as_gpio = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_clk_as_gpio");
    if (IS_ERR(hct_finger_spi0_clk_as_gpio)) {
        ret = PTR_ERR(hct_finger_spi0_clk_as_gpio);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_clk_as_gpio!\n");
        return ret;
    }
    hct_finger_spi0_cs_as_spi0_cs = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_cs_as_spi0_cs");
    if (IS_ERR(hct_finger_spi0_cs_as_spi0_cs)) {
        ret = PTR_ERR(hct_finger_spi0_cs_as_spi0_cs);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_cs_as_spi0_cs!\n");
        return ret;
    }
    hct_finger_spi0_cs_as_gpio = pinctrl_lookup_state(hct_finger_pinctrl, "finger_spi0_cs_as_gpio");
    if (IS_ERR(hct_finger_spi0_cs_as_gpio)) {
        ret = PTR_ERR(hct_finger_spi0_cs_as_gpio);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_cs_as_gpio!\n");
        return ret;
    }
    hct_finger_int_as_int = pinctrl_lookup_state(hct_finger_pinctrl, "finger_int_as_int");
    if (IS_ERR(hct_finger_int_as_int)) {
        ret = PTR_ERR(hct_finger_int_as_int);
        dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_spi0_cs_as_gpio!\n");
        return ret;
    }
    /*
       hct_finger_power18_on = pinctrl_lookup_state(hct_finger_pinctrl, "finger_power18_en1");
       if (IS_ERR(hct_finger_power_on)) {
       ret = PTR_ERR(hct_finger_power_on);
       dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_power_on!\n");
       return ret;
       }
       hct_finger_power18_off = pinctrl_lookup_state(hct_finger_pinctrl, "finger_power18_en0");
       if (IS_ERR(hct_finger_power_off)) {
       ret = PTR_ERR(hct_finger_power_off);
       dev_err(&spi_dev->spi->dev, " Cannot find hct_finger pinctrl hct_finger_power_off!\n");
       return ret;
       }
     */

    printk("nasri...%s %d\n",__func__,__LINE__);
    return 0;

}
static int cs_power_on(bool onoff){

    int ret = -1;
    cs_debug("[%s] start\n",__func__);
    if(onoff){
        /*power on*/
        ret = pinctrl_select_state(hct_finger_pinctrl, hct_finger_power_on);
        //ret = pinctrl_select_state(hct_finger_pinctrl, hct_finger_power18_on);
        if(ret){
            cs_error("[%s]pinctrl_select_state power_output1 error",__func__);
        }
    }else{
        //hwPowerDown
        ret = pinctrl_select_state(hct_finger_pinctrl, hct_finger_power_off);
        //  ret = pinctrl_select_state(hct_finger_pinctrl, hct_finger_power18_off);
        if(ret){
            cs_error("[%s]pinctrl_select_state power_output0 error",__func__);
        }
    }

    return 0;
}
int finger_set_spi_mode(int cmd)
{
    switch (cmd)
    {
        case 0 :
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_clk_as_gpio);
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_cs_as_gpio);
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_mi_as_gpio);
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_mo_as_gpio);
            break;
        case 1 :
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_clk_as_spi0_clk);
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_cs_as_spi0_cs);
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_mi_as_spi0_mi);
            pinctrl_select_state(hct_finger_pinctrl, hct_finger_spi0_mo_as_spi0_mo);
            break;
    }
    return 0;
}

static int cs_fp_probe(struct spi_device *spi)
{
    int status;
    struct cs_fp_data *spidev = NULL;
    u32 ints[2]={0};
    cs_debug("ChipSailing :  %s\n",__func__);

    // Allocate driver data
    spidev = kzalloc(sizeof(*spidev), GFP_KERNEL);
    if (!spidev)
        return -ENOMEM;

    //init head
    INIT_LIST_HEAD(&spidev->device_entry);
    spin_lock_init(&spidev->spi_lock);
    //init lock
    mutex_init(&spidev->buf_lock);
    mutex_lock(&device_list_lock);
    //init device file
    cdev_init(&(spidev->cdev),&cs_fp_fops);
    spidev->cdev.owner = THIS_MODULE;
    alloc_chrdev_region(&(spidev->cdev.dev),10,1,"cs_spi");
    spidev->devno = spidev->cdev.dev ;
    cdev_add(&(spidev->cdev),spidev->devno,1);
    spidev->class = class_create(THIS_MODULE,"cs_spi");
    spidev->device = device_create(spidev->class, NULL, spidev->devno,NULL, "cs_spi");
    list_add(&spidev->device_entry, &device_list);

    status = IS_ERR(spidev->device)? PTR_ERR(spidev->device):0;
    if (status !=0){
        cs_error("ChipSailing : init device file error !\n");
        return status;
    }
    spi_dev = spidev;
    spi_set_drvdata(spi,spidev);

    // Initialize the driver data 
    spidev->spi = spi;
    spidev->spi->bits_per_word = 8;
    spidev->spi->mode = SPI_MODE_0;
    spi_setup(spidev->spi);		

    // if(cs_read_info==0xa062)   //read ID
    // {

    memset(tx, 0, DATA_LENGTH);
    memset(rx, 0, DATA_LENGTH);
    //memset(image, 0, IMG_LENGTH);

    spidev->spi->dev.of_node=of_find_compatible_node(NULL, NULL, "mediatek,hct_finger");
    cs_debug("[%s] of_node->name is %s\n",__func__,spidev->spi->dev.of_node->name);

    //gpio
    status=cs_gpio_init(spidev);
    if(status<0){
        cs_error("[%s]:cs_gpio_init error\n",__func__);
        return -1;
    }
    finger_set_spi_mode(1);
    cs_power_on(1);
    cs_fp_reset(20);

    //for IC INPUT device Initialize
    cs15xx_create_inputdev();

    //for IC Initialize
    cs_init();

    //irq

    //cs_finger_irq_node = irq_of_parse_and_map(cs_finger_irq_node, cs_fp_match_table);   //it works

    if(spidev->spi->dev.of_node)
    {
        //of_property_read_u32_array(cs_finger_irq_node,"debounce",ints,ARRAY_SIZE(ints));
        //cs_debug(DEFAULT_DEBUG,"ints[0]=%d,ints[1]=%d\n",ints[0],ints[1]);
        gpio_set_debounce(fingerprint_irq, 0);
        fingerprint_irq = irq_of_parse_and_map(spidev->spi->dev.of_node,0);
        //cs15xx->irq = irq_of_parse_and_map(cs_finger_irq_node,0);
        //cs_debug(DEFAULT_DEBUG,"cs15xx->irq num=%d\n",cs15xx->irq);

        if(request_irq(fingerprint_irq,cs_irq_handler,IRQF_TRIGGER_RISING,"Cs_Finger",NULL)){ 	 //"
            // cs_debug(DEFAULT_DEBUG,"cs finger sensor ---IRQ LINE NOT AVAILABLE!!\n");
            //goto err_free_input;
        }

        enable_irq(fingerprint_irq);


        mutex_unlock(&device_list_lock);
        cs_irq_thread = kthread_run(cs_work_func,NULL,"cs_thread");
        if(IS_ERR(cs_irq_thread)) {
            printk("ChipSailing Failed to create kernel thread !\n");
        }
        timer_init();
        KEY_MODE_FLAG=1;


    }
    return status;
}

/*-------------------------------------------------------------------------*/

static int __init cs_fp_init(void)
{
    int status;
    cs_debug("ChipSailing :  %s\n",__func__);

    status = spi_register_board_info(spi_board_chipsailing,ARRAY_SIZE(spi_board_chipsailing));

    status = spi_register_driver(&cs_fp_spi_driver);
    if (status < 0) {
        //class_destroy(dev.class);
        //unregister_chrdev(SPIDEV_MAJOR, spidev_spi_driver.driver.name);
        cs_error("ChipSailing : spi_register_driver error ! %d \n",status);
        return status;
    }
    cs_debug("ChipSailing : spi_register_driver success ! %d \n",status);
    return status;
}

static void __exit cs_fp_exit(void)
{	
    timer_exit();
    cs_debug("%s\n", __func__);
}

module_init(cs_fp_init);
module_exit(cs_fp_exit);

MODULE_AUTHOR("Andrea Paterniani, <a.paterniani@swapp-eng.it>");
MODULE_DESCRIPTION("User mode SPI device interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:spidev");
