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

// #define TRACE TraceObject x = TraceObject(__FUNCTION__)
// #define TRACE printf("%s\n", __FUNCTION__)
#define TRACE 


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
  local_c = 0;
  for (i = param_1; i != 0; i = i >> 1) {
    local_c = local_c + (i & 1);
  }
  return local_c;
}


uint getEvenParityBit(uint param_1)
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
    swap_int(intArray + POS[i * 8 + 2],intArray + POS[i * 8 + 7]);
    swap_int(intArray + POS[i * 8 + 4],intArray + POS[i * 8 + 2]);
    swap_int(intArray + POS[i << 3],intArray + POS[i * 8 + 3]);
    for (j = 0; j < 8; j = j + 1) {
      local_c = intArray[POS[j + i * 8]];
      uVar1 = getEvenParityBit(local_c);
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

    for (i = 3; i >= 0; i--)
    {
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
                int tmp2 = tmp ^ k;
                if (numberOfSetBitsInInt(tmp2) == k)
                {
                    if (getEvenParityBit(tmp2))
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
                        }
                    }
                }
            }
            if (!bFound)
            {
                intArray[POS[j + i * 8]] = tmp;
            }
        }
        swap_int(intArray + POS[i << 3],intArray + POS[i * 8 + 3]);
        swap_int(intArray + POS[i * 8 + 4],intArray + POS[i * 8 + 2]);
        swap_int(intArray + POS[i * 8 + 2],intArray + POS[i * 8 + 7]);
    }
}


void convert_char_array_to_int_array(unsigned char *charArray,uint *intArray)
{
    int local_c;

    for (local_c = 0; local_c < 0x20; local_c = local_c + 1) {
        intArray[local_c] = (uint)charArray[local_c];
    }
    return;
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

int main()
{
    reverse_encrypt(ENCRYPTED);
    printf("Password:");
    printArray(ENCRYPTED);
    printArrayAsString(ENCRYPTED);

    return 0;
}