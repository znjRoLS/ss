.public main
.extern testInstr
.data
    a: .word 0x12345678
.text
main:

    ldc r6, timerInterrupt
    ldc r7, 4
    str r6, r7, 0

    ldc r6, kbInterrupt
    ldc r7, 12
    str r6, r7, 0

    ldc r5, advance
    ldc r4, loop

loop:
    cmps r13, 1
    moveq pc, r5
    mov pc, r4

advance:
    in r2, r12
    add r2, 5
    ldc r4, testInstr
    call r4, 0
    add r1, 345
    halt

