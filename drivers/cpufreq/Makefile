# CPUfreq core###############################################################
obj-$(CONFIG_CPU_FREQ)			+= cpufreq.o freq_table.o
obj-$(CONFIG_PM_OPP)			+= cpufreq_opp.o
# CPUfreq stats##############################################################
obj-$(CONFIG_CPU_FREQ_STAT)             += cpufreq_stats.o
# CPUfreq governors########################################################## 
obj-$(CONFIG_CPU_FREQ_GOV_POWERSAVE)	+= cpufreq_powersave.o cpufreq_thunderx.o 
obj-$(CONFIG_CPU_FREQ_DEFAULT_GOV_THUNDERX)  += cpufreq_thunderx.o
obj-$(CONFIG_CPU_FREQ_GOV_LIONHEART)    += cpufreq_lionheart.o
obj-$(CONFIG_CPU_FREQ_GOV_INTERACTIVEPLUS)	+= cpufreq_sakuractive.o
obj-$(CONFIG_CPU_FREQ_GOV_COMMON)		+= cpufreq_governor.o
