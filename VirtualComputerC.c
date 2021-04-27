#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

///Register array
unsigned int registersArray[10];

///Memory
int memoryArray[1024];

///Compare flag
bool compareFlag = false;

///3 Arrays created to store Command, Operand1 and Operand2
int CommandArray[100];
char operand1Array[100][30];
char operand2Array[100][30];

typedef enum {MOVE=1, STORE=2, LOAD=3, INPUT=4, OUTPUT=5, ADD=6, XOR=7, TEST=8, JUMP=9,JUMPEQ=10, JUMPNEQ=11, EXIT=12} COMMANDS;

int current_instr_index = 0;
int next_instr_index = 0;

int read_file()
{
    ///Open the file to read the instructions
    FILE *fPointer;

    if ((fPointer = fopen("comp_prog.bin", "r")) == NULL)
    {
        printf("File does not exist");
        return -1;
    }

    ///An array is created to store all the instructions
    char textInFile[57][20];

    ///New array to be read the lines in the file line by line
    char readLine[20];

    int i = 0;
    ///While you're not at the end of the file keep reading it
    while (fgets(readLine, 20, fPointer)!=NULL)
    {
        char* token1=" ";
        char* token2=" ";
        char* token3=" ";

        printf("Read Line %d %s",i, readLine);
        if (strcasecmp(readLine,"EXIT\n")==0)
        {
            token1 = "EXIT";
            token2 = NULL;
            token3=NULL;
        }
        else
        {

            token1 = strtok (readLine, " ,");
            token2 = strtok (NULL, " ,");
            token3 = strtok (NULL, " ,");
        }
        //printf("fn: %d %d %s %s ",i,token1, token2, token3);

        if (strcasecmp(token1, "MOVE")==0)
            CommandArray[i] = MOVE;
        if (strcasecmp(token1, "STORE")==0)
            CommandArray[i] = STORE;
        if (strcasecmp(token1, "LOAD")==0)
            CommandArray[i] = LOAD;
        if (strcasecmp(token1, "INPUT")==0)
            CommandArray[i] = INPUT;
        if (strcasecmp(token1, "OUTPUT")==0)
            CommandArray[i] = OUTPUT;
        if (strcasecmp(token1, "ADD")==0)
            CommandArray[i] = ADD;
        if (strcasecmp(token1, "XOR")==0)
            CommandArray[i] = XOR;
        if (strcasecmp(token1, "TEST")==0)
            CommandArray[i] = TEST;
        if (strcasecmp(token1, "JUMP")==0)
            CommandArray[i] = JUMP;
        if (strcasecmp(token1, "JUMPNEQ")==0)
            CommandArray[i] = JUMPNEQ;
        if (strcasecmp(token1, "JUMPEQ")==0)
            CommandArray[i] = JUMPEQ;
        if (strcasecmp(token1, "EXIT")==0)
            CommandArray[i] = EXIT;

        if (token2!=NULL)
            strcpy(operand1Array[i], token2);
        else
            operand1Array[i][0] = '\0';
        if (token3!=NULL)
            strcpy(operand2Array[i], token3);
        else
            operand2Array[i][0] = '\0';

        printf("The CommandArray array: %d \n", CommandArray[i]);
        printf("The operand1Array array: %s \n", operand1Array[i]);
        printf("The operand2Array array: %s \n", operand2Array[i]);
        i++;
    }

    ///Close the file
    fclose(fPointer);

    /// return the number of commands read
    return i-1;
}

