#include <stdio.h>
#include <stdint.h>

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


void executeInstruction(uint32_t instruction, Memoire *memoire) {
    uint32_t inst = instruction & 0b1111111;

    switch (inst) {

    case 0b0110011: // remplacer par #DEFINE
    {
        uint32_t funct7 = instruction >> 25;
        uint32_t rd = (instruction >> 7) & 0b00000;
        uint32_t rs1 = (instruction >> 15) & 0b00000;
        uint32_t rs2 = (instruction >> 20) & 0b00000;

        
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
    case 0b0010011:
    {
        uint32_t imm = instruction >> 20;
        uint32_t rs1 = (instruction >> 14) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t rd = (instruction >> 7) & 0b11111;
        memoire->registres[rd] = memoire->registres[rs1] + imm;
        break;
    }

    //ld
    case 0b0000011:
    {
        uint32_t imm1 = instruction >> 25;
        uint32_t rs1 = (instruction >> 14) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t rd = (instruction >> 7) & 0b11111;
        memoire->registres[rd] = memoire->registres[rs1 + imm1];
        break;
    }

    // sd
    case 0b0100011:
    {
        uint32_t imm1 = instruction >> 25;
        uint32_t rs2 = (instruction >> 20) & 0b11111;
        uint32_t rs1 = (instruction >> 15) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t imm2 = (instruction >> 7) & 0b11111;
        uint32_t imm = (imm1 << 5) | imm2;
        memoire->memoire[rs1 + imm] = memoire->registres[rs2];
        break;
    }

    case 0b1100011:
    {
        uint32_t imm1 = instruction >> 25;
        uint32_t rs2 = (instruction >> 20) & 0b11111;
        uint32_t rs1 = (instruction >> 14) & 0b11111;
        uint32_t funct3 = (instruction >> 12) & 0b111;
        uint32_t imm2 = (instruction >> 7) & 0b11111;
        uint32_t imm = ((imm1 >> 7) << 31) | (imm2 & 1) << 30 | (imm1 && 0b111111) << 23 | (imm2 >> 1);

        // beq
        if (funct3 == 0x0 && rs1 == rs2) {
            memoire->pc = imm;
        }
        // bne
        if (funct3 == 0x1 && rs1 != rs2) {
            memoire->pc = imm;
        }
        // blt
        if (funct3 == 0x4 && rs1 < rs2) {
            memoire->pc = imm;
        }
        // bge
        if (funct3 == 0x5 && rs1 >= rs2) {
            memoire->pc = imm;
        }
        break;
    }
    
    // jal
    case 0b1101111:
    {
        uint32_t rd = (instruction >> 7) & 0b11111;
        uint32_t imm = (instruction >> 12);

        // remise en ordre de l'immediat
        uint32_t new_imm = (instruction >> 31) | ((instruction >> 21) & 0b1111111111) | ((instruction >> 19) & 0b1) | ((instruction >> 12) & 0b11111111);
        
        memoire->registres[rd] = memoire->pc + 4;
        memoire->pc += new_imm;
        
        break;
    }
    
    default:
        break;
    }
}

uint32_t lectureInstruction(Memoire *memoire) {
    uint32_t instruction = 0;
    if ((memoire->pc % 2) == 0) {
        instruction = memoire->memoire[memoire->pc] & 0xffffffff;
    }
    else {
        instruction = memoire->memoire[memoire->pc] >> 32;
    }
    return instruction;
}


int main(int argc, char **argv)
{
    /* ->/riscv-emulator <HEX INPUT> <EMULATION OUTPUT> */
    if(argc != 3) {
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

    // lecture dans la memoire 
    instruction = lectureInstruction(&memoire);

    while (instruction != 0) {
        //uint32_t inst;
        //uint32_t arguments[3];
        // lecture
        executeInstruction(instruction, &memoire);
        memoire.pc = memoire.pc + 1;
        instruction = lectureInstruction(&memoire);
    }
    
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
