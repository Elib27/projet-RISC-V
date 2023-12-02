#include <stdint.h>

void convertionDecimalBinaire(char *sortie, int taille, int entree);
uint32_t operationR(char *instruction, int op1, int op2, int op3);
uint32_t operationI(char *instruction, int op1, int op2, int op3);
uint32_t operationS(char *instruction, int op1, int op2, int op3);
uint32_t operationB(char *instruction, int op1, int op2, int op3);
uint32_t operationJ(char *instruction, int op1, int op2);