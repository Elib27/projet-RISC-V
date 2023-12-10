#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "instructions.h"
#include "main.h"

int main(int argc, char **argv) {
    /* ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT> */
    if (argc != 3) {
        printf("usage: ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT>\n");
        return 1;
    }

    char *asm_input_file = argv[1];
    char *hex_output_file = argv[2];

    FILE *inputFile = fopen(asm_input_file, "r");
    FILE *outputFile = fopen(hex_output_file, "w");

    size_t len;
    char *line = NULL;
    int count = 0;
    while (getline(&line, &len, inputFile) != -1) {
        char instruction[5];
        char arguments[3][10];
        int args_count = getInstructionFromLine(line, instruction, arguments);
        if (args_count > 0) {
            printf("\n\n-------------- INSTRUCTION no %d --------------\n", count);
            printf("line : %s\n", line);
            printf("instruction: %s, args_count: %d\n", instruction, args_count);
            count++;
            uint32_t instru = 0;
            printf("nom: %s\n", instruction);
            switch (getIntructionType(instruction)) {
            case 'R':
                printf("INSTRUCTION R : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                instru = operationR(instruction, getRegisterWithAlias(arguments[0]), getRegisterWithAlias(arguments[1]), getRegisterWithAlias(arguments[2]));
                break;

            case 'I':
                printf("INSTRUCTION I : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                instru = operationI(instruction, arguments[0], arguments[1], arguments[2]);
                break;
            case 'S':
                printf("INSTRUCTION S : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                instru = operationS(instruction, getRegisterWithAlias(arguments[0]), atoi(arguments[1]), getRegisterWithAlias(arguments[2]));
                break;
            case 'B':
                printf("INSTRUCTION B : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                instru = operationB(instruction, getRegisterWithAlias(arguments[0]), getRegisterWithAlias(arguments[1]), atoi(arguments[2]));
                break;
            case 'J':
                printf("INSTRUCTION J : %s | OP1 : %s | OP2 : %s\n", instruction, arguments[0], arguments[1]);
                instru = operationJ(instruction, getRegisterWithAlias(arguments[0]), atoi(arguments[1]));
                break;
            case 'j':
                printf("INSTRUCTION j | OP1 : %s\n", arguments[0]);
                instru = operationj(atoi(arguments[0]));
                break;
            case 'l':
                printf("INSTRUCTION li | OP1 : %s | OP2 : %s\n", arguments[0], arguments[1]);
                instru = operationli(arguments[0], arguments[1]);
                break;
            case 'm':
                printf("INSTRUCTION mv | OP1 : %s | OP2 : %s\n",arguments[0], arguments[1]);
                instru = operationmv(arguments[0], arguments[1]);
                break;
            default:
                printf("Instruction non reconnue \n");
                break;
            }
            fprintf(outputFile, "%08x", instru);
            fprintf(outputFile, "\n");
        }
    }

    free(line);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int getInstructionFromLine(char *line, char instruction[5], char arguments[3][10]) {

    if (line[0] == '#' || line[0] == '\n') return 0; // ignore les lignes vides et les commentaires

    // nettoyage de la chaine
    int i = 0;
    while (line[i] != '\n') {
        if (line[i] == ',') line[i] = ' ';
        if (line[i] == '(' || line[i] == ')') line[i] = ' ';
        i++;
    }

    // extraction des instructions
    int args_counter = sscanf(line, "%s %s %s %s", instruction, arguments[0], arguments[1], arguments[2]);

    return args_counter;
}

int getRegisterWithAlias(char *alias) { // refaire en testant si commence par x
    char aliases[64][5] = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31" };
    int register_number = 0;
    while (strcmp(alias, aliases[register_number])) register_number++;
    return register_number % 32;
}

#define R_TYPE_LENGTH 2
#define I_TYPE_LENGTH 2
#define S_TYPE_LENGTH 1
#define B_TYPE_LENGTH 4
#define J_TYPE_LENGTH 1
#define Pseudo_TYPE_LENGTH 3

/*
    @brief renvoie le type de l'instruction ou 'X' si non reconnue
*/
char getIntructionType(char *instruction) {

    /* tableaux des instructions par types*/

    char Rtype[R_TYPE_LENGTH][5] = { "add", "sub" };
    char Itype[I_TYPE_LENGTH][5] = { "addi", "ld" };
    char Stype[S_TYPE_LENGTH][5] = { "sd" };
    char Btype[B_TYPE_LENGTH][5] = { "beq", "bne", "blt", "bge" };
    char Jtype[J_TYPE_LENGTH][5] = { "jal" };
    //char Pseudotype[Pseudo_TYPE_LENGTH][5] = {"j", "li", "mv"};

    /* recherche dans les tableaux */
    for (int i = 0; i < R_TYPE_LENGTH; i++) {
        if (!strcmp(instruction, Rtype[i])) {
            return 'R';
        }
    }
    for (int i = 0; i < I_TYPE_LENGTH; i++) {
        if (!strcmp(instruction, Itype[i])) {
            return 'I';
        }
    }
    for (int i = 0; i < S_TYPE_LENGTH; i++) {
        if (!strcmp(instruction, Stype[i])) {
            return 'S';
        }
    }
    for (int i = 0; i < B_TYPE_LENGTH; i++) {
        if (!strcmp(instruction, Btype[i])) {
            return 'B';
        }
    }
    for (int i = 0; i < J_TYPE_LENGTH; i++) {
        if (!strcmp(instruction, Jtype[i])) {
            return 'J';
        }
    }
    for (int i = 0; i < Pseudo_TYPE_LENGTH; i++) {
        if (!strcmp(instruction, "j")) {
            return 'j';
        } else if (!strcmp(instruction, "li")) {
            return 'l';
        } else if (!strcmp(instruction, "mv")) {
            return 'm';
        }
    }
    return 'X';
}

/*
    @brief converti une instruction en representation binaire en une instruction en representation hexa. Les longueurs sont fixes, 32 pour binaire et 8 pour hexa
    @param hexInstruction chaine de sortie
    @param binInstruction chaine d'entree
*/
void convertBinIntructionToHexIntruction(char hexInstruction[], char binInstruction[]) {
    int outputIndex = 0;
    for (int i = 0; i < 32; i += 4) {
        int hexValue = 0;
        if (binInstruction[i] == '1') {
            hexValue += 8;
        }
        if (binInstruction[i + 1] == '1') {
            hexValue += 4;
        }
        if (binInstruction[i + 2] == '1') {
            hexValue += 2;
        }
        if (binInstruction[i + 3] == '1') {
            hexValue += 1;
        }
        char hexRepresentation[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
        hexInstruction[outputIndex] = hexRepresentation[hexValue];
        outputIndex++;
    }
}