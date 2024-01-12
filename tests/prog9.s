li a0, 5
li a1, 2
li a2, 3
sub a0, a0, a1
bge a0, a2, -4
mv a3, a2
mv a4, a3

# EXPECTED
# a0: 1
# a1: 2
# a2: 3
# a3: 3
# a4: 3
# sp: 16384