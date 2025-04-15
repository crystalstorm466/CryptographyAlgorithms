#include <stdio.h>
#include <math.h>

#define MODULUS 0x11B

int get_degree(unsigned int a) {
    int degree = -1;

    while(a != 0) {
        ++degree;
        a >>= 1;
    }
    return degree;
}

void divide_galosi(unsigned int a, unsigned int m, unsigned int *r, unsigned int *q) {
    *r = a;
    while (get_degree(*r) >= get_degree(m)) {
        int shift = get_degree(*r) - get_degree(m);
        *q ^= (1 << shift); 
        *r ^= ( m << *q);
    
    } 
}

unsigned int divide_galosi(unsigned int a, unsigned int m) {
   unsigned int result = a;

    while(get_degree(result) >= get_degree(m)) {
        int shift = get_degree(result) - get_degree(m);
        result ^= (m << shift);
    }
    return result;

}

unsigned int multiply_galois(unsigned int A, unsigned int B, unsigned int m) {
    int index = 0;
    unsigned int result = 0;

    while (B) {
        if (B & 1) { //if lowest bit of B is sett, add  (XOR) shifted A to result
            result ^= A;
        }
        A <<= 1;
        if (A & 0x100) { //if A exceeds 8 bit (x^8) reduce module m(x)
            A^= MODULUS; //0x11b
        }
        B >>= 1;
    }
    return result;
        
}
int main(int argc, char* argv[]) {

    // if (argc < 3) {
    //     printf("Usage: %s <multiplicand> <multiplier>\n", argv[0]);
    //     return 1;
    // }


    // unsigned int a = strtoul(argv[1], NULL, 0); //multiplicand
    // unsigned int b = strtoul(argv[2], NULL, 0); //multiplier


    unsigned int oneDivide = divide_galosi(0x1000, 0x11B);
    unsigned int twoDivide = divide_galosi(0xE1, 0x11B);
    unsigned int threeDivide = divide_galosi(0x32CFE1, 0x11B);
    unsigned int fourDivide = divide_galosi(0xE1, 0x11B);

    unsigned int oneMultiply = multiply_galois(0xD5, 0x61, 0x11B);
    unsigned int twoMultiply = multiply_galois(0x1E3C, 0x1E3C, 0x11B);
    printf("Divide Galosi (0x1000, 0x11B): 0x%x\n", oneDivide);
    printf("Divide Galosi (0xE1, 0x11B): 0x%x\n", twoDivide);
    printf("Divide Galosi (0x32CFE1, 0x11B): 0x%x\n", threeDivide);
    printf("Divide Galosi (0xE1, 0x11B): 0x%x\n", fourDivide);


    printf("Multiply Galosi (0xD5, 0x61, 0x11B): 0x%x\n", oneMultiply);
    printf("Multiply Galosi (0x1E3C, 0x1E3C, 0x11B): 0x%x\n", twoMultiply);

    //     printf("Multiply Galosi (0xD5, 0x61, 0x11B): 0%d\n", oneMultiply);
    // printf("Multiply Galosi (0x1E3C, 0x1E3C, 0x11B): 0%d\n", twoMultiply);
    
    printf("Get Degree\n");
    printf("Degree of 0x3CF0: %d\n", get_degree(0x3CF0));
    printf("Degree of 0x10000: %d\n", get_degree(0x10000));
    printf("Degree of 0x00: %d\n", get_degree(0x00));
    printf("Degree of 0x01: %d\n", get_degree(0x01));


   // printf("Multiplication result: 0x%x\n", multiply_galois(a, b, MODULUS));
}


