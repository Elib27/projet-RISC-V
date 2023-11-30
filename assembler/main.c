#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        char arguments[3][5];
        int args_count = getInstructionFromLine(line, instruction, arguments);
        if (args_count > 0) {
            printf("instruction: %s, args_count: %d\n", instruction, args_count);
            char inst[33];
            inst[0] = '\0';
            printf("\n\n-------------- INSTRUCTION no %d --------------\n", count);
            count++;
            switch (getIntructionType(instruction)) {
            case 'R':
                printf("INSTRUCTION R : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                operationR(inst, instruction, getRegisterWithAlias(arguments[0]), getRegisterWithAlias(arguments[1]), getRegisterWithAlias(arguments[2]));
                break;

            case 'I':
                printf("INSTRUCTION I : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                operationI(inst, instruction, getRegisterWithAlias(arguments[0]), getRegisterWithAlias(arguments[1]), stringToInt(arguments[2]));
                break;
            case 'S':
                printf("INSTRUCTION S : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                operationS(inst, instruction, getRegisterWithAlias(arguments[0]), stringToInt(arguments[1]), getRegisterWithAlias(arguments[2]));
                break;
            case 'B':
                printf("INSTRUCTION B : %s | OP1 : %s | OP2 : %s | OP3 : %s\n", instruction, arguments[0], arguments[1], arguments[2]);
                operationB(inst, instruction, getRegisterWithAlias(arguments[0]), getRegisterWithAlias(arguments[1]), stringToInt(arguments[2]));
                break;
            default:
                printf("Instruction non reconnue \n");
                break;
            }
            char hexinst[9];
            hexinst[8] = '\0';
            convertBinIntructionToHexIntruction(hexinst, inst);
            fprintf(outputFile, hexinst);
            fprintf(outputFile, "\n");
        }
    }

    free(line);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}


int getInstructionFromLine(char *line, char instruction[5], char arguments[3][5]) {
    if (line[0] == '#' || line[0] == '\n') return 0; // ignore les lignes vides et les commentaires
    // nettoyage de la chaine
    int i = 0;
    while (line[i] != '\n') {
        if (line[i] == ',') line[i] = ' ';
        else if (line[i] == '(' || line[i] == ')') line[i] = ' ';
        i++;
    }

    int j = 0;
    // skip les premiers espaces
    while (line[j] == ' ') j++;

    // extraction de l'instruction
    while (line[j] != ' ') {
        instruction[j] = line[j];
        j++;
    }
    instruction[j] = '\0';

    // extraction des arguments
    int arg_counter = 0;
    while (line[j] != '\n' && arg_counter < 3) {
        while (line[j] == ' ') j++; // skip les espaces
        int l = 0;
        while (line[j] != ' ' && line[j] != '\n') {
            arguments[arg_counter][l] = line[j];
            j++;
            l++;
        }
        arguments[arg_counter][l] = '\0';
        arg_counter++;
    }
    return arg_counter;
}

int getRegisterWithAlias(char *alias) {
    char aliases[32][5] = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6" };
    int register_number = 0;
    while (strcmp(alias, aliases[register_number])) register_number++;
    return register_number;
}

int stringToInt(char *string) {
    int result = 0;
    int i = 0;
    while (string[i] != '\0') {
        int c = string[i] - '0';
        result = result * 10 + c;
        i++;
    }
    return result;
}


#define R_TYPE_LENGTH 2
#define I_TYPE_LENGTH 2
#define S_TYPE_LENGTH 1
#define B_TYPE_LENGTH 4
#define J_TYPE_LENGTH 1

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