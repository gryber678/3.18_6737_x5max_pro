cmd_drivers/misc/mediatek/video/mt6735/videox/debug.o := /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/aarch64-linux-android-gcc -Wp,-MD,drivers/misc/mediatek/video/mt6735/videox/.debug.o.d  -nostdinc -isystem /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/../lib/gcc/aarch64-linux-android/5.3.1/include -I../arch/arm64/include -Iarch/arm64/include/generated  -I../include -Iinclude -I../arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I../include/uapi -Iinclude/generated/uapi -include ../include/linux/kconfig.h  -I../drivers/misc/mediatek/video/mt6735/videox -Idrivers/misc/mediatek/video/mt6735/videox -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -mgeneral-regs-only -fno-pic -mtune=cortex-a53 -march=armv8-a -O2 -ftree-vectorize -fno-delete-null-pointer-checks -Os -Wno-maybe-uninitialized --param=allow-store-data-races=0 -Werror=frame-larger-than=1 -Wframe-larger-than=1760 -fstack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -DCC_HAVE_ASM_GOTO  -I../../drivers/misc/mediatek/hibcore -I../drivers/misc/mediatek/hibcore  -I../../drivers/misc/mediatek/include -I../drivers/misc/mediatek/include  -I../../drivers/misc/mediatek/include/mt-plat/mt6735/include -I../drivers/misc/mediatek/include/mt-plat/mt6735/include  -I../../drivers/misc/mediatek/include/mt-plat -I../drivers/misc/mediatek/include/mt-plat  -I../../drivers/mmc/host/mediatek/mt6735 -I../drivers/mmc/host/mediatek/mt6735  -I../../drivers/misc/mediatek/m4u/mt6735 -I../drivers/misc/mediatek/m4u/mt6735  -I../../drivers/misc/mediatek/m4u/mt6735/mt6735 -I../drivers/misc/mediatek/m4u/mt6735/mt6735  -I../../drivers/misc/mediatek/mmp/ -I../drivers/misc/mediatek/mmp/  -I../../drivers/misc/mediatek/ext_disp/mt6735m/ -I../drivers/misc/mediatek/ext_disp/mt6735m/  -I../../drivers/misc/mediatek/video/mt6735/dispsys/mt6735m/ -I../drivers/misc/mediatek/video/mt6735/dispsys/mt6735m/  -I../../drivers/misc/mediatek/video/mt6735/videox/mt6735m/ -I../drivers/misc/mediatek/video/mt6735/videox/mt6735m/  -I../../drivers/misc/mediatek/video/include/ -I../drivers/misc/mediatek/video/include/  -I../../drivers/misc/mediatek/video/common/ -I../drivers/misc/mediatek/video/common/  -I../../drivers/misc/mediatek/video/common/rdma10/ -I../drivers/misc/mediatek/video/common/rdma10/  -I../../drivers/misc/mediatek/video/common/wdma10/ -I../drivers/misc/mediatek/video/common/wdma10/  -I../../drivers/misc/mediatek/video/mt6735/dispsys/ -I../drivers/misc/mediatek/video/mt6735/dispsys/  -I../../drivers/misc/mediatek/sync/ -I../drivers/misc/mediatek/sync/  -I../../drivers/misc/mediatek/cmdq/v2/ -I../drivers/misc/mediatek/cmdq/v2/  -I../../drivers/misc/mediatek/cmdq/v2/mt6735/ -I../drivers/misc/mediatek/cmdq/v2/mt6735/  -I../../drivers/misc/mediatek/ext_disp/ -I../drivers/misc/mediatek/ext_disp/  -I../../drivers/misc/mediatek/mach/mt6735/include/mach/ -I../drivers/misc/mediatek/mach/mt6735/include/mach/  -I../../drivers/misc/mediatek/base/power/mt6735 -I../drivers/misc/mediatek/base/power/mt6735  -I../../drivers/misc/mediatek/smi/ -I../drivers/misc/mediatek/smi/  -I../../drivers/misc/mediatek/include/mt-plat/ -I../drivers/misc/mediatek/include/mt-plat/  -I../../drivers/misc/mediatek/lcm/inc -I../drivers/misc/mediatek/lcm/inc  -I../../drivers/staging/android/ion -I../drivers/staging/android/ion  -I../../drivers/staging/android/ion/mtk -I../drivers/staging/android/ion/mtk -DHWGPU_SUPPORT    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(debug)"  -D"KBUILD_MODNAME=KBUILD_STR(debug)" -c -o drivers/misc/mediatek/video/mt6735/videox/debug.o ../drivers/misc/mediatek/video/mt6735/videox/debug.c

source_drivers/misc/mediatek/video/mt6735/videox/debug.o := ../drivers/misc/mediatek/video/mt6735/videox/debug.c

