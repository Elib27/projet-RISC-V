#include <stdio.h>
#include <stdint.h>

#define         ADD_SUB 0b0110011
#define            ADDI 0b0010011
#define              LD 0b0000011
#define              SD 0b0100011
#define BEQ_BNE_BLT_BGE 0b1100011
#define             JAL 0b1101111


typedef struct {
    uint32_t pc; // un registre pc 
    uint64_t registres[32]; // 32 registes
    uint64_t memoire[2048]; // 2048 lignes de 8 octets dans la memoire
} Memoire;

void printmem(Memoire *mem) {
    for (int i = 0; i < 20; i++) {
        printf("%16lx \n", mem->memoire[i]);
    }

}
void printregistres(Memoire *mem) {
    for (int i = 0; i < 32; i++) {
        printf(" x%d : %ld \n", i, mem->registres[i]);
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
        if ((j % 2) == 0) {
            mem->memoire[i] = line;
        }
        else {
            uint64_t line2 = line;
            line2 <<= 32;
            mem->memoire[i] |= line2;
            i++;
        }
        j++;
    }
}


int executeInstruction(uint32_t instruction, Memoire *memoire) {
    uint32_t inst = instruction & 0b1111111;
    int skip_pc = 0;

    switch (inst) {

    case ADD_SUB: // remplacer par #DEFINE
    {
        printf("------------ADD_SUB----------\n");
        uint32_t funct7 = instruction >> 25;
        uint32_t rd = (instruction >> 7) & 0b11111;
        uint32_t rs1 = (instruction >> 15) & 0b11111;
        uint32_t rs2 = (instruction >> 20) & 0b11111;

        
        if (funct7 == 0b0) {
            // addition
            memoire->registres[rd] = memoire->registres[rs1] + memoire->registres[rs2];
        } else {// funct7== 0x02
            // soustraction
            memoire->registres[rd] = memoire->registres[rs1] - memoire->registres[rs2];
        } 
        
        break;
    }
    
    //addi => voir bit de signe
    case ADDI:
    {
        printf("------------ADDI----------\n");
        uint64_t imm = instruction >> 20;

        if ((imm >> 11) == 1) {                     // dans le cas d'ne valeur néagtve
            imm = imm | 0xfffffffffffff000;
        }
        uint32_t rs1 = (instruction >> 15) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t rd = (instruction >> 7) & 0b11111;
        printf("rs1 : %d\n", rs1);
        printf("rd : %d\n", rd);
        printf("imm : %lx \n", imm);
        memoire->registres[rd] = memoire->registres[rs1] + imm;
        break;
    }

    //ld
    case LD:
    {
        uint64_t imm = instruction >> 20;
        if ((imm >> 11) == 1) {                     // dans le cas d'ne valeur néagtve
            imm = imm | 0xfffffffffffff000;
        }
        uint32_t rs1 = (instruction >> 14) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t rd = (instruction >> 7) & 0b11111;
        memoire->registres[rd] = memoire->registres[rs1 + imm];
        break;
    }

    // sd
    case SD:
    {
        uint32_t imm1 = instruction >> 25;
        uint32_t rs2 = (instruction >> 20) & 0b11111;
        uint32_t rs1 = (instruction >> 15) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t imm2 = (instruction >> 7) & 0b11111;
        uint64_t imm = (imm1 << 5) | imm2;
        if ((imm >> 11) == 1) {                     // dans le cas d'ne valeur néagtve
            imm = imm | 0xfffffffffffff000;
        }
        memoire->memoire[rs1 + imm] = memoire->registres[rs2];
        break;
    }

    case BEQ_BNE_BLT_BGE:
    {
        printf("------------BEQ_BNE_BLT_BGE----------\n");
        uint32_t imm1 = instruction >> 25;
        uint32_t rs2 = (instruction >> 20) & 0b11111;
        uint32_t rs1 = (instruction >> 15) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t imm2 = (instruction >> 7) & 0b11111;
      //uint32_t imm = ((imm1 >> 7) << 31) | (imm2 & 1) << 30 | (imm1 && 0b111111) << 23 | (imm2 >> 1);
        uint64_t imm = ((imm1 >> 7) << 12) | (imm2 & 1) << 11 | (imm1 && 0b111111) << 5 | (imm2 >> 1) << 1;

        if ((imm >> 12) == 1) {                     // dans le cas d'ne valeur néagtve
            imm = imm | 0xffffffffffffe000;
        }

        // DEBUG

        printf("Value rs1 : %ld\n", memoire->registres[rs1]);
        printf("rs1 = x%d\n", rs1);
        printf("Value rs2 : %ld\n", memoire->registres[rs2]);
        printf("Value imm : %ld\n", imm);
        

        // beq
        if ((funct3 == 0x0) && (memoire->registres[rs1] == memoire->registres[rs2])) {
            printf("beq ok\n");
            memoire->pc += imm;
            skip_pc = 1;
        }
        // bne
        if (funct3 == 0x1 && memoire->registres[rs1] != memoire->registres[rs2]) {
            memoire->pc += imm;
            skip_pc = 1;
        }
        // blt
        if (funct3 == 0x4 && memoire->registres[rs1] < memoire->registres[rs2]) {
            memoire->pc += imm;
            skip_pc = 1;
        }
        // bge
        if (funct3 == 0x5 && memoire->registres[rs1] >= memoire->registres[rs2]) {
            memoire->pc += imm;
            skip_pc = 1;
        }
        break;
    }
    
    // jal
    case JAL:
    {
        printf("---------JAL-----------\n");
        uint32_t rd = (instruction >> 7) & 0b11111;
        uint32_t imm = (instruction >> 12) & 0b111111111111111111111;
        printf("imm de j (mélangé) : %x\n", imm);

        // remise en ordre de l'immediat
      //uint32_t new_imm = (instruction >> 31) | ((instruction >> 21) & 0b1111111111) | ((instruction >> 19) & 0b1) | ((instruction >> 12) & 0b11111111);
        uint64_t new_imm = (imm & 0b10000000000000000000) << 1 | (imm & 0b01111111111000000000) >> 8 | (imm & 0b00000000000100000000) << 3 | (imm & 0b00000000000011111111) << 12;

        if ((new_imm >> 20) == 1) {                     // dans le cas d'ne valeur néagtve
            new_imm = new_imm | 0xffffffffffe00000;
        }

        memoire->registres[rd] = memoire->pc + 4;
        memoire->pc += new_imm;
        printf("imm de j (trié en hexa)): %16lx\n", new_imm);
        printf("imm de j (trié en décimal): %ld\n", new_imm);
        skip_pc = 1;
        
        break;
    }
    
    default:
        break;
    }
    return skip_pc;
}

