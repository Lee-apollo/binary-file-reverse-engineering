
#include <stdio.h>
#include <string.h>

char encrpytion_bytes[] = {0,0,0,0,0x45, 0x36, 0xab, 0xc8, 0xcc, 0x11, 0xe3, 0x7a, 0x0, 0x0, 0x0, 0x0};

int main(int argc,char **argv)

{
  size_t inputLen;
  unsigned int uVar1;
  int iVar2;
  char userInput [17];
  
  printf("KeyGenMe1 by ascii for http://crackmes.de/\n");
  printf("Key: ");
  fgets(userInput + 1,0x10,stdin);
  inputLen = strlen(userInput + 1);
  if (inputLen == 10) {
    iVar2 = 1;
    do {
      printf("hex: %x, ", userInput[iVar2] );
      userInput[iVar2] = userInput[iVar2] ^ *(char *)(encrpytion_bytes + iVar2 + 3);
      printf("XORed hex: %x\n", userInput[iVar2]);
      iVar2 = iVar2 + 1;
    } while (iVar2 != 9);
    uVar1 = 0;
    iVar2 = 0;
    do {
      uVar1 = uVar1 + (int)userInput[iVar2 + 1];
      printf("checksum: %x", uVar1);
      iVar2 = iVar2 + 1;
    } while (iVar2 != 8);
    if (((int)userInput[9] == (uVar1 & 0xff)) && ((int)userInput[9] - 'a' < 0x1a)) {
      printf("Good key !\n");
      return 0;
    }
    else
    {
      printf("Provided checksum: %x, Required checksum: %x", userInput[9], uVar1 & 0xff);
    }
  }
  printf("Invalid key !\n");
  return 1;
}