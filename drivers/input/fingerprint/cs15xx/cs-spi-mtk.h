#define MTK
//#define THREAD
//#define INPUT_WORK_QUEUE	//input key && workqueue for test

#ifdef MTK
#include "../../../spi/mediatek/mt6735/mt_spi.h"
#include <mt_gpio.h>
#include <linux/pinctrl/pinctrl.h>
//#include "cust_eint.h"
//#include <mach/eint.h>
//#include "cust_gpio_usage.h"
//#include <mach/irqs.h>
#include <linux/gpio.h>
#endif

#define SPI_CPHA		0x01
#define SPI_CPOL		0x02
#define SPI_MODE_0		(0|0)
#define SPI_MODE_1		(0|SPI_CPHA)
#define SPI_MODE_2		(SPI_CPOL|0)
#define SPI_MODE_3		(SPI_CPOL|SPI_CPHA)

#define SPI_CS_HIGH		0x04
#define SPI_LSB_FIRST		0x08
#define SPI_3WIRE		0x10
#define SPI_LOOP		0x20
#define SPI_NO_CS		0x40
#define SPI_READY		0x80
#define CS_DEBUG		1



/*---------------------------------------------------------------------------*/
#define KEY_KICK1S       KEY_F11//KEY_VOLUMEDOWN
#define KEY_KICK2S 			 KEY_F6//KEY_VOLUMEUP
#define KEY_LONGTOUCH  	 KEY_F5

#define NAV_UP 	         KEY_F7  
#define NAV_DOWN 	       KEY_F8		 
#define NAV_LEFT 	       KEY_F9	 
#define NAV_RIGHT  	     KEY_F10		 
  


#define 	_HOME_KEY  0
#define 	_MENU_KEY 1
#define 	_BACK_KEY 2
#define 	_PWR_KEY  3
#define 	_F11_KEY  4 
#define 	_F5_KEY   5 
#define 	_F6_KEY   6 

#define SPI_MODE_MASK		(SPI_CPHA | SPI_CPOL | SPI_CS_HIGH \
    | SPI_LSB_FIRST | SPI_3WIRE | SPI_LOOP \
    | SPI_NO_CS | SPI_READY)
#define SPI_IOC_MAGIC			'k'
#define SPI_MSGSIZE(N) \
  ((((N)*(sizeof (struct spi_ioc_transfer))) < (1 << _IOC_SIZEBITS)) \
   ? ((N)*(sizeof (struct spi_ioc_transfer))) : 0)
#define SPI_IOC_MESSAGE(N) _IOW(SPI_IOC_MAGIC, 0, char[SPI_MSGSIZE(N)])
/* Read / Write of SPI mode (SPI_MODE_0..SPI_MODE_3) */
#define SPI_IOC_RD_MODE			_IOR(SPI_IOC_MAGIC, 1, __u8)
#define SPI_IOC_WR_MODE			_IOW(SPI_IOC_MAGIC, 1, __u8)

/* Read / Write SPI bit justification */
#define SPI_IOC_RD_LSB_FIRST		_IOR(SPI_IOC_MAGIC, 2, __u8)
#define SPI_IOC_WR_LSB_FIRST		_IOW(SPI_IOC_MAGIC, 2, __u8)

/* Read / Write SPI device word length (1..N) */
#define SPI_IOC_RD_BITS_PER_WORD	_IOR(SPI_IOC_MAGIC, 3, __u8)
#define SPI_IOC_WR_BITS_PER_WORD	_IOW(SPI_IOC_MAGIC, 3, __u8)

/* Read / Write SPI device default max speed hz */
#define SPI_IOC_RD_MAX_SPEED_HZ		_IOR(SPI_IOC_MAGIC, 4, __u32)
#define SPI_IOC_WR_MAX_SPEED_HZ		_IOW(SPI_IOC_MAGIC, 4, __u32)


#define SPI_IOC_RD_SENSOR_ID		_IOWR(SPI_IOC_MAGIC, 5, __u8*)
#define SPI_IOC_SCAN_IMAGE		_IOWR(SPI_IOC_MAGIC, 6, __u8*)
#define SPI_IOC_RD_INT_FLAG		_IOR(SPI_IOC_MAGIC, 7, __u8)
#define SPI_IOC_SENSOR_RESET		_IO(SPI_IOC_MAGIC, 8) 

#define SPI_IOC_KEYMODE_ON     	_IO(SPI_IOC_MAGIC, 12)
#define SPI_IOC_KEYMODE_OFF     _IO(SPI_IOC_MAGIC, 13)
struct spi_ioc_transfer {
  __u64		tx_buf;
  __u64		rx_buf;

  __u32		len;
  __u32		speed_hz;

  __u16		delay_usecs;
  __u8		bits_per_word;
  __u8		cs_change;
  __u32		pad;

};

#if CS_DEBUG
#define cs_debug( fmt, args...) do{ \
	printk("CS15XX "fmt"\n", ##args); \
}while(0)
#else
#define cs_debug(fmt,args...) 
#endif
#define cs_error(fmt,arg...)          printk("<<CS2511_error>> "fmt"\n",##arg) 



static u16 cs_read_info(void);
static int cs_read_image(u8 *buffer);
static int config_image_info(void);
static int cs_fp_probe(struct spi_device *spi);
static int cs_fp_remove(struct spi_device *spi);
static ssize_t cs_fp_read(struct file *filp, char __user *buf, size_t 
    count, loff_t *f_pos);

static ssize_t cs_fp_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos);

static long cs_fp_ioctl(struct file *filp, unsigned int cmd, unsigned 
    long arg);

static int cs_fp_open(struct inode *inode, struct file *filp);
static int cs_fp_release(struct inode *inode, struct file *filp);
int cs_fp_config(void);
static void cs_write_sram( u16 addr, u16 value );
static void cs_write_sfr( u8 addr, u8 value );
static u8 cs_read_sfr( u8 addr, u8 count );
//static void cs_write_sfr_bit( u8 addr, u8 bit, bool flag );
static u16 cs_read_sram( u16 addr, u8 count );
static void cs_write_sram_bit( u16 addr, u16 bit, bool flag );

static int cs106x_fasync(int fd, struct file *filp, int mode);
