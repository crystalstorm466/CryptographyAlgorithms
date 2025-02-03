#include <stdio.h>

void DecimalToBinary(int n) {
    // Print binary in 8-bit format
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <ASCII_character>\n", argv[0]);
        return 1;
    }

    // Convert ASCII character to integer
    char ascii_char = argv[1][0];
    int decimal = (int)ascii_char;

    // Print results
    printf("Character: %c\n", ascii_char);
    printf("Hexadecimal: 0x%X\n", decimal);
    printf("Binary: ");
    DecimalToBinary(decimal);
    printf("\n");

    return 0;
}
