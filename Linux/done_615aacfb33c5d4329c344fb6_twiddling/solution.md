
# Solution for machina's Twiddling by Apollo

Link to the original crackme: https://crackmes.one/crackme/615aacfb33c5d4329c344fb6

I extracted following functions and variables from the binary file using Ghidra. I renamed some function according to their implementation and added few comments.

## Global variables:
<details>
    <summary>uint ENCRYPTED[32]</summary>

    // Expected result
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
</details>

<details>
  <summary>uint POS[32]</summary> 

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
</details>

## Functions:
<details>
<summary>void reverse_bits_in_int(uint *param_1)</summary>
This function reverses order of bits in an unsigned int variable. But it seems that its output is not used in the code at all.

    void reverse_bits_in_int(uint *param_1)
    {
        uint local_c;
        
        local_c = 0;
        while (*param_1 != 0) {
            local_c = local_c << 1 | *param_1 & 1;
            *param_1 = (uint)*param_1 >> 1;
        }
        *param_1 = local_c;
        return;
    }
</details>

<details>
  <summary>int numberOfSetBitsInInt(uint param_1)</summary>
  Returns a number of set bits from and unsigned int

    // Number of Set bits in uint
    int numberOfSetBitsInInt(uint param_1)
    {
        unsigned int i;
        int local_c;
        local_c = 0;
        for (i = param_1; i != 0; i = i >> 1) {
            local_c = local_c + (i & 1);
        }
        return local_c;
    }
</details>

<details>
  <summary>uint getEvenParityBit(uint param_1)</summary>
Calculate even parity bit. This means that number of set bits (ones) in the input value plus this bit is even number.

    uint getEvenParityBit(uint param_1)
    {
        uint local_7c;
        uint local_6c;
        local_6c = 0;
        for (local_7c = param_1; local_7c != 0; local_7c = local_7c & local_7c - 1) {
            local_6c = local_6c ^ 1;
        }
        return local_6c;
    }

</details>

<details>
  <summary>void swap_uint(uint *param_1,uint *param_2)</summary>
Swap to uint numbers provided as pointers to uint.

    void swap_uint(uint *param_1,uint *param_2)
    {
        *param_1 = *param_1 ^ *param_2;
        *param_2 = *param_2 ^ *param_1;
        *param_1 = *param_1 ^ *param_2;
        return;
    }

</details>

<details>
  <summary>void encrypt(uint *intArray)</summary>
Encrypt provided array

    void encrypt(uint *intArray)
    {
        uint uVar1;
        int i;
        int j;
        uint local_c;

        for (i = 0; i < 4; i = i + 1) {
            swap_uint(intArray + POS[i * 8 + 2],intArray + POS[i * 8 + 7]);
            swap_uint(intArray + POS[i * 8 + 4],intArray + POS[i * 8 + 2]);
            swap_uint(intArray + POS[i << 3],intArray + POS[i * 8 + 3]);
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
            swap_uint(intArray + POS[i * 8 + 1],intArray + POS[i * 8 + 3]);
            swap_uint(intArray + POS[i * 8 + 7],intArray + POS[i * 8 + 6]);
            swap_uint(intArray + POS[i * 8 + 5],intArray + POS[i * 8 + 1]);
        }
        return;
    }

</details>

<details>
  <summary>main(void)</summary>
