#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Convert ASCII to binary (decimal representation of binary)
int asciiToBinary(int character) {
    int binary = 0, multiplier = 1;
    while (character > 0) {
        int remainder = character % 2;
        binary += (remainder * multiplier);
        character /= 2;
        multiplier *= 10;
    }
    return binary;
}
int main(int argc, char *argv[]) {

    if (argc < 2) { 
        printf("Not enough requirements!:\n");
        return 2;
    }
    int arraySize = strlen(argv[1]);
    int binaryString[arraySize];

    for (int i = 0; i < arraySize; i++) {
        binaryString[i] = asciiToBinary(toascii(argv[1][i]));
    }
    printf("Character | ASCII | Binary\n");
    printf("--------------------------\n");

    int i = 0;
    char *input = argv[1];
    while (input[i] != '\0') {
        char ch = input[i];
        int ascii = toascii(ch);
        //binaryString[i] = asciiToBinary(ascii);
         printf("    %c     |  %3d  | %08d\n", ch, ascii, binaryString[i]);
        i++;
    }
    printf("Made it here \n");
    
    // Calculate half and arraySize correctly
    int half = arraySize / 2;

    int L1[half];
    int R1[arraySize - half];
  // Copy first half to L1
    for (int j = 0; j < half; j++) { 
        L1[j] = binaryString[j];
    }
    printf("Created L1\n");
    printf("half = %zu, arraySize = %lu\n", half, sizeof(binaryString) / sizeof(binaryString[0]));
   // Fill R1 with the second half
    for (size_t j = half; j < arraySize; j++) {
        R1[j - half] = binaryString[j];
    }
    printf("Created R1\n");
    int K1 = 0;
    int K2 = 0;
    printf("Created K1 and K2\n");
     K1 = rand() % (1 << arraySize);
    K2 = rand() % (1 << arraySize);
    printf("Created K2\n");

    int f1 = 0;
    printf("Created f1\n");
    int size_R1 =  arraySize - half;
    for (int i = 0; i < size_R1; i++) {
        printf("In for loop\n"); // f1 = xor(R1, K1);
         f1 = f1 + (R1[i] ^ K1);
    }
    printf("Outside for loop\n");
    int R2[half/2];
    int L2[half/2];
    int R2_size = (sizeof(R2) / sizeof(R2[0]));
    int L2_size = (sizeof(L2) / sizeof(L2[0]));

    printf("Created R2 and L2\n");
    for (int i = 0; i < R2_size; i++) { //R2 = xor(f1, l1);
        R2[i] = f1 ^ L1[i];
        L2[i] = R1[i];
    }
    printf("Outside of next for loop\n");
    //second round
    
    int R3[R2_size/2];
    int L3[L2_size/2];
    int R3_size = (sizeof(R3) / sizeof(R3[0]));
    int L3_size = (sizeof(L3) / sizeof(L3[0]));
    int f2 = 0;
    printf("Created R3\n");
    for (int i = 0; R3_size; i++) {
        f2 = f2 + (R2[i] ^ K2);
        R3[i] = f2 ^ L2[i];
        L3[i] = R2[i];
    }
    printf("Done.\n");




    return 0;
}