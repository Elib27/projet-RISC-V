#include <stdio.h>
#include <string.h>
#include "main.h"

/* @brief Convertion d'un entier decimal en "représentation" binaire (char '0' et '1')
 * @param sortie tableau de sortie
 * @param taille taille de la sortie
 * @param entree nombre en decimal a convertir
*/
void convertionDecimalBinaire(char *sortie, int taille, int entree) {

    // deprecated 
    // a faire en dehors de cete fonction
    for (int t = 0; t < taille - 1; t++) {
        sortie[t] = '0';
    }


    for (int i = taille - 1; entree > 0; i--) {
        sortie[i] = entree % 2 + '0';
        entree = entree / 2;
    }
}


/*
    @brief transform R type instrucion un binary representation
*/
void operationR(char inst[], char *instruction, int op1, int op2, int op3) {

    /* funct7 */
    char funct7[8] = "0000000";
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
    char funct3[4] = "000";
    strcat(inst, funct3);

    /* rd */
    char rd[6];
    rd[5] = '\0';
    convertionDecimalBinaire(rd, 5, op1);
    strcat(inst, rd);

    /* opcode */
    char opcode[8] = "0110011";
    strcat(inst, opcode);

    printf("%s\n", inst);
}

/*
    @brief transform I type instrucion un binary representation
*/
void operationI(char inst[], char *instruction, int op1, int op2, int op3) {

    char imm[13] = "000000000000";
    convertionDecimalBinaire(imm, 12, op3);
    strcat(inst, imm);

    char rs1[6] = "00000";
    convertionDecimalBinaire(rs1, 5, op2);
    strcat(inst, rs1);

    char funct3[4] = "000";
    strcat(inst, funct3);

    char rd[6] = "00000";
    convertionDecimalBinaire(rd, 5, op1);
    strcat(inst, rd);

    char opcode[8] = "0000000";
    /* choix selon l'instruction */
    if (!strcmp("addi", instruction)) {
        opcode[2] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
    }
    else if (!strcmp("ld", instruction)) {
        funct3[1] = '1';
        funct3[2] = '1';
        opcode[5] = '1';
        opcode[6] = '1';
    }
    strcat(inst, opcode);

    printf("%s\n", inst);
}