deps_drivers/misc/mediatek/video/mt6735/videox/debug.o := \
    $(wildcard include/config/mtk/legacy.h) \
    $(wildcard include/config/struct.h) \
    $(wildcard include/config/mt/eng/build.h) \
    $(wildcard include/config/mtk/gmo/ram/optimize.h) \
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
  ../include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  ../include/linux/cache.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  ../include/uapi/linux/kernel.h \
  ../include/uapi/linux/sysinfo.h \
  ../arch/arm64/include/asm/cache.h \
  ../arch/arm64/include/asm/cachetype.h \
  ../arch/arm64/include/asm/cputype.h \
  ../include/linux/bitops.h \
  ../arch/arm64/include/asm/bitops.h \
  ../arch/arm64/include/asm/barrier.h \
  ../include/asm-generic/bitops/builtin-__ffs.h \
  ../include/asm-generic/bitops/builtin-ffs.h \
  ../include/asm-generic/bitops/builtin-__fls.h \
  ../include/asm-generic/bitops/builtin-fls.h \
  ../include/asm-generic/bitops/ffz.h \
  ../include/asm-generic/bitops/fls64.h \
  ../include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  ../include/asm-generic/bitops/sched.h \
  ../include/asm-generic/bitops/hweight.h \
  ../include/asm-generic/bitops/arch_hweight.h \
  ../include/asm-generic/bitops/const_hweight.h \
  ../include/asm-generic/bitops/lock.h \
  ../include/asm-generic/bitops/non-atomic.h \
  ../include/asm-generic/bitops/le.h \
  ../arch/arm64/include/uapi/asm/byteorder.h \
  ../include/linux/byteorder/little_endian.h \
  ../include/uapi/linux/byteorder/little_endian.h \
  ../include/linux/swab.h \
  ../include/uapi/linux/swab.h \
  arch/arm64/include/generated/asm/swab.h \
  ../include/uapi/asm-generic/swab.h \
  ../include/linux/byteorder/generic.h \
  ../include/linux/seqlock.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  ../include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  ../include/linux/typecheck.h \
  ../include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  ../include/linux/linkage.h \
  ../include/linux/stringify.h \
  ../include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  ../arch/arm64/include/asm/linkage.h \
  ../include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  ../include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  ../include/uapi/linux/const.h \
  ../include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  ../include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  ../include/linux/printk.h \
    $(wildcard include/config/mt/printk/uart/console.h) \
    $(wildcard include/config/log/too/much/warning.h) \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  ../include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/lto.h) \
  ../include/linux/kern_levels.h \
  ../include/linux/dynamic_debug.h \
  ../include/linux/errno.h \
  ../include/uapi/linux/errno.h \
  arch/arm64/include/generated/asm/errno.h \
  ../include/uapi/asm-generic/errno.h \
  ../include/uapi/asm-generic/errno-base.h \
  arch/arm64/include/generated/asm/preempt.h \
  ../include/asm-generic/preempt.h \
  ../include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  ../include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  arch/arm64/include/generated/asm/bug.h \
  ../include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  ../arch/arm64/include/asm/thread_info.h \
    $(wildcard include/config/arm64/64k/pages.h) \
  ../include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/preempt/monitor.h) \
    $(wildcard include/config/mtprof.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  ../arch/arm64/include/asm/irqflags.h \
  ../arch/arm64/include/asm/ptrace.h \
    $(wildcard include/config/cpu/big/endian.h) \
  ../arch/arm64/include/uapi/asm/ptrace.h \
  ../arch/arm64/include/asm/hwcap.h \
  ../arch/arm64/include/uapi/asm/hwcap.h \
  ../include/linux/bottom_half.h \
  ../include/linux/preempt_mask.h \
  ../include/linux/spinlock_types.h \
  ../arch/arm64/include/asm/spinlock_types.h \
  ../include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  ../include/linux/rwlock_types.h \
  ../arch/arm64/include/asm/spinlock.h \
  ../arch/arm64/include/asm/processor.h \
  ../arch/arm64/include/asm/fpsimd.h \
  ../arch/arm64/include/asm/hw_breakpoint.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
  ../include/linux/rwlock.h \
  ../include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  ../include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  ../include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  ../arch/arm64/include/asm/atomic.h \
  ../arch/arm64/include/asm/cmpxchg.h \
  ../include/asm-generic/atomic-long.h \
  ../include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  arch/arm64/include/generated/asm/div64.h \
  ../include/asm-generic/div64.h \
  ../include/linux/time64.h \
  ../include/uapi/linux/time.h \
  ../include/linux/uaccess.h \
  ../arch/arm64/include/asm/uaccess.h \
  ../arch/arm64/include/asm/memory.h \
    $(wildcard include/config/arm64/va/bits.h) \
  arch/arm64/include/generated/asm/sizes.h \
  ../include/asm-generic/sizes.h \
  ../include/linux/sizes.h \
  ../include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  ../arch/arm64/include/asm/compiler.h \
  ../include/linux/fb.h \
    $(wildcard include/config/fb/deferred/io.h) \
    $(wildcard include/config/fb/tileblitting.h) \
    $(wildcard include/config/fb/backlight.h) \
    $(wildcard include/config/fb/foreign/endian.h) \
    $(wildcard include/config/fb/both/endian.h) \
    $(wildcard include/config/fb/big/endian.h) \
    $(wildcard include/config/fb/little/endian.h) \
  ../include/linux/kgdb.h \
    $(wildcard include/config/have/arch/kgdb.h) \
    $(wildcard include/config/kgdb.h) \
    $(wildcard include/config/serial/kgdb/nmi.h) \
  ../arch/arm64/include/asm/kgdb.h \
  ../include/linux/ptrace.h \
  ../include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/mtk/sched/cmp/tgs.h) \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/disable/cpu/sched/domain/balance.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/mtk/sched/cmp.h) \
    $(wildcard include/config/sched/hmp.h) \
    $(wildcard include/config/sched/hmp/prio/filter.h) \
    $(wildcard include/config/hmp/tracer.h) \
    $(wildcard include/config/mtprof/cputime.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/mt/rt/throttle/mon.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/mt/sched/trace.h) \
    $(wildcard include/config/mt/sched/debug.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/stack/growsup.h) \
  ../include/uapi/linux/sched.h \
  ../include/linux/sched/prio.h \
  ../arch/arm64/include/uapi/asm/param.h \
  ../include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  ../include/uapi/asm-generic/param.h \
  ../include/linux/capability.h \
  ../include/uapi/linux/capability.h \
  ../include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  ../include/linux/timex.h \
  ../include/uapi/linux/timex.h \
  ../include/uapi/linux/param.h \
  ../arch/arm64/include/asm/timex.h \
  ../arch/arm64/include/asm/arch_timer.h \
  ../include/clocksource/arm_arch_timer.h \
    $(wildcard include/config/arm/arch/timer.h) \
  ../include/linux/clocksource.h \
    $(wildcard include/config/arch/clocksource/data.h) \
    $(wildcard include/config/clocksource/watchdog.h) \
    $(wildcard include/config/clksrc/of.h) \
  ../include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  ../include/linux/ktime.h \
  ../include/linux/jiffies.h \
  ../include/linux/timekeeping.h \
  ../include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  ../arch/arm64/include/asm/io.h \
  ../include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  ../arch/arm64/include/asm/pgtable.h \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/have/rcu/table/free.h) \
    $(wildcard include/config/arm64/pgtable/levels.h) \
  ../arch/arm64/include/asm/proc-fns.h \
  ../arch/arm64/include/asm/page.h \
    $(wildcard include/config/have/arch/pfn/valid.h) \
  ../arch/arm64/include/asm/pgtable-types.h \
  ../include/asm-generic/pgtable-nopud.h \
  ../include/asm-generic/getorder.h \
  ../arch/arm64/include/asm/pgtable-hwdef.h \
  ../include/asm-generic/pgtable.h \
    $(wildcard include/config/have/arch/soft/dirty.h) \
  ../include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/compaction.h) \
  ../include/linux/auxvec.h \
  ../include/uapi/linux/auxvec.h \
  ../arch/arm64/include/uapi/asm/auxvec.h \
  ../include/linux/rbtree.h \
  ../include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  ../include/linux/osq_lock.h \
  arch/arm64/include/generated/asm/rwsem.h \
  ../include/asm-generic/rwsem.h \
  ../include/linux/completion.h \
  ../include/linux/wait.h \
  arch/arm64/include/generated/asm/current.h \
  ../include/asm-generic/current.h \
  ../include/uapi/linux/wait.h \
  ../include/linux/cpumask.h \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  ../include/linux/bitmap.h \
  ../include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  ../include/linux/uprobes.h \
  ../include/linux/page-flags-layout.h \
  ../include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/generated/bounds.h \
  ../arch/arm64/include/asm/sparsemem.h \
  ../arch/arm64/include/asm/mmu.h \
  arch/arm64/include/generated/asm/early_ioremap.h \
  ../include/asm-generic/early_ioremap.h \
    $(wildcard include/config/generic/early/ioremap.h) \
  ../arch/arm64/include/asm/alternative.h \
  ../arch/arm64/include/asm/cpufeature.h \
  ../include/xen/xen.h \
    $(wildcard include/config/xen.h) \
    $(wildcard include/config/xen/dom0.h) \
    $(wildcard include/config/xen/pvh.h) \
  ../include/asm-generic/iomap.h \
    $(wildcard include/config/has/ioport/map.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/generic/iomap.h) \
  ../include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  ../include/asm-generic/timex.h \
  ../include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  ../include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/movable/node.h) \
  ../include/linux/cputime.h \
  arch/arm64/include/generated/asm/cputime.h \
  ../include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  ../include/asm-generic/cputime_jiffies.h \
  ../include/linux/smp.h \
  ../include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  ../arch/arm64/include/asm/smp.h \
  ../include/linux/sem.h \
  ../include/linux/rcupdate.h \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/rcu/nocb/cpu/all.h) \
    $(wildcard include/config/no/hz/full/sysidle.h) \
  ../include/linux/rcutree.h \
  ../include/uapi/linux/sem.h \
  ../include/linux/ipc.h \
  ../include/linux/uidgid.h \
    $(wildcard include/config/user/ns.h) \
  ../include/linux/highuid.h \
  ../include/uapi/linux/ipc.h \
  arch/arm64/include/generated/asm/ipcbuf.h \
  ../include/uapi/asm-generic/ipcbuf.h \
  arch/arm64/include/generated/asm/sembuf.h \
  ../include/uapi/asm-generic/sembuf.h \
  ../include/linux/shm.h \
  ../include/uapi/linux/shm.h \
  arch/arm64/include/generated/asm/shmbuf.h \
  ../include/uapi/asm-generic/shmbuf.h \
  ../arch/arm64/include/asm/shmparam.h \
  ../include/uapi/asm-generic/shmparam.h \
  ../include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  ../include/uapi/linux/signal.h \
  ../arch/arm64/include/uapi/asm/signal.h \
  ../include/asm-generic/signal.h \
  ../include/uapi/asm-generic/signal.h \
  ../include/uapi/asm-generic/signal-defs.h \
  ../arch/arm64/include/uapi/asm/sigcontext.h \
  ../arch/arm64/include/uapi/asm/siginfo.h \
  ../include/asm-generic/siginfo.h \
  ../include/uapi/asm-generic/siginfo.h \
  ../include/linux/pid.h \
  ../include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  ../include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  ../include/linux/pfn.h \
  ../arch/arm64/include/asm/percpu.h \
  ../include/asm-generic/percpu.h \
  ../include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  ../include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
  ../include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  ../include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  ../include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  ../include/linux/notifier.h \
  ../include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  ../include/linux/srcu.h \
  ../include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/sysfs.h) \
  ../arch/arm64/include/asm/topology.h \
    $(wildcard include/config/arm/cpu/topology.h) \
    $(wildcard include/config/mtk/cpu/topology.h) \
  ../include/asm-generic/topology.h \
  ../include/linux/proportions.h \
  ../include/linux/percpu_counter.h \
  ../include/linux/gfp.h \
    $(wildcard include/config/pm/sleep.h) \
  ../include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/have/arch/seccomp/filter.h) \
    $(wildcard include/config/seccomp/filter.h) \
  ../include/uapi/linux/seccomp.h \
  ../arch/arm64/include/asm/seccomp.h \
  ../arch/arm64/include/asm/unistd.h \
  ../arch/arm64/include/uapi/asm/unistd.h \
  ../include/asm-generic/unistd.h \
  ../include/uapi/asm-generic/unistd.h \
  ../include/asm-generic/seccomp.h \
  ../include/uapi/linux/unistd.h \
  ../include/linux/rculist.h \
  ../include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  ../include/linux/resource.h \
  ../include/uapi/linux/resource.h \
  arch/arm64/include/generated/asm/resource.h \
  ../include/asm-generic/resource.h \
  ../include/uapi/asm-generic/resource.h \
  ../include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  ../include/linux/timerqueue.h \
  ../include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  ../include/linux/latencytop.h \
  ../include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  ../include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  ../include/linux/sysctl.h \
  ../include/uapi/linux/sysctl.h \
  ../include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  ../include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  ../include/uapi/linux/magic.h \
  ../include/linux/err.h \
  ../include/linux/pid_namespace.h \
    $(wildcard include/config/pid/ns.h) \
  ../include/linux/mm.h \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/metag.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  ../include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  ../include/linux/range.h \
  ../include/linux/bit_spinlock.h \
  ../include/linux/shrinker.h \
  ../include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/toi/incremental.h) \
  ../include/linux/huge_mm.h \
  ../include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
    $(wildcard include/config/debug/tlbflush.h) \
    $(wildcard include/config/debug/vm/vmacache.h) \
  ../include/linux/vm_event_item.h \
    $(wildcard include/config/migration.h) \
    $(wildcard include/config/memory/balloon.h) \
    $(wildcard include/config/balloon/compaction.h) \
  ../include/linux/nsproxy.h \
  ../include/linux/kref.h \
  ../include/uapi/linux/ptrace.h \
  ../arch/arm64/include/asm/debug-monitors.h \
  ../include/uapi/linux/fb.h \
  ../include/linux/i2c.h \
    $(wildcard include/config/mtk/i2c/extension.h) \
    $(wildcard include/config/i2c.h) \
    $(wildcard include/config/i2c/boardinfo.h) \
    $(wildcard include/config/i2c/mux.h) \
    $(wildcard include/config/of.h) \
  ../include/linux/mod_devicetable.h \
  ../include/linux/uuid.h \
  ../include/uapi/linux/uuid.h \
  ../include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/acpi.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/dma/cma.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  ../include/linux/ioport.h \
  ../include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  ../include/linux/sysfs.h \
  ../include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  ../include/linux/idr.h \
  ../include/linux/kobject_ns.h \
  ../include/linux/stat.h \
  ../arch/arm64/include/asm/stat.h \
  ../arch/arm64/include/uapi/asm/stat.h \
  ../include/uapi/asm-generic/stat.h \
  ../arch/arm64/include/asm/compat.h \
  ../include/uapi/linux/stat.h \
  ../include/linux/klist.h \
  ../include/linux/pinctrl/devinfo.h \
    $(wildcard include/config/pm.h) \
  ../include/linux/pinctrl/consumer.h \
  ../include/linux/seq_file.h \
  ../include/linux/pinctrl/pinctrl-state.h \
  ../include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  ../include/linux/ratelimit.h \
  ../arch/arm64/include/asm/device.h \
    $(wildcard include/config/iommu/api.h) \
  ../include/linux/pm_wakeup.h \
  ../include/linux/of.h \
    $(wildcard include/config/sparc.h) \
    $(wildcard include/config/of/dynamic.h) \
    $(wildcard include/config/attach/node.h) \
    $(wildcard include/config/detach/node.h) \
    $(wildcard include/config/add/property.h) \
    $(wildcard include/config/remove/property.h) \
    $(wildcard include/config/update/property.h) \
    $(wildcard include/config/of/resolve.h) \
  ../include/linux/property.h \
  ../include/uapi/linux/i2c.h \
  ../include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/async/fsync.h) \
    $(wildcard include/config/fs/xip.h) \
  ../include/linux/kdev_t.h \
  ../include/uapi/linux/kdev_t.h \
  ../include/linux/dcache.h \
  ../include/linux/rculist_bl.h \
  ../include/linux/list_bl.h \
  ../include/linux/lockref.h \
    $(wildcard include/config/arch/use/cmpxchg/lockref.h) \
  ../include/linux/path.h \
  ../include/linux/list_lru.h \
  ../include/linux/radix-tree.h \
  ../include/linux/semaphore.h \
  ../include/uapi/linux/fiemap.h \
  ../include/linux/migrate_mode.h \
  ../include/linux/percpu-rwsem.h \
  ../include/uapi/linux/fs.h \
  ../include/uapi/linux/limits.h \
  ../include/uapi/linux/ioctl.h \
  arch/arm64/include/generated/asm/ioctl.h \
  ../include/asm-generic/ioctl.h \
  ../include/uapi/asm-generic/ioctl.h \
  ../include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  ../include/uapi/linux/dqblk_xfs.h \
  ../include/linux/dqblk_v1.h \
  ../include/linux/dqblk_v2.h \
  ../include/linux/dqblk_qtree.h \
  ../include/linux/projid.h \
  ../include/uapi/linux/quota.h \
  ../include/linux/nfs_fs_i.h \
  ../include/linux/fcntl.h \
  ../include/uapi/linux/fcntl.h \
  ../arch/arm64/include/uapi/asm/fcntl.h \
  ../include/uapi/asm-generic/fcntl.h \
  ../include/linux/backlight.h \
  ../include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/memcg/kmem.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
  ../include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  ../include/linux/vmalloc.h \
  ../include/linux/debugfs.h \
    $(wildcard include/config/debug/fs.h) \
  ../include/linux/delay.h \
  arch/arm64/include/generated/asm/delay.h \
  ../include/asm-generic/delay.h \
  ../drivers/misc/mediatek/m4u/mt6735/m4u.h \
  ../drivers/misc/mediatek/m4u/mt6735/../2.0/m4u_v2.h \
  ../drivers/misc/mediatek/m4u/mt6735/m4u_port.h \
    $(wildcard include/config/arch/mt6735.h) \
    $(wildcard include/config/arch/mt6735m.h) \
    $(wildcard include/config/arch/mt6753.h) \
  ../drivers/misc/mediatek/m4u/mt6735/mt6735m/m4u_port.h \
  ../include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
    $(wildcard include/config/arch/has/sg/chain.h) \
  arch/arm64/include/generated/asm/scatterlist.h \
  ../include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  ../drivers/misc/mediatek/video/mt6735/videox/disp_drv_log.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/display_recorder.h \
  ../drivers/misc/mediatek/mmp/mmprofile.h \
    $(wildcard include/config/mmprofile.h) \
  ../drivers/misc/mediatek/mmp/mmprofile_static_event.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/disp_event.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_info.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_hal.h \
  ../drivers/misc/mediatek/video/include/DpDataType.h \
  ../drivers/misc/mediatek/lcm/inc/lcm_drv.h \
    $(wildcard include/config/params.h) \
    $(wildcard include/config/arch/mt6580.h) \
  ../include/linux/platform_device.h \
    $(wildcard include/config/suspend.h) \
    $(wildcard include/config/hibernate/callbacks.h) \
  ../include/linux/regulator/consumer.h \
    $(wildcard include/config/regulator.h) \
  ../drivers/misc/mediatek/video/mt6735/dispsys/mt6735m/ddp_ovl.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_hal.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_info.h \
  ../drivers/misc/mediatek/video/include/disp_session.h \
    $(wildcard include/config/for/source/pq.h) \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_debug.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_mmp.h \
  ../drivers/misc/mediatek/mmp/mmprofile_internal.h \
    $(wildcard include/config/mmprofile/path.h) \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_dump.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_path.h \
  ../drivers/misc/mediatek/video/mt6735/videox/mt6735m/disp_drv_platform.h \
    $(wildcard include/config/mtk/clkmgr.h) \
    $(wildcard include/config/all/in/trigger/stage.h) \
    $(wildcard include/config/fpga/early/porting.h) \
    $(wildcard include/config/mtk/gpu/support.h) \
    $(wildcard include/config/single/panel/output.h) \
  ../include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  ../include/linux/dma-attrs.h \
  ../include/linux/dma-direction.h \
  ../arch/arm64/include/asm/dma-mapping.h \
  ../include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  ../arch/arm64/include/asm/xen/hypervisor.h \
  ../arch/arm64/include/../../arm/include/asm/xen/hypervisor.h \
  ../include/asm-generic/dma-mapping-common.h \
  ../include/linux/kmemcheck.h \
  ../include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  ../drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_clkmgr.h \
  ../drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_clkmgr2.h \
    $(wildcard include/config/clkmgr/stat.h) \
    $(wildcard include/config/base.h) \
  ../drivers/misc/mediatek/video/mt6735/videox/disp_assert_layer.h \
  ../drivers/misc/mediatek/include/mt-plat/sync_write.h \
  ../include/linux/io.h \
  ../arch/arm64/include/asm/cacheflush.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_path.h \
  ../drivers/misc/mediatek/video/include/mtkfb.h \
    $(wildcard include/config/immediate/update.h) \
    $(wildcard include/config/dirty.h) \
    $(wildcard include/config/arch/mt6797.h) \
  ../drivers/misc/mediatek/video/include/mtkfb_info.h \
  ../include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  ../include/linux/irqreturn.h \
  ../include/linux/irqnr.h \
  ../include/uapi/linux/irqnr.h \
  ../include/linux/hardirq.h \
  ../include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  ../include/linux/vtime.h \
  ../include/linux/context_tracking_state.h \
  ../include/linux/static_key.h \
  ../include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  ../arch/arm64/include/asm/jump_label.h \
  ../arch/arm64/include/asm/insn.h \
  ../arch/arm64/include/asm/hardirq.h \
  ../arch/arm64/include/asm/irq.h \
  ../include/asm-generic/irq.h \
  ../include/linux/irq_cpustat.h \
  include/generated/uapi/linux/version.h \
  ../include/../drivers/staging/android/sw_sync.h \
    $(wildcard include/config/sw/sync.h) \
  ../include/../drivers/staging/android/sync.h \
  ../include/linux/fence.h \
    $(wildcard include/config/fence/trace.h) \
  ../include/../drivers/staging/android/uapi/sync.h \
  ../include/../drivers/staging/android/uapi/sw_sync.h \
  ../drivers/misc/mediatek/video/mt6735/videox/debug.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/mt6735m/ddp_reg.h \
    $(wildcard include/config/stream/dirty.h) \
    $(wildcard include/config/00.h) \
    $(wildcard include/config/mmsys/inten.h) \
    $(wildcard include/config/mmsys/intsta.h) \
    $(wildcard include/config/pwm/apb/err/addr.h) \
    $(wildcard include/config/isp/mout/en.h) \
    $(wildcard include/config/mdp/rdma0/mout/en.h) \
    $(wildcard include/config/mdp/prz0/mout/en.h) \
    $(wildcard include/config/mdp/prz1/mout/en.h) \
    $(wildcard include/config/mdp/tdshp0/mout/en.h) \
    $(wildcard include/config/disp/ovl0/mout/en.h) \
    $(wildcard include/config/disp/dither/mout/en.h) \
    $(wildcard include/config/disp/ufoe/mout/en.h) \
    $(wildcard include/config/mmsys/mout/rst.h) \
    $(wildcard include/config/mdp/prz0/sel/in.h) \
    $(wildcard include/config/mdp/prz1/sel/in.h) \
    $(wildcard include/config/mdp/tdshp0/sel/in.h) \
    $(wildcard include/config/mdp/wdma0/sel/in.h) \
    $(wildcard include/config/mdp/wrot0/sel/in.h) \
    $(wildcard include/config/disp/color0/sel/in.h) \
    $(wildcard include/config/disp/wdma0/sel/in.h) \
    $(wildcard include/config/disp/ufoe/sel/in.h) \
    $(wildcard include/config/dsi0/sel/in.h) \
    $(wildcard include/config/dpi0/sel/in.h) \
    $(wildcard include/config/disp/rdma0/sout/sel/in.h) \
    $(wildcard include/config/disp/rdma1/sout/sel/in.h) \
    $(wildcard include/config/mmsys/misc.h) \
    $(wildcard include/config/mmsys/cg/con0.h) \
    $(wildcard include/config/mmsys/cg/set0.h) \
    $(wildcard include/config/mmsys/cg/clr0.h) \
    $(wildcard include/config/mmsys/cg/con1.h) \
    $(wildcard include/config/mmsys/cg/set1.h) \
    $(wildcard include/config/mmsys/cg/clr1.h) \
    $(wildcard include/config/mmsys/hw/dcm/dis0.h) \
    $(wildcard include/config/mmsys/hw/dcm/dis/set0.h) \
    $(wildcard include/config/mmsys/hw/dcm/dis/clr0.h) \
    $(wildcard include/config/mmsys/sw0/rst/b.h) \
    $(wildcard include/config/mmsys/sw1/rst/b.h) \
    $(wildcard include/config/mmsys/lcm/rst/b.h) \
    $(wildcard include/config/disp/fake/eng/en.h) \
    $(wildcard include/config/disp/fake/eng/rst.h) \
    $(wildcard include/config/disp/fake/eng/con0.h) \
    $(wildcard include/config/disp/fake/eng/con1.h) \
    $(wildcard include/config/disp/fake/eng/rd/addr.h) \
    $(wildcard include/config/disp/fake/eng/wr/addr.h) \
    $(wildcard include/config/disp/fake/eng/state.h) \
    $(wildcard include/config/mmsys/mbist/con.h) \
    $(wildcard include/config/mmsys/mbist/done.h) \
    $(wildcard include/config/mmsys/mbist/holdb.h) \
    $(wildcard include/config/mmsys/mbist/mode.h) \
    $(wildcard include/config/mmsys/mbist/fail0.h) \
    $(wildcard include/config/mmsys/mbist/fail1.h) \
    $(wildcard include/config/mmsys/mbist/bsel0.h) \
    $(wildcard include/config/mmsys/mbist/bsel1.h) \
    $(wildcard include/config/mmsys/mem/delsel0.h) \
    $(wildcard include/config/mmsys/mem/delsel1.h) \
    $(wildcard include/config/mmsys/mem/delsel2.h) \
    $(wildcard include/config/mmsys/mem/delsel3.h) \
    $(wildcard include/config/mmsys/debug/out/sel.h) \
    $(wildcard include/config/mmsys/dummy.h) \
    $(wildcard include/config/disp/dl/valid/0.h) \
    $(wildcard include/config/disp/dl/ready/0.h) \
    $(wildcard include/config/mdp/dl/valid/0.h) \
    $(wildcard include/config/mdp/dl/ready/0.h) \
    $(wildcard include/config/smi/larb0/greq.h) \
    $(wildcard include/config/c08.h) \
    $(wildcard include/config/c09.h) \
    $(wildcard include/config/c10.h) \
    $(wildcard include/config/c11.h) \
    $(wildcard include/config/c12.h) \
    $(wildcard include/config/c13.h) \
    $(wildcard include/config/mutex/inten.h) \
    $(wildcard include/config/mutex/intsta.h) \
    $(wildcard include/config/mutex/hw/dcm.h) \
    $(wildcard include/config/mutex0/en.h) \
    $(wildcard include/config/mutex0/rst.h) \
    $(wildcard include/config/mutex0/mod.h) \
    $(wildcard include/config/mutex0/sof.h) \
    $(wildcard include/config/mutex1/en.h) \
    $(wildcard include/config/mutex1/rst.h) \
    $(wildcard include/config/mutex1/mod.h) \
    $(wildcard include/config/mutex1/sof.h) \
    $(wildcard include/config/mutex2/en.h) \
    $(wildcard include/config/mutex2/rst.h) \
    $(wildcard include/config/mutex2/mod.h) \
    $(wildcard include/config/mutex2/sof.h) \
    $(wildcard include/config/mutex3/en.h) \
    $(wildcard include/config/mutex3/rst.h) \
    $(wildcard include/config/mutex3/mod.h) \
    $(wildcard include/config/mutex3/sof.h) \
    $(wildcard include/config/mutex4/en.h) \
    $(wildcard include/config/mutex4/rst.h) \
    $(wildcard include/config/mutex4/mod.h) \
    $(wildcard include/config/mutex4/sof.h) \
    $(wildcard include/config/mutex5/en.h) \
    $(wildcard include/config/mutex5/rst.h) \
    $(wildcard include/config/mutex5/mod.h) \
    $(wildcard include/config/mutex5/sof.h) \
    $(wildcard include/config/debug/out/sel.h) \
    $(wildcard include/config/mutex/en.h) \
    $(wildcard include/config/mutex/rst.h) \
    $(wildcard include/config/mutex/mod.h) \
    $(wildcard include/config/mutex/sof.h) \
  ../drivers/misc/mediatek/cmdq/v2/cmdq_record.h \
  ../drivers/misc/mediatek/cmdq/v2/cmdq_def.h \
    $(wildcard include/config/mtk/fpga.h) \
    $(wildcard include/config/aal.h) \
    $(wildcard include/config/primary/gamma.h) \
    $(wildcard include/config/sub/gamma.h) \
    $(wildcard include/config/primary/dither.h) \
    $(wildcard include/config/sub/dither.h) \
    $(wildcard include/config/primary/pwm.h) \
    $(wildcard include/config/sub/pwm.h) \
    $(wildcard include/config/primary/pq.h) \
    $(wildcard include/config/sub/pq.h) \
    $(wildcard include/config/od.h) \
    $(wildcard include/config/general.h) \
  ../drivers/misc/mediatek/cmdq/v2/mt6735/cmdq_engine.h \
  ../drivers/misc/mediatek/cmdq/v2/cmdq_event_common.h \
  ../drivers/misc/mediatek/cmdq/v2/cmdq_subsys_common.h \
  ../drivers/misc/mediatek/cmdq/v2/cmdq_core.h \
  ../drivers/misc/mediatek/cmdq/v2/cmdq_core.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_log.h \
  ../drivers/misc/mediatek/include/mt-plat/aee.h \
    $(wildcard include/config/mtk/aee/dram/console.h) \
  ../drivers/misc/mediatek/include/mt-plat/mtk_ram_console.h \
    $(wildcard include/config/mtk/ram/console.h) \
    $(wildcard include/config/mtk/emmc/support.h) \
    $(wildcard include/config/mtk/aee/ipanic.h) \
  ../include/linux/console.h \
    $(wildcard include/config/hw/console.h) \
    $(wildcard include/config/tty.h) \
    $(wildcard include/config/vga/console.h) \
  ../include/linux/pstore.h \
    $(wildcard include/config/pstore.h) \
  ../include/linux/kmsg_dump.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/display_recorder.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_debug.h \
  ../drivers/misc/mediatek/video/mt6735/videox/primary_display.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_manager.h \
  ../drivers/misc/mediatek/video/mt6735/videox/disp_lcm.h \
  ../drivers/misc/mediatek/video/common/mtkfb_fence.h \
    $(wildcard include/config/mtk/hdmi/3d/support.h) \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_dsi.h \
  ../drivers/misc/mediatek/video/include/fbconfig_kdebug.h \
    $(wildcard include/config/kdebug/h.h) \
    $(wildcard include/config/record.h) \
    $(wildcard include/config/record/list.h) \
    $(wildcard include/config/disp/if.h) \
  ../include/linux/compat.h \
    $(wildcard include/config/compat/old/sigaction.h) \
    $(wildcard include/config/odd/rt/sigaction.h) \
  ../include/linux/socket.h \
  arch/arm64/include/generated/asm/socket.h \
  ../include/uapi/asm-generic/socket.h \
  arch/arm64/include/generated/asm/sockios.h \
  ../include/uapi/asm-generic/sockios.h \
  ../include/uapi/linux/sockios.h \
  ../include/linux/uio.h \
  ../include/uapi/linux/uio.h \
  ../include/uapi/linux/socket.h \
  ../include/uapi/linux/if.h \
  ../include/uapi/linux/hdlc/ioctl.h \
  ../include/uapi/linux/aio_abi.h \
  ../drivers/misc/mediatek/video/mt6735/dispsys/ddp_manager.h \
  ../drivers/misc/mediatek/video/mt6735/videox/disp_assert_layer.h \

drivers/misc/mediatek/video/mt6735/videox/debug.o: $(deps_drivers/misc/mediatek/video/mt6735/videox/debug.o)

$(deps_drivers/misc/mediatek/video/mt6735/videox/debug.o):
