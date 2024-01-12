
li a0, 10
li a1, 1
li a2, 2
sub a0, a0, a1
bge a0, a1, -4
# mv a3, a2
# addi a3, a2, 0
# addi a4, a3, 0
# mv a4, a3

# EXPECTED
# a0: 1
# a1: 1
# a2: 1
# sp: 16384