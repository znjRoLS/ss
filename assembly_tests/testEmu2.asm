.public testInstr
.public timerInterrupt
.public kbInterrupt
.extern fact
.text.neki

testInstr:

    ldc r6, wait
    ldc r5, calc

wait:
    cmps r13, 1
    moveq pc, r5
    mov pc, r6

calc:

    ldc r1, 1
    ldc r2,5
    ldc r5, fact

    call r5,0

kraj:
    str r7, r14,0
    ret

.text.intr
timerInterrupt:
       add r12, 1
       iret

   kbInterrupt:
       cmps r13, 1
       addne r13, 1
       ldc r8, 0x1000

       ldrne r7, r8, 0
       subne

       iret