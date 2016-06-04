.public _start
.data
    a: .word 0x12345678
.text
_start:
    call r1, 5
    call r2, 6
    add r2,r1
    add r2,0x12345678
    add r2,0xffffffff
    addeq r2,r1
    addeqs r2,r1