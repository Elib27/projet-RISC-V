addi s0, zero, 3
addi a0, zero, 0
addi t1, zero, 0
addi s1, zero, 8
beq t1, s1, 16
add a0, a0, s0
addi t1, t1, 1
j -12

# EXPECTED
# s0: 3
# t1: 8
# a0: 24
# s1: 8
# sp: 16384