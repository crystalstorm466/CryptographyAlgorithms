#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

#define MODULUS 0x13B




// Multiplication over GF(2^4) with the reduction polynomial m(x)=x^4 + x + 1 (0x13)
unsigned int multiply_galois_4bit(unsigned int A, unsigned int B) {
    unsigned int result = 0;
    for (int i = 0; i < 4; i++) {
        if (B & 1) {
            result ^= A; // XOR A into result if the current bit of B is 1
        }
        // Shift A left; if the top bit (bit 3) is set, reduce modulo 0x13
        unsigned int topBit = A & 0x8;
        A <<= 1;
        if (topBit) {
            A ^= 0x13;
        }
        A &= 0x0F; // keep only 4 bits
        B >>= 1;
    }
    return result & 0x0F;
}

// Multiplication over GF(2^8) with the AES reduction polynomial m(x)=x^8 + x^4 + x^3 + x + 1 (0x11B)
unsigned int multiply_galois_8bit(unsigned int A, unsigned int B) {
    unsigned int result = 0;
    for (int i = 0; i < 8; i++) {
        if (B & 1) {
            result ^= A;
        }
        // Shift A left; if the top bit (bit 7) is set, reduce modulo 0x11B
        unsigned int topBit = A & 0x80;
        A <<= 1;
        if (topBit) {
            A ^= 0x11B;
        }
        A &= 0xFF; // keep only 8 bits
        B >>= 1;
    }
    return result & 0xFF;
}


