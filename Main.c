#include <stdio.h>
#include <math.h>

unsigned int findOp(unsigned int instruction);
unsigned int findVar(int start, int end, unsigned int instruction);
void iInst(unsigned int opcBin, unsigned int instruction, unsigned int address, unsigned int nextAddress, int *loadCount, int *storeCount, int *arithCount, int *fwdCount, int *bkdCount, int *notCount, int *regReads, int *regWrites, int last);
void jInst(unsigned int opcBin, unsigned int instruction, unsigned int address, unsigned int nextAddress, int *notCount, int *fwdCount, int *bkdCount, int *regReads, int *regWrites, int last);
void rInst(unsigned int opcBin, unsigned int instruction, unsigned int address, unsigned int nextAddress, int *arithCount, int *fwdCount, int *bkdCount, int *notCount, int *regReads, int *regWrites, int last);

int main()
{
    // Declare variables for reading in file
    FILE *fptrRead;
    FILE *fptrWrite;
    unsigned int input;

    // Open the file
    fptrRead = fopen("trace.txt", "r");
    fptrWrite = fopen("statistics.txt", "w");
    // Check if the file was opened successfully
    if (fptrRead == NULL)
    {
        printf("Error: Could not open trace file. \n");
        return 1;
    }
    if (fptrWrite == NULL)
    {
        printf("Error: Could not open statistics file.\n");
        return 1;
    }
    // Read characters line by line until the end of the file
    int last = 0;
    int count = 0;
    unsigned int opcBin = 0;
    unsigned int address;
    unsigned int instruction;
    unsigned int nextAddress;
    unsigned int nextInstruction;
    int rCount = 0;
    int jCount = 0;
    int iCount = 0;
    int loadCount = 0;
    int storeCount = 0;
    int arithCount = 0;
    int fwdCount = 0;
    int bkdCount = 0;
    int notCount = 0;
    int regReads[32] = {0};
    int regWrites[32] = {0};

    // Read in the first line
    fscanf(fptrRead, "%x %x", &address, &instruction) == 2;

    while ((fscanf(fptrRead, "%x %x", &nextAddress, &nextInstruction) == 2 || last == 0))
    {

        if (nextAddress == address)
        {
            last = 1;
        }

        opcBin = findOp(instruction);
        count++;

        if (opcBin == 0)
        {
            rCount++;
            rInst(opcBin, instruction, address, nextAddress, &arithCount, &fwdCount, &bkdCount, &notCount, regReads, regWrites, last);
        }
        else if (opcBin == 2 || opcBin == 3)
        {
            jCount++;
            jInst(opcBin, instruction, address, nextAddress, &notCount, &fwdCount, &bkdCount, regReads, regWrites, last);
        }
        else
        {
            iCount++;
            iInst(opcBin, instruction, address, nextAddress, &loadCount, &storeCount, &arithCount, &fwdCount, &bkdCount, &notCount, regReads, regWrites, last);
        }
        // printf("Here is the nextAddress: %u", nextAddress);
        address = nextAddress;
        instruction = nextInstruction;
    }

    fprintf(fptrWrite, "insts: %d\n", count);
    fprintf(fptrWrite, "r-type: %d\n", rCount);
    fprintf(fptrWrite, "i-type: %d\n", iCount);
    fprintf(fptrWrite, "j-type: %d\n", jCount);
    fprintf(fptrWrite, "fwd-taken: %f\n", (double)fwdCount / count * 100);
    fprintf(fptrWrite, "bkw-taken: %f\n", (double)bkdCount / count * 100);
    fprintf(fptrWrite, "not-taken: %f\n", (double)notCount / count * 100);
    fprintf(fptrWrite, "loads: %f\n", (double)loadCount / count * 100);
    fprintf(fptrWrite, "stores: %f\n", (double)storeCount / count * 100);
    fprintf(fptrWrite, "arith: %f\n", (double)arithCount / count * 100);
    for (int i = 0; i < 32; i++)
    {
        fprintf(fptrWrite, "reg-%d: %d %d\n", i, regReads[i], regWrites[i]);
    }
    fclose(fptrRead);
    fclose(fptrWrite);
    return 0;
}

