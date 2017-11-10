# Vanzo:yucheng on: Mon, 25 Jan 2016 11:32:34 +0800
# Disable warnning to error
#subdir-ccflags-y += -Werror
include $(srctree)/drivers/misc/mediatek/Makefile.custom
# End of Vanzo: yucheng
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek

# Vanzo:wangfei on: Wed, 12 Nov 2014 21:08:36 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)
endif
# End of Vanzo:wangfei

# In case the platform does NOT support this type of sensors
obj-y   +=  mtk_tpd.o
obj-y   +=  tpd_button.o
obj-y   +=  tpd_calibrate.o
obj-y   +=  tpd_debug.o
obj-y   +=  tpd_default.o
obj-y   +=  tpd_init.o
obj-y   +=  tpd_misc.o
obj-y   +=  tpd_setting.o
# Vanzo:wangfei on: Wed, 12 Nov 2014 21:06:42 +0800
# for compatible the old style ,so use CUSTOM_KERNEL_IMGSENSOR
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(project_name),)

ifneq ($(strip $(BOOT_LOGO)),)
boot_logo := $(shell echo $(BOOT_LOGO) | tr '[a-z]' '[A-Z]')
$(info boot_logo = $(boot_logo))
subdir-ccflags-y += -D$(boot_logo)
endif

custom_kernel_touchpanel:= $(addsuffix /, $(shell echo $(CUSTOM_KERNEL_TOUCHPANEL)))

$(info "custom_kernel_touchpanel:$(custom_kernel_touchpanel)")
obj-y += $(custom_kernel_touchpanel)
else
# End of Vanzo:wangfei

obj-$(CONFIG_TOUCHSCREEN_MTK_GT1151)	+=  GT1151/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT1XX)	+=  GT1XX/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT910)	+=  GT910/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT9XXTB_HOTKNOT)	+=  GT9XXTB_hotknot/
obj-$(CONFIG_TOUCHSCREEN_MTK_SYNAPTICS_I2C_RMI4)	+=  synaptics_i2c_rmi4/
obj-$(CONFIG_TOUCHSCREEN_MTK_SYNAPTICS_DSX)	+=  synaptics_dsx/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT928)	+=  GT928/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT9XX_HOTKNOT_SCP)	+=  GT9XX_hotknot_scp/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT911)	+=  GT911/
obj-$(CONFIG_TOUCHSCREEN_MTK_FT5X0X)	+=  ft5x0x/
obj-$(CONFIG_TOUCHSCREEN_MTK_FOCALTECH)	+=  focaltech/
# Vanzo:wangfei on: Sat, 28 Feb 2015 16:22:43 +0800
# for use new style to config touchscreen
endif
# End of Vanzo:wangfei
