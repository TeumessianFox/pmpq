# This assembly file is automatically generated. Do not modify it directly.
# The code generator is available at https://github.com/mupq/polymul-z2mx-m4
# - Matthias Kannwischer, Joost Rijneveld, and Peter Schwabe, 2018, Public Domain

.syntax unified
.cpu cortex-m4
.global schoolbook_24x24
.type schoolbook_24x24, %function
.align 2
schoolbook_24x24:
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
mov.w lr, r1
pkhbt r1, r1, r2
pkhbt r2, r2, r9
pkhbt r9, r9, sl
pkhbt sl, sl, fp
pkhbt fp, fp, ip
pkhbt ip, ip, lr
ldr.w lr, [sp, #4]
ldr.w r3, [lr, #24]
ldr.w r4, [lr, #28]
ldr.w r5, [lr, #32]
ldr.w r6, [lr, #36]
ldr.w r7, [lr, #40]
ldr.w r8, [lr, #44]
str.w lr, [sp, #4]
ldrh.w lr, [r0, #26]
smladx lr, r3, r1, lr
strh.w lr, [r0, #26]
ldrh.w lr, [r0, #30]
smladx lr, r3, r2, lr
smladx lr, r4, r1, lr
strh.w lr, [r0, #30]
ldrh.w lr, [r0, #34]
smladx lr, r3, r9, lr
smladx lr, r4, r2, lr
smladx lr, r5, r1, lr
strh.w lr, [r0, #34]
ldrh.w lr, [r0, #38]
smladx lr, r3, sl, lr
smladx lr, r4, r9, lr
smladx lr, r5, r2, lr
smladx lr, r6, r1, lr
strh.w lr, [r0, #38]
ldrh.w lr, [r0, #42]
smladx lr, r3, fp, lr
smladx lr, r4, sl, lr
smladx lr, r5, r9, lr
smladx lr, r6, r2, lr
smladx lr, r7, r1, lr
strh.w lr, [r0, #42]
smuadx lr, r3, ip
smladx lr, r4, fp, lr
smladx lr, r5, sl, lr
smladx lr, r6, r9, lr
smladx lr, r7, r2, lr
smladx lr, r8, r1, lr
strh.w lr, [r0, #46]
smuadx lr, r4, ip
smladx lr, r5, fp, lr
smladx lr, r6, sl, lr
smladx lr, r7, r9, lr
smladx lr, r8, r2, lr
strh.w lr, [r0, #50]
smuadx lr, r5, ip
smladx lr, r6, fp, lr
smladx lr, r7, sl, lr
smladx lr, r8, r9, lr
strh.w lr, [r0, #54]
smuadx lr, r6, ip
smladx lr, r7, fp, lr
smladx lr, r8, sl, lr
strh.w lr, [r0, #58]
smuadx lr, r7, ip
smladx lr, r8, fp, lr
strh.w lr, [r0, #62]
smuadx lr, r8, ip
strh.w lr, [r0, #66]
pkhbt lr, r2, r1
pkhbt r2, r9, r2
pkhbt r9, sl, r9
pkhbt sl, fp, sl
pkhbt fp, ip, fp
pkhbt r1, r1, ip
ldrh.w ip, [r0, #24]
mla ip, r3, r1, ip
strh.w ip, [r0, #24]
ldrh.w ip, [r0, #28]
smlad ip, r3, lr, ip
mla ip, r4, r1, ip
strh.w ip, [r0, #28]
ldrh.w ip, [r0, #32]
smlad ip, r3, r2, ip
smlad ip, r4, lr, ip
mla ip, r5, r1, ip
strh.w ip, [r0, #32]
ldrh.w ip, [r0, #36]
smlad ip, r3, r9, ip
smlad ip, r4, r2, ip
smlad ip, r5, lr, ip
mla ip, r6, r1, ip
strh.w ip, [r0, #36]
ldrh.w ip, [r0, #40]
smlad ip, r3, sl, ip
smlad ip, r4, r9, ip
smlad ip, r5, r2, ip
smlad ip, r6, lr, ip
mla ip, r7, r1, ip
strh.w ip, [r0, #40]
ldrh.w ip, [r0, #44]
smlad ip, r3, fp, ip
smlad ip, r4, sl, ip
smlad ip, r5, r9, ip
smlad ip, r6, r2, ip
smlad ip, r7, lr, ip
mla ip, r8, r1, ip
strh.w ip, [r0, #44]
smultt ip, r3, r1
smlad ip, r4, fp, ip
smlad ip, r5, sl, ip
smlad ip, r6, r9, ip
smlad ip, r7, r2, ip
smlad ip, r8, lr, ip
strh.w ip, [r0, #48]
smultt ip, r4, r1
smlad ip, r5, fp, ip
smlad ip, r6, sl, ip
smlad ip, r7, r9, ip
smlad ip, r8, r2, ip
strh.w ip, [r0, #52]
smultt ip, r5, r1
smlad ip, r6, fp, ip
smlad ip, r7, sl, ip
smlad ip, r8, r9, ip
strh.w ip, [r0, #56]
smultt ip, r6, r1
smlad ip, r7, fp, ip
smlad ip, r8, sl, ip
strh.w ip, [r0, #60]
smultt ip, r7, r1
smlad ip, r8, fp, ip
strh.w ip, [r0, #64]
smultt ip, r8, r1
strh.w ip, [r0, #68]
ldr.w ip, [sp]
ldr.w sl, [ip, #24]
ldr.w lr, [ip, #28]
ldr.w r9, [ip, #32]
ldr.w r1, [ip, #36]
ldr.w fp, [ip, #40]
ldr.w r2, [ip, #44]
str.w ip, [sp]
ldrh.w ip, [r0, #50]
smladx ip, r3, sl, ip
strh.w ip, [r0, #50]
ldrh.w ip, [r0, #54]
smladx ip, r3, lr, ip
smladx ip, r4, sl, ip
strh.w ip, [r0, #54]
ldrh.w ip, [r0, #58]
smladx ip, r3, r9, ip
smladx ip, r4, lr, ip
smladx ip, r5, sl, ip
strh.w ip, [r0, #58]
ldrh.w ip, [r0, #62]
smladx ip, r3, r1, ip
smladx ip, r4, r9, ip
smladx ip, r5, lr, ip
smladx ip, r6, sl, ip
strh.w ip, [r0, #62]
ldrh.w ip, [r0, #66]
smladx ip, r3, fp, ip
smladx ip, r4, r1, ip
smladx ip, r5, r9, ip
smladx ip, r6, lr, ip
smladx ip, r7, sl, ip
strh.w ip, [r0, #66]
smuadx ip, r3, r2
smladx ip, r4, fp, ip
smladx ip, r5, r1, ip
smladx ip, r6, r9, ip
smladx ip, r7, lr, ip
smladx ip, r8, sl, ip
strh.w ip, [r0, #70]
smuadx ip, r4, r2
smladx ip, r5, fp, ip
smladx ip, r6, r1, ip
smladx ip, r7, r9, ip
smladx ip, r8, lr, ip
strh.w ip, [r0, #74]
smuadx ip, r5, r2
smladx ip, r6, fp, ip
smladx ip, r7, r1, ip
smladx ip, r8, r9, ip
strh.w ip, [r0, #78]
smuadx ip, r6, r2
smladx ip, r7, fp, ip
smladx ip, r8, r1, ip
strh.w ip, [r0, #82]
smuadx ip, r7, r2
smladx ip, r8, fp, ip
strh.w ip, [r0, #86]
smuadx ip, r8, r2
strh.w ip, [r0, #90]
pkhbt ip, lr, sl
pkhbt lr, r9, lr
pkhbt r9, r1, r9
pkhbt r1, fp, r1
pkhbt fp, r2, fp
pkhbt sl, sl, r2
ldrh.w r2, [r0, #48]
mla r2, r3, sl, r2
strh r2, [r0, #48]
ldrh r2, [r0, #52]
smlad r2, r3, ip, r2
mla r2, r4, sl, r2
strh r2, [r0, #52]
ldrh r2, [r0, #56]
smlad r2, r3, lr, r2
smlad r2, r4, ip, r2
mla r2, r5, sl, r2
strh r2, [r0, #56]
ldrh r2, [r0, #60]
smlad r2, r3, r9, r2
smlad r2, r4, lr, r2
smlad r2, r5, ip, r2
mla r2, r6, sl, r2
strh.w r2, [r0, #60]
ldrh.w r2, [r0, #64]
smlad r2, r3, r1, r2
smlad r2, r4, r9, r2
smlad r2, r5, lr, r2
smlad r2, r6, ip, r2
mla r2, r7, sl, r2
strh.w r2, [r0, #64]
ldrh.w r2, [r0, #68]
smlad r2, r3, fp, r2
smlad r2, r4, r1, r2
smlad r2, r5, r9, r2
smlad r2, r6, lr, r2
smlad r2, r7, ip, r2
mla r2, r8, sl, r2
strh.w r2, [r0, #68]
smultt r2, r3, sl
smlad r2, r4, fp, r2
smlad r2, r5, r1, r2
smlad r2, r6, r9, r2
smlad r2, r7, lr, r2
smlad r2, r8, ip, r2
strh.w r2, [r0, #72]
smultt r2, r4, sl
smlad r2, r5, fp, r2
smlad r2, r6, r1, r2
smlad r2, r7, r9, r2
smlad r2, r8, lr, r2
strh.w r2, [r0, #76]
smultt r2, r5, sl
smlad r2, r6, fp, r2
smlad r2, r7, r1, r2
smlad r2, r8, r9, r2
strh.w r2, [r0, #80]
smultt r2, r6, sl
smlad r2, r7, fp, r2
smlad r2, r8, r1, r2
strh.w r2, [r0, #84]
smultt r2, r7, sl
smlad r2, r8, fp, r2
strh.w r2, [r0, #88]
smultt r2, r8, sl
strh.w r2, [r0, #92]
mov.w r2, sl
pkhbt sl, sl, ip
pkhbt ip, ip, lr
pkhbt lr, lr, r9
pkhbt r9, r9, r1
pkhbt r1, r1, fp
pkhbt fp, fp, r2
ldr r2, [sp, #4]
ldr r3, [r2, #0]
ldr r4, [r2, #4]
ldr r5, [r2, #8]
ldr r6, [r2, #12]
ldr r7, [r2, #16]
ldr.w r8, [r2, #20]
str r2, [sp, #4]
ldrh r2, [r0, #26]
smladx r2, r3, sl, r2
strh r2, [r0, #26]
ldrh r2, [r0, #30]
smladx r2, r3, ip, r2
smladx r2, r4, sl, r2
strh r2, [r0, #30]
ldrh r2, [r0, #34]
smladx r2, r3, lr, r2
smladx r2, r4, ip, r2
smladx r2, r5, sl, r2
strh r2, [r0, #34]
ldrh r2, [r0, #38]
smladx r2, r3, r9, r2
smladx r2, r4, lr, r2
smladx r2, r5, ip, r2
smladx r2, r6, sl, r2
strh r2, [r0, #38]
ldrh r2, [r0, #42]
smladx r2, r3, r1, r2
smladx r2, r4, r9, r2
smladx r2, r5, lr, r2
smladx r2, r6, ip, r2
smladx r2, r7, sl, r2
strh r2, [r0, #42]
ldrh r2, [r0, #46]
smladx r2, r3, fp, r2
smladx r2, r4, r1, r2
smladx r2, r5, r9, r2
smladx r2, r6, lr, r2
smladx r2, r7, ip, r2
smladx r2, r8, sl, r2
strh r2, [r0, #46]
ldrh r2, [r0, #50]
smladx r2, r4, fp, r2
smladx r2, r5, r1, r2
smladx r2, r6, r9, r2
smladx r2, r7, lr, r2
smladx r2, r8, ip, r2
strh r2, [r0, #50]
ldrh r2, [r0, #54]
smladx r2, r5, fp, r2
smladx r2, r6, r1, r2
smladx r2, r7, r9, r2
smladx r2, r8, lr, r2
strh r2, [r0, #54]
ldrh r2, [r0, #58]
smladx r2, r6, fp, r2
smladx r2, r7, r1, r2
smladx r2, r8, r9, r2
strh r2, [r0, #58]
ldrh r2, [r0, #62]
smladx r2, r7, fp, r2
smladx r2, r8, r1, r2
strh.w r2, [r0, #62]
ldrh.w r2, [r0, #66]
smladx r2, r8, fp, r2
strh.w r2, [r0, #66]
pkhbt r2, ip, sl
pkhbt ip, lr, ip
pkhbt lr, r9, lr
pkhbt r9, r1, r9
pkhbt r1, fp, r1
pkhbt sl, sl, fp
ldrh.w fp, [r0, #24]
mla fp, r3, sl, fp
strh.w fp, [r0, #24]
ldrh.w fp, [r0, #28]
smlad fp, r3, r2, fp
mla fp, r4, sl, fp
strh.w fp, [r0, #28]
ldrh.w fp, [r0, #32]
smlad fp, r3, ip, fp
smlad fp, r4, r2, fp
mla fp, r5, sl, fp
strh.w fp, [r0, #32]
ldrh.w fp, [r0, #36]
smlad fp, r3, lr, fp
smlad fp, r4, ip, fp
smlad fp, r5, r2, fp
mla fp, r6, sl, fp
strh.w fp, [r0, #36]
ldrh.w fp, [r0, #40]
smlad fp, r3, r9, fp
smlad fp, r4, lr, fp
smlad fp, r5, ip, fp
smlad fp, r6, r2, fp
mla fp, r7, sl, fp
strh.w fp, [r0, #40]
ldrh.w fp, [r0, #44]
smlad fp, r3, r1, fp
smlad fp, r4, r9, fp
smlad fp, r5, lr, fp
smlad fp, r6, ip, fp
smlad fp, r7, r2, fp
mla fp, r8, sl, fp
strh.w fp, [r0, #44]
ldrh.w fp, [r0, #48]
smlatt fp, r3, sl, fp
smlad fp, r4, r1, fp
smlad fp, r5, r9, fp
smlad fp, r6, lr, fp
smlad fp, r7, ip, fp
smlad fp, r8, r2, fp
strh.w fp, [r0, #48]
ldrh.w fp, [r0, #52]
smlatt fp, r4, sl, fp
smlad fp, r5, r1, fp
smlad fp, r6, r9, fp
smlad fp, r7, lr, fp
smlad fp, r8, ip, fp
strh.w fp, [r0, #52]
ldrh.w fp, [r0, #56]
smlatt fp, r5, sl, fp
smlad fp, r6, r1, fp
smlad fp, r7, r9, fp
smlad fp, r8, lr, fp
strh.w fp, [r0, #56]
ldrh.w fp, [r0, #60]
smlatt fp, r6, sl, fp
smlad fp, r7, r1, fp
smlad fp, r8, r9, fp
strh.w fp, [r0, #60]
ldrh.w fp, [r0, #64]
smlatt fp, r7, sl, fp
smlad fp, r8, r1, fp
strh.w fp, [r0, #64]
ldrh.w fp, [r0, #68]
smlatt fp, r8, sl, fp
strh.w fp, [r0, #68]
pop {r2}
pop {r1}
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
mov r0, 0
bx lr
