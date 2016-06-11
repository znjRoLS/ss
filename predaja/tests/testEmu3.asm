.public fact
.text.fact

fact:
    ldc r15, fact
    mul r1, r2
    sub r2, 1
    cmps r2, 0
    callne r15, 0
    ret
