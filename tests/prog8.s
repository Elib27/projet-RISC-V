addi a0, zero, 0
addi t0, zero, 5
beq a0, t0, 20
addi sp, sp, -8
sd a0, 0(sp)
addi a0, a0, 1
j -16

# EXPECTED
# sp: 16344
# a0: 5
# t0: 5