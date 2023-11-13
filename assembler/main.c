#include <stdio.h>

int main(int argc, char **argv)
{
    /* ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT> */
    if(argc != 3) {
        printf("usage: ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT>\n");
        return 1;
    }

    char *asm_input_file = argv[1];
    char *hex_output_file = argv[2];

    FILE *myfile = fopen(asm_input_file, "r");
    FILE *myfile2 = fopen(hex_output_file,"w");

    fclose(myfile);
    fclose(myfile2);

    return 0;
}
