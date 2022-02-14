
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <iostream>


class TraceObject
{
public: 

  std::string mValue;

  TraceObject(char* string)
  {
      mValue = string;
      std::cout << "Start: " << this->mValue << std::endl;      
  }

  ~TraceObject()
  {
    std::cout << "END: " << this->mValue << std::endl;
  }
};

typedef unsigned int uint;

//#define TRACE TraceObject x = TraceObject(__FUNCTION__)

#define TRACE 
// #define TRACE printf("%s\n", __FUNCTION__)

uint ENCRYPTED[32] = {
0x32, 0x33, 0x68, 0x6B, 
0x5F, 0x5F, 0x35, 0x37, 
0x6C, 0x32, 0x5F, 0x62, 
0x77, 0x6B, 0x44, 0x32, 
0x32, 0x32, 0x33, 0x32, 
0x66, 0x6B, 0x7B, 0x51, 
0x7C, 0x61, 0x35, 0x7D, 
0x4F, 0x7C, 0x62, 0x36, 
};

uint POS[32] = 
{
  0x12, 0x1A, 0x0C, 0x1D,
  0x06, 0x19, 0x1F, 0x1B,
  0x1E, 0x0B, 0x10, 0x03,
  0x0E, 0x02, 0x01, 0x08,
  0x07, 0x0F, 0x16, 0x15,
  0x04, 0x13, 0x17, 0x18,
  0x11, 0x09, 0x05, 0x1C,
  0x0D, 0x0A, 0x00, 0x14,
};


int compare(uint * array1, uint* array2, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        if (array1[i] != array2[i])
        {
            return 0;
        }
    }
    return 1;
}


void reverse_bits_in_int(uint *param_1)
{
  TRACE;
  uint local_c;
  
  local_c = 0;
  while (*param_1 != 0) {

    local_c = local_c << 1 | *param_1 & 1;
    *param_1 = (uint)*param_1 >> 1;
  }
  *param_1 = local_c;
  return;
}

void printBits(uint value)
{    
  TRACE;
    for (int i = 0; i < 32; i++) {
        int bitValue = (value & (1 << (32 - 1 -i))) ? 1 : 0;
        printf("%u", bitValue);
    }
    puts("");
}

// Number of Set bits in int
int numberOfSetBitsInInt(uint param_1)
{
  TRACE;
  unsigned int i;
  int local_c;
  //printf("param: %u\n", param_1);
  local_c = 0;
  for (i = param_1; i != 0; i = i >> 1) {
    local_c = local_c + (i & 1);
    //printf("i: %u\n", i);
  }
  return local_c;
}


uint lichaParita(uint param_1)
{
  TRACE;
  uint local_7c;
  uint local_6c;
  
  local_6c = 0;
  for (local_7c = param_1; local_7c != 0; local_7c = local_7c & local_7c - 1) {
    local_6c = local_6c ^ 1;
  }

  return local_6c;
}


uint * basePointer = nullptr;
void set_intArray(uint * base)
{
  basePointer = base;
  printf("*************************\n");
}

void swap_int(uint *param_1,uint *param_2)
{
  //printf("SWAP index1: %d, index2: %d\n", param_1 - basePointer, param_2 - basePointer);
  TRACE;
  *param_1 = *param_1 ^ *param_2;
  *param_2 = *param_2 ^ *param_1;
  *param_1 = *param_1 ^ *param_2;
  return;
}



void encrypt(uint *intArray)
{
  TRACE;
  uint uVar1;
  int i;
  int j;
  uint local_c;

  set_intArray(intArray);

  for (i = 0; i < 4; i = i + 1) {
    //printf("i: %d\n", i);
    swap_int(intArray + POS[i * 8 + 2],intArray + POS[i * 8 + 7]);
    swap_int(intArray + POS[i * 8 + 4],intArray + POS[i * 8 + 2]);
    swap_int(intArray + POS[i << 3],intArray + POS[i * 8 + 3]);
    for (j = 0; j < 8; j = j + 1) {
      local_c = intArray[POS[j + i * 8]];
      uVar1 = lichaParita(local_c);
      if (uVar1 == 0) {
        reverse_bits_in_int(intArray + POS[j + i * 8]);
      }
      else {
        uVar1 = numberOfSetBitsInInt(local_c);
        local_c = local_c ^ uVar1;
      }
      intArray[POS[j + i * 8]] = local_c;
    }
    swap_int(intArray + POS[i * 8 + 1],intArray + POS[i * 8 + 3]);
    swap_int(intArray + POS[i * 8 + 7],intArray + POS[i * 8 + 6]);
    swap_int(intArray + POS[i * 8 + 5],intArray + POS[i * 8 + 1]);
  }
  return;
}


