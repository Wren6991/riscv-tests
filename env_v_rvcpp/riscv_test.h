// See LICENSE for license details.

#ifndef _ENV_VIRTUAL_SINGLE_CORE_H
#define _ENV_VIRTUAL_SINGLE_CORE_H

#include "../env/p/riscv_test.h"

//-----------------------------------------------------------------------
// Begin Macro
//-----------------------------------------------------------------------

#undef RVTEST_FP_ENABLE
#define RVTEST_FP_ENABLE fssr x0

#undef RVTEST_VECTOR_ENABLE
#define RVTEST_VECTOR_ENABLE                                            \
  csrwi fcsr, 0;                                                        \
  csrwi vcsr, 0;

#undef RVTEST_CODE_BEGIN
#define RVTEST_CODE_BEGIN                                               \
        .text;                                                          \
        .global extra_boot;                                             \
extra_boot:                                                             \
        EXTRA_INIT                                                      \
        ret;                                                            \
.global trap_filter;                                                    \
trap_filter:                                                            \
        FILTER_TRAP                                                     \
        li a0, 0;                                                       \
        ret;                                                            \
.global pf_filter;                                                      \
pf_filter:                                                              \
        FILTER_PAGE_FAULT                                               \
        li a0, 0;                                                       \
        ret;                                                            \
        .global userstart;                                              \
userstart:                                                              \
        init

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#undef RVTEST_PASS
#undef RVTEST_FAIL
// Hazard3 testbench IO addresses:

#define IO_BASE 0xe0000000
#define IO_PRINT_CHAR (IO_BASE + 0x0)
#define IO_PRINT_U32  (IO_BASE + 0x4)
#define IO_EXIT       (IO_BASE + 0x8)

#define RVTEST_PASS                                                     \
        fence;                                                          \
        li TESTNUM, 1;                                                  \
        li a0, IO_EXIT;                                                 \
        sw zero, (a0)                                                   \
        // li a7, 93;                                                   \
        // li a0, 0;                                                    \
        // ecall

#define TESTNUM gp
#define RVTEST_FAIL                                                     \
        fence;                                                          \
1:      beqz TESTNUM, 1b;                                               \
        sll TESTNUM, TESTNUM, 1;                                        \
        or TESTNUM, TESTNUM, 1;                                         \
        li a0, IO_EXIT;                                                 \
        sw TESTNUM, (a0)                                                \
        // li a7, 93;                                                   \
        // addi a0, TESTNUM, 0;                                         \
        // ecall

//-----------------------------------------------------------------------
// Data Section Macro
//-----------------------------------------------------------------------

#undef RVTEST_DATA_END
#define RVTEST_DATA_END

//-----------------------------------------------------------------------
// Supervisor mode definitions and macros
//-----------------------------------------------------------------------

#define MAX_TEST_PAGES 63 // this must be the period of the LFSR below
#define LFSR_NEXT(x) (((((x)^((x)>>1)) & 1) << 5) | ((x) >> 1))

#define PGSHIFT 12
#define PGSIZE (1UL << PGSHIFT)

#define SIZEOF_TRAPFRAME_T ((__riscv_xlen / 8) * 36)

#ifndef __ASSEMBLER__

typedef unsigned long pte_t;
#define LEVELS (sizeof(pte_t) == sizeof(uint64_t) ? 3 : 2)
#define PTIDXBITS (PGSHIFT - (sizeof(pte_t) == 8 ? 3 : 2))
#define VPN_BITS (PTIDXBITS * LEVELS)
#define VA_BITS (VPN_BITS + PGSHIFT)
#define PTES_PER_PT (1UL << RISCV_PGLEVEL_BITS)
#define MEGAPAGE_SIZE (PTES_PER_PT * PGSIZE)

typedef struct
{
  long gpr[32];
  long sr;
  long epc;
  long badvaddr;
  long cause;
} trapframe_t;
#endif

#endif
