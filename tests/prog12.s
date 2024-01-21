# test jal

addi x1, zero, 100
addi x2, zero, 200
add t0, x1, x2
jal x3, 8
add t0, t0, x1

# EXPECTED
# x1: 100
# x2: 200
# x3: 16
# t0: 300