uint32_t lectureInstruction(Memoire *memoire) {
    uint32_t instruction = 0;
    if ((memoire->pc % 8) == 0) {
        instruction = memoire->memoire[(memoire->pc) / 8] & 0xffffffff;
    }
    else {
        instruction = memoire->memoire[(memoire->pc - 4) / 8] >> 32;
    }
    return instruction;
}

void ecrireSortie(Memoire mem, FILE *output) {

    for (int i = 0; i < 32; i++) {
        fprintf(output, "x%d: %ld\n", i, mem.registres[i]);
    }

}


int main(int argc, char **argv) {
    /* ->/riscv-emulator <HEX INPUT> <EMULATION OUTPUT> */
    if (argc != 3) {
        printf("Usage: ->/riscv-emulator <HEX INPUT> <EMULATION OUTPUT>\n");
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
    int skip_pc = 1;

    // lecture dans la memoire 
    instruction = lectureInstruction(&memoire);

    while (instruction != 0) {
        //uint32_t inst;
        //uint32_t arguments[3];
        // lecture
        skip_pc = executeInstruction(instruction, &memoire);
        if (!skip_pc) {
            memoire.pc = memoire.pc + 4;
            skip_pc = 1;
        }
        memoire.registres[0] = 0;
        instruction = lectureInstruction(&memoire);
    }

    ecrireSortie(memoire, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
