.option push
.option norvc
.option norelax

// This program quickly takes an illegal instruction trap into a handler which
// itself immediately traps. This puts the core into a loop where it will
// continuously except, without ever retiring an instruction.
//
// The test harness repeatedly halts and resumes the core, and ensures that
// DPC only ever points to the trap handler, not the unreachable next
// instruction.

.p2align 2

.global main
main:
	la a0, bad_trap_handler
	csrw mtvec, a0
	unimp

.global unreachable1
unreachable1:
	j unreachable1

// Should loop here without retiring an instruction. If the debugger samples a
// bad DPC value, it will end up in one of the unreachable loops above and
// below.
.global bad_trap_handler
bad_trap_handler:
	unimp

.global unreachable2
unreachable2:
	j unreachable2

.option pop
