.public sth123
.public testInstr
.data
sth123: .long 0x87654321
.text.neki
testInstr:
    and r1,r2
    ret