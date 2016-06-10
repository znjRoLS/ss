.public main
.extern testInstr
.data
    a: .word 0x12345678
.text
main:
    ldch r1, 0
    ldcl r1, 0
    add r1, 0x1234
    sub r1, 0x1230
    ldc r4, testInstr
    call r4, 0
    add r1, 345