#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInstructionFromLine(char *line, char instruction[5], char arguments[3][5]);
int getRegisterWithAlias(char *alias);
int stringToInt(char *string);

int main(int argc, char **argv)
{
    /* ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT> */
    if(argc != 3) {
        printf("usage: ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT>\n");
        return 1;
    }

    char *asm_input_file = argv[1];
    char *hex_output_file = argv[2];

    FILE *inputFile = fopen(asm_input_file, "r");
    FILE *outputFile = fopen(hex_output_file,"w");
    
    size_t len;
    char *line = NULL;
    while (getline(&line, &len, inputFile) != -1) {
        char instruction[10];
        char arguments[3][5];
        int args_counter = 0;
        args_counter = getInstructionFromLine(line, instruction, arguments);
        if (args_counter) printf("%s\n", instruction);
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
    while(line[i] != '\n') {
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
    while(line[j] != '\n' && arg_counter < 3) {
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
    char aliases[32][5] = {"zero", "ra", "sp", "gp","tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9" ,"s10" ,"s11", "t3", "t4", "t5", "t6"};
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