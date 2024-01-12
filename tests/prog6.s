addi s0, zero, 2
addi a0, zero, 3
sub a0, a0, s0
addi s0, s0, 1
add a0, a0, s0
mv a1, a0
li a2, 1
add a1, a1, a2
mv a2, a1
sub a0, a0, a2

# EXPECTED
# s0: 3
# a0: -1
# a1: 5
# a2: 5
# sp: 16384