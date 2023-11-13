#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("%s\n", line);
        if (line[0] == '#' || line[0] == '\n') continue; // ignore les lignes vides et les commentaires
        // nettoyage de la chaine
        int i = 0;
        while(line[i] != '\n') {
            if (line[i] == ',') line[i] = ' ';
            else if (line[i] == '(' || line[i] == ')') line[i] = ' ';
            i++;
        }
        // extraction de l'instruction
        char instruction[5];
        int j = 0;
        while (line[j] == ' ') {
            j++;
        }
        strcpy(instruction, line + j); // nb caractères ?
        printf("%s", instruction);
    }

    free(line);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