Simplified version of main function. Loads 32 characters from stdin to char[32] array. Later this array is converted to an array of uints. This newly created array is encrypted and result is compared to the expected result. If they match, message "You did it! What a good reverser you are ;)" is printed. Message "I\'m sorry little one\n" is printed otherwise.

    undefined8 main(void)
    {
    size_t sVar1;
    int i;
    int length;
    uint inputAsIntArray [32];
    char inputAsCharArray [40];
    
    puts("Enter flag (e.g. flag{3x4mpl3_fl4g})");
    fgets(inputAsCharArray,0x21,stdin);
    convert_char_array_to_int_array(inputAsCharArray,inputAsIntArray);
    length = 0;
    sVar1 = strlen(inputAsCharArray);
    if (sVar1 == 0x20) {
        encrypt(inputAsIntArray);
        for (i = 0; i < 0x20; i = i + 1) {
        if (ENCRYPTED[i] == inputAsIntArray[i]) {
            length = length + 1;
        }
        }
    }
    if (length == 0x20) {
        printf("You did it! What a good reverser you are ;)\n");
    }
    else {
        printf("I\'m sorry little one\n");
    }
    return 0;
    }

</details>

# Code functionality
Main function loads 32 characters from stdin to char[32] array. Later this array is converted to an array of uints. This newly created array is encrypted and result is compared to the expected result. If they match, message "You did it! What a good reverser you are ;)" is printed. Message "I\'m sorry little one\n" is printed otherwise.

Encrypt function swaps some values in the uint array multiple times. It also checks if a value has an odd or even number of set bits. Based on this value it might be XORed with a result of function numberOfSetBitsInInt.

We need to reverse this process. It is easy for swaping of values, just call them in a reverse order.

We need to find K which holds number of set bits in the Decrypted value which when XORed with Encypted value has an odd number of set bits.

    for (int K = 0; K < 32; K++)
    {
        if (numberOfSetBitsInInt(E ^ K) == K)
        {
            if (getEvenParityBit(E ^ K))
            {
                T = E ^ K;
            }
        }
    }

E = encrypted value 

K = number of set bits in X

T = decrypted value

In some cases there might be multiple possibilities of value of T. This algorithm find all of them.

Output of my keygen:

    *************************
    Char: 7, could also be: 1
    Char: n, could also be: h
    Char: 7, could also be: 1
    Char: 7, could also be: 1
    Char: n, could also be: h
    Char: 7, could also be: 1
    Char: 7, could also be: 1
    Char: g, could also be: a
    Char: n, could also be: h
    Char: 7, could also be: 1
    Char: g, could also be: a
    Password: flaa{1w1DL1h6_1h3_b1h4Ry_5y513m}

As you can see characters in groups {7,1}, {n,h} and {g,a} can be exchanged, because they produce the same encrypted value.

All possible password can be generated with the following command:

    echo fl{a,g}{a,g}\{{1,7}w{1,7}DL{1,7}{n,h}6_{1,7}{n,h}3_b{1,7}{n,h}4Ry_5y5{1,7}3m\}

For more details see implementation of a function "reverse_encrypt".

<details>
  <summary>void reverse_encrypt(uint *intArray)</summary>

    void reverse_encrypt(uint *intArray)
    {
        uint uVar1;
        int i;
        int j;
        uint local_c;

        set_intArray(intArray);

        for (i = 3; i >= 0; i--) {
            swap_uint(intArray + POS[i * 8 + 5],intArray + POS[i * 8 + 1]);
            swap_uint(intArray + POS[i * 8 + 7],intArray + POS[i * 8 + 6]);
            swap_uint(intArray + POS[i * 8 + 1],intArray + POS[i * 8 + 3]);

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
                                // break;
                            }
                        }
                    }
                }
                if (!bFound)
                {
                    intArray[POS[j + i * 8]] = tmp;
                }
            }
            // ************** MAGIC end

            swap_uint(intArray + POS[i << 3],intArray + POS[i * 8 + 3]);
            swap_uint(intArray + POS[i * 8 + 4],intArray + POS[i * 8 + 2]);
            swap_uint(intArray + POS[i * 8 + 2],intArray + POS[i * 8 + 7]);
        }
        return;
    }
</details>

<details>
  <summary>void findPassword()</summary>

    void findPassword()
    {
        reverse_encrypt(ENCRYPTED);
        printf("Password:");
        printArray(ENCRYPTED);
        printArrayAsString(ENCRYPTED);
    }

</details>