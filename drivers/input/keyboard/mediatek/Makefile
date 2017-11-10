include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Vanzo:yangzhihong on: Mon, 25 Jan 2016 19:45:04 +0800
#subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(CONFIG_MTK_PLATFORM)/include/
# End of Vanzo:yangzhihong

subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(CONFIG_MTK_PLATFORM)/include/
subdir-ccflags-y += -I$(srctree)/drivers/input/keyboard/mediatek/$(CONFIG_MTK_PLATFORM)/
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/
obj-$(CONFIG_KEYBOARD_MTK) := kpd.o

obj-y += $(subst ",,$(CONFIG_MTK_PLATFORM))/

