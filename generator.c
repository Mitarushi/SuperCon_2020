#include <stdlib.h>
#include <stdio.h>

#define SEED 0x0000000000000001
#define MIN_S_LENG 1
#define MAX_S_LENG 10000
#define MAX_T_LENG 100000

unsigned long long xor_shift() {
  static unsigned long long x = SEED;
  x = x ^ (x << 13);
  x = x ^ (x >> 7);
  x = x ^ (x << 17);
  return x;
}

// [0, x)
unsigned long long rnd(unsigned long long x) {
  return xor_shift() % x;
}

// [l, r]
unsigned long long range_rnd(unsigned long long l, unsigned long long r) {
  return l + rnd(r - l + 1);
}

int comp(const void *a, const void *b) {
  return *(char *)a - *(char *)b;
}

int main () {
  for (int t = 0; t < 10; t++) {
    char file_name[64];
    sprintf(file_name, "02_random_%02d.in", t + 1);
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
      return 1;
    }

    int s_leng = range_rnd(MIN_S_LENG, MAX_S_LENG);
    for (int i = 0; i < s_leng; i++) {
      fprintf(fp, "%c", (char)range_rnd('a', 'z'));
    }
    fprintf(fp, "\n");

    int t_leng = range_rnd(s_leng, MAX_T_LENG);
    char *t = malloc(sizeof(char) * (t_leng + 1));
    if (t == NULL) {
      return 1;
    }
    t[t_leng] = '\0';

    for (int i = 0; i < t_leng; i++) {
      t[i] = range_rnd('a', 'z');
    }
    qsort(t, t_leng, sizeof(char), comp);
    fprintf(fp, "%s", t);

    free(t);
    fprintf(fp, "\n");

    fclose(fp);
  }

  return 0;
}