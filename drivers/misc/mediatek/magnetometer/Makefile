# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/accelerometer/inc
ccflags-y += -I$(srctree)/drivers/misc/mediatek/magnetometer/inc
# Vanzo:wangfei on: Wed, 12 Nov 2014 21:08:36 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)
endif
# End of Vanzo:wangfei

obj-y   +=  mag.o
obj-y   +=  mag_factory.o

# Vanzo:wangfei on: Wed, 12 Nov 2014 21:06:42 +0800
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
 
ifneq ($(project_name),)
custom_kernel_magnetometer:= $(addsuffix /, $(shell echo $(CUSTOM_KERNEL_MAGNETOMETER)))
$(info "custom_kernel_magnetometer:$(custom_kernel_magnetometer)")
ifneq ($(custom_kernel_magnetometer),)
obj-y += $(custom_kernel_magnetometer)
endif
else
# End of Vanzo:wangfei
ifeq ($(CONFIG_MTK_AKM8963_NEW),y)
obj-y	+=  akm8963-new/
endif

ifeq ($(CONFIG_MTK_BMM050_NEW),y)
obj-y	+=  bmm050-new/
endif

ifeq ($(CONFIG_MTK_BMM056_NEW),y)
obj-y	+=  bmm056-new/
endif
ifeq ($(CONFIG_MTK_BMM056_NEW),y)
obj-y	+=  bmm056-new/
endif
ifeq ($(CONFIG_MTK_AKM8963),y)
obj-y	+=  akm8963/
endif

ifeq ($(CONFIG_MTK_YAS532),y)
obj-y   +=  yamaha532/
endif

ifeq ($(CONFIG_MTK_YAS532_NEW),y)
obj-y   +=  yamaha532-new/
endif

ifeq ($(CONFIG_MTK_AKM09911),y)
obj-y   +=  akm09911/
endif

ifeq ($(CONFIG_MTK_AKM09911_NEW),y)
obj-y   +=  akm09911-new/
endif

ifeq ($(CONFIG_MTK_S62X),y)
obj-y   +=  s62x/
endif

ifeq ($(CONFIG_MTK_IST8303), y)
obj-y += ist8303-new/
endif

ifeq ($(CONFIG_MTK_ST480),y)
obj-y   +=  st480/
endif
obj-$(CONFIG_MTK_QMC6983_NEW)   +=  qmc6983-new/

# Vanzo:wangfei on: Tue, 06 Jan 2015 10:34:15 +0800
# added for new structure config alsps
endif
# End of Vanzo:wangfei
