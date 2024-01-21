# test de bge

addi a0, zero, 5
addi a1, zero, 3

add a2, a0, a1
sub a3, a0, a1

bge a2, a3, 24
addi a4, zero, 1
add a5, a2, a3
j 32
addi a4, zero, 2
add a5, a3, a2

# EXPECTED
# sp: 16384
# a0: 5
# a1: 3
# a2: 8
# a3: 2
# a4: 0
# a5: 0