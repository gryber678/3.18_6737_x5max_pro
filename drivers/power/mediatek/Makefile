
include $(srctree)/drivers/misc/mediatek/Makefile.custom
ARCH_MTK_PROJECT := $(subst ",,$(CONFIG_ARCH_MTK_PROJECT))
MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))
subdir-ccflags-y +=  -I$(srctree)/drivers/misc/mediatek/include
subdir-ccflags-y +=  -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
subdir-ccflags-y +=  -I$(srctree)/drivers/misc/mediatek/include/mt-plat
subdir-ccflags-y +=  -I$(srctree)/drivers/misc/mediatek/mach/mt6735/$(ARCH_MTK_PROJECT)/dct/dct



ifeq ($(CONFIG_MTK_HAFG_20),y)
	obj-$(CONFIG_MTK_SMART_BATTERY) += battery_meter_fg_20.o battery_common_fg_20.o
else
obj-$(CONFIG_MTK_SMART_BATTERY) += battery_meter.o battery_common.o
endif


ifeq ($(CONFIG_MTK_FAN5402_SUPPORT),y)
	obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
else ifeq ($(CONFIG_MTK_FAN5405_SUPPORT),y)
	obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
else
	ifeq ($(CONFIG_MTK_SWCHR_SUPPORT),y)
			obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
	else
		ifeq ($(CONFIG_MTK_BQ24158_SUPPORT),y)
			obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
		else
			ifeq ($(CONFIG_MTK_BQ24196_SUPPORT),y)
				obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
			else
				ifeq ($(CONFIG_MTK_BQ24296_SUPPORT),y)
					obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
				else
					ifeq ($(CONFIG_MTK_NCP1851_SUPPORT),y)
						obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
					else
						ifeq ($(CONFIG_MTK_NCP1854_SUPPORT),y)
							obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
						else
							ifeq ($(CONFIG_MTK_BQ24160_SUPPORT),y)
								obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
							else
								ifeq ($(CONFIG_MTK_BQ24157_SUPPORT),y)
									obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
								else
									ifeq ($(CONFIG_MTK_BQ24250_SUPPORT),y)
										obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
									else
										ifeq ($(CONFIG_MTK_BQ24261_SUPPORT),y)
											obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
										else
											ifeq ($(CONFIG_MTK_RT9536_SUPPORT),y)
												obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
											else
												ifeq ($(CONFIG_MTK_BQ25896_SUPPORT),y)
													obj-$(CONFIG_MTK_SMART_BATTERY) += switch_charging.o
												else
													obj-$(CONFIG_MTK_SMART_BATTERY) += linear_charging.o
												endif
											endif
										endif
									endif
								endif
							endif
						endif
					endif
				endif
			endif
		endif
	endif
endif
