.syntax unified
.cpu cortex-m4
.global schoolbook_16x16
.type schoolbook_16x16, %function
.align 2
schoolbook_16x16:
stmdb sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
ldr r6, [r1, #0]
ldr.w ip, [r1, #4]
ldr.w r3, [r1, #8]
ldr.w sl, [r1, #12]
ldr.w r7, [r2, #0]
ldr.w r8, [r2, #4]
ldr.w r4, [r2, #8]
ldr.w lr, [r2, #12]
smulbb r9, r7, r6
smuadx fp, r7, r6
pkhbt r9, r9, fp, lsl #16
str.w r9, [r0]
smuadx fp, r7, ip
smulbb r5, r7, ip
pkhbt r9, r8, r7
smladx fp, r8, r6, fp
smlad r5, r9, r6, r5
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #4]
smulbb r5, r3, r7
smuadx fp, r3, r7
smlad r5, r9, ip, r5
pkhbt r9, r4, r8
smlad r5, r9, r6, r5
smladx fp, r8, ip, fp
smladx fp, r4, r6, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #8]
smulbb r5, sl, r7
smuadx fp, sl, r7
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, r3, r9, r5
pkhbt r9, lr, r4
smlad r5, r6, r9, r5
smladx fp, r3, r8, fp
smladx fp, ip, r4, fp
smladx fp, r6, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #12]
smultt r5, r6, lr
smuadx fp, sl, r8
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, sl, r9, r5
pkhbt r9, r4, r8
smlad r5, r3, r9, r5
smladx fp, r3, r4, fp
smladx fp, ip, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #16]
smultt r5, ip, lr
smuadx fp, sl, r4
smlad r5, sl, r9, r5
pkhbt r9, lr, r4
smlad r5, r3, r9, r5
smladx fp, r3, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #20]
smuad r5, sl, r9
smuadx fp, sl, lr
smlatt r5, r3, lr, r5
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #24]
smultt fp, sl, lr
movt fp, #0
str.w fp, [r0, #28]
ldr.w r7, [r2, #16]
ldr.w r8, [r2, #20]
ldr.w r4, [r2, #24]
ldr.w lr, [r2, #28]
ldr.w r9, [r0, #16]
ldr.w r5, [r0, #20]
mov.w fp, r9, lsr #16
smlabb r9, r7, r6, r9
smladx fp, r7, r6, fp
pkhbt r9, r9, fp, lsl #16
str.w r9, [r0, #16]
mov.w fp, r5, lsr #16
smladx fp, r7, ip, fp
smlabb r5, r7, ip, r5
pkhbt r9, r8, r7
smladx fp, r8, r6, fp
smlad r5, r9, r6, r5
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #24]
str.w fp, [r0, #20]
mov.w fp, r5, lsr #16
smlabb r5, r3, r7, r5
smladx fp, r3, r7, fp
smlad r5, r9, ip, r5
pkhbt r9, r4, r8
smlad r5, r9, r6, r5
smladx fp, r8, ip, fp
smladx fp, r4, r6, fp
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #28]
str.w fp, [r0, #24]
mov.w fp, r5, lsr #16
smlabb r5, sl, r7, r5
smladx fp, sl, r7, fp
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, r3, r9, r5
pkhbt r9, lr, r4
smlad r5, r6, r9, r5
smladx fp, r3, r8, fp
smladx fp, ip, r4, fp
smladx fp, r6, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #28]
smultt r5, r6, lr
smuadx fp, sl, r8
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, sl, r9, r5
pkhbt r9, r4, r8
smlad r5, r3, r9, r5
smladx fp, r3, r4, fp
smladx fp, ip, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #32]
smultt r5, ip, lr
smuadx fp, sl, r4
smlad r5, sl, r9, r5
pkhbt r9, lr, r4
smlad r5, r3, r9, r5
smladx fp, r3, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #36]
smuad r5, sl, r9
smuadx fp, sl, lr
smlatt r5, r3, lr, r5
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #40]
smultt fp, sl, lr
movt fp, #0
str.w fp, [r0, #44]
ldr.w r6, [r1, #16]
ldr.w ip, [r1, #20]
ldr.w r3, [r1, #24]
ldr.w sl, [r1, #28]
ldr.w r9, [r0, #32]
ldr.w r5, [r0, #36]
mov.w fp, r9, lsr #16
smlabb r9, r7, r6, r9
smladx fp, r7, r6, fp
pkhbt r9, r9, fp, lsl #16
str.w r9, [r0, #32]
mov.w fp, r5, lsr #16
smladx fp, r7, ip, fp
smlabb r5, r7, ip, r5
pkhbt r9, r8, r7
smladx fp, r8, r6, fp
smlad r5, r9, r6, r5
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #40]
str.w fp, [r0, #36]
mov.w fp, r5, lsr #16
smlabb r5, r3, r7, r5
smladx fp, r3, r7, fp
smlad r5, r9, ip, r5
pkhbt r9, r4, r8
smlad r5, r9, r6, r5
smladx fp, r8, ip, fp
smladx fp, r4, r6, fp
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #44]
str.w fp, [r0, #40]
mov.w fp, r5, lsr #16
smlabb r5, sl, r7, r5
smladx fp, sl, r7, fp
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, r3, r9, r5
pkhbt r9, lr, r4
smlad r5, r6, r9, r5
smladx fp, r3, r8, fp
smladx fp, ip, r4, fp
smladx fp, r6, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #44]
smultt r5, r6, lr
smuadx fp, sl, r8
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, sl, r9, r5
pkhbt r9, r4, r8
smlad r5, r3, r9, r5
smladx fp, r3, r4, fp
smladx fp, ip, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #48]
smultt r5, ip, lr
smuadx fp, sl, r4
smlad r5, sl, r9, r5
pkhbt r9, lr, r4
smlad r5, r3, r9, r5
smladx fp, r3, lr, fp
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #52]
smuad r5, sl, r9
smuadx fp, sl, lr
smlatt r5, r3, lr, r5
pkhbt fp, r5, fp, lsl #16
str.w fp, [r0, #56]
smultt fp, sl, lr
strh.w fp, [r0, #60]
ldr.w r7, [r2, #0]
ldr.w r8, [r2, #4]
ldr.w r4, [r2, #8]
ldr.w lr, [r2, #12]
ldr.w r9, [r0, #16]
ldr.w r5, [r0, #20]
mov.w fp, r9, lsr #16
smlabb r9, r7, r6, r9
smladx fp, r7, r6, fp
pkhbt r9, r9, fp, lsl #16
str.w r9, [r0, #16]
mov.w fp, r5, lsr #16
smladx fp, r7, ip, fp
smlabb r5, r7, ip, r5
pkhbt r9, r8, r7
smladx fp, r8, r6, fp
smlad r5, r9, r6, r5
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #24]
str.w fp, [r0, #20]
mov.w fp, r5, lsr #16
smlabb r5, r3, r7, r5
smladx fp, r3, r7, fp
smlad r5, r9, ip, r5
pkhbt r9, r4, r8
smlad r5, r9, r6, r5
smladx fp, r8, ip, fp
smladx fp, r4, r6, fp
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #28]
str.w fp, [r0, #24]
mov.w fp, r5, lsr #16
smlabb r5, sl, r7, r5
smladx fp, sl, r7, fp
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, r3, r9, r5
pkhbt r9, lr, r4
smlad r5, r6, r9, r5
smladx fp, r3, r8, fp
smladx fp, ip, r4, fp
smladx fp, r6, lr, fp
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #32]
str.w fp, [r0, #28]
mov.w fp, r5, lsr #16
smlatt r5, r6, lr, r5
smladx fp, sl, r8, fp
smlad r5, ip, r9, r5
pkhbt r9, r8, r7
smlad r5, sl, r9, r5
pkhbt r9, r4, r8
smlad r5, r3, r9, r5
smladx fp, r3, r4, fp
smladx fp, ip, lr, fp
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #36]
str.w fp, [r0, #32]
mov.w fp, r5, lsr #16
smlatt r5, ip, lr, r5
smladx fp, sl, r4, fp
smlad r5, sl, r9, r5
pkhbt r9, lr, r4
smlad r5, r3, r9, r5
smladx fp, r3, lr, fp
pkhbt fp, r5, fp, lsl #16
ldr.w r5, [r0, #40]
str.w fp, [r0, #36]
mov.w fp, r5, lsr #16
smlad r5, sl, r9, r5
smladx fp, sl, lr, fp
smlatt r5, r3, lr, r5
pkhbt fp, r5, fp, lsl #16
ldrh.w r5, [r0, #44]
str.w fp, [r0, #40]
smlatt fp, sl, lr, r5
strh.w fp, [r0, #44]
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
mov r0, 0
bx lr
