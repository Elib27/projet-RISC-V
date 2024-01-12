# test fonction addi, bne, ld, sd, j
addi a0 , zero, 0
addi t0, zero, 10


# empilage
beq a0, t0, 20
addi sp, sp, -8
sd a0, 0(sp)
addi a0, a0, 1
j -16
#addi s0, zero, 1

# depilage
addi a0, a0, -1
blt a0, zero, 32
ld a1, 0(sp)
addi sp, sp, 8
bne a0, a1, 16 # différents
addi a0, a0, -1
addi a1, a1, -1
j -24
addi a0, zero, -11


# EXPECTED
# sp : 16384
# a0 : 0
# t0 : 10
# a1 : 0
