
addi a0 , zero, 0
addi t0, zero, 10


# empilage
beq a0, t0, 20
addi sp, sp, -8
sd a0, 0(sp)
addi a0, a0, 1
j -12
#addi s0, zero, 1




# EXPECTED
# sp : 16384
# a0 : 0
# t0 : 10
# a1 : 0
