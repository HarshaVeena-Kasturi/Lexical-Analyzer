#include <stdio.h>
#include "functions.h"
#include <ctype.h>
#include <string.h>


//------------------------------------------------------------
// Function Name : printToken()
// Purpose       : Common helper to print every token in a
//                 fixed-width aligned format so that the ':'
//                 always appears in the same column.
//------------------------------------------------------------


void printToken(const char *type, const char *value)
{
    printf("%-25s : %s\n", type, value);
}


//------------------------------------------------------------
// Function Name : processFile()
// Purpose       : Reads the input file one character at a time
//                 until End Of File (EOF).
//------------------------------------------------------------


void processFile(void)
{
    int ch;

    /*
        Why int?
        fgetc() returns an integer.
        It can return
            0 - 255  -> valid ASCII character
            EOF(-1)  -> End Of File
        If we use char,
        EOF may not be detected correctly.
         Read characters until EOF
    */
    

    while((ch = fgetc(fp)) != EOF)
    {
        if(isalpha(ch)|| ch == '_')                  //checks if the chaaracter is an alphabet
        {
        
          readIdentifier(ch);
        }
        else if (isdigit(ch))                       //checks if the character is a digit
        {
           readNumber(ch);
        }
        else if (isspace(ch))                        //checks if the character is a whitespace
        {
            continue;
        }

        else if(ch=='(' || ch==')' ||
        ch=='{' || ch=='}' ||
        ch=='[' || ch==']' ||
        ch==';' || ch==',')
        {
            readDelimiter(ch);
        }
        else if(ch == '"')
        {
           readStringLiteral();
        }
        else if(ch == '\'')
        {
           readCharacterLiteral();
        }
        
        else if(ch == '#')
{
    readPreprocessor(ch);
}
        else                                          // Remaining characters are special characters
        {
            readOperator(ch);
        }
 
    }
}

//------------------------------------------------------------
// Function Name : readIdentifier()
// Purpose       : Reads an identifier or keyword from file
// Input         : First character of the identifier
//------------------------------------------------------------

void readIdentifier(int ch)
{
    char buffer[100];
    int i = 0;                                      // Store the first character received from processFile()
    buffer[i++] = ch;
    while((ch = fgetc(fp)) != EOF)                 // Read remaining characters
    {
        /*
            Valid identifier characters:
            - Letter
            - Digit
            - Underscore
        */ 

        if(isalnum(ch) || ch == '_')
        {
            buffer[i++] = ch;
        }
        else
        {
            /*
                This character does not belong to the
                identifier.

                Put it back into the input stream because
                processFile() must process it later.
            */

            ungetc(ch, fp);
            break;
        }
    }
    if(ch==EOF)
    {
        printf("Error : Unterminated String Literal\n");                // Null terminate the string
    }
    buffer[i] = '\0';
    if(isKeyword(buffer))
    {
        printToken("Keyword", buffer);
    }
    else
    {
        printToken("Identifier", buffer);
    }
}


//------------------------------------------------------------
// List of C keywords
//------------------------------------------------------------


char *keywords[] =
{
    "auto",     "break",    "case",      "char",
    "const",    "continue", "default",   "do",
    "double",   "else",     "enum",      "extern",
    "float",    "for",      "goto",      "if",
    "int",      "long",     "register",  "return",
    "short",    "signed",   "sizeof",    "static",
    "struct",   "switch",   "typedef",   "union",
    "unsigned", "void",     "volatile",  "while"
};


//------------------------------------------------------------
// Function Name : isKeyword()
// Purpose       : Checks whether the given token
//                 is a C keyword.
// Input         : Character array
// Output        : 1 -> Keyword
//                 0 -> Identifier
//------------------------------------------------------------


