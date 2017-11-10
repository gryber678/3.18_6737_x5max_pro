#include $(srctree)/drivers/misc/mediatek/Makefile.custom

ccflags-y += -I$(srctree)/drivers/misc/mediatek/leds/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/video/include

obj-$(CONFIG_MTK_LEDS) += leds_drv.o

obj-y += $(subst ",,$(CONFIG_MTK_PLATFORM))/

obj-$(CONFIG_LED_BREATHLIGHT_AW2013) += aw2013/
obj-$(CONFIG_LED_BREATHLIGHT_AW2028) += aw2028/
