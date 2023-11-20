#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInstructionFromLine(char *line, char instruction[5], char arguments[3][5]);
int getRegisterWithAlias(char *alias);
int stringToInt(char *string);
void convertionDecimalBinaire(char *sortie, int entree);
void operationR(char *instruction, int op1, int op2, int op3);

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
    while (getline(&line, &len, inputFile) != -1) {
        char instruction[5];
        char arguments[3][5];
        int args_count;
        args_count = getInstructionFromLine(line, instruction, arguments);
        if (args_count > 0) printf("instruction: %s, args_count: %d\n", instruction, args_count);

        /* Recherche de l'instruction */
        // switch (instruction)
        // {
        // case "add":
        //     add(op1, op2, op3);
        //     break;
        // }
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

void operationR(char *instruction, int op1, int op2, int op3) {

    char inst[33];
    inst[0] = '\0';
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
    convertionDecimalBinaire(rs2, op3);

    strcat(inst, rs2);
    /* rs1 */
    char rs1[6];
    rs1[5] = '\0';
    convertionDecimalBinaire(rs1, op2);
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
    convertionDecimalBinaire(rd, op1);
    strcat(inst, rd);
    /* opcode */
    char opcode[8];

    printf("%s\n", inst);
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

void convertionDecimalBinaire(char *sortie, int entree) {
    for (int t = 0; t < 5; t++) {
        sortie[t] = '0';
    }
    for (int i = 4; entree > 0; i--) {
        sortie[i] = entree % 2 + '0';
        entree = entree / 2;
    }
}
