cmd_arch/arm64/boot/dts/n370b.dtb := /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/aarch64-linux-android-gcc -E -Wp,-MD,arch/arm64/boot/dts/.n370b.dtb.d.pre.tmp -nostdinc -I../arch/arm64/boot/dts -Iarch/arm64/boot/dts -I../arch/arm64/boot/dts/include -I./include/ -I../drivers/of/testcase-data -undef -D__DTS__ -x assembler-with-cpp -o arch/arm64/boot/dts/.n370b.dtb.dts.tmp ../arch/arm64/boot/dts/n370b.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm64/boot/dts/n370b.dtb -b 0 -i ../arch/arm64/boot/dts/  -d arch/arm64/boot/dts/.n370b.dtb.d.dtc.tmp arch/arm64/boot/dts/.n370b.dtb.dts.tmp ; cat arch/arm64/boot/dts/.n370b.dtb.d.pre.tmp arch/arm64/boot/dts/.n370b.dtb.d.dtc.tmp > arch/arm64/boot/dts/.n370b.dtb.d

source_arch/arm64/boot/dts/n370b.dtb := ../arch/arm64/boot/dts/n370b.dts

deps_arch/arm64/boot/dts/n370b.dtb := \
  ../arch/arm64/boot/dts/mt6735m.dtsi \
    $(wildcard include/config/base.h) \
    $(wildcard include/config/addr.h) \
  ../arch/arm64/boot/dts/include/dt-bindings/interrupt-controller/arm-gic.h \
  ../arch/arm64/boot/dts/include/dt-bindings/interrupt-controller/irq.h \
  ../arch/arm64/boot/dts/mt6735m-pinfunc.h \
  ../arch/arm64/boot/dts/mt65xx.h \
  ../arch/arm64/boot/dts/include/dt-bindings/mmc/mt67xx-msdc.h \
  arch/arm64/boot/dts/cust.dtsi \
  ../arch/arm64/boot/dts/trusty.dtsi \

arch/arm64/boot/dts/n370b.dtb: $(deps_arch/arm64/boot/dts/n370b.dtb)

$(deps_arch/arm64/boot/dts/n370b.dtb):
