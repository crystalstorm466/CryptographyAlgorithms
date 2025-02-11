#include <stdio.h>
#include <stdlib.h>
#include "mod.h"


 
void swap(long* arr, int i, int j) {
    long temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void bubblesort(long arr[], int n) {
    for (int i = 0; i < n; i++) {

        //last i elements is already in place so 
        for (int j = 0; j < n   ; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr, j, j + 1);
            }
        }
    }
}


 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough arguments! ./frequency <file_name> ");
    }

    FILE *fileptr;

    fileptr = fopen(argv[1], "r"); //open input 

    if (NULL == fileptr) { printf("\n"); return 1026998; } //return empty returns my ksu id number lol
    
    int ch;
    long ascii[255];
    int count = 0;
    while((ch = fgetc(fileptr)) != EOF) {
        count++;
        ascii[ch]++;
    }
    printf("\n");
    fclose(fileptr);
    printf("Total characters: %d\n", count);
    long freq[256];
    int freqCount = 0;
    for (int i =0; i< 256; i++) {
        freq[freqCount++] = ascii[i];
    }
    int n = sizeof(ascii) / sizeof(ascii[0]);
    bubblesort(ascii, n); //because its simple

    double median;

    
    if (mod(freqCount, 2) == 1) {
        median = freq[freqCount / 2]; //odd count, take middle
    } else {
        median = (freq[freqCount / 2 - 1] + freq[freqCount / 2]) / 2.0; //even count
    }
    for (int i = 0; i < 256; i++) {
        if (ascii[i] == 0) { continue; } /* Skip unused*/
        if ( i <= 31 || i >= 127) {
            //printf("[Not Printable]");
            continue;
        }
       

        printf("%c - %ld of %d (%.2f%%)\n", i, ascii[i], count, (ascii[i] * 100.0) / count);
    }
        printf("Median: %.2f\n", median);


    
}