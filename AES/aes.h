#ifndef AES_H
#define AES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
    
    #endif
enum keySize {
        SIZE_16 = 16,
        SIZE_24 = 24,
        SIZE_32 = 32
    };
    //AES S-box
    extern const unsigned char sbox[256];
    extern const unsigned char rsbox[]; //inverse
    extern const unsigned char Rcon[]; //rcon

     //function
    unsigned char getSBoxValue(unsigned char num);
    unsigned char getSBoxInvert(unsigned char num);
    unsigned char getRconValue(unsigned char num);

    // Function declarations

    void rotate(unsigned char *word);
    void core(unsigned char *word, int iteration);
    unsigned int multiply_galois(unsigned int A, unsigned int B);

   

    //key expan
    void expandKey(unsigned char *expandedKey, const unsigned char *key, enum keySize size , size_t expandedKeySize);

    //cipher fun
    // AES Cipher Functions
    void subBytes(unsigned char *state);
    void shiftRows(unsigned char *state);
    void mixColumns(unsigned char *state);
    void addRoundKey(unsigned char *state, const unsigned char *roundKey);
    void aes_round(unsigned char *state, const unsigned char *roundKey);
    void final_aes_round(unsigned char *state, const unsigned char *roundKey);

    void createRoundKey(unsigned char *expandedKey, unsigned char *roundkey);

    // AES Decryption Functions
    void invSubBytes(unsigned char *state);
    void invShiftRows(unsigned char *state);
    void invMixColumns(unsigned char *state);
    void inv_aes_final(unsigned char *state, const unsigned char *roundKey);
    void aes_invRound(unsigned char *state, const unsigned char *roundKey);
    void aes_invMain(unsigned char *state, const unsigned char *expandedKey, int nbrRounds);
    void aes_main(unsigned char *state, const unsigned char *roundKey);

    // AES Encryption/Decryption
    void aes_encrypt( unsigned char *input, unsigned char *output,  unsigned char *key, size_t keySize);
    void aes_decrypt(const unsigned char *input, unsigned char *output, const unsigned char *key, size_t keySize);
    #ifdef __cplusplus
}
#endif

#endif // AES_H
