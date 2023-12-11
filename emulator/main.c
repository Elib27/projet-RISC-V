#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t pc; // un registre pc 
    uint32_t registres[32]; // 32 registes
    uint64_t memoire[2048]; // 2048 lignes de 8 octets dans la memoire
} Memoire;

void printmem(Memoire *mem) {
    for (int i = 0; i < 20; i++) {
        printf("%16lx \n", mem->memoire[i]);
    }
    
}
void printregistres(Memoire *mem) {
    for (int i = 0; i < 32; i++) {
        printf(" x%d : %d \n", i, mem->registres[i]);
    }  
}

void initRegistres(Memoire *mem) {
    mem->pc = 0;
    for (int i = 0; i < 32; i++) {
        mem->registres[i] = 0;
    }
    mem->registres[2] = 16384; // pc
}

void lectureProgramme(Memoire *mem, FILE *inputFile) {
    // mise en mémoire du programme
    uint32_t line;
    int i = 0;
    int j = 0;
    while (fscanf(inputFile, "%08x\n", &line) != -1) {

        
        if ((j%2) == 0) {
            mem->memoire[i] = line;
        } else {
            uint64_t line2 = line;
            line2 <<= 32;
            mem->memoire[i] |= line2;
            i++;
        }
        j++;
    }

}


void extractInstruction(uint32_t instruction, uint32_t* inst, uint32_t arguments[3]) {
    // a faire
}

void executeInstruction(uint32_t inst, uint32_t arguments[3], Memoire memoire) {
    // GROS switch
}

uint32_t lectureInstruction(Memoire *memoire) {
    uint32_t instruction = 0;
    if ((memoire->pc %2) == 0) {
        instruction = memoire->memoire[memoire->pc] & 0xffffffff;
    } else {
        instruction = (memoire->memoire[memoire->pc] >> 32) & 0xffffffff;
    }
    return instruction;
}


int main(int argc, char **argv)
{
    /* ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT> */
    if(argc != 3) {
        printf("Usage: ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT>\n");
        printf("error: invalid command\n");
        return 1;
    }

    char *hex_input_file = argv[1];
    char *emu_output_file = argv[2];

    FILE *inputFile = fopen(hex_input_file, "r");
    FILE *outputFile = fopen(emu_output_file, "w");


    Memoire memoire;

    // mise a zero des registres
    initRegistres(&memoire);

    // mise de programme en mémoire
    lectureProgramme(&memoire, inputFile);


    printmem(&memoire);
    printregistres(&memoire);

    // boucle

    uint32_t instruction;

    // lecture dans la memoire 
    instruction = lectureInstruction(&memoire);

    while (instruction != 0) {
        uint32_t inst;
        uint32_t arguments[3];
        // lecture
        extractInstruction(instruction, &inst, arguments[3]);
        executeInstruction(inst, arguments[3], &memoire);
    }
    
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
