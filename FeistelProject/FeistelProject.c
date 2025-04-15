#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mod.h"
#include <stdbool.h>


#define MODULUS 0x13B


unsigned int multiply_galois(unsigned int A, unsigned int B, unsigned int m) { //this is actualyl the feistel function huh who knew 
    int index = 0;
    unsigned int result = 0;

    while (B) {
        if (B & 1) { //if lowest bit of B is sett, add  (XOR) shifted A to result
            result ^= A;
        }
        A <<= 1;
        if (A & 0x100) { //if A exceeds 8 bit (x^8) reduce module m(x)
            A^= m; //0x11b
        }
        B >>= 1;
    }
    return result;
        
}

int f(int R, int K, int m) {
    //simple
    // = (Rn-1 * keyn) mod 0x13)16 over GF(24) and ai ∈ GF(2) with m(x) = x4 + x1 + 1           i.e. 0x13)16 
    // f = Rn-1 * key n % 0x13) for 8 bit 
    //0x11b for 16 bit

    return multiply_galois(R, K, m);
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

    if (argc != 6) {  
        //key is also # of rounds
        printf("Error! Too few arguments\n");
        printf("Usage: %s <input file> <output file> <key> <block size: 8|16> <e|d>\n", argv[0]);
        return 2;
    }
    
    
 
    int i = 0;
    char *input = argv[1];
    char *output = argv[2];
    char *key_input = argv[3];
    char *block = argv[4];
    int rounds = strlen(key_input);
    int block_size = atof(block);
    int *keys = malloc(rounds * sizeof(int));
    
    if ((block_size != 8) && (block_size != 16)) {
        printf("Error! Block size invalid! Must be either 8 or 16!");
        return 4;
    }
    if (keys == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < rounds; i++) {
        keys[i] = (unsigned char) key_input[i];
    }
    char *mode = argv[5];
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
        if (block_size == 8) {
            FILE *fp;
            fp = fopen(argv[2], "w");
            printf("Encryption: 8 blocks\n");

            for (int i = 0; i < filesize; i ++) {
                unsigned char block = buffer[i];
                unsigned char L = (block >> 4) & 0x0F; //split the left block int 8 bit block sizes
                unsigned char R = block & 0x0F; //split the right block in half by 8 bit block sizes
           
            for (int r = 0; r < rounds; r++) {
                // unsigned char temp = R;
                // R = L ^ f(R, keys[r]);
                // L = temp;
                unsigned char temp = R;
                R = L ^ f(R, keys[r], 0x13);
                R &= 0x0F;
                L = temp;
            }
            unsigned char out = (L << 4 ) | (R & 0x0F);
            printf("%02x", out);
            fwrite(&out, 1, 1, fp);

        }
        fclose(fp);    
        } else if (block_size == 16) {
            //normal approach
            FILE *fp;
            fp = fopen(argv[2], "w");
            printf("Encryption: 16 blocks\n");

            for (int i = 0; i < filesize; i += 2) {
                unsigned char L = buffer[i];
                unsigned char R = buffer[i + 1];
        
            for (int r = 0; r < rounds; r++) {
                unsigned char temp = R;
                R = L ^ f(R, keys[r], 0x11B);
                L = temp;
            }

            printf("%02x", L);
            printf("%02x",R);
           
            fwrite(&L, 1, 1, fp);
            fwrite(&R, 1, 1, fp);

        }
        fclose(fp);  
        }
    } else {
        if (block_size == 8) {
            printf("Decryption: 8 Blocks\n");
            FILE *fp;
            fp = fopen(argv[2], "w");

            for (int i = 0; i < filesize; i ++) {
                unsigned char block = buffer[i];
                unsigned char L = (block >> 4) & 0x0F; //split left side of ciphertext in 8 bit sizes
                unsigned char R = block & 0x0F; //split the right side of ciphertext in 8 bit sizes

                for (int r = rounds - 1; r >= 0; r--) {
                    unsigned char temp = L;
                    L = R ^ f(L, keys[r], 0x13);
                    L &= 0x0F;
                    R = temp;
                    // L = R ^ f(L, keys[r]);
                    // R = temp;
                }
        
                unsigned char out = ( L << 4) | (R & 0x0F);
                printf("%c", out);
                fwrite(&out, 1, 1, fp);
            }
        }
        else if (block_size == 16) { //normal way
            printf("Decryption: 16 blocks\n"); //decrypt
        FILE *fp;
        fp = fopen(argv[2], "w"); //output

        for (int i = 0; i < filesize; i += 2) {

            unsigned char L = buffer[i];
            unsigned char R = buffer[i+1];

            for (int r = rounds-1; r >= 0; r--) {
                unsigned char temp = L;
                L = R ^ f(L, keys[r], 0x11B);
                R = temp;

            }

            printf("%c", L);
            printf("%c",R);
           
            fwrite(&L, 1, 1, fp);
            fwrite(&R, 1, 1, fp);

        }
        fclose(fp);
        }
        
    }

    return 0;
} 
