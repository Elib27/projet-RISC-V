addi a1, zero, 20
addi a2, zero, 22
add a0, a1, a2
sd a0, 8(a1)
beq a0, a1, 22
beq a0, a1, 32
blt t1, t2, 12
jal x0, 22
j 16


li x0, 0
li x0, 2047
li x31, 0
li x31, 2047



mv x0, x0
mv x31, x31




# EXPECTED
# sp: 16384
# a0: 42
# a1: 20
# a2: 22
