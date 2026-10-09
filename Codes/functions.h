
#ifndef FUNCTIONS_H                  // Header guard begins
#define FUNCTIONS_H
#include <stdio.h>                   // 'extern' tells the compiler that the variable
                                     // is defined in another source file (main.c)

extern FILE *fp;
                                     // Function prototype
void processFile(void);
void readIdentifier(int ch);
int isKeyword(char *buffer);
void readNumber(int ch);
void readOperator(int ch);
void readDelimiter(int ch);
void readComment(int type);
void readStringLiteral(void);
void readCharacterLiteral(void);
void readPreprocessor(int ch);
#endif                                // Header guard ends