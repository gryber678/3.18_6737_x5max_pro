# Vanzo:yucheng on: Sun, 24 Jan 2016 18:13:26 +0800
# Modified for vanzo customization
include $(srctree)/drivers/misc/mediatek/Makefile.custom
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(project_name),)
ifeq ($(strip $(VANZO_MTK_PLATFORM)),MT6753)
    ccflags-y += -DVANZO_MTK_PLATFORM_MT6753
endif
endif
# End of Vanzo: yucheng


obj-$(CONFIG_MTK_GPIO)	+= mt_gpio_core.o
obj-$(CONFIG_MTK_GPIO)  += $(subst ",,$(CONFIG_MTK_PLATFORM))/
