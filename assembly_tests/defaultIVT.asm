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
    ldc r13, 1
    iret