void reverse_encrypt(uint *intArray)
{
  TRACE;
  uint uVar1;
  int i;
  int j;
  uint local_c;

  set_intArray(intArray);

  for (i = 3; i >= 0; i--) {
    swap_int(intArray + POS[i * 8 + 5],intArray + POS[i * 8 + 1]);
    swap_int(intArray + POS[i * 8 + 7],intArray + POS[i * 8 + 6]);
    swap_int(intArray + POS[i * 8 + 1],intArray + POS[i * 8 + 3]);

    //********** MAGIC
    for (j = 0; j < 8; j = j + 1)
    {
      uint tmp = intArray[POS[j + i * 8]];

      int bFound = 0;
      for (int k = 0; k < 32; k++)
      {
        // Hledam takove cislo, ktere je pocet bitu v puvodnim intu a kdyz s nim XORnu tmp int, tak ten vysledek bude mit lichou paritu.
        int tmp2 = tmp ^ k;
        if (numberOfSetBitsInInt(tmp2) == k)
        {
            if (lichaParita(tmp2))
            {
                if (bFound)
                {
                    // Solution already found
                    printf("Char: %c, could also be: %c\n", (char)tmp2, (char)intArray[POS[j + i * 8]]);
                }
                else
                {
                    intArray[POS[j + i * 8]] = tmp2;
                    bFound = 1;
                    // break;
                }
            }
        }
      }
      if (!bFound)
      {
          intArray[POS[j + i * 8]] = tmp;
      }

/*          // Pokud je licha parita, tak 
          if (lichaParita(tmp) == 0) {
              reverse_bits_in_int(intArray + POS[j + i * 8]);
          }
          else {
              uVar1 = numberOfSetBitsInInt(tmp);
              tmp = tmp ^ uVar1;
          }
          intArray[POS[j + i * 8]] = tmp;
          */
      }

    // ************** MAGIC end

    //printf("i: %d\n", i);
    swap_int(intArray + POS[i << 3],intArray + POS[i * 8 + 3]);
    swap_int(intArray + POS[i * 8 + 4],intArray + POS[i * 8 + 2]);
    swap_int(intArray + POS[i * 8 + 2],intArray + POS[i * 8 + 7]);

  }
  return;
}


void convert_char_array_to_int_array(unsigned char *charArray,uint *intArray)

{
    int local_c;

    for (local_c = 0; local_c < 0x20; local_c = local_c + 1) {
        intArray[local_c] = (uint)charArray[local_c];
    }
    return;
}

#define STEP printf("%d\n", __LINE__)

void crackThisShit()
{
    TRACE;
    srand(time(NULL));   // Initialization, should only be called once.

    int bFound = 0;
    int cycleCount = 0;
    while(!bFound)
    {
        cycleCount++;
        unsigned char charArray[0x20] = {0};

        // Create random input    
        for (int i = 0; i < 0x20; i++)
        {
            charArray[i] = (unsigned char )rand();
        }

        uint intArray[0x20] = {0};

        convert_char_array_to_int_array(charArray, intArray);

        encrypt(intArray);

        // Check output
        int length = 0;
        for (int i = 0; i < 0x20; i = i + 1) {
            if (ENCRYPTED[i] == intArray[i]) {
                length = length + 1;
            }
        }
        if (length == 0x20)
        {
            bFound = 1;
            printf("DONE: ");
            for (int i = 0; i < 0x20; i++)
            {
                printf("%u, ", intArray[i]);
            }
            printf("\n");
        }

        if (length > 4) 
        {
            printf("Not found: %d\n", length);
            for (int i = 0; i < 0x20; i++)
            {
                printf("%u, ", intArray[i]);
            }
            printf("\n");
        }
    }

}

void printArray(uint* array, size_t len = 0x20)
{
    printf("Array: ");
    for (int i = 0; i < len; i++)
    {
        printf("%u, ", array[i]);
    }
    printf("\n");
}


void printArrayAsString(uint* array, size_t len = 0x20)
{
    printf("Array: ");
    for (int i = 0; i < len; i++)
    {
        printf("%c", (char)array[i]);
    }
    printf("\n");
}

void debugThisShit()
{
    TRACE;

    unsigned char charArray[0x20] = {0};

    // Create random input    
    for (int i = 0; i < 0x20; i++)
    {
        charArray[i] = (unsigned char )i;
    }

    uint intArray[0x20] = {0};

    convert_char_array_to_int_array(charArray, intArray);
/*
    printf("Input:");
    printArray(intArray);

    encrypt(intArray);

    printf("Encrypted:");
    printArray(intArray);

    reverse_encrypt(intArray);

    printf("Reversed: ");
    printArray(intArray);


    encrypt(intArray);

    printf("Encrypted again:");
    printArray(intArray);

*/
    reverse_encrypt(ENCRYPTED);
    printf("Password:");
    printArray(ENCRYPTED);
    printArrayAsString(ENCRYPTED);

}


int main()
{;
    /*
    for (int i = 0; i < 17; i++)
    {
      printf("%d, %d\n", lichaParita(i), lichaParita(i^1));
    }
    return 1;

    */
    TRACE;
    debugThisShit();
    return 1;


    // This sucks as it takes way too long!
    crackThisShit();
    return 1;
    char charArray[0x20] = {1,2,3,4,5,6,7,8,9};
    int intArray[0x20] = {0};


    convert_char_array_to_int_array(charArray, intArray);

    for (int i = 0; i < 0x20; i++)
    {
        printf("%d, ", intArray[i]);
    }
    printf("\n");

    encrypt(intArray);

    for (int i = 0; i < 0x20; i++)
    {
        printf("%d, ", intArray[i]);
    }

    printf("\n");

    return 0;

    for (int i = 0; i < 255; i++)
    {
        printBits(i);
        printf("i: %d, number of Set bits in int: %d, parity: %d\n", i, numberOfSetBitsInInt(i), lichaParita(i));
    }


    int a[] = {1,2,3,4};
    int b[] = {9,8,7,6};

    for (int i = 0; i < 4; i++)
    {
        swap_int(&a[i], &b[i]);
    }    

    printf("a: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%d, ", a[i]);
    }
    printf("\n");


    printf("b: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%d, ", b[i]);
    }    
    printf("\n");


/*
    for (int i = 0; i < 255; i++)
    {
        uint x = i;
        reverse_bits_in_int(&x);
        printBits(i);
        printBits(x);
        printf("pppp i: 0x%02x, result: 0x%02x\n", i, x^0xff);
    }
    */
    return 0;
    


}