void rInst(unsigned int opcBin, unsigned int instruction, unsigned int address, unsigned int nextAddress, int *arithCount, int *fwdCount, int *bkdCount, int *notCount, int *regReads, int *regWrites, int last)
{
    unsigned int rs = findVar(25, 21, instruction);
    unsigned int rt = findVar(20, 16, instruction);
    unsigned int rd = findVar(15, 11, instruction);
    unsigned int funct = findVar(5, 0, instruction);
    // Check all of the r-type opCode instructions
    // printf("THIS IS THE FUNCT: %u", funct);
    switch (funct)
    {
    case 32:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        // printf("arithCount: %d", *arithCount);
        break;
    case 33:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 36:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 39:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 37:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 42:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 43:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 0:
        regWrites[rd]++;
        regReads[rt]++;
        (*arithCount)++;
        break;
    case 3:
        regWrites[rd]++;
        regReads[rt]++;
        (*arithCount)++;
        break;
    case 2:
        regWrites[rd]++;
        regReads[rt]++;
        (*arithCount)++;
        break;
    case 34:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 35:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 38:
        regWrites[rd]++;
        regReads[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 8:
        // jr
        if (last == 1)
        {
            break;
        }
        if (nextAddress < address)
        {
            (*bkdCount)++;
        }
        else if (nextAddress == address + 4)
        {
            (*notCount)++;
        }
        else if (nextAddress > address + 4)
        {
            (*fwdCount)++;
        }
        regReads[rs]++;
        break;
    case 12:
        // syscall idk
        (*arithCount)++;
        break;
    }
}
void jInst(unsigned int opcBin, unsigned int instruction, unsigned int address, unsigned int nextAddress, int *notCount, int *fwdCount, int *bkdCount, int *regReads, int *regWrites, int last)
{
    switch (opcBin)
    {
    case 2:
        if (last == 1)
        {
            break;
        }
        if (nextAddress < address)
        {
            (*bkdCount)++;
        }
        else if (nextAddress > address)
        {
            (*fwdCount)++;
        }
        break;
    case 3:
        if (last == 1)
        {
            break;
        }
        if (nextAddress < address)
        {
            (*bkdCount)++;
        }
        else if (nextAddress > address)
        {
            (*fwdCount)++;
        }
        break;
    }
}
void iInst(unsigned int opcBin, unsigned int instruction, unsigned int address, unsigned int nextAddress, int *loadCount, int *storeCount, int *arithCount, int *fwdCount, int *bkdCount, int *notCount, int *regReads, int *regWrites, int last)
{
    unsigned int rs = findVar(25, 21, instruction);
    unsigned int rt = findVar(20, 16, instruction);
    unsigned int imm = findVar(15, 0, instruction);
    // Check all of the memory instructions and update accordingly
    // printf("I'M KIND OF IN THE DOMAIN WITH AN OPCODE OF %u", opcBin);

    switch (opcBin)
    {
    case 8:
        regWrites[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 9:
        regWrites[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 12:
        regWrites[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 15:
        regWrites[rt]++;
        (*arithCount)++;
        break;
    case 13:
        regWrites[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 10:
        regWrites[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 11:
        regWrites[rt]++;
        regReads[rs]++;
        (*arithCount)++;
        break;
    case 35:
        regReads[rs]++;
        regWrites[rt]++;
        (*loadCount)++;
        break;
    case 36:
        regReads[rs]++;
        regWrites[rt]++;
        (*loadCount)++;
        break;
    case 37:
        regReads[rs]++;
        regWrites[rt]++;
        (*loadCount)++;
        break;
    case 40:
        regReads[rt]++;
        regReads[rs]++;
        (*storeCount)++;
        break;
    case 41:
        regReads[rt]++;
        regReads[rs]++;
        (*storeCount)++;
        break;
    case 43:
        regReads[rt]++;
        regReads[rs]++;
        (*storeCount)++;
        break;
    case 4:
        if (last == 1)
        {
            break;
        }
        if (nextAddress < address)
        {
            (*bkdCount)++;
        }
        else if (nextAddress == address + 4)
        {
            (*notCount)++;
        }
        else if (nextAddress > address + 4)
        {
            (*fwdCount)++;
        }
        regReads[rt]++;
        regReads[rs]++;
        break;
    case 5:
        if (last == 1)
        {
            break;
        }
        if (nextAddress < address)
        {
            (*bkdCount)++;
        }
        else if (nextAddress == address + 4)
        {
            (*notCount)++;
        }
        else if (nextAddress > address + 4)
        {
            (*fwdCount)++;
        }
        regReads[rt]++;
        regReads[rs]++;
        break;
    }
}
// put in the branch statements

// Find the first 6 bits of the instruction to get the operation type and opcode
unsigned int findOp(unsigned int instruction)
{
    unsigned int opcBin = 0;
    // Bitwise shift 26 bits and mask the remaining 6 bits
    opcBin = (instruction >> 26) & 0x3f;
    // printf("This is the decimal: %u\n", opcBin);
    return opcBin;
}
// Find the specified bits
unsigned int findVar(int start, int end, unsigned int instruction)
{
    unsigned int val = 0;
    // Bitwise shift until the starting point and then mask the required number of bits
    val = (instruction >> end & (1 << (start - end + 1)) - 1);
    // printf("This is the variable: %u\n", val);
    return val;
}