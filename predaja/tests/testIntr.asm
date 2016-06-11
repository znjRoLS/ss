.public main
.text.inf
main:
    ldc r4, loop
    ldc r5, progEnd

    ldc r6, timerInterrupt
    ldc r7, 4
    str r6, r7, 0

    ldc r6, kbInterrupt
    ldc r7, 12
    str r6, r7, 0

loop:
    add r3, 1
    cmps r12, 10
    moveq pc, r5
    mov pc, r4

progEnd:
    halt

timerInterrupt:
    add r12, 1
    iret

kbInterrupt:
    add r13, 1
    ldc r8, 0x1000
    ldc r9, 0x2000

    ldr r7, r8, 0
    str r7, r9, 0
    str r7, r9, 0
    iret