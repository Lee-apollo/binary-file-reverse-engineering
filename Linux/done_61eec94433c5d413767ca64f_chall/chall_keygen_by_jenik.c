// let's crack Chall
// https://crackmes.one/crackme/61eec94433c5d413767ca64f

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* 
  Description:
  Analysis of the chall binary shows that following rules are applied on the user provided password.
  Each byte of the provided password is checked whether it is a prime number or not.
  It it is prime number, that this value is shifted right by 1 bit (and therefore divided by 2).
  If it is not a prime number, then the number is shifted right by 4 bits.
  Result value is stored into an array of integers.
  After this our calculated value is compared with a hardcoded expected value.


  Password generator:
    We have to reverse right shift operations.
    But the problem is that the number might have been shifted right by 1 bit or by 4 bits. We have to find out.

    Original number might be a prime number and therefore it was shifted by 1 bit, let's try to reconstruct it.
    As all prime numbers must be an odd numbers, we have to add +1.
    char T = ((E & 0xFF) << 1) + 1;

    where
    T = plaintext value
    E = encrypted value

    if T is a prime number and it is printable character, we have found it.
    IF these conditions are not met, we have to find a number which is NOT a prime number, is printable and if shifted right by 4 bits, creates our encrypted value E.

    for (int j = 0; j < 0xF; j++)
    {
        tmpT = ((E & 0xFF) << 4) + j;
        if (isPrimeNumber(new_char))
        {
            continue;
        }

        if (isprint(new_char))
        {
            T = tmpT;
            break;
        }
    }

    Repeat this for all values in hardcoded array of expected values.

    Generated password:
        Password (decimal): 96, 96, 97, 103, 112, 112, 48, 112, 101, 96, 48, 80, 107, 48, 80, 112, 112, 112, 96, 109, 112, 80, 96, 96, 48, 80, 48, 112, 97, 112, 
        Password (hex): \x60\x60\x61\x67\x70\x70\x30\x70\x65\x60\x30\x50\x6b\x30\x50\x70\x70\x70\x60\x6d\x70\x50\x60\x60\x30\x50\x30\x70\x61\x70
        Password (string): "``agpp0pe`0Pk0Pppp`mpP``0P0pap"

/************************************************************
 * Information extracted from the binary using ghidra - START
 ************************************************************/

// Check if number is prime number
int isPrimeNumber(int inputNumber)
{
    int local_c;

    local_c = 2;
    while (1)
    {
        if (inputNumber < local_c * local_c)
        {
            return 1;
        }
        if (inputNumber % local_c == 0) break;
        local_c = local_c + 1;
    }
    return 0;
}

// Compare two arrays of ints
char areIntArraysSame(char *param_1,char *param_2,int length)
{
    int local_c;
    
    local_c = 0;
    while( 1 )
    {
        if (length <= local_c)
        {
            return 1;
        }
        if (*(int *)(param_1 + (long)local_c * 4) != *(int *)(param_2 + (long)local_c * 4)) break;
        local_c = local_c + 1;
    }
    return 0;
}

// Final required value. Is compared to the result of our calculation.
int required_final_array[] = {0x06 , 0x06, 0x30, 0x33,
 0x07 , 0x07, 0x03, 0x07,
 0x32 , 0x06, 0x03, 0x05,
 0x35 , 0x03, 0x05, 0x07,
 0x07 , 0x07, 0x06, 0x36,
 0x07 , 0x05, 0x06, 0x06,
 0x03 , 0x05, 0x03, 0x07,
 0x30 , 0x07};


/************************************************************
 * Tools
 ************************************************************/

#define ARRAY_SIZE(x)(sizeof(x) / sizeof(x[0]))

void diffIntBuffers(int * p1, int * p2, int size)
{
    printf("Int buffer: ");
    for (int i = 0; i < size; i++)
    {
        if (p1[i] != p2[i])
        {
            printf("Diff on location %d: 0x%08x, 0x%08x\n", i, p1[i], p2[i]);
        }
    }
}


/******************************
 * Main magic takes place here
 ******************************/

void generatePassword(char outputBuffer[30])
{
    char password[30] = {0};

    for (int i = 0; i < ARRAY_SIZE(password); i++)
    {

        // We have to reverse right shift operation
        // Original number might be a prime number, let's try to reconstruct it.
        // As all prime numbers must be an odd numbers, we have to add +1.
        // My assumption is that left shift is allways creating zeros on the right (least significant bit), but I am too lazy to verify it now. But it works!
        char new_char = ((required_final_array[i] & 0xFF) << 1) + 1;

        // DEBUGGING - Flag whether a number was found 
        int found = 0;

        if ( isPrimeNumber(new_char))
        {
          // We have found a suitable number, but it also has to be printable. Some non printable characters might terminate scanf function.
          if (isprint(new_char))
          {
            printf("INFO: index: %d - Selected prime number 0x%02x .\n", i, new_char);
            found = 1;
            password[i] = new_char;
            continue;
          }
          else
          {
            printf("index: %d - Number 0x%02x is prime, but is not printable\n", i, new_char);
          }
        }

        // Suitable number was not found yet
        for (int j = 0; j < 0xF; j++)
        {
          // Let's find a number which is NOT a prime number, is printable and if shifted right by 4 bits, creates required value.
          new_char = ((required_final_array[i] & 0xFF) << 4) + j;
          if (isPrimeNumber(new_char))
          {
            //  TODO Isn't this branch useless?
            printf("ERROR: index: %d - Number 0x%02x should not be prime, but it is!\n", i, new_char);
            continue;
          }

          if (isprint(new_char))
          {
            printf("INFO: index: %d - Selected NON-prime number 0x%02x .\n", i, new_char);
            found = 1;
            password[i] = new_char;
            break;
          }
        }

        if (found == 0)
        {
            printf("ERROR: Suitable printable character was not found! Is this even possible?\n");
        }

    }

    printf("Password (decimal): ");
    for (int i = 0; i < ARRAY_SIZE(password); i++)
    {
        printf("%d, ", password[i]);
    }
    printf("\n");

    printf("Password (hex): ");
    for (int i = 0; i < ARRAY_SIZE(password); i++)
    {
        printf("\\x%02x", password[i]);
    }
    printf("\n");

    printf("Password (string): \"%.30s\"\n", password);

    memcpy(outputBuffer, password, sizeof(password));
}


// Main logic of this function was extracted from the binary file
void verifyPassword(char* password)
{
    // Verify our password
    int tmpBuffer[30];

    for (int i = 0; i < 0x1e; i = i + 1)
    {
        if (isPrimeNumber(password[i]) == 0)
        {
            tmpBuffer[i] = (int)(password[i] >> 4);
        }
        else
        {
            tmpBuffer[i] = (int)(password[i] >> 1);
        }
    }

    // Dirty casting just to preserve function signature as detected by ghidra.
    if (areIntArraysSame((char*)tmpBuffer, (char*)required_final_array, 0x1e))
    {
        puts("correct");

    } else {
        puts("wrong");

        // Something went wrong - let's diff those buffers
        diffIntBuffers(tmpBuffer, required_final_array, 30);
    }
}


int main()
{
    // Local buffer to store generated password
    char password[40] = {0};

    generatePassword(password);

    verifyPassword(password);
}
