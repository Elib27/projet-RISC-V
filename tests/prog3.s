
addi a0 , zero, 1

# empilage
bne a0, 10, 20
addi sp, sp, -8
sd a0, 0(sp)
addi a0, a0, 1
j -16

# depilage
addi a0, aO -1
bne a0, 0, 24
ld a1, 0(sp)
addi sp, sp 8
bne a0, a1, 8
j -20
addi a0, zero, -1




# EXPECTED