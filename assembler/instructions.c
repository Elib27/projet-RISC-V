#include <stdio.h>
#include <string.h>
#include<stdint.h>

#include "main.h"



/*
    @brief transform R type instrucion un binary representation
*/
uint32_t operationR(char *instruction, int op1, int op2, int op3) {

    // declaration des elements de l'instruction
    uint32_t funct7 = 0;
    uint32_t rs2 = 0;
    rs2 = rs2 | op3;
    uint32_t rs1 = 0;
    rs1 = rs1 | op2;
    uint32_t funct3 = 0;
    uint32_t rd = 0;
    rd = rd | op1;
    uint32_t opcode = 0b0110011;

    if (!strcmp(instruction, "sub")) {
        funct7 = funct7 | 0x20;
    }

    // concatenation

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst = inst | funct7;
    printf("inst : %08x \n", funct7);
    inst = inst << 5;
    inst = inst | rs2;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    printf("inst : %08x \n", inst);
    inst = inst | rs1;
    printf("inst : %08x \n", inst);
    inst = inst << 3;
    inst = inst | funct3;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | rd;
    printf("inst : %08x \n", inst);
    inst = inst << 7;
    inst = inst | opcode;
    printf("inst : %08x \n", inst);
    printf("instructio finale = 0b%d \n", inst);
    return inst;
}

/*
    @brief transform I type instrucion un binary representation
*/
uint32_t operationI(char *instruction, int op1, int op2, int op3) {

    // declaration des elements de l'instruction
    uint32_t imm = 0;

    uint32_t rs1 = 0;

    uint32_t funct3 = 0;

    uint32_t rd = 0;
    rd = rd | op1;

    uint32_t opcode = 0;

    if (!strcmp(instruction, "addi")) {
        imm = imm | op3;
        rs1 = rs1 | op2;
        opcode = 0b0010011;
    } else if (!strcmp(instruction, "ld")) {
        imm = imm | op2;
        rs1 = rs1 | op3;
        funct3 = 0x3;
        opcode = 0b0000011;
    }

    // concatenation

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst = inst | imm;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | rs1;
    printf("inst : %08x \n", inst);
    inst = inst << 3;
    printf("inst : %08x \n", inst);
    inst = inst | funct3;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | rd;
    printf("inst : %08x \n", inst);
    inst = inst << 7;
    inst = inst | opcode;
    printf("instructio finale = %08x \n", inst);
    return inst;
}




/*
    @brief transform S type instrucion un binary representation
*/
uint32_t operationS(char *instruction, int op1, int op2, int op3) {

    // declaration des elements de l'instruction
    uint32_t imm = 0;
    imm = imm | op2;

    uint32_t rs2 = 0;
    rs2 = rs2 | op1;

    uint32_t rs1 = 0;
    rs1 = rs1 | op3;

    uint32_t funct3 = 0x3;


    uint32_t opcode = 0b0100011;


    // decoupage de la valeur immediate

    uint32_t imm1 = imm >> 5;
    uint32_t imm2 = imm & 0b11111;

    // concatenation

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst = inst | imm1;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | rs2;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    printf("inst : %08x \n", inst);
    inst = inst | rs1;
    printf("inst : %08x \n", inst);
    inst = inst << 3;
    inst = inst | funct3;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | imm2;
    inst = inst << 7;
    inst = inst | opcode;
    printf("instructio finale = %08x \n", inst);
    return inst;
}


/*
    @brief transform B type instrucion un binary representation
*/
uint32_t operationB(char *instruction, int op1, int op2, int op3) {

    // declaration des elements de l'instruction
    uint32_t imm = 0;
    imm = imm | op3;

    uint32_t rs2 = 0;
    rs2 = rs2 | op2;

    uint32_t rs1 = 0;
    rs1 = rs1 | op1;

    uint32_t funct3 = 0x3;

    uint32_t opcode = 0b1100011;

    // en fonction de l'instruction

    if (!strcmp("beq", instruction)) {
        funct3 = 0x0;
    }
    else if (!strcmp("bne", instruction)) {
        funct3 = 0x1;
    }
    else if (!strcmp("blt", instruction)) {
        funct3 = 0x4;
    }
    else if (!strcmp("bge", instruction)) {  // else if si ajout d'autres instructions
        funct3 = 0x5;
    }



    // decoupage de la valeur immediate

    uint32_t imm1 = ((imm >> 5) & 0b111111) | ((imm >> 6) & 0b1000000);
    uint32_t imm2 = (imm & 0b11110) | ((imm >> 11) & 0b1);

    // concatenation

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst = inst | imm1;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | rs2;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    printf("inst : %08x \n", inst);
    inst = inst | rs1;
    printf("inst : %08x \n", inst);
    inst = inst << 3;
    inst = inst | funct3;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | imm2;
    inst = inst << 7;
    inst = inst | opcode;
    printf("instructio finale = %08x \n", inst);
    return inst;
}



/*
    @brief transform J type instrucion un binary representation
*/
uint32_t operationJ(char *instruction, int op1, int op2) {

    // declaration des elements de l'instruction
    uint32_t imm = 0;
    imm = imm | op2;

    uint32_t rd = 0;
    rd = rd | op1;

    uint32_t opcode = 0b1101111;


    // mise en forme de la valeur immediate
    
    uint32_t imm1 = (imm >> 20) & 0b1;
    uint32_t imm2 = (imm >> 1) & 0b11111111;
    uint32_t imm3 = (imm >> 11) & 0b1;
    uint32_t imm4 = (imm >> 12) & 0b11111111;

    imm = 0;
    
    imm = imm | imm1;
    imm = imm << 10;
    imm = imm | imm2;
    imm = imm << 1;
    imm = imm | imm3;
    imm = imm << 8;
    imm = imm | imm4;

    
    // concatenation

    uint32_t inst = 0;
    printf("inst : %08x \n", inst);
    inst = inst | imm;
    printf("inst : %08x \n", inst);
    inst = inst << 5;
    inst = inst | rd;
    printf("inst : %08x \n", inst);
    inst = inst << 7;
    printf("inst : %08x \n", inst);
    inst = inst | opcode;
    printf("instructio finale = %08x \n", inst);
    return inst;
}

uint32_t operationj(int op1) {
    printf("valeur de l'offset j (decimal)%d\n", op1);
    return operationJ("jal", 0, op1);
}
uint32_t operationli(int op1, int op2) {
    return operationI("addi", op1, 0, op2);
}
uint32_t operationmv(int op1, int op2) {
    return operationI("addi", op1, op2, 0);
}