int execute_cmds(int inst_count)
{
    printf("Commands to execute %d",inst_count);
    current_instr_index = 0;

    while(current_instr_index <= inst_count)
    {
        printf("\n Current cmd %d\n",current_instr_index);
        ///Iterate the index by 1 each time
        next_instr_index = current_instr_index + 1;
        switch(CommandArray[current_instr_index])
        {
        case MOVE:
            moveFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case STORE:
            storeFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case INPUT:
            inputFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case OUTPUT:
            outputFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case ADD:
            addFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case XOR:
            xorFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case TEST:
            testFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case JUMP:
            jumpFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case JUMPEQ:
            jumpeqFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case JUMPNEQ:
            jumpneqFunction(operand1Array[current_instr_index], operand2Array[current_instr_index]);
            break;
        case EXIT:
            //No Arguments
            exitFunction();
            break;
        }

        current_instr_index = next_instr_index;
    }
}


///Checks to see if the operand has an R in it
///No R means it's a constant
bool isRegister(char* op)
{
    int registerLetter = 'R';
    if (strchr(op, registerLetter)==NULL)
        return false;
    else
        return true;
}

///This parses the Register and gets the second character (1 from R1 etc)
int parsereg(char* reg)
{
    return atoi(reg+1);
}

int moveFunction(char* param1, char* param2)
{
    printf("\nMove %s %s",param1,param2);
    ///If operand/parameter2 has an R then do..
    if (isRegister(param2)==true)
    {
        ///copy register into another register
        ///Find the index of the register using a parse/readline (indexes are from 0 to 9)
        int k = parsereg(param1);
        int j = parsereg(param2);
        printf("MOVE: Register %d index %d\n",k,j);

        registersArray[k] = registersArray[j];

        printf("Value in Register %s: %d\n",param1, registersArray[k]);
    }

    ///If operand/parameter2 does not have an R then its a Constant so do..
    else
    {
        int k = parsereg(param1);
        int j = atoi(param2);
        printf("MOVE: Register %d index %d\n",k,j);
        registersArray[k] = j;

        printf("Value in Register %s: %d\n",param1, registersArray[k]);
    }
}


int storeFunction(char* param1, char* param2)
{
    printf("\nStore %s %s",param1,param2);
    ///Check if param1 is a constant
    if (isRegister(param2)==false)
    {
        ///Not a register
        int k = parsereg(param1);
        int j = atoi(param2);

        printf("STORE: Register %d constant %d\n", k, j);

        ///Add j (Int of Param2) to MemoryArray
        int i = 0;
        memoryArray[i] = j;
        printf("Value stored in MemoryArray is: %d\n",memoryArray[i]);

//        int memoryArray[i];
//        int* addressOfArrayElement = &memoryArray[i];
//
//        printf("Value of %p", addressOfArrayElement);

        //int memoryArray[i];
        int* param1 = &memoryArray[i];

        printf("Memory address stored in Register %d: %p\n", k, param1);

    }
    else
    {
        printf("Error. The Register can only store a constant");
    }
}


int addFunction(char* param1, char* param2)
{
    printf("\nAdd %s %s",param1,param2);

    //No R in param2
    if (isRegister(param2)==false)
    {
        //Add constant to value at register
        int k = parsereg(param1);
        int j = atoi(param2);
        printf("ADD: Register %d constant %d \n",k,j);
        printf("Value in Register %d before adding constant is: %d\n", k, registersArray[k]);

        int addValues = j + registersArray[k];
        registersArray[k] = addValues;

        printf("Value in Register %d after adding constant is: %d\n", k, addValues);
    }

    ///If operand/parameter2 does not have an R then its a Constant so do..
    else
    {
        printf("Error");
    }
}

int inputFunction(char* param1, char* param2)
{
    printf("\nINPUT\n");

    //No R in param2
    if (isRegister(param2)==false)
    {
        int k = parsereg(param1);
        int j = parsereg(param2);

        char line[256];
        char character;
        printf("Enter a single character:");
        if (fgets(line, sizeof line, stdin) == NULL)
        {
            printf("Error.\n");
            exit(1);
        }
        character = line[0];
        printf("Character read: %c\n", character);

        int i = 0;
        memoryArray[i] = character;
        printf("Character stored in MemoryArray: %c \n",memoryArray[i]);

        int* param1 = &memoryArray[i];

        printf("Memory address stored in Register %d: %p\n", k, param1);
    }
    else
    {
        printf("Error");
    }
}

