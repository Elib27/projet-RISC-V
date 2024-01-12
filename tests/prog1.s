# tests fonction add

addi s0, zero, 1
addi a0, zero, 1
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0
add a0, a0, s0

# EXPECTED
# s0: 1
# a0: 10
# sp: 16384