int getInstructionFromLine(char *line, char instruction[5], char arguments[3][5]);
int getRegisterWithAlias(char *alias);
int stringToInt(char *string);
char getIntructionType(char *instruction);
void convertBinIntructionToHexIntruction(char hexInstruction[], char binInstruction[]);