#include <stdio.h>
#include <string.h>
int mod(int a, int b) { //custom implementation of modulus r = ((a/b) - q )* b 
    
    // float q = a/b;
    // int decimal = (int) q;
    // q = decimal - q;
    // return q * b;

    if (b == 0) {  return -1; }

    int r = a - (a / b) * b;
    if ( r < 0) {
      r += b;
    }
    return r;

}



int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage: %s <ciphertext>\n", argv[0] ); return 1; }

  char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
  char ciphertext[100];
  char outputBuffer[100];
    strcpy(ciphertext, argv[1]);
    for (int y = 0; y < 26; y++) {
      memset(outputBuffer, 0, sizeof(outputBuffer));
      for (int x = 0; x < strlen(ciphertext); x++) {
        if (ciphertext[x] >= 'A' && ciphertext[x] <= 'Z') { //upercase letters
          outputBuffer[x] = mod((ciphertext[x] - 'A' + y), 26) + 'A';
        } else if (ciphertext[x] >= 'a' && ciphertext[x] <= 'z') { //lowercase letters
          outputBuffer[x] = mod((ciphertext[x] - 'a' + y), 26) + 'a';
        } else { //unchanged
          outputBuffer[x] = ciphertext[x];
        }
      }
      outputBuffer[strlen(ciphertext)] = '\0';
      printf("adding %d to each letter: %s\n", y, outputBuffer);
    }
    return 0;
    

 
}
