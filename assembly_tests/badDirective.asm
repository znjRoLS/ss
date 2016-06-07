.public _start
.externn nesto123
.data
    a: .word 0x12345678
    t: .long 0x12343
    r: .char 34
.align
.skip 1
.text
_start:
    call r1, 0xffff
    call r2, 6

.data.ert
    b: .char 3
.text.ert
    add r1, r2
label1:
    ldch r2, 0x567
