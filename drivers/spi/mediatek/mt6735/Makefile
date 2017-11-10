subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include \
					-I$(srctree)/drivers/misc/mediatek/include/mt-plat
ifeq ($(CONFIG_TRUSTONIC_TEE_SUPPORT),y)
subdir-ccflags-y += -I$(srctree)/drivers/spi/trustzone/spi/Tlspi/inc
endif
# Vanzo:wangfei on: Wed, 12 Nov 2014 21:08:36 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)

dtv_name := $(shell echo $(CUSTOM_KERNEL_DTV))
ifeq ($(strip $(dtv_name)),nm32x)
    ccflags-y += -DCUSTOM_KERNEL_DTV_NM32X
endif
endif
# End of Vanzo:wangfei

ifeq ($(CONFIG_TRUSTONIC_TEE_SUPPORT),y)
	include $(srctree)/drivers/misc/mediatek/gud/Makefile.include
endif

obj-$(CONFIG_MTK_SPI) += spi.o spi-dev.o
