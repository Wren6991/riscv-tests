#ifndef INIT_H
#define INIT_H

#ifdef CLINT
// #define MTIME           (*(volatile long long *)(CLINT + 0xbff8))
// #define MTIMECMP        ((volatile long long *)(CLINT + 0x4000))
// different hardcoded values for hazard3 tb
#define MTIME           (*(volatile long long *)(0x80000100))
#define MTIMECMP        ((volatile long long *)(0x80000108))
#endif

typedef void* (*trap_handler_t)(unsigned hartid, unsigned mcause, void *mepc,
        void *sp);
void set_trap_handler(trap_handler_t handler);
void enable_timer_interrupts();

#endif
