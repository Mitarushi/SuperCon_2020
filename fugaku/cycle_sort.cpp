#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

// #include "main.cpp"
using ll = long long;
using namespace std;

template <typename T>
void show_vector(vector<T>& a) {
    for (int i = 0; i < a.size(); i++) {
        printf("%02d ", a[i]);
    }
    printf("\n");
}

template <typename T>
void cycle_sort(vector<T>& a, vector<int>& cycle) {
    if (cycle.size() <= 1) return;

    int n = cycle.size();
    int i;
    for (i = 0; i < n - 1; i++) {
        if (a[cycle[i]] > cycle[i] && a[cycle[i + 1]] < cycle[i + 1]) {
            swap(a[cycle[i]], a[cycle[i + 1]]);
            break;
        }
    }
    vector<int> cycle_a, cycle_b;
    int j = cycle[i];
    do {
        cycle_a.push_back(j);
        j = a[j];
    } while (j != cycle[i]);
    j = cycle[i + 1];
    do {
        cycle_b.push_back(j);
        j = a[j];
    } while (j != cycle[i + 1]);

    sort(cycle_a.begin(), cycle_a.end());
    sort(cycle_b.begin(), cycle_b.end());

    cycle_sort(a, cycle_a);
    cycle_sort(a, cycle_b);
}

template <typename T>
void cycle_sort(vector<T>& a) {
    int n = a.size();
    vector<bool> is_used(n, false);

    for (int i = 0; i < n; i++) {
        if (is_used[i]) continue;

        vector<int> cycle;
        int j = i;
        do {
            cycle.push_back(j);
            is_used[j] = true;
            j = a[j];
        } while (j != i);

        sort(cycle.begin(), cycle.end());
        cycle_sort(a, cycle);
    }
}

//乱数
unsigned int seed[4];
void init_xor128(unsigned int s) {
    for (unsigned int i = 0; i < 4; ++i)
        seed[i] = s = 1812433253U * (s ^ (s >> 30)) + i;
}
unsigned int xor128() {
    unsigned int t = (seed[0] ^ (seed[0] << 11));
    seed[0] = seed[1];
    seed[1] = seed[2];
    seed[2] = seed[3];
    return (seed[3] = (seed[3] ^ (seed[3] >> 19)) ^ (t ^ (t >> 8)));
}

int range_random(int a, int b) { return xor128() % (b - a) + a; }
int range_random(int a) { return range_random(0, a); }

int main() {
    init_xor128(1234);
    int n = 10000;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i;
    }
    for (int i = 0; i < n; i++) {
        int j = range_random(i, n);
        swap(a[i], a[j]);
    }

    cycle_sort(a);
}