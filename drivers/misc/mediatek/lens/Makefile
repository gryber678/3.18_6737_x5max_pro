#
# Makefile for misc devices that really don't fit anywhere else.
#

MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))
ARCH_MTK_PROJECT := $(subst ",,$(CONFIG_ARCH_MTK_PROJECT))

MTK_CUSTOM_PATH  := $(srctree)/drivers/misc/mediatek

# Vanzo:wangfei on: Wed, 12 Nov 2014 21:08:36 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)
endif
# End of Vanzo:wangfei

ifneq ($(CONFIG_ARM64), y)
# For arm32, mach folder
MTK_PLATFORM_PATH_ROOT := $(srctree)/arch/arm/mach-$(MTK_PLATFORM)
MTK_PROJECT_PATH_ROOT := $(srctree)/arch/arm/mach-$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)
  ifneq ($(filter y,$(CONFIG_ARCH_MT6752) $(CONFIG_ARCH_MT6732) $(CONFIG_ARCH_MT6595) $(CONFIG_ARCH_MT6795) $(CONFIG_ARCH_MT6735) $(CONFIG_ARCH_MT6735M) $(CONFIG_ARCH_MT6753) $(CONFIG_ARCH_MT6580)),)
    # For armv8 32bit
    MTK_PLATFORM_PATH_ROOT := $(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)
    MTK_PROJECT_PATH_ROOT := $(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)
  endif
else
# For arm64, no mach-xxx folder
MTK_PLATFORM_PATH_ROOT := $(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)
MTK_PROJECT_PATH_ROOT := $(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)
endif



subdir-ccflags-y += -I$(MTK_CUSTOM_PATH)/lens/inc
subdir-ccflags-$(CONFIG_MTK_LENS_LC898212XDAF_SUPPORT) += -I$(MTK_CUSTOM_PATH)/lens/common/lc898212xdaf/inc
subdir-ccflags-$(CONFIG_MTK_LENS_BU63165AF_SUPPORT) += -I$(MTK_CUSTOM_PATH)/lens/common/bu63165af/inc

obj-y      += main_lens.o
obj-y      += sub_lens.o

# Vanzo:wangfei on: Wed, 12 Nov 2014 21:06:42 +0800
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))

ifneq ($(project_name),)
LENS_LISTS := $(subst ",,$(CUSTOM_KERNEL_LENS))
obj-y += $(foreach LEN,$(LENS_LISTS),common/$(LEN)/)

LENS_CODE_DEFS := $(foreach LEN, $(LENS_LISTS),$(foreach v,$(shell echo $(LEN) | tr '[a-z]' '[A-Z]'), -DCONFIG_MTK_LENS_$(v)_SUPPORT))
$(info LENS_CODE_DEFS $(LENS_CODE_DEFS))
subdir-ccflags-y += $(LENS_CODE_DEFS)
else
obj-$(CONFIG_MTK_LENS_FM50AF_SUPPORT)      += common/fm50af/
obj-$(CONFIG_MTK_LENS_BU6424AF_SUPPORT)    += common/bu6424af/
obj-$(CONFIG_MTK_LENS_BU6429AF_SUPPORT)    += common/bu6429af/
obj-$(CONFIG_MTK_LENS_BU63165AF_SUPPORT)  += common/bu63165af/
obj-$(CONFIG_MTK_LENS_DW9714AF_SUPPORT)    += common/dw9714af/
obj-$(CONFIG_MTK_LENS_DW9814AF_SUPPORT)    += common/dw9814af/
obj-$(CONFIG_MTK_LENS_DW9718AF_SUPPORT)    += common/dw9718af/
obj-$(CONFIG_MTK_LENS_LC898212AF_SUPPORT)  += common/lc898212af/
obj-$(CONFIG_MTK_LENS_LC898212XDAF_SUPPORT)  += common/lc898212xdaf/
obj-$(CONFIG_MTK_LENS_LC898122AF_SUPPORT)  += common/lc898122af/
obj-$(CONFIG_MTK_LENS_AD5820AF_SUPPORT)    += common/ad5820af/
obj-$(CONFIG_MTK_LENS_DW9800AF_SUPPORT)    += common/dw9800af/
# Vanzo:wangfei on: Tue, 06 Jan 2015 10:34:15 +0800
# added for new structure config alsps
endif
# End of Vanzo:wangfei
