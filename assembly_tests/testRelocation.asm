.public main
.extern nesto123
.data
    a: .word 0x12345678
    t: .long 0x12343
    r: .char 34
    tt: .long 0x12343
    ttt: .long 0x12343
    tttt: .long 0x12343
.align
.skip 1
.align
.long nesto123
.text
main:
    call r1, r-t
    call r2, 6

.data.ert
    b: .char 3
.text.ert
    add r1, r2
label1:
    ldch r2, 0x567
