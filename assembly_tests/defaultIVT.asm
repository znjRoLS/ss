.text.start
    ldc r10, 0x12344321
    iret

.text.timer
    add r11, 1
    iret

.text.illegal
    ldc r12, 1
    iret

.text.keyboard
    add r13, 1
    ldc r8, 0x1000
    ldc r9, 0x2000

    ldr r7, r8, 0
    str r7, r9, 0

    iret