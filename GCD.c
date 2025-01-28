#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int mod(int a, int b) { //custom implementation of modulus r = ((a/b) - q )* b 
    if (b == 0) { printf("Error: Division by 0 is not allowed.\n"); return 0;}
    int q = a/b; 
    return a - q*b;
}
int extended_gcd(int a, int b, int *x, int *y) {

    if (b == 0) { //base case if b is 0 gcd is a. 
        *x = 1; //set x = 1 
        *y = 0; // set y to 0
        return a; //return gcd which is a
    }

    int x1, y1; //coeffic for recursion
    int gcd = extended_gcd(b, mod(a,b), &x1, &y1); //recursive call swap a and b and reduce b using mod

    *x = y1; //new x becomes y1 from previous step
    *y = x1 - ( a / b ) * y1; //new y is calculated using formula

    return gcd;
}

int normal_gcd(int a, int b) { //long way
    int r = mod(a,b);
    while (r != 0) {
        int q = (a/b);
        int decimal = (int) q;
        q = decimal - q;
        r = mod (a,b);
        if (r == 0) { return b;}
        a = b;
        b = r;
    }
    return 0;
}

int recursive_gcd(int a, int b) { //recursive way
    if (a == 0) {
        return b;
    }
    return recursive_gcd(mod(b, a), a); 
}
int main(int argc, char *argv[]) {
     if (argc != 3) {
        printf("Usage: %s <a> <b>\n", argv[0]);
        return 1;
    }
    
    float q, r;
    
    int a,b;
    a = atof(argv[1]);
    b = atof(argv[2]);
    int gcd = recursive_gcd(a, b);
    int x, y;

    printf("GCD(%d, %d) = %d\n", a, b, recursive_gcd(a, b));

    printf("Extended GCD(%d, %d) = %d\n", a, b, extended_gcd(a, b, &x, &y));
    printf("Coefficients x = %d, y = %d\n", x, y);
    
    }
   
    


