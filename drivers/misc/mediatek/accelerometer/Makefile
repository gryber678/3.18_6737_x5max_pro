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

obj-y   +=  accel.o accel_factory.o
# Vanzo:wangfei on: Wed, 12 Nov 2014 21:06:42 +0800
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
 
ifneq ($(project_name),)
custom_kernel_accelerometer:= $(addsuffix /, $(shell echo $(CUSTOM_KERNEL_ACCELEROMETER)))
ifneq ($(custom_kernel_accelerometer),)
obj-y += $(custom_kernel_accelerometer)
endif
else
# End of Vanzo:wangfei

obj-$(CONFIG_MTK_K2DH)   +=  k2dh/
obj-$(CONFIG_MTK_BMA050)   +=  bma050/
obj-$(CONFIG_MTK_BMA2XX)   +=  bma2xx/
obj-$(CONFIG_MTK_BMA050_NEW)   +=  bma050-new/
obj-$(CONFIG_MTK_BMA222E)   +=  bma222E/
obj-$(CONFIG_MTK_BMA222E_NEW)   +=  bma222E-new/
obj-$(CONFIG_MTK_MPU6050G)   +=  mpu6050g/
obj-$(CONFIG_MTK_MPU6050G_NEW)   +=  mpu6050g-new/
obj-$(CONFIG_MTK_BMA250)   +=  bma250/
obj-$(CONFIG_MTK_KXTIK1004)   +=  kxtik1004/
obj-$(CONFIG_MTK_MPU6515A)   +=  mpu6515/
obj-$(CONFIG_MTK_KXTJ2_1009)   +=  KXTJ2_1009/
obj-$(CONFIG_MTK_KXTJ2_1009_NEW)   +=  KXTJ2_1009-new/
obj-$(CONFIG_MTK_BMA250E)   +=  bma250e/
obj-$(CONFIG_MTK_MC3410_NEW)   +=  mc3410-new/
obj-$(CONFIG_MTK_MC3XXX_AUTO)   +=  mc3xxx_auto/
obj-$(CONFIG_MTK_MPU60X0)   +=  mpu60x0/
obj-$(CONFIG_MTK_BMA255_SDO0)   +=  bma255-sdo0/
obj-$(CONFIG_MTK_BMA255_SDO1)   +=  bma255-sdo1/
obj-$(CONFIG_MTK_BMA056)   +=  bma056/
obj-$(CONFIG_MTK_LSM6DS3)   +=  lsm6ds3/
obj-$(CONFIG_MTK_ICM20645G)   +=  icm20645g/
# Vanzo:wangfei on: Tue, 06 Jan 2015 10:34:15 +0800
# added for new structure config alsps
endif
# End of Vanzo:wangfei
