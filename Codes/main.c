#include <stdio.h>                                       // Standard Input/Output library
#include "functions.h"                                   // User-defined header file
FILE *fp;                                                // Global file pointer
                                                         // It is defined here and will be accessed in other files using 'extern'
int main()
{
    fp = fopen("test.c", "r");                          // Open the source file in read mode
    if(fp == NULL)                                      // Check whether file is opened successfully
    {
        printf("Error : Unable to open file.\n");
        return 1;                                      // Return non-zero value to indicate error
    }
    processFile();                                     // Start lexical analysis
    fclose(fp);                                        // Close the file to release system resources
    return 0;                                          // Successful program execution
}