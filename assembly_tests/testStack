.data
.int 0
a: .char 'F'
.char 'O'
.char 'O'
.text
.global main
main:
     mov r0, 0x1
     mov r1, a
     call equal
     add r0, 1, equal #B
     bne $t0, 42, foo  #C

equal:
     la $a0, TEXT
     jal printf #A
     mov pc, lr