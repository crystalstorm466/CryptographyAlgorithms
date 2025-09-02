#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include "mod.h"

int main(int argc, char *argv[]) {
 MPI_Init(&argc, &argv);
 
 int rank, size;
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 
if (argc < 2) {
  if( rank == 0 ) {
     printf("Usage: %s <ciphertext>\n", argv[0]);
   }
   MPI_Finalize();
   return 1;
}


  char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
  char ciphertext[100];
  char outputBuffer[100];
    strcpy(ciphertext, argv[1]);
  //3 divide work
  int shift_pp = 26 / size;
  int start_shift = rank * shift_pp;
  int end_shift = start_shift + shift_pp;

  if (rank == size - 1) {
    end_shift += 26 % size;
  }

  for (int y = start_shift; y < end_shift; y++) {
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
      printf("Process %d (rank) adding %d to each letter: %s\n", rank , y, outputBuffer);
    }
    MPI_Finalize();
    return 0;
    

 
}
