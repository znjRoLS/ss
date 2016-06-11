.public main
.text.inf
main:
    add r2, 1
    ldc r3, 0
    ldc r4, loop
    ldc r5, progEnd
loop:
    add r3, 1
    cmps r11, 10
    moveq pc, r5
    mov pc, r4
progEnd:
    halt