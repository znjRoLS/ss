.public main
.extern sth123
.extern testInstr
.data
    a: .word 0x12345678
    t: .long 0x12343
    r: .char 34
.align
.skip 1
.align
.skip 0x6
.align
.align
sth: .long sth123
.bss.bbb

    aint: .skip 4
    achar: .skip 1
    aword: .skip 2
    .align

.text
main:
    ldc r4, nastavak
    mov pc, r4
    call r3, r-t

.data.ert
    b: .char 3

.text.ert
nastavak:
    add r1, r2
    ldc r4, testInstr
    call r4, 0
label1:
    ldch r2, 0x567



