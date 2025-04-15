#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


#define MODULUS 0x11B


// Convert ASCII to binary (decimal representation of binary)
void toBinaryString(int number, char *binary, int size) {
    binary[size] = '\0';
    for (int i = size - 1; i >= 0; i--) {
        binary[i] = (number & 1) ? '1' : '0';
        number >>= 1;
    }

}

unsigned int multiply_galois(unsigned int A, unsigned int B) {
    int index = 0;
    unsigned int result = 0;

    while (B) {
        if (B & 1) { //if lowest bit of B is sett, add  (XOR) shifted A to result
            result ^= A;
        }
        A <<= 1;
        if (A & 0x100) { //if A exceeds 8 bit (x^8) reduce module m(x)
            A^= MODULUS; //0x11b
        }
        B >>= 1;
    }
    return result;
        
}

int f(int R, int K) {
    //simple
    //F = (Rn-1  * keyn ) mod 0x11b) over GF(2^8) 
    //Round 1 key: 'B'
    //Round 2 key: 'e'
    //return (((R * K) >> 4) ^ R) % 2;
    //return mod((((R * K) >> 4) ^ R),2);
    int m = 0x11b;

//    return ((R-1) * K) % 0x11B;
    return multiply_galois(R, K);
//    return (mod((multiply_galois((R-1) * K, MODULUS)), MODULUS));

}


int DecimalToBinary(int n) {
    int binary=0, base = 1;
    while (n >0) {
    binary += (n % 2) * base;
    n /= 2;
    base *= 10;
  }
    
    return binary;
}
int main(int argc, char *argv[]) {

    if (argc != 5) { 
        //key is also # of rounds
        printf("Error! Too few arguments\n");
        printf("Usage: %s <input file> <output file> <key> <e|d>\n", argv[0]);
        return 2;
    }
    
    int arraySize = strlen(argv[1]);
    int binaryString[arraySize];

    for (int i = 0; i < arraySize; i++) {
        binaryString[i] = (unsigned char)argv[1][i]; //store ascii values
    }
    int i = 0;
    char *input = argv[1];
    char *output = argv[2];
    char *key_input = argv[3];
    int rounds = strlen(key_input);
    int *keys = malloc(rounds * sizeof(int));
    
    if (keys == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < rounds; i++) {
        keys[i] = (unsigned char) key_input[i];
    }
    char *mode = argv[4];
    bool encrypt = true;

   
    FILE *fp = fopen(argv[1], "rb"); //input.txt (ciphertext or plaintext)
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(filesize); //input array
    fread(buffer, 1, filesize, fp);
    fclose(fp);
    
    if (strcmp(mode, "e") == 0) {
        encrypt = true;
    } else if (strcmp(mode, "d") == 0) {
        encrypt = false;
    }
    
    if (encrypt) {
        FILE *fp;
        fp = fopen(argv[2], "w");
        printf("Encryption:\n");

        for (int i = 0; i < filesize; i += 2) {
            unsigned char L = buffer[i];
            unsigned char R = buffer[i + 1];
        
            for (int r = 0; r < rounds; r++) {
                unsigned char temp = R;
                R = L ^ f(R, keys[r]);
                L = temp;
            }

            printf("%02x", L);
            printf("%02x",R);
           
            fwrite(&L, 1, 1, fp);
            fwrite(&R, 1, 1, fp);

        }
        fclose(fp);    
    } else {
        printf("Decryption\n"); //decrypt
        FILE *fp;
        fp = fopen(argv[2], "w"); //output

        for (int i = 0; i < filesize; i += 2) {

            unsigned char L = buffer[i];
            unsigned char R = buffer[i+1];

            for (int r = rounds-1; r >= 0; r--) {
                unsigned char temp = L;
                L = R ^ f(L, keys[r]);
                R = temp;

            }

            printf("%c", L);
            printf("%c",R);
           
            fwrite(&L, 1, 1, fp);
            fwrite(&R, 1, 1, fp);

        }
        fclose(fp);
    }

    return 0;
} 
