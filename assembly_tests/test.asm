.global _start
.text
	add r9,r8,0x0
	cmp r10,r9,0x0
	je dalje
	ldc r2,0x1000
	ldc r3,0x54
	ldc r12,0x1004
	out r12,r7
	out r2,r3
	iret
dalje:
	ldc r7,0x1
	iret


.text
_start:

	ldc r5,0x3000
	ldc r6,0x40000000
	out r5,r6

	ldc r9, 0x0
	ldc r10,0x9

	ldc r7,0x0
	ldc r8,0x1

petlja:
	cmp r7,r8,0x0
	jne petlja
	ldc r6,0x0
	out r5,r6
	int 0xf
.end


