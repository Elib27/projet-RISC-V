
###############################################
############### TEST ASSEMBLEUR ###############
###############################################


# add
add x0, x0, x0
add x20, x21, x22
add x31, x31, x31

# sub
sub x0, x0, x0
sub x3, x20, x30
sub x31, x31, x31

# addi
addi a1, zero, 0
addi x1, zero, 23
addi t6, x31, 2047
addi a2, zero, -2048

# ld
ld x0, 0(x1)
ld t5, 1024(x20)
ld x31, 2047(x31)
ld x31, -2048(x31)

# sd
sd x0, 0(zero)
sd x20, 1024(x20)
sd x31, 2047(x31)
sd x31, -2048(x31)

# beq
beq zero, zero, 0
beq x31, x31, 0
beq x10, x20, 200
beq a0, a1, 4094
beq a0, a1, -4096

# bne
bne zero, zero, 0
bne x20, x0, 20
bne x31, x31, 4094
bne x31, x31, -4096

# blt
blt zero, zero, 0
blt x20, x20, 20
blt x31, x31, 4094
blt x31, x31, -4096

# bge
bge zero, zero, 0
bge x20, x20, 20
bge x31, x31, 4094
bge x31, x31, -4096

# jal
jal x0, 0
jal x20, 200
jal x31, 1048574
jal x31, -1048576

# j
j 0
j 64
j 1048574
j -1048576

# li
li x0, 0
li x20, 300
li x0, 2047
li x31, 0
li x31, 2047

# mv
mv x0, x0
mv x20, x15
mv x31, x31


# test de reconnaissance de tous les registres

mv	x0	,	zero
mv	x1	,	ra
mv	x2	,	sp
mv	x3	,	gp
mv	x4	,	tp
mv	x5	,	t0
mv	x6	,	t1
mv	x7	,	t2
mv	x8	,	s0
mv	x9	,	s1
mv	x10	,	a0
mv	x11	,	a1
mv	x12	,	a2
mv	x13	,	a3
mv	x14	,	a4
mv	x15	,	a5
mv	x16	,	a6
mv	x17	,	a7
mv	x18	,	s2
mv	x19	,	s3
mv	x20	,	s4
mv	x21	,	s5
mv	x22	,	s6
mv	x23	,	s7
mv	x24	,	s8
mv	x25	,	s9
mv	x26	,	s10
mv	x27	,	s11
mv	x28	,	t3
mv	x29	,	t4
mv	x30	,	t5
mv	x31	,	t6
