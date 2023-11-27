

#include <stdio.h>
#include <string.h>
#include "main.h"



/* @brief Convertion d'un entier decimal en "représentation" binaire (char '0' et '1')
 * @param sortie tableau de sortie
 * @param taille taille de la sortie
 * @param entree nombre en decimal a convertir*/
void convertionDecimalBinaire(char *sortie, int taille, int entree) {

    // deprecated 
    // a faire en dehors de cete fonction
    for (int t = 0; t < taille-1; t++) {
        sortie[t] = '0';
    }


    for (int i = taille-1; entree > 0; i--) {
        sortie[i] = entree % 2 + '0';
        entree = entree / 2;
    }
}


/*
    @brief transform R type instrucion un binary representation  
*/
void operationR(char inst[],char *instruction, int op1, int op2, int op3) {


    /* funct7 */
    char funct7[8];
    for (int i = 0; i < 7; i++) {
        funct7[i] = '0';
    }
    funct7[7] = '\0';
    if (!strcmp(instruction, "sub")) {
        funct7[5] = '1';
    }
    strcat(inst, funct7);
    /* rs2 */
    char rs2[6];
    rs2[5] = '\0';
    convertionDecimalBinaire(rs2, 5, op3);

    strcat(inst, rs2);
    /* rs1 */
    char rs1[6];
    rs1[5] = '\0';
    convertionDecimalBinaire(rs1, 5, op2);
    strcat(inst, rs1);
    /* funct3 */
    char funct3[4];
    for (int i = 0; i < 3; i++) {
        funct3[i] = '0';
    }
    funct3[3] = '\0';
    strcat(inst, funct3);
    /* rd */
    char rd[6];
    rd[5] = '\0';
    convertionDecimalBinaire(rd, 5, op1);
    strcat(inst, rd);
    /* opcode */
    char opcode[8];

    for (int i = 0; i < 7; i++) {
        opcode[i] = '0';
    }
    opcode[7] = '\0';
    opcode[1] = '1';
    opcode[2] = '1';
    opcode[5] = '1';
    opcode[6] = '1';
    strcat(inst, opcode);
    printf("%s\n", inst);
}

/*
    @brief transform I type instrucion un binary representation  
*/
void operationI(char inst[], char *instruction, int op1, int op2, int op3) {
    char imm[13];
    imm[12] = '\0';
    for (int i = 0; i<12; i++) {
        imm[i] = '0';
    }
    convertionDecimalBinaire(imm, 12, op3);
    char rs1[6];
    rs1[5] = '\0';
    for(int i = 0; i<5; i++) {
        rs1[i] = '0';
    }
    convertionDecimalBinaire(rs1, 5, op2);
    char funct3[4];
    funct3[3] = '\0';
    for (int i = 0; i<3; i++) {
        funct3[i] = '0';
    }
    char rd[6];
    rd[5] = '\0';
    for (int i = 0; i<5; i++) {
        rd[i] = '0';
    }
    convertionDecimalBinaire(rd, 5, op1);
    char opcode[8];
    opcode[7] = '\0';
    for (int i = 0; i < 7; i++) {
        opcode[i] = '0';
    }
    
    /* choix selon l'instruction */

    if (!strcmp("addi", instruction)) {

        opcode[2] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
    } else if (!strcmp("ld", instruction)) {

        funct3[1] = '1';
        funct3[2] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
    }
    
    if (!strcmp("ld", instruction)) {
        strcat(inst, rs1);
        strcat(inst, imm);
    } else {
        strcat(inst, imm);
        strcat(inst, rs1);  
    }

    strcat(inst, funct3);
    strcat(inst, rd);
    strcat(inst, opcode);
    printf("%s\n", inst);

}


