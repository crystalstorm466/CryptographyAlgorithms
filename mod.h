#ifndef MOD_H
#define MOD_H

// Inline function definition inside the header
static inline int mod(int a, int b) {
    if (b == 0) { return -1; }

    int r = a - (a / b) * b;
    if (r < 0) {
        r += b;
    }
    return r;
}

#endif // MOD_H
