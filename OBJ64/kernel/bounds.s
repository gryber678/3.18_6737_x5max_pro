	.arch armv8-a+fp+simd
	.file	"bounds.c"
// GNU C89 (UBERTC-5.3.x) version 5.3.1 20160401 (optimized) (aarch64-linux-android)
//	compiled by GNU C version 5.2.1 20151010, GMP version 6.1.99, MPFR version 3.1.3, MPC version 1.0.3
// warning: MPFR header version 3.1.3 differs from library version 3.1.4.
// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed:  -nostdinc -I ../arch/arm64/include
// -I arch/arm64/include/generated -I ../include -I include
// -I ../arch/arm64/include/uapi -I arch/arm64/include/generated/uapi
// -I ../include/uapi -I include/generated/uapi -I ../. -I .
// -iprefix /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/../lib/gcc/aarch64-linux-android/5.3.1/
// -D __KERNEL__ -D CC_HAVE_ASM_GOTO -D KBUILD_STR(s)=#s
// -D KBUILD_BASENAME=KBUILD_STR(bounds)
// -D KBUILD_MODNAME=KBUILD_STR(bounds)
// -isystem /home/senya/Source/builder/aarch64-linux-android-5.3-kernel/bin/../lib/gcc/aarch64-linux-android/5.3.1/include
// -include ../include/linux/kconfig.h -MD kernel/.bounds.s.d
// ../kernel/bounds.c -mlittle-endian -mgeneral-regs-only -mtune=cortex-a53
// -march=armv8-a -mabi=lp64 -auxbase-strip kernel/bounds.s -O2 -Os -Wall
// -Wundef -Wstrict-prototypes -Wno-trigraphs
// -Werror=implicit-function-declaration -Wno-format-security
// -Wno-maybe-uninitialized -Werror=frame-larger-than=1
// -Wframe-larger-than=1760 -Wno-unused-but-set-variable
// -Wdeclaration-after-statement -Wno-pointer-sign -Werror=implicit-int
// -Werror=strict-prototypes -Werror=date-time -std=gnu90
// -fno-strict-aliasing -fno-common -fno-pic -ftree-vectorize
// -fno-delete-null-pointer-checks -fstack-protector
// -fno-omit-frame-pointer -fno-optimize-sibling-calls
// -fno-var-tracking-assignments -fno-strict-overflow -fconserve-stack
// -fverbose-asm --param allow-store-data-races=0
// options enabled:  -faggressive-loop-optimizations -falign-functions
// -falign-jumps -falign-labels -falign-loops -fauto-inc-dec
// -fbranch-count-reg -fcaller-saves -fchkp-check-incomplete-type
// -fchkp-check-read -fchkp-check-write -fchkp-instrument-calls
// -fchkp-narrow-bounds -fchkp-optimize -fchkp-store-bounds
// -fchkp-use-static-bounds -fchkp-use-static-const-bounds
// -fchkp-use-wrappers -fcombine-stack-adjustments -fcompare-elim
// -fcprop-registers -fcrossjumping -fcse-follow-jumps -fdefer-pop
// -fdevirtualize -fdevirtualize-speculatively -fdwarf2-cfi-asm
// -fearly-inlining -feliminate-unused-debug-types
// -fexpensive-optimizations -fforward-propagate -ffunction-cse -fgcse
// -fgcse-lm -fgnu-runtime -fgnu-unique -fguess-branch-probability
// -fhoist-adjacent-loads -fident -fif-conversion -fif-conversion2
// -findirect-inlining -finline -finline-atomics -finline-functions
// -finline-functions-called-once -finline-small-functions -fipa-cp
// -fipa-cp-alignment -fipa-icf -fipa-icf-functions -fipa-icf-variables
// -fipa-profile -fipa-pure-const -fipa-ra -fipa-reference -fipa-sra
// -fira-hoist-pressure -fira-share-save-slots -fira-share-spill-slots
// -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
// -fleading-underscore -flifetime-dse -flra-remat -flto-odr-type-merging
// -fmath-errno -fmerge-constants -fmerge-debug-strings
// -fmove-loop-invariants -fomit-frame-pointer -fpartial-inlining
// -fpeephole -fpeephole2 -fprefetch-loop-arrays -free -freg-struct-return
// -freorder-blocks -freorder-functions -frerun-cse-after-loop
// -fsched-critical-path-heuristic -fsched-dep-count-heuristic
// -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
// -fsched-pressure -fsched-rank-heuristic -fsched-spec
// -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-fusion
// -fschedule-insns2 -fsection-anchors -fsemantic-interposition
// -fshow-column -fshrink-wrap -fsigned-zeros -fsplit-ivs-in-unroller
// -fsplit-wide-types -fssa-phiopt -fstack-protector -fstdarg-opt
// -fstrict-volatile-bitfields -fsync-libcalls -fthread-jumps
// -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
// -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
// -ftree-copy-prop -ftree-copyrename -ftree-cselim -ftree-dce
// -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
// -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
// -ftree-loop-optimize -ftree-loop-vectorize -ftree-parallelize-loops=
// -ftree-phiprop -ftree-pre -ftree-pta -ftree-reassoc -ftree-scev-cprop
// -ftree-sink -ftree-slp-vectorize -ftree-slsr -ftree-sra
// -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vectorize
// -ftree-vrp -funit-at-a-time -fverbose-asm -fzero-initialized-in-bss
// -mbionic -mfix-cortex-a53-835769 -mgeneral-regs-only -mlittle-endian
// -momit-leaf-frame-pointer

	.text
	.align	2
	.global	foo
	.type	foo, %function
foo:
#APP
// 18 "../kernel/bounds.c" 1
	
->NR_PAGEFLAGS 22 __NR_PAGEFLAGS	//
// 0 "" 2
// 19 "../kernel/bounds.c" 1
	
->MAX_NR_ZONES 3 __MAX_NR_ZONES	//
// 0 "" 2
// 21 "../kernel/bounds.c" 1
	
->NR_CPUS_BITS 3 ilog2(CONFIG_NR_CPUS)	//
// 0 "" 2
// 23 "../kernel/bounds.c" 1
	
->SPINLOCK_SIZE 4 sizeof(spinlock_t)	//
// 0 "" 2
#NO_APP
	ret
	.size	foo, .-foo
	.ident	"GCC: (UBERTC-5.3.x) 5.3.1 20160401 (optimized)"
	.section	.note.GNU-stack,"",%progbits
