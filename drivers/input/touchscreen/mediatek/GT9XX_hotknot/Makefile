# Linux driver folder
# Vanzo:yucheng on: Sun, 24 Jan 2016 18:13:26 +0800
# Modified for vanzo customization
include $(srctree)/drivers/misc/mediatek/Makefile.custom
# End of Vanzo: yucheng

ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/GT9XX_hotknot/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/

obj-y	+=  goodix_tool.o
obj-y	+=  gt9xx_driver.o
obj-y	+=  gt9xx_update.o