/*
    @brief transform S type instrucion un binary representation  
*/
void operationS(char inst[], char *instruction, int op1, int op2, int op3) {
    char imm_full[13];
    imm_full[12] = '\0';
    for (int i = 0; i<12; i++) {
        imm_full[i] = '0';
    }
    convertionDecimalBinaire(imm_full, 12, op2); // valeur immediate dans l'argument 2
    printf("imm-full : %s\n", imm_full);
    char rs2[6];
    rs2[5] = '\0';
    for (int i = 0; i<5; i++) {
        rs2[i] = '0';
    }
    convertionDecimalBinaire(rs2, 5, op1); // rs2 dans l'argument 1
    char rs1[6];
    rs1[5] = '\0';
    for(int i = 0; i<5; i++) {
        rs1[i] = '0';
    }
    convertionDecimalBinaire(rs1, 5, op3); // valeur rs1 dans l'argument 3
    char funct3[4];
    funct3[3] = '\0';
    for (int i = 0; i<3; i++) {
        funct3[i] = '0';
    }
    
    
    char opcode[8];
    opcode[7] = '\0';
    for (int i = 0; i < 7; i++) {
        opcode[i] = '0';
    }
    
    /* choix selon l'instruction */

    if (!strcmp("sd", instruction)) {

        funct3[1] = '1';
        funct3[2] = '1';

        opcode[1] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
    }
    
    // pour ce type d'instruction, il faut couper en deux la valeur immédiate

    char imm1[8];
    imm1[7] = '\0';
    for (int i = 0; i<7; i++){
        imm1[i] = imm_full[i];
    }
    char imm2[6];
    imm2[5] = '\0';
    for( int i = 7; i<12; i++) {
        imm2[i-7] = imm_full[i];
    }
    printf("imm2 : %s\n", imm2);
    printf("imm_full[11] : %c\n", imm_full[11]);

    strcat(inst, imm1);
    strcat(inst, rs2);
    strcat(inst, rs1);
    strcat(inst, funct3);
    strcat(inst, imm2);
    strcat(inst, opcode);
    printf("%s\n", inst);

}



/*
    @brief transform B type instrucion un binary representation  
*/
void operationB(char inst[], char *instruction, int op1, int op2, int op3) {
    char imm_full[14];
    imm_full[13] = '\0';
    for (int i = 0; i<13; i++) {
        imm_full[i] = '0';
    }
    convertionDecimalBinaire(imm_full, 13, op3); // valeur immediate dans l'argument 2
    printf("imm-full : %s\n", imm_full);
    char rs2[6];
    rs2[5] = '\0';
    for (int i = 0; i<5; i++) {
        rs2[i] = '0';
    }
    convertionDecimalBinaire(rs2, 5, op2); // rs2 dans l'argument 1
    char rs1[6];
    rs1[5] = '\0';
    for(int i = 0; i<5; i++) {
        rs1[i] = '0';
    }
    convertionDecimalBinaire(rs1, 5, op1); // valeur rs1 dans l'argument 3
    char funct3[4];
    funct3[3] = '\0';
    for (int i = 0; i<3; i++) {
        funct3[i] = '0';
    }
    
    
    char opcode[8];
    opcode[7] = '\0';
    for (int i = 0; i < 7; i++) {
        opcode[i] = '0';
    }
    
    /* choix selon l'instruction */

    if (!strcmp("beq", instruction)) {


        opcode[0] = '1';
        opcode[1] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
    } else if (!strcmp("bne", instruction)){
        opcode[0] = '1';
        opcode[1] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
        funct3[2] = '1';
    } else if (!strcmp("blt", instruction)){
        opcode[0] = '1';
        opcode[1] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
        funct3[0] = '1';
    } else if (!strcmp("bge", instruction)){  // else if si ajout d'autres instructions
        opcode[0] = '1';
        opcode[1] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
        funct3[0] = '1';
        funct3[2] = '1';
    }
    
    // pour ce type d'instruction, il faut couper en deux la valeur immédiate

    char imm1[8];
    imm1[7] = '\0';
    for (int i = 0; i<7; i++){
        imm1[i] = imm_full[i+1];
    }
    char imm2[6];
    imm2[5] = '\0';
    for( int i = 7; i<12; i++) {
        imm2[i-7] = imm_full[i+1];
    }
    printf("imm1 : %s\n", imm1);
    printf("imm2 : %s\n", imm2);
    printf("imm_full[11] : %c\n", imm_full[11]);

    strcat(inst, imm1);
    strcat(inst, rs2);
    strcat(inst, rs1);
    strcat(inst, funct3);
    strcat(inst, imm2);
    strcat(inst, opcode);

    // modifiaction de l'instruction B
    char temp = inst[0];
    inst[0] = imm_full[0];
    inst[24] = temp;
    


    printf("%s\n", inst);

}




/*
    @brief transform B type instrucion un binary representation  
*//*
void operationB(char inst[], char *instruction, int op1, int op2, int op3) {
    
    operationS(inst, instruction, op1, op2, op3);

    // modifiaction de l'instruction B
    char temp = inst[1];
    for (int i = 1; i<6; i++) {
        inst[i] = inst[i+1];
        
    }
    inst[6] = inst[20];
    inst[20] = inst[21];
    inst[21] = inst[22];
    inst[22] = inst[23];
    inst[23] = inst[24];
    inst[24] = temp;

    //choix selon l'instruction

    if (!strcmp("beq", instruction)) {


        inst[25+0] = '1'; // offset 25 = debut de opcode
        inst[25+1] = '1';
        inst[25+5] = '1';
        inst[25+6] = '1';
    }
    

}

*/