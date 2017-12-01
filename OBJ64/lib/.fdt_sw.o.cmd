cmd_lib/fdt_sw.o := /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/aarch64-linux-android-gcc -Wp,-MD,lib/.fdt_sw.o.d  -nostdinc -isystem /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/../lib/gcc/aarch64-linux-android/5.3.1/include -I../arch/arm64/include -Iarch/arm64/include/generated  -I../include -Iinclude -I../arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I../include/uapi -Iinclude/generated/uapi -include ../include/linux/kconfig.h  -I../lib -Ilib -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -mgeneral-regs-only -fno-pic -mtune=cortex-a53 -march=armv8-a -O2 -ftree-vectorize -fno-delete-null-pointer-checks -Os -Wno-maybe-uninitialized --param=allow-store-data-races=0 -Werror=frame-larger-than=1 -Wframe-larger-than=1760 -fstack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -DCC_HAVE_ASM_GOTO  -I../lib/../scripts/dtc/libfdt -Ilib/../scripts/dtc/libfdt    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(fdt_sw)"  -D"KBUILD_MODNAME=KBUILD_STR(fdt_sw)" -c -o lib/fdt_sw.o ../lib/fdt_sw.c

source_lib/fdt_sw.o := ../lib/fdt_sw.c

deps_lib/fdt_sw.o := \
  ../include/linux/libfdt_env.h \
  ../include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  ../include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  ../include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  ../include/linux/compiler-gcc5.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  ../include/uapi/linux/types.h \
  arch/arm64/include/generated/asm/types.h \
  ../include/uapi/asm-generic/types.h \
  ../include/asm-generic/int-ll64.h \
  ../include/uapi/asm-generic/int-ll64.h \
  ../arch/arm64/include/uapi/asm/bitsperlong.h \
  ../include/asm-generic/bitsperlong.h \
  ../include/uapi/asm-generic/bitsperlong.h \
  ../include/uapi/linux/posix_types.h \
  ../include/linux/stddef.h \
  ../include/uapi/linux/stddef.h \
  ../arch/arm64/include/uapi/asm/posix_types.h \
  ../include/uapi/asm-generic/posix_types.h \
  ../include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/lib/gcc/aarch64-linux-android/5.3.1/include/stdarg.h \
  ../include/uapi/linux/string.h \
  ../arch/arm64/include/asm/string.h \
  ../arch/arm64/include/uapi/asm/byteorder.h \
  ../include/linux/byteorder/little_endian.h \
  ../include/uapi/linux/byteorder/little_endian.h \
  ../include/linux/swab.h \
  ../include/uapi/linux/swab.h \
  arch/arm64/include/generated/asm/swab.h \
  ../include/uapi/asm-generic/swab.h \
  ../include/linux/byteorder/generic.h \
  ../lib/../scripts/dtc/libfdt/fdt_sw.c \
  ../lib/../scripts/dtc/libfdt/libfdt_env.h \
  ../lib/../scripts/dtc/libfdt/fdt.h \
  ../lib/../scripts/dtc/libfdt/libfdt.h \
  ../lib/../scripts/dtc/libfdt/libfdt_env.h \
  ../lib/../scripts/dtc/libfdt/libfdt_internal.h \

lib/fdt_sw.o: $(deps_lib/fdt_sw.o)

$(deps_lib/fdt_sw.o):
