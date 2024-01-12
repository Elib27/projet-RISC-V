# test fonction add, addi, beq et j

addi s0, zero, 2

addi a0, zero, 0

#compteur
addi t1, zero, 0

# constant
addi s1, zero, 10

beq t1, s1, 16
add a0, a0, s0
addi t1, t1, 1
j -12


# EXPECTED
# s0: 2
# t1: 10
# a0: 20
# s1: 10
# sp: 16384
