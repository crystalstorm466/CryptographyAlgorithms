#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int asciiToBinary(int character) {
    int binary = 0, multiplier = 1;
    int i = 0;
    while ( character > 0) {
        int remainder = character % 2;
        binary = binary + (remainder * multiplier);
        character = character / 2;
        multiplier = multiplier * 10;
    }
    
    return binary;
}
int main(int argc, char *argv[]) {

    if (argc < 2) { 
        printf("Not enough requirements!:\n");
        return 2;
    }


    printf("Character | ASCII | Binary\n");
    printf("--------------------------\n");
    int inputLength = strlen(argv[1]);
    int binaryString[inputLength];

    int i = 0;
    char *input = argv[1];
    while (input[i] != '\0') {
        char ch = input[i];
        int ascii = toascii(ch);
        binaryString[i] = asciiToBinary(ascii);
         printf("    %c     |  %3d  | %08d\n", ch, ascii, binaryString[i]);
        i++;
    }
    printf("Made it here \n");
    
    // Calculate half and arraySize correctly
    int arraySize = sizeof(binaryString) / sizeof(binaryString[0]);
    int half = arraySize / 2;
    int L1[half];
    int R1[half];
  // Copy first half to L1
    for (int j = 0; j < half; j++) { 
        L1[j] = binaryString[j];
    }
    printf("Created L1\n");
    printf("half = %zu, arraySize = %lu\n", half, sizeof(binaryString) / sizeof(binaryString[0]));
    for (int j = half; j < arraySize; j++) {
        R1[j - half] = binaryString[j];
    }
    printf("Created R1");

    int K1 = rand() % (sizeof(half)); //generate random binary keys 
    int K2 = rand() % (sizeof(half)); //generate random binary keys
    int f1 = 0;
    for (int i = 0; i < sizeof(R1); i++) { // f1 = xor(R1, K1);
         f1 = f1 + (R1[i] ^ K1);
    }
    int R2[half/2];
    int L2[half/2];
    for (int i = 0; i < sizeof(R2); i++) { //R2 = xor(f1, l1);
        R2[i] = f1 ^ L1[i];
        L2[i] = R1[i];
    }
    //second round
    int R3[sizeof(R2)/2];
    int L3[sizeof(L2)/2];
    int f2 = 0;
    for (int i = 0; sizeof(R2); i++) {
        f2 = f2 + (R2[i] ^ K2);
        R3[i] = f2 ^ L2[i];
        L3[i] = R2[i];
    }
   




    return 0;
}