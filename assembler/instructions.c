#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


#include "main.h"

/*
    @brief transform R type instruction in binary representation
*/
uint32_t operationR(char *instruction, int op1, int op2, int op3) {

    uint32_t funct7 = 0;
    uint32_t rs2 = op3;
    uint32_t rs1 = op2;
    uint32_t funct3 = 0;
    uint32_t rd = op1;
    uint32_t opcode = 0b0110011;

    if (!strcmp(instruction, "sub")) {
        funct7 = 0x20;
    }

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst |= funct7;
    printf("inst : %08x \n", funct7);
    inst <<= 5;
    inst |= rs2;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    printf("inst : %08x \n", inst);
    inst |= rs1;
    printf("inst : %08x \n", inst);
    inst <<= 3;
    inst |= funct3;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    inst |= rd;
    printf("inst : %08x \n", inst);
    inst <<= 7;
    inst |= opcode;
    printf("inst : %08x \n", inst);
    printf("instruction finale : 0b%d \n", inst);
    return inst;
}

/*
    @brief transform I type instruction in binary representation
*/
uint32_t operationI(char *instruction, char *op1, char *op2, char *op3) {

    uint32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t funct3 = 0;
    uint32_t rd = getRegisterWithAlias(op1);
    uint32_t opcode = 0;

    if (!strcmp(instruction, "addi")) {
        imm = atoi(op3);
        rs1 = getRegisterWithAlias(op2);
        opcode = 0b0010011;
    }
    else if (!strcmp(instruction, "ld")) {
        imm = atoi(op2);
        rs1 = getRegisterWithAlias(op3);
        funct3 = 0x3;
        opcode = 0b0000011;
    }

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst |= imm;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    inst |= rs1;
    printf("inst : %08x \n", inst);
    inst <<= 3;
    printf("inst : %08x \n", inst);
    inst |= funct3;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    inst |= rd;
    printf("inst : %08x \n", inst);
    inst = inst << 7;
    inst = inst | opcode;
    printf("instruction finale = %08x \n", inst);
    return inst;
}

/*
    @brief transform S type instruction in binary representation
*/
uint32_t operationS(char *instruction, int op1, int op2, int op3) {

    uint32_t imm = op2;
    uint32_t rs2 = op1;
    uint32_t rs1 = op3;
    uint32_t funct3 = 0x3;
    uint32_t opcode = 0b0100011;
    // decoupage de la valeur immediate
    uint32_t imm1 = imm >> 5;
    uint32_t imm2 = imm & 0b11111;

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst |= imm1;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    inst |= rs2;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    printf("inst : %08x \n", inst);
    inst |= rs1;
    printf("inst : %08x \n", inst);
    inst <<= 3;
    inst |= funct3;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | imm2;
    inst = inst << 7;
    inst = inst | opcode;
    printf("instruction finale = %08x \n", inst);
    return inst;
}

/*
    @brief transform B type instruction in binary representation
*/
uint32_t operationB(char *instruction, int op1, int op2, int op3) {

    // declaration des elements de l'instruction
    uint32_t imm = op3;
    uint32_t rs2 = op2;
    uint32_t rs1 = op1;
    uint32_t funct3 = 0x3;
    uint32_t opcode = 0b1100011;

    if (!strcmp("beq", instruction)) {
        funct3 = 0x0;
    }
    else if (!strcmp("bne", instruction)) {
        funct3 = 0x1;
    }
    else if (!strcmp("blt", instruction)) {
        funct3 = 0x4;
    }
    else if (!strcmp("bge", instruction)) {
        funct3 = 0x5;
    }

    // decoupage de la valeur immediate
    uint32_t imm1 = ((imm >> 5) & 0b111111) | ((imm >> 6) & 0b1000000);
    uint32_t imm2 = (imm & 0b11110) | ((imm >> 11) & 0b1);

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst |= imm1;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    inst |= rs2;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    printf("inst : %08x \n", inst);
    inst |= rs1;
    printf("inst : %08x \n", inst);
    inst <<= 3;
    inst |= funct3;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | imm2;
    inst = inst << 7;
    inst = inst | opcode;
    printf("instruction finale = %08x \n", inst);
    return inst;
}

/*
    @brief transform J type instruction in binary representation
*/
uint32_t operationJ(char *instruction, int op1, int op2) {

    uint32_t imm = 0;
    imm = imm | op2;
    uint32_t rd = op1;
    uint32_t opcode = 0b1101111;

    // mise en forme de la valeur immediate
    uint32_t imm1 = (imm >> 20) & 0b1;
    uint32_t imm2 = (imm >> 1) & 0b1111111111;
    uint32_t imm3 = (imm >> 11) & 0b1;
    uint32_t imm4 = (imm >> 12) & 0b11111111;

    imm = 0;
    imm |= imm1;
    imm <<= 10;
    imm |= imm2;
    imm <<= 1;
    imm |= imm3;
    imm <<= 8;
    imm |= imm4;

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst |= imm;
    printf("inst : %08x \n", inst);
    inst <<= 5;
    inst |= rd;
    printf("inst : %08x \n", inst);
    inst <<= 7;
    printf("inst : %08x \n", inst);
    inst = inst | opcode;
    printf("instruction finale = %08x \n", inst);
    return inst;
}

uint32_t operationj(int op1) {
    printf("valeur de l'offset j (decimal)%d\n", op1);
    return operationJ("jal", 0, op1);
}
uint32_t operationli(char *op1, char *op2) {
    return operationI("addi", op1, "x0", op2);
}
uint32_t operationmv(char *op1, char *op2) {
    return operationI("addi", op1, op2, "0");
}
