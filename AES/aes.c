#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include "aes.h"
#include <string.h>


    const unsigned char sbox[256] = {
        // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
        }; // F
    // Implementation: Rcon
    const unsigned char Rcon[255] = {
        0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
        0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
        0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
        0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d,
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab,
        0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d,
        0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25,
        0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01,
        0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
        0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa,
        0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a,
        0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02,
        0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
        0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
        0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
        0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
        0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f,
        0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5,
        0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33,
        0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb
        };
    const unsigned char rsbox[256] =
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};


    unsigned char getSBoxValue(unsigned char num) { return sbox[num]; }

   
    //rotate word 8 bits to the left
    //word is char array of size 4 (32 bit)
    void rotate(unsigned char *word) {
        unsigned char c;
        int i;

        c = word[0];
        for (int i = 0; i < 3; i++) {
            word[i] = word[i + 1];

    }
}

    void core(unsigned char *word, int iteration) {
        int i;
        rotate(word); //rotate 32 bit word 8 bits to left;
        //apply s-box on all 4 parts
        for (int i = 0; i < 4; ++i) {
            word[i] = getSBoxValue(word[i]);
        }
        //XOR output of rcon with i  to first part
        word[0] = word[0] ^ getRconValue(iteration);
    }


    void expandKey(unsigned char *expandedKey, const unsigned char *key, enum keySize size, size_t expandedKeySize) {
        int i, j;
        unsigned char temp[4];
        for (i= 0; i < 16; i++) {
             expandedKey[i] = key[i];
        }
        for (int i = 4; i < expandedKeySize/4; i++) {
         for (j = 0; j < 4; j++) {
             temp[j] = expandedKey[(i - 1) * 4 +j];
     
         }
         if (i % 4  == 0) {
             rotate(temp);
             for ( j = 0; j < 4; j++) {
                 temp[j] = getSBoxValue(temp[j]);
             }
             temp[0] ^= getRconValue(i/4);
         }
         for ( j = 0; j < 4; j++) {
             expandedKey[ i * 4 + j] = expandedKey[( i -4 ) * 4 + j] ^ temp[j];
         }
        }
     }




    void subBytes(unsigned char *state) {
        for (int i = 0; i < 16; i++) {
            state[i] = getSBoxValue(state[i]);
        }
    }
     void shiftRow(unsigned char *state, unsigned char nbr) {
        unsigned char tmp;

        for (int i = 0; i < nbr; i++) {
            //if nbr is 3
            tmp = state[0];
            for (int j = 0; j < 3; j++) {
                state[j] = state[j + 1];
            } 
            state[3] = tmp;
        }

    }
    void shiftRows(unsigned char *state) {
        //row 0 - no shift
        //row 1 - shift to left by 1
        //row 2 - shift to left by 2
        //row 3 - shift to left by 3

        for (int i = 0; i < 4; i++) {
            shiftRow(state + 1 * 4, i);
        }
    }
   

   void addRoundKey(unsigned char *state, const unsigned char *roundKey) {
    for (int i = 0; i < 16; i++) {
        state[i] = state[i] ^ roundKey[i];
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
            A^= 0x1b; //0x1b
        }
        B >>= 1;
    }
    return result;
        
}


void mixColumns(unsigned char *state) {
    unsigned char tmp[4];
    for (int i = 0; i < 4; i++) {
        tmp[0] = multiply_galois(0x02, state[i]) ^ multiply_galois(0x03, state[i + 4]) ^ state[i + 8] ^ state[i + 12];
        tmp[1] = state[i] ^ multiply_galois(0x02, state[i + 4]) ^ multiply_galois(0x03, state[i + 8]) ^ state[i + 12];
        tmp[2] = state[i] ^ state[i + 4] ^ multiply_galois(0x02, state[i + 8]) ^ multiply_galois(0x03, state[i + 12]);
        tmp[3] = multiply_galois(0x03, state[i]) ^ state[i + 4] ^ state[i + 8] ^ multiply_galois(0x02, state[i + 12]);
        for (int j = 0; j < 4; j++) {
            state[i + j * 4] = tmp[j];
        }
    }
}

// void mixColumns(unsigned char *state) {
//     //c1 = 0x02 * b1 ^ 0x03  * b2 ^ 0x01*b3 ^ 0x01 ^ b4
//     //c2 = 0x01 * b1 ^ 0x02 * b2 ^ 0x03 *b3 ^ 0x01 ^ b4
//     //c3 = 0x01 * b1 ^ 0x01 * b2 ^ 0x02 * b3 ^ 0x03 ^ b4
//     //c4 = 0x03 * b1 ^ 0x01 * b2 ^ 0x01 * b3 ^ 0x02 ^ b4
//     //state box is 1d array of 4x4 matrix
//     unsigned char a[4];
//     unsigned char b[4];
//     unsigned char c[4];
//     unsigned char d[5];
//     unsigned char column[4];
//     char c1 = 0;
//     char c2 = 0;
//     char c3 = 0;
//     char c4 = 0;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             column[j] = state[(j *4) + i];
//         }
//     }

//     column[0] = multiply_galois(0x02, state[0]) ^ multiply_galois(0x03, state[1]) ^ multiply_galois(0x01, state[2]) ^ multiply_galois(0x01, state[3]);
//     column[1] = multiply_galois(0x01, state[4]) ^ multiply_galois(0x02, state[5]) ^ multiply_galois(0x03, state[6]) ^ multiply_galois(0x01, state[7]);
//     column[2] = multiply_galois(0x01, state[8]) ^ multiply_galois(0x01, state[9]) ^ multiply_galois(0x02, state[10]) ^ multiply_galois(0x03, state[11]);
//     column[3] = multiply_galois(0x04, state[12]) ^ multiply_galois(0x01, state[13]) ^ multiply_galois(0x01, state[14]) ^ multiply_galois(0x02, state[15]);

//     for (int i = 0; i < 4; i++) {//put values back into state
//         for (int j = 0; j < 4; j++) {
//             state[(j * 4) + i] = column[j];

//         }
//     }
    
// }



void createRoundKey(unsigned char *expandedKey, unsigned char *roundkey) { //xor expanded key with roundkey 
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            roundkey[(j*4) + i] = expandedKey[(i*4) + j];
        }
    }
}


void final_aes_round(unsigned char *state, const unsigned char *roundKey) {
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}
void aes_round(unsigned char *state, const unsigned char *roundKey) {
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey);
}
