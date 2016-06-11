.public main
.text.inf
main:
    ldc r4, loop
    ldc r5, progEnd
loop:
    add r3, 1
    cmps r11, 10
    moveq pc, r5
    mov pc, r4
progEnd:
    halt