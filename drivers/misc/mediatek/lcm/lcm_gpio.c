#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <asm-generic/gpio.h>
#include <mt-plat/mt_gpio.h>
#include <mt-plat/upmu_common.h>
#include <mach/upmu_sw.h>
#include <mach/upmu_hw.h>

#include "lcm_define.h"
#include "lcm_drv.h"
#include "lcm_gpio.h"

#endif

#ifndef BUILD_LK
#define LCM_GPIO_DEVICE "lcm_mode"
static struct pinctrl *_lcm_gpio;
static struct pinctrl_state *_lcm_gpio_mode_default;
static struct pinctrl_state *_lcm_enp_bias_l=NULL;
static struct pinctrl_state *_lcm_enp_bias_h=NULL;


/* function definitions */
static int __init _lcm_gpio_init(void);
static void __exit _lcm_gpio_exit(void);
static int _lcm_gpio_probe(struct device *dev);
static int _lcm_gpio_remove(struct device *dev);
static const struct of_device_id _lcm_gpio_of_ids[] = {
  {.compatible = "mediatek,lcm_mode",},
  {}
};

static struct platform_driver _lcm_gpio_driver = {
  .driver = {
    .name = LCM_GPIO_DEVICE,
    .probe = _lcm_gpio_probe,
    .remove = _lcm_gpio_remove,
    .of_match_table = _lcm_gpio_of_ids,
  },
};
/* LCM GPIO probe */
static int _lcm_gpio_probe(struct device *dev)
{
  int ret;
  const struct of_device_id *match;

  pr_debug("[LCM][GPIO] enter %s, %d\n", __func__, __LINE__);

  _lcm_gpio = devm_pinctrl_get(dev);
  if (IS_ERR(_lcm_gpio)) {
    ret = PTR_ERR(_lcm_gpio);
    dev_err(dev, "[LCM][ERROR] Cannot find _lcm_gpio!\n");
    return ret;
  }
  _lcm_gpio_mode_default = pinctrl_lookup_state(_lcm_gpio, "default");
  if (IS_ERR(_lcm_gpio_mode_default)) {
    ret = PTR_ERR(_lcm_gpio_mode_default);
    dev_err(dev, "[LCM][ERROR] Cannot find lcm_mode_default %d!\n", ret);
  }
  _lcm_enp_bias_l = pinctrl_lookup_state(_lcm_gpio, "lcm_enp_bias_l");
  if (IS_ERR(_lcm_enp_bias_l)) {
    ret = PTR_ERR(_lcm_enp_bias_l);
    dev_err(dev, "[LCM][ERROR] Cannot find lcm_enp_bias_l %d!\n", ret);
  }
  _lcm_enp_bias_h = pinctrl_lookup_state(_lcm_gpio, "lcm_enp_bias_h");
  if (IS_ERR(_lcm_enp_bias_h)) {
    ret = PTR_ERR(_lcm_enp_bias_h);
    dev_err(dev, "[LCM][ERROR] Cannot find lcm_enp_bias_h %d!\n", ret);
  }

  if (dev->of_node) {
    match = of_match_device(of_match_ptr(_lcm_gpio_of_ids), dev);
    if (!match) {
      pr_err("[LCM][ERROR] No device match found\n");
      return -ENODEV;
    }
  }
  pr_debug("[LCM][GPIO] _lcm_gpio_get_info end!\n");

  return 0;
}

static int _lcm_gpio_remove(struct device *dev)
{
  return 0;
}

/* called when loaded into kernel */
static int __init _lcm_gpio_init(void)
{
  pr_debug("MediaTek LCM GPIO driver init\n");
  if (platform_driver_register(&_lcm_gpio_driver) != 0) {
    pr_err("unable to register LCM GPIO driver.\n");
    return -1;
  }
  return 0;
}
/* should never be called */
static void __exit _lcm_gpio_exit(void)
{
  pr_debug("MediaTek LCM GPIO driver exit\n");
  platform_driver_unregister(&_lcm_gpio_driver);
}

void lcm_set_enp_bias(bool Val)
{
  if (Val == 0 && !IS_ERR(_lcm_enp_bias_l))
  {
    printk("lcm set enp low\n");
    pinctrl_select_state(_lcm_gpio, _lcm_enp_bias_l);
  }
  else if (Val == 1 && !IS_ERR(_lcm_enp_bias_h))
  {
    printk("lcm set enp high\n");
    pinctrl_select_state(_lcm_gpio, _lcm_enp_bias_h);
  }
}

module_init(_lcm_gpio_init);
module_exit(_lcm_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MediaTek LCM GPIO driver");
MODULE_AUTHOR("Joey Pan<joey.pan@mediatek.com>");
#endif
