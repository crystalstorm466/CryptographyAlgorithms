#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Convert ASCII to binary (decimal representation of binary)
void toBinaryString(int number, char *binary, int size) {
    binary[size] = '\0';
    for (int i = size - 1; i >= 0; i--) {
        binary[i] = (number & 1) ? '1' : '0';
        number >>= 1;
    }

}

int f(int R, int K) {
    //simple
    //F = (((REi * Key) >> 4) + REi ) mod 2
    //Round 1 key: 'B'
    //Round 2 key: 'e'
    return (((R * K) >> 4) ^ R) % 2;
}

int DecimalToBinary(int n) {
    int num = n;
    int dec_value = 0;

    int base = 1;
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;

        base = base * 2;
    }

    
    return dec_value;
}
int main(int argc, char *argv[]) {

    if (argc < 2) { 
        printf("Not enough requirements!:\n");
        return 2;
    }
    int arraySize = strlen(argv[1]);
    int binaryString[arraySize];

    for (int i = 0; i < arraySize; i++) {
        binaryString[i] = (unsigned char)argv[1][i]; //store ascii values
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
    for (int i = 0; i < half; i++) { 
        L1[i] = binaryString[i];
    }
    printf("Created L1\n");
   // Fill R1 with the second half
    for (int j = half; j < arraySize-half; j++) {
        R1[j] = binaryString[half + j];
    }
    printf("Created R1\n");
    int K1 = 'B';
    int K2 = 'e';

    int L2[half], R2[half];
    for (int i = 0; i < half; i++) {
        int f1 = f(R1[i], K1);
        R2[i] = L1[i] ^ f1;
        L2[i] = R1[i];
    }
    //second round
    
    int R3[half];
    int L3[half];
    for (int i = 0; i < half; i++) {
        int f2 = f(R2[i], K2);
        R3[i] = L1[i] ^ f2;
        L3[i] = R2[i];
    }
    printf("Done\n");

    printf("Ciphertext:\n");
    for (int i = 0; i < half; i++) {
        printf("%c", L3[i]);
    }
    for (int i = 0; i < half; i++) {
        printf("%c", R3[i]);
    }
    printf("\n");
   
    return 0;
} 