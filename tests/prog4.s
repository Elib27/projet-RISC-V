
li a0, 10
li a1, 1
li a2, 1
sub a0, a0, a1
bge a0, a1, -4
mv a3, a2
mv a4, a3

# EXPECTED
# a0: 0
# a1: 1
# a2: 1
# a3: 1
# a4: 1
# sp: 16384