int f(int R, int K, int m) {
    //simple
    // = (Rn-1 * keyn) mod 0x13)16 over GF(24) and ai ∈ GF(2) with m(x) = x4 + x1 + 1           i.e. 0x13)16 
    // f = Rn-1 * key n % 0x13) for 8 bit 
    //0x11b for 16 bit
    if (m == 0x11B) {
        return multiply_galois_8bit(R, K);
    } else if ( m == 0x13) {
        return multiply_galois_4bit(R, K);
    }
    //return multiply_galois(R, K, m);
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
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
  
    if (rank == 0) {

  
  printf("This project took so long to make\n");
    printf("It is held together with hopes and dreams\n");
    printf("On behalf of my sanity, please just let me pass this class\n");
    
    if (argc != 6) {  
        //key is also # of rounds
        printf("Error! Too few arguments\n");
        printf("Usage: %s <input file> <output file> <key> <block size: 8|16> <e|d>\n", argv[0]);
        MPI_Finalize();
        return 2;
    }
} 

    // Get command-line arguments
    char *input  = argv[1];
    char *output = argv[2];
    char *key_input = argv[3];
    int rounds = strlen(key_input);
    int block_size = atoi(argv[4]);  // use atoi since block size is an integer
    int input_length = strlen(input);

    if (block_size != 8 && block_size != 16) {
        fprintf(stderr, "Error! Block size invalid! Must be either 8 or 16.\n");
        return 4;
    }
    
    // Allocate keys; each character in key_input serves as one round key.
    int *keys = malloc(rounds * sizeof(int));
    if (!keys) {
        fprintf(stderr, "Memory allocation failed for keys.\n");
        return 1;
    }
    int shift_pp = rounds/ size;
   int start_shift = rank * shift_pp;
   int end_shift = start_shift + shift_pp;
    for (int i = 0; i < rounds; i++) {
        keys[i] = (unsigned char) key_input[i];
    }
    MPI_File fp_in;
    MPI_File_open(MPI_COMM_WORLD, input, MPI_MODE_RDONLY, MPI_INFO_NULL, &fp_in);

    MPI_Offset filesize;
    MPI_File_get_size(fp_in, &filesize);
    
   MPI_Offset chunk_size = filesize / size;
  MPI_Offset start_offset = rank * chunk_size;

  if (rank == size - 1) {
    chunk_size = filesize - start_offset;
  }

  char *local_buffer = malloc(chunk_size);

  MPI_File_read_at(fp_in, start_offset, local_buffer, chunk_size, MPI_CHAR, MPI_STATUS_IGNORE);
  MPI_File_close(&fp_in);

  //encryption
    // Determine the mode
    char mode = argv[5][0];
    bool encrypt = true;
    if (mode == 'e')
        encrypt = true;
    else if (mode == 'd')
        encrypt = false;
    else {
        fprintf(stderr, "Error! Mode must be 'e' for encryption or 'd' for decryption.\n");
        free(keys);
        return 5;
    }
    
   
   // Open input file
   //FILE *fp_in = MPI_File_open(input, "rb");
   if (!fp_in) {
       fprintf(stderr, "Error opening input file: %s\n", input);
       free(keys);
       return 3;
   }
   
   // Read input file into buffer
   if (!local_buffer) {
       fprintf(stderr, "Memory allocation failed for input buffer.\n");
       MPI_File_close(&fp_in);
       free(keys);
       return 1;
   }
   
   long padded_filesize = filesize;
   // For encryption, apply padding (PKCS#7 style padding)
   if (encrypt) {
    int padding_need = (filesize % block_size == 0) ? block_size : block_size - (filesize % block_size);
    padded_filesize = filesize + padding_need;
   // char *tmp = realloc(local_buffer, chunk_size);
   //if (!tmp) {
   //     fprintf(stderr, "Memory reallocation failed for padding.\n");
   //     free(local_buffer);
   //     free(keys);
   //     return 1;
    //}
    //local_buffer = tmp;
    unsigned char padding_value = (unsigned char) padding_need;
    for (long i = filesize; i < chunk_size; i++) {
        local_buffer[i] = padding_value;
    }
}

    shift_pp = padded_filesize/ size;
    start_shift = rank * shift_pp;
    end_shift = start_shift + shift_pp;

// Process file data according to mode and block size
    if(encrypt) {   
        if (block_size == 8) {
              if (rank == 0) {
                  printf("Encryption 8-bit blocks\n");
                }

            // Process data one byte at a time (8-bit split into two 4-bit halves)
            for (long i = 0; i < chunk_size; i++) {
                unsigned char block = local_buffer[i];
                unsigned char L = (block >> 4) & 0x0F;
                unsigned char R = block & 0x0F;
                for (int r = 0; r < rounds; r++) {
                    unsigned char temp = R;
                    // Use GF(2^4) arithmetic with modulus 0x13 and keep 4-bit result
                    R = L ^ (f(R, keys[r], 0x13) & 0x0F);
                    L = temp;
                }
                local_buffer[i] = (L << 4) | (R & 0x0F);

        //MPI_File_write_all(&out, 1, 1, fp_out);
                printf("%02x", local_buffer[i]);
            }
        } else if (block_size == 16) {
            printf("Encryption 16-bit blocks\n");
            //question 1 approach 
            // Process data two bytes at a time
            for (long i = 0; i < chunk_size; i+=2) {
                unsigned char L = local_buffer[i];
                unsigned char R = local_buffer[i + 1];
                for (int r = 0; r < rounds; r++) {
                    unsigned char temp = R;
                    // Use GF(2^8) arithmetic with modulus 0x11B
                    R = L ^ (f(R, keys[r], 0x11B) & 0xFF);
                    L = temp;
                }
            //    MPI_File_write_all(&L, 1, 1, fp_out);
            //    MPI_File_write_all(&R, 1, 1, fp_out);
                printf("%02x", L);
                printf("%02x", R);

                local_buffer[i] = (L << 4) | (R & 0x0F);

            }
        }    
    } else { //Begin Decryption Block
        if (block_size == 8) {
            if (rank == 0) {
            printf("Decryption: 8 Blocks\n");
              }     
          //allocate decrypt buffer
          unsigned char *decBuffer = malloc(chunk_size);
            if (!decBuffer) {
                fprintf(stderr, "Memory allocation failed for decryption buffer.\n");
                MPI_File fp_out;
                MPI_File_close(&fp_out);
                free(local_buffer);
                free(keys);
                return 5;
            }
             shift_pp = filesize/ size;
            start_shift = rank * shift_pp;
            end_shift = start_shift + shift_pp;


            for (long i = start_offset; i < chunk_size; i++) {
                unsigned char block = decBuffer[i];
                unsigned char L = (block >> 4) & 0x0F;
                unsigned char R = block & 0x0F;
                for (int r = rounds - 1; r >= 0; r--) {
                    unsigned char temp = L;
                    L = R ^ (f(L, keys[r], 0x13) & 0x0F);
                    R = temp;
                }
                decBuffer[i] = (L << 4) | (R & 0x0F);
            }
            // Remove padding: assume last byte indicates the number of padding bytes
            int unpadded_len = filesize;
            if (unpadded_len > 0) {
                unsigned char pad_val = decBuffer[unpadded_len - 1];
                if (pad_val > 0 && pad_val <= block_size) {
                    unpadded_len -= pad_val;
                }
            }
               
              free(local_buffer);
              free(keys);
 
            //MPI_File_write_all(decBuffer, 1, unpadded_len, fp_out);
            printf("%s\n", decBuffer);
            free(decBuffer);
        } else if (block_size == 16) { //normal way
            printf("Decryption 16-bit block sizes\n");

            // For 16-bit mode, also allocate a decryption buffer
            unsigned char *decBuffer = malloc(filesize);
            if (!decBuffer) {
                MPI_File fp_out;
                fprintf(stderr, "Memory allocation failed for decryption buffer.\n");
                MPI_File_close(&fp_out);
                free(local_buffer);
                free(keys);
                return 5;
            }
     shift_pp = filesize/ size;
    start_shift = rank * shift_pp;
    end_shift = start_shift + shift_pp;


            for (long i = start_offset; i < chunk_size; i += 2) {
                unsigned char L = decBuffer[i];
                unsigned char R = decBuffer[i + 1];
                for (int r = rounds - 1; r >= 0; r--) {
                    unsigned char temp = L;
                    L = R ^ (f(L, keys[r], 0x11B) & 0xFF);
                    R = temp;
                }
                local_buffer[i] = L;
                local_buffer[i + 1] = R;
            }
            // Remove padding (last byte gives the number of padding bytes)
            int unpadded_len = filesize;
            if (unpadded_len > 0) {
                unsigned char pad_val = decBuffer[unpadded_len - 1];
                if (pad_val > 0 && pad_val <= block_size) {
                    unpadded_len -= pad_val;
                }
            }
            local_buffer = decBuffer;
         //   MPI_File_write_all(decBuffer, 1, unpadded_len, fp_out);
            printf("%s\n", local_buffer);


            free(decBuffer);
        }
        
        }
        //write to buffer
        MPI_File fp_out;
        MPI_File_open(MPI_COMM_WORLD, local_buffer, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp_out);
        MPI_File_write_at(fp_out, start_offset, local_buffer, chunk_size, MPI_CHAR, MPI_STATUS_IGNORE);
        MPI_File_close(&fp_out);


        //MPI_File_write_all(fp_out);
        
        free(local_buffer);
        free(keys);
        MPI_Finalize();
        return 0;

    }