int outputFunction(char* Param1, char* Param2)
{
    printf("\nOUTPUT %s", Param1);

    ///If Param1 is a register
    if(isRegister(Param1) == true)
    {

        //Output register
        int k = parsereg(Param1);
        int j = parsereg(Param2);

        printf("Contents of %s%d", Param1,registersArray[k]);
    }
    ///If Param1 is a constant
    else
    {
        //Output constant
        int k = parsereg(Param1);
        int j = atoi(Param1);

        printf("Constant: %d\n", j);
    }
}

int xorFunction(char* Param1, char* Param2)
{
    printf("\nXOR\n");

    ///Param1 and Param2 have to be Registers
    if(isRegister(Param2) == true)
    {
        int k = parsereg(Param1);
        int j = parsereg(Param2);
//        int k1 = atoi(Param1);
//        int j1 = atoi(Param2);

        printf("Contents of %s %d %s%d\n", Param1,registersArray[k],Param2,registersArray[j]);

        int xor = k ^ j;
        //printf("XOR operation on  Register %s and Register %s is: %d\n", Param1, Param2, xor);
        printf("XOR result is: %d\n", xor);

        registersArray[k] =  xor;
        printf("Exclusive value now stored in register %s: %d", Param1, registersArray[k]);
    }
    else
    {
        printf("Error. XOR requires 2 Registers\n");
    }
}

int testFunction(char* Param1, char* Param2)
{
    printf("\nTEST %s %s", Param1, Param2);
    ///If Param2 has an R/Is a Register

    if(isRegister(Param2) == true)
    {
        int k = parsereg(Param1);
        int j = parsereg(Param2);

        if(registersArray[k] == registersArray[j])
        {
            compareFlag = true;
            printf("Compare flag is %s", compareFlag?"true":"false");
        }
        else
        {
            compareFlag = false;
            printf("Compare flag is %s", compareFlag?"true":"false");
        }
    }
    else
    {
        int k = parsereg(Param1);
        int j = atoi(Param2);
        if(registersArray[k] == j)
        {
            compareFlag = true;
            printf("Compare flag is %s", compareFlag?"true":"false");
        }
        else
        {
            compareFlag = false;
            printf("Compare flag is %s", compareFlag?"true":"false");
        }
    }
}

int jumpFunction(char* Param1, char* Param2)
{
    printf("JUMP %s", Param1);
    ///Jump 37 = Go to Line 37 in text file and execute
    ///No R in param1
    if(isRegister(Param1) == false)
    {
        int k = atoi(Param1);
        printf("Next instruction is %d", k);
        next_instr_index = k;

        ///Compare flag is set to false after each jump
        compareFlag = false;
    }
    else
    {
        printf("Error. A Register cannot be jumped");
    }

}

int jumpeqFunction(char* Param1, char* Param2)
{
    printf("JUMPEQ %s", Param1);

    ///No R in param1
    if(compareFlag == true){
        int k = atoi(Param1);
        printf("Next instruction is %d", k);
        next_instr_index = k;

        ///Compare flag is set to false after each jump
        compareFlag = false;
    }
    else{
        printf("Compare Flag is false, not jumping");
    }
}

int jumpneqFunction(char* Param1, char* Param2)
{
    printf("JUMPNEQ %s", Param1);

    ///No R in param1
    if(compareFlag == false){
        int k = atoi(Param1);
        printf("Next instruction is %d", k);
        next_instr_index = k;

        ///Compare flag is set to false after each jump
        compareFlag = false;
    }
    else{
        printf("Compare Flag is true, not jumping");
    }
}

int exitFunction()
{
    printf("Exiting....bye for now");
    exit(0);
}

int main()
{
    int inst_count = read_file();

    if (inst_count >0)
        execute_cmds(inst_count);
    return 0;
}
