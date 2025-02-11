#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mod.h"


void encrypt(char *input, char *key, char *output) {
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int alpha_len = strlen(alphabet);
    int length = strlen(input);
    int key_len = strlen(key);

    for (int i = 0, key_index = 0; i < length; i++) {
        char input_char = input[i];
        char *input_pos = strchr(alphabet, input_char);

        if (input_pos) {
            //char key_char = key[key_index % key_len];
            char key_char = key[mod(key_index, key_len)];
            char *key_pos = strchr(alphabet, key_char);

            if (!key_pos) {
                printf("Error: Key contains invalid character '%c'.\n", key_char);
                exit(1);
            }

            int index_input =input_pos - alphabet;
            int index_key = key_pos - alphabet;
            //int new_index = (index_input + index_key) % alpha_len;
            int new_index = mod( (index_input + index_key), alpha_len);
            output[i] = alphabet[new_index];
            key_index++;

        } else {
          output[i] = input_char;
        }
      }
      output[length] = '\0';

  }


void decrypt(char *input, char *key, char* output) {
  char alphabet[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; //og alphabet
  //char alphabet[] = "aeiouycdx_IVXLCDMK012"; //for response 2
  int alpha_len = strlen(alphabet);
  int length = strlen(input);
  int key_len = strlen(key);
 for (int i = 0, key_index = 0; i < length; i++) {
        char input_char = input[i];
        char *input_pos = strchr(alphabet, input_char);

        if (input_pos) {
            char key_char = key[mod(key_index, key_len)];
            char *key_pos = strchr(alphabet, key_char);

            if (!key_pos) {
                printf("Error: Key contains invalid character '%c'.\n", key_char);
                exit(1);
            }

            int index_input =input_pos - alphabet;
            int index_key = key_pos - alphabet;
            int new_index = mod( (index_input + index_key), alpha_len);
            output[i] = alphabet[new_index];
            key_index++;

        } else {
          output[i] = input_char;
        }
      }
      output[length] = '\0';
}


int main(int argc, char *argv[]) {
  if (argc < 5) { printf("Usage: <input file> <key> <output> < e | d >"); return 1; }

  FILE *fptr;
  FILE *outptr;

  fptr = fopen(argv[1], "r"); //open input 
  outptr = fopen(argv[3], "w"); //open output
  char method = argv[4][0]; // Or use strcmp as needed
  printf("%s", &method);
    char input[100];
    char output[100];

    if (method != 'e' && method != 'd') { // Check method using char comparison
        fprintf(stderr, "Error: Invalid method. Use 'e' for encrypt or 'd' for decrypt.\n");
        fclose(fptr);
        fclose(outptr);
        return 1;
    }

  while (fgets(input, sizeof(input), fptr) != NULL) {
    if (method == 'e') {
      encrypt(input, argv[2], output); // Now 'input' has data
    } else if (method == 'd') {
      decrypt(input, argv[2], output); // Now 'input' has data
    }
    fprintf(outptr, "%s", output); // Write the output to the file
  } 
  fclose(fptr);
  fclose(outptr);

  return 0;


}

