.public main
.data.inf
    output: .long 0x2000
    tmrIntrOut: .long 4
    kbIntrOut: .long 12
    kbOutput: .long 0x2000
    kbInput: .long 0x1000
.text.inf
main:
    ldc r4, loop
    ldc r5, progEnd

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


    int 1
    int 1
    int 1
    int 1
    int 1

loop:
    add r3, 1
    cmps r12, 10
    moveq pc, r5
    mov pc, r4

progEnd:
    halt

timerInterrupt:
    add r12, 1
    in r7, r12
    add r7, 0x30

    str r7, r9, 0

    iret

kbInterrupt:
    add r13, 1

    ldr r7, r8, 0
    str r7, r9, 0
    str r7, r9, 0
    iret