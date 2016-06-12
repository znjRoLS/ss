.public fact
.text.fact

fact:
    mul r1, r2
    sub r2, 1
    cmps r2, 0
    callne r5, 0
    ret
