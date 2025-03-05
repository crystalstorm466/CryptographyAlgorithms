#include "aes.h"
#include <stdio.h>


int main(int argc, char* argv[]) {
    printf("AES");

    int expandedKeySize = 176; 
    unsigned char expandedKey[expandedKeySize];
    unsigned char key[16] = {'k', 'k', 'k', 'k', 'e', 'e', 'e', 'e', 'y', 'y', 'y', 'y', '.', '.', '.', '.'};
    enum keySize size = SIZE_16;

    unsigned char plaintext[16] = {'H', 'A', 'C', 'K', ' ', 'T', 'H', 'E', ' ', 'B', 'O', 'X', '!', '!', '!', '!'};
    unsigned char ciphertext[16];
    unsigned char decrypted[16];
    int i;

      printf("\nCipher Key (HEX format):\n");

    for (i = 0; i < 16; i++)
    {
        // Print characters in HEX format, 16 chars per line
        printf("%2.2x%c", key[i], ((i + 1) % 16) ? ' ' : '\n');
    }
    //key expansion
    expandKey(expandedKey, key, size, expandedKeySize);
    createRoundKey(expandedKey, key);
     printf("\nExpanded Key (HEX format):\n");

    for (i = 0; i < expandedKeySize; i++)
    {
        printf("%2.2x%c", expandedKey[i], ((i + 1) % 16) ? ' ' : '\n');
    }

    printf("\nPlaintext (HEX format):\n");

    for (i = 0; i < 16; i++)
    {
        printf("%2.2x%c", plaintext[i], ((i + 1) % 16) ? ' ' : '\n');
    }

    for (int i = 0; i < 10; i++) {

    
    aes_round(plaintext, key);
    }
     printf("\nCiphertext (HEX format):\n");

    for (i = 0; i < 16; i++)
    {
        printf("%2.2x%c", plaintext[i], ((i + 1) % 16) ? ' ' : '\n');
    }
    


}