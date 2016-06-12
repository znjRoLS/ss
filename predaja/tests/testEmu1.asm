.public main
.extern testInstr
.extern timerInterrupt
.extern kbInterrupt
.data
    a: .word 0x12345678
.align
.data.intrlabele
    output: .long 0x2000
    tmrIntrOut: .long 4
    kbIntrOut: .long 12
    kbOutput: .long 0x2000
    kbInput: .long 0x1000
.text
main:

    ldc r6, timerInterrupt
    ldc r7, tmrIntrOut
    ldr r7, r7, 0
    str r6, r7, 0

    ldc r6, kbInterrupt
    ldc r7, kbIntrOut
    ldr r7, r7, 0
    str r6, r7, 0

    ldc r9, kbOutput
    ldr r9, r9, 0
    ldc r8, kbInput
    ldr r8, r8, 0

    ldc r5, advance
    ldc r4, loop

loop:
    cmps r13, 1
    moveq pc, r5
    mov pc, r4

advance:
    in r2, r12
    add r2, 3
    ldc r4, loop2
    ldc r5, advance2

loop2:
    cmps r2, r12
    movle pc, r5
    mov pc, r4

advance2:
    ldc r4, testInstr
    call r4, 0

    halt

