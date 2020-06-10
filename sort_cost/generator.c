#include <stdio.h>
#include <stdlib.h>

#define SEED 0x0000000000000001
#define MIN_S_LENG 1
#define MAX_S_LENG 10000
#define MAX_T_LENG 100000
#define LINE_N 10000

unsigned long long xor_shift() {
    static unsigned long long x = SEED;
    x = x ^ (x << 13);
    x = x ^ (x >> 7);
    x = x ^ (x << 17);
    return x;
}

// [0, x)
unsigned long long rnd(unsigned long long x) { return xor_shift() % x; }

// [l, r]
unsigned long long range_rnd(unsigned long long l, unsigned long long r) {
    return l + rnd(r - l + 1);
}

void half_sort(char *s, int t_leng, int k) {
    int h = t_leng * 10 / 13;
    while (1) {
        for (int i = 0; i + h < t_leng; i++) {
            if (s[i] > s[i + h]) {
                char tmp = s[i];
                s[i] = s[i + h];
                s[i + h] = tmp;
                k--;
                if (k <= 0) {
                    return;
                }
            }
        }
        if (h <= 2) {
            return;
        } else {
            h = h * 10 / 13;
        }
    }
}

int main() {
    char file_name[64] = "random_input.txt";
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
        return 1;
    }
    for (int j = 0; j < LINE_N; j++) {
        int s_leng = range_rnd(MIN_S_LENG, MAX_S_LENG);

        int t_leng = range_rnd(s_leng, MAX_T_LENG);
        char *t = malloc(sizeof(char) * (t_leng + 1));
        if (t == NULL) {
            return 1;
        }
        t[t_leng] = '\0';

        for (int i = 0; i < t_leng; i++) {
            t[i] = range_rnd('a', 'z');
        }
        half_sort(t, t_leng, range_rnd(0, t_leng * 3 / 2));
        fprintf(fp, "%s", t);

        free(t);
        fprintf(fp, "\n");
    }
    fclose(fp);

    return 0;
}