int isKeyword(char *buffer)
{ 
    int i;                                                      // Total number of keywords
    int size = sizeof(keywords) / sizeof(keywords[0]);          //256/8=32 
    for(i = 0; i < size; i++)                                   // Compare buffer with every keyword
    {
        if(strcmp(buffer, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}


//------------------------------------------------------------
// Function Name : readNumber()
// Purpose       : Reads integer or floating-point constants
//------------------------------------------------------------


void readNumber(int ch)
{
    char buffer[100];
    int i = 0;
    int dotCount = 0;
    buffer[i++] = ch;                                  // Store first digit
    while((ch = fgetc(fp)) != EOF)
    {

        if(isdigit(ch))                               // Keep reading digits
        {
            buffer[i++] = ch;
        }
        else if(ch == '.')                           // Allow one decimal point
        {
            dotCount++;
            if(dotCount > 1)
            {
                printf("Invalid Number\n");
                break;
            }
            buffer[i++] = ch;
        }
        else
        {
            ungetc(ch, fp);                       // Next token begins here
            break;
        }
    }
    buffer[i] = '\0';
    if(dotCount == 0)
    {
        printToken("Integer Constant", buffer);
    }
    else
    {
        printToken("Floating Constant", buffer);
    }
}


//------------------------------------------------------------
// Function Name : readOperator()
// Purpose       : Detects single and double character operators
//------------------------------------------------------------


void readOperator(int ch)
{
    int next;
    switch(ch)
    {
        case '=':
        next = fgetc(fp);
        if(next == '=')
        {
            printToken("Relational Operator", "==");
        }
        else
        {
            ungetc(next, fp);
            printToken("Assignment Operator", "=");
        }
        break;

        case '+':
        next = fgetc(fp);
        if(next == '+')
        {
            printToken("Increment Operator", "++");
        }
        else if(next == '=')
        {
            printToken("Assignment Operator", "+=");
        }
        else
        {
            ungetc(next, fp);
            printToken("Arithmetic Operator", "+");
        }
        break;

        case '-':
        next = fgetc(fp);
        if(next == '-')
        {
            printToken("Decrement Operator", "--");
        }
        else if(next == '=')
        {
            printToken("Assignment Operator", "-=");
        }
        else
        {
            ungetc(next, fp);
            printToken("Arithmetic Operator", "-");
        }
        break;


        case '<':                                          
        next = fgetc(fp);
        if(next == '=')
        {
            printToken("Relational Operator", "<=");
        }
         else
        {
            ungetc(next, fp);
            printToken("Relational Operator", "<");
        }
        break;

        case '>':
        next = fgetc(fp);
        if(next == '=')
        {
            printToken("Relational Operator", ">=");
        }
          else
        {
            ungetc(next, fp);
            printToken("Relational Operator", ">");
        }
         break;

        case '!':
        next = fgetc(fp);
        if(next == '=')
        {
            printToken("Relational Operator", "!=");
        }
        else
        {
            ungetc(next, fp);
            printToken("Logical Operator", "!");
        }
        break;
        
        case '&':
        next = fgetc(fp);
        if(next == '&')
        {
            printToken("Logical Operator", "&&");
        }
        else
        {
            ungetc(next, fp);
            printToken("Bitwise Operator", "&");
        }
        break;
        
        case '|':
        next = fgetc(fp);
        if(next == '|')
        {
            printToken("Logical Operator", "||");
        }
        else
        {
            ungetc(next, fp);
            printToken("Bitwise Operator", "|");
        }
        break;
        
        case '*':
        next = fgetc(fp);
        if(next == '=')
        {
            printToken("Assignment Operator", "*=");
        }
        else
        {
            ungetc(next, fp);
            printToken("Arithmetic Operator", "*");
        }
        break;
        
        case '/':
        {
            next = fgetc(fp);
            if(next == '/')
            {
                readComment(1);
            }
            else if(next == '*')
            {
                readComment(2);
            }
            else if(next == '=')
            {
                printToken("Assignment Operator", "/=");
            }
            else
            {
                ungetc(next, fp);
                printToken("Arithmetic Operator", "/");
            }
            break;
        }
        
        case '%':
        next = fgetc(fp);
        if(next == '=')
        {
            printToken("Assignment Operator", "%=");
        }
        else
        {
            ungetc(next, fp);
            printToken("Arithmetic Operator", "%");
        }
        break;

        case '^':
        printToken("Bitwise Operator", "^");
        break;
        
        case '~':
        printToken("Bitwise Operator", "~");
        break;

        default:
        {
            char temp[2];
            temp[0] = (char)ch;
            temp[1] = '\0';
            printToken("Invalid Token", temp);
        }
        break;
    }
}


//------------------------------------------------------------
// Function Name : readDelimiter()
// Purpose       : Identifies delimiters and separators
//------------------------------------------------------------


void readDelimiter(int ch)
{
    switch(ch)
    {
        case '(':
            printToken("Delimiter", "(");
            break;

        case ')':
            printToken("Delimiter", ")");
            break;

        case '{':
            printToken("Delimiter", "{");
            break;

        case '}':
            printToken("Delimiter", "}");
            break;

        case '[':
            printToken("Delimiter", "[");
            break;

        case ']':
            printToken("Delimiter", "]");
            break;

        case ';':
            printToken("Delimiter", ";");
            break;

        case ',':
            printToken("Separator", ",");
            break;
    }
}


//------------------------------------------------------------
// Function Name : readComment()
// Purpose       : Reads comments
//------------------------------------------------------------


void readComment(int type)
{
    int ch;
    if(type == 1)
    {
        while((ch = fgetc(fp)) != EOF)
        {
            if(ch == '\n')
            {
                break;
            }
        }
        printToken("Single Line Comment", "");
    }
    else if(type == 2)                                 // Multi-line comment
    {
        int prev = 0;
        int closed = 0;                              // Flag to check whether comment is closed
        while((ch = fgetc(fp)) != EOF)
        {
            if(prev == '*' && ch == '/')
            {
                closed = 1;
                break;
            }
            prev = ch;
        }
        if(closed)
        {
            printToken("Multi Line Comment", "");
        }
        else
        {
            printf("Error : Unterminated Multi-line Comment\n");
        }
    }
}


//------------------------------------------------------------
// Function Name : readStringLiteral()
// Purpose       : Reads a string enclosed in double quotes
//------------------------------------------------------------


void readStringLiteral(void)
{
    char buffer[200];
    int i = 0;
    int ch;
    while((ch = fgetc(fp)) != EOF)
    {
        if(ch == '"')
        {
            break;
        }
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    {
        char temp[204];
        sprintf(temp, "\"%s\"", buffer);
        printToken("String Literal", temp);
    }
}


//------------------------------------------------------------
// Function Name : readCharacterLiteral()
// Purpose       : Reads a character enclosed in single quotes
//------------------------------------------------------------


void readCharacterLiteral(void)
{
    char buffer[10];
    int i = 0;
    int ch;
    while((ch = fgetc(fp)) != EOF)
    {
        if(ch == '\'')
        {
            break;
        }
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    {
        char temp[14];
        sprintf(temp, "'%s'", buffer);
        printToken("Character Literal", temp);
    }
}


//------------------------------------------------------------
// Function Name : readPreprocessor()
// Purpose       : Reads an entire preprocessor directive
//------------------------------------------------------------


void readPreprocessor(int ch)
{
    char buffer[200];
    int i = 0;
    buffer[i++] = ch;
    while((ch = fgetc(fp)) != EOF)
    {
        if(ch == '\n')
        {
            break;
        }
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    printToken("Preprocessor Directive", buffer);
}