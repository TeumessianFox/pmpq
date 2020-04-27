.syntax unified
.cpu cortex-m4
.global asm_testing
.type asm_testing, %function
.align 2
asm_testing:
stmdb sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
mov r0, 0
bx lr
