# This assembly file is automatically generated. Do not modify it directly.
# The code generator is available at https://github.com/mupq/polymul-z2mx-m4
# - Matthias Kannwischer, Joost Rijneveld, and Peter Schwabe, 2018, Public Domain

.syntax unified
.cpu cortex-m4
.global schoolbook_12x12
.type schoolbook_12x12, %function
.align 2
schoolbook_12x12:
stmdb sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
ldr r3, [r1, #0]
ldr r4, [r1, #4]
ldr r5, [r1, #8]
ldr r6, [r1, #12]
ldr r7, [r1, #16]
ldr.w r8, [r1, #20]
push {r1}
ldr r1, [r2, #0]
ldr.w r9, [r2, #4]
ldr.w sl, [r2, #8]
ldr.w fp, [r2, #12]
ldr.w ip, [r2, #16]
ldr.w lr, [r2, #20]
push.w {r2}
smuadx r2, r3, r1
strh.w r2, [r0, #2]
smuadx r2, r3, r9
smladx r2, r4, r1, r2
strh.w r2, [r0, #6]
smuadx r2, r3, sl
smladx r2, r4, r9, r2
smladx r2, r5, r1, r2
strh.w r2, [r0, #10]
smuadx r2, r3, fp
smladx r2, r4, sl, r2
smladx r2, r5, r9, r2
smladx r2, r6, r1, r2
strh.w r2, [r0, #14]
smuadx r2, r3, ip
smladx r2, r4, fp, r2
smladx r2, r5, sl, r2
smladx r2, r6, r9, r2
smladx r2, r7, r1, r2
strh.w r2, [r0, #18]
smuadx r2, r3, lr
smladx r2, r4, ip, r2
smladx r2, r5, fp, r2
smladx r2, r6, sl, r2
smladx r2, r7, r9, r2
smladx r2, r8, r1, r2
strh.w r2, [r0, #22]
smuadx r2, r4, lr
smladx r2, r5, ip, r2
smladx r2, r6, fp, r2
smladx r2, r7, sl, r2
smladx r2, r8, r9, r2
strh.w r2, [r0, #26]
smuadx r2, r5, lr
smladx r2, r6, ip, r2
smladx r2, r7, fp, r2
smladx r2, r8, sl, r2
strh.w r2, [r0, #30]
smuadx r2, r6, lr
smladx r2, r7, ip, r2
smladx r2, r8, fp, r2
strh.w r2, [r0, #34]
smuadx r2, r7, lr
smladx r2, r8, ip, r2
strh.w r2, [r0, #38]
smuadx r2, r8, lr
strh.w r2, [r0, #42]
pkhbt r2, r9, r1
pkhbt r9, sl, r9
pkhbt sl, fp, sl
pkhbt fp, ip, fp
pkhbt ip, lr, ip
pkhbt r1, r1, lr
mul.w lr, r3, r1
strh.w lr, [r0]
smuad lr, r3, r2
mla lr, r4, r1, lr
strh.w lr, [r0, #4]
smuad lr, r3, r9
smlad lr, r4, r2, lr
mla lr, r5, r1, lr
strh.w lr, [r0, #8]
smuad lr, r3, sl
smlad lr, r4, r9, lr
smlad lr, r5, r2, lr
mla lr, r6, r1, lr
strh.w lr, [r0, #12]
smuad lr, r3, fp
smlad lr, r4, sl, lr
smlad lr, r5, r9, lr
smlad lr, r6, r2, lr
mla lr, r7, r1, lr
strh.w lr, [r0, #16]
smuad lr, r3, ip
smlad lr, r4, fp, lr
smlad lr, r5, sl, lr
smlad lr, r6, r9, lr
smlad lr, r7, r2, lr
mla lr, r8, r1, lr
strh.w lr, [r0, #20]
smultt lr, r3, r1
smlad lr, r4, ip, lr
smlad lr, r5, fp, lr
smlad lr, r6, sl, lr
smlad lr, r7, r9, lr
smlad lr, r8, r2, lr
strh.w lr, [r0, #24]
smultt lr, r4, r1
smlad lr, r5, ip, lr
smlad lr, r6, fp, lr
smlad lr, r7, sl, lr
smlad lr, r8, r9, lr
strh.w lr, [r0, #28]
smultt lr, r5, r1
smlad lr, r6, ip, lr
smlad lr, r7, fp, lr
smlad lr, r8, sl, lr
strh.w lr, [r0, #32]
smultt lr, r6, r1
smlad lr, r7, ip, lr
smlad lr, r8, fp, lr
strh.w lr, [r0, #36]
smultt lr, r7, r1
smlad lr, r8, ip, lr
strh.w lr, [r0, #40]
smultt lr, r8, r1
strh.w lr, [r0, #44]
pop {r2}
pop {r1}
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
mov r0, 0
bx lr
