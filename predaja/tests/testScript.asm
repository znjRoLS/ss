.public main
.extern testInstr
.data
    a: .word 0x12345678
    t: .long 0x12343
    r: .char 34
.text
main:
    ldc r4, nastavak
    mov pc, r4
.data.ert
    b: .char 3
    .align
.text.ert
nastavak:
    add r1, r2
    ldc r4, testInstr
    halt


