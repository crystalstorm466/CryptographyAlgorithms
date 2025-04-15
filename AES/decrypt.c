#include <endian.h>
#include <stdint.h>
#include "aes.h"

unsigned char getSBoxInvert(unsigned char num) { return rsbox[num]; }
unsigned char getRconValue(unsigned char num) { return Rcon[num]; }


    void invSubBytes(unsigned char *state) {
        for (int i = 0; i < 16; i++) {
            state[i] = getSBoxInvert(state[i]);
        }
    }
    void invShiftRow(unsigned char *state, unsigned char nbr) {
        unsigned char tmp;

        for (int i = 0; i < nbr; i++) {
            //if nbr is 3
            tmp = state[3];
            for (int j = 3; j> 0; j--) {
                state[j] = state[j - 1];
            } 
            state[0] = tmp;
        }

    }
    void invShiftRows(unsigned char *state) {
         //row 0 - no shift
        //row 1 - shift to left by 1
        //row 2 - shift to left by 2
        //row 3 - shift to left by 3

        for (int i = 0; i < 4; i++) {
            invShiftRow(state + 1 * 4, i);
        }
    }
  
  
    void invMixColumns(unsigned char *state) {
        // c1 = 0x0E * b1 ^ 0x09 * b2 ^ 0x0D * b3 ^ 0x0B * b4
        // c2 = 0x0B * b1 ^ 0x0E * b2 ^ 0x09 * b3 ^ 0x0D * b4
        // c3 = 0x0D * b1 ^ 0x0B * b2 ^ 0x0E * b3 ^ 0x09 * b4
        // c4 = 0x09 * b1 ^ 0x0D * b2 ^ 0x0B * b3 ^ 0x0E * b4
        // state is a 1D array representing a 4x4 matrix
    
        unsigned char column[4];
        unsigned char result[4];
    
        for (int i = 0; i < 4; i++) {
            // Extract the column
            column[0] = state[i];
            column[1] = state[i + 4];
            column[2] = state[i + 8];
            column[3] = state[i + 12];
    
            // Perform the inverse MixColumns transformation
            column[0] = multiply_galois(0x0E, column[0]) ^ multiply_galois(0x09, column[1]) ^ multiply_galois(0x0D, column[2]) ^ multiply_galois(0x0B, column[3]);
            column[1] = multiply_galois(0x0B, column[0]) ^ multiply_galois(0x0E, column[1]) ^ multiply_galois(0x09, column[2]) ^ multiply_galois(0x0D, column[3]);
            column[2] = multiply_galois(0x0D, column[0]) ^ multiply_galois(0x0B, column[1]) ^ multiply_galois(0x0E, column[2]) ^ multiply_galois(0x09, column[3]);
            column[3] = multiply_galois(0x09, column[0]) ^ multiply_galois(0x0D, column[1]) ^ multiply_galois(0x0B, column[2]) ^ multiply_galois(0x0E, column[3]);
    
            // Write the result back to the state
            state[i] = column[0];
            state[i + 4] = column[1];
            state[i + 8] = column[2];
            state[i + 12] = column[3];
        }
    }
   
    
    void inv_aes_final(unsigned char *state, const unsigned char *roundKey) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, roundKey);
    }

  void aes_invRound(unsigned char *state, const unsigned char *roundKey) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, roundKey);
        invMixColumns(state);
    }

    