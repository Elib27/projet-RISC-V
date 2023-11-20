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


    /* Recherche de l'instruction */
    switch (instruction) {
        case "add":
            add(op1, op2, op3);
        break;



    free(line);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}


void operationR(char *instruction, int op1, int op2, int op3) {

    char inst[33];
    inst[0] = '\0';
    /* funct7 */
    char funct7[8];
    for(int i = 0; i<7; i++) {
        funct7[i] = '0';
    }
    funct7[7] = '\0';
    if( !strcmp(instruction, "sub")) {
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
    for (int i = 0; i<3; i++) {
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
    for (int  i = 0; i < 7; i++) {
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
    for(int t = 0; t<5; t++) {
        sortie[t] = '0';
    }
    for( int i = 4; entree>0; i--) {
        sortie[i] = entree%2 + '0';
        entree = entree/2;
    }
}
