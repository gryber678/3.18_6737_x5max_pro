# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include
# Vanzo:wangfei on: Wed, 12 Nov 2014 21:08:36 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)
endif
# End of Vanzo:wangfei
obj-y   +=  lsm6ds3gy/
obj-y   +=  gyroscope.o gyro_factory.o 

# Vanzo:wangfei on: Wed, 12 Nov 2014 21:06:42 +0800
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(project_name),)
custom_kernel_gyroscope:= $(addsuffix /, $(shell echo $(CUSTOM_KERNEL_GYROSCOPE)))
ifneq ($(custom_kernel_gyroscope),)
obj-y += $(custom_kernel_gyroscope)
obj-y +=  lsm6ds3gy/
endif
else
# End of Vanzo:wangfei

ifeq ($(CONFIG_MTK_LSM6DS3),y)
obj-y   +=  lsm6ds3gy/
endif
# Vanzo:wangfei on: Tue, 06 Jan 2015 10:34:15 +0800
# added for new structure config alsps
endif
# End of Vanzo:wangfei
