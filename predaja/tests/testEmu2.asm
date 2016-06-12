.public testInstr
.public timerInterrupt
.public kbInterrupt
.extern fact

.text.neki

testInstr:

    ldc r6, wait
    ldc r5, calc
    ldc r4, ispis
    ldc r3, kraj

wait:
    cmps r13, 1
    moveq pc, r5
    mov pc, r6

calc:

    ldc r6, ispis2

    ldc r1, 1
    in r2, r7
    sub r2, 0x30
    ldc r5, fact
    call r5,0

    ldc r10, 0x2020

ispis:
    cmps r1, 0
    moveq pc, r6

    in r5, r1
    div r5, 10
    mul r5, 10

    in r2, r1
    sub r2, r5
    add r2, 0x30
    str r2, r10++, 0

    div r1, 10

    mov pc, r4

ispis2:

    and r10, r10
    cmps r10, 0x2020
    movle pc, r3

    ldr r2, --r10, 0
    str r2, r9, 0
    mov pc, r6

kraj:
    ret

.text.intr
timerInterrupt:
   add r12, 1
   iret

kbInterrupt:
   cmps r13, 1
   addne r13, 1

   ldrne r7, r8, 0

   iret