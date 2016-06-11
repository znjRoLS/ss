.public main
.data
    a: .word 0x12345678
.text
main:
    ldch r1, 0
    ldcl r1, 0
    add r1, 0x1234
    sub r1, 0x1230
    call r4, 0
    add r1, 345
    halt
.bss
    aa: .skip 4
    bb: .skip 2
    .align