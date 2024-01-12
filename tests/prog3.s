
addi a0 , zero, 0
addi t0, zero, 10


# empilage
beq a0, t0, 20
addi sp, sp, -8
sd a0, 0(sp) #pb
addi a0, a0, 1
j -16
#addi s0, zero, 1




# EXPECTED
# sp : 16304
# a0 : 10
# t0 : 10
# a1 : 0
