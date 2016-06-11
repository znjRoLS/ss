.public main
.text
main:
    ldc r2, 50
    sub r2, 1
    ldc r7, 0x2000

    str r2, r7, 0
    halt