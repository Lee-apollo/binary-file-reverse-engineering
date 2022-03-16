# kgm1_keygen

import random

encyption_array = [int(x, 16) for x in "45 36 ab c8 cc 11 e3 7a 00 00 00 00".split()]
print([hex(i) for i in encyption_array])

def check_input(user_input):
    print(user_input)
    user_input_len = 9
    if len(user_input) != user_input_len:
        print("FAIL - invalid len")
        return False

    for i in range(user_input_len - 1):
        print("hex: ", hex(user_input[i])),
        user_input[i] = user_input[i] ^ encyption_array[i]
        print("XORed: ", hex(user_input[i]))
    
    input_sum = 0
    for i in range(user_input_len - 1):
        input_sum += user_input[i]
        print("checksum: ", hex(input_sum))
    #input_sum = sum(user_input[:-1])

    if (user_input[8] == (input_sum & 0xff)) and (user_input[8] >= ord('a') and (user_input[8]) <= ord('z')):
        print("Fuck yeah!")
        return True
    else:
        print("FAIL - invalid checksum")
        print("Required checksum: ", hex(input_sum & 0xff))
        print("Provided checksum: ", hex(user_input[8]))
        return False


def generate():
    #def is_ascii(s):
    #    return all(ord(c) < 128 for c in s)

    def getRandomAscii():
        x = random.randint(0x21, 0x7e)
        return x

    while 1:
        key = [getRandomAscii() for i in range(8)]


    #key = [0] * 9
    #ascii_key = "123456789"
    #for i in range(len(ascii_key)):
    #    key[i] = ord(ascii_key[i]) ^ encyption_array[i]

    #key = [1,2,3,4,5,6,7,8]


        #print("HOHOHO")

        checksum = 0
        for i in range(len(key)):
            #key[i] = key[i] ^ encyption_array[i]
            checksum += key[i] ^ encyption_array[i]

        checksum = checksum & 0xff

        key += [checksum]


        if checksum not in range(0x21, 0x7E):
            continue

        break
        #ascii_key ="".join([chr(i) for i in key]) 
        #if is_ascii(ascii_key):
        #    break;
    print("key: ", [hex(i) for i in key])
    print(len(key))
    return key


def run():
    key = generate()
    #user_input = [ord(x) for x in "aivowskjn"]
    #print(user_input)
    is_valid = check_input(key[:])
    
    
    if is_valid:
        ascii_key ="".join([chr(i) for i in key]) 
        if ascii_key.isprintable():
            print("KEY: ", ascii_key)
            return True

    return False

while 1:
    if run():
        break;

###
###  printf("KeyGenMe1 by ascii for http://crackmes.de/\n");
###  printf("Key: ");
###  fgets(userInput + 1,0x10,stdin);
###  inputLen = strlen(userInput + 1);
###  if (inputLen == 10) {
###    iVar2 = 1;
###    do {
###      userInput[iVar2] = userInput[iVar2] ^ *(byte *)((int)&PTR_DAT_08049704 + iVar2 + 3);
###      iVar2 = iVar2 + 1;
###    } while (iVar2 != 9);
###    uVar1 = 0;
###    iVar2 = 0;
###    do {
###      uVar1 = uVar1 + (int)userInput[iVar2 + 1];
###      iVar2 = iVar2 + 1;
###    } while (iVar2 != 8);
###    if (((int)userInput[9] == (uVar1 & 0xff)) && ((int)userInput[9] - L'a'U < 0x1a)) {
###      printf("Good key !\n");
###      return 0;
###    }
###  }
###  printf("Invalid key !\n");
###  return 1;
###}