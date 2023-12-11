#include <stdint.h>
uint32_t operationR(char *instruction, int op1, int op2, int op3);
uint32_t operationI(char *instruction, char* op1, char* op2, char*  op3);
uint32_t operationS(char *instruction, int op1, int op2, int op3);
uint32_t operationB(char *instruction, int op1, int op2, int op3);
uint32_t operationJ(char *instruction, int op1, int op2);
uint32_t operationj(int op1);
uint32_t operationli(char* op1, char* op2);
uint32_t operationmv(char* op1, char* op2);