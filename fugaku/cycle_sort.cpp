#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

//#include "main.cpp"
using ll = long long;
using namespace std;

template <typename T>
void show_vector(vector<T>& a) {
    for (int i = 0; i < a.size(); i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

template <typename T>
void cycle_sort(vector<T>& a, vector<int>& cycle) {
    if (cycle.size() <= 1) return;

    int n = cycle.size();
    int i;
    for (i = 0; i < n - 1; i++) {
        if (a[i] > i && a[i] < i + 1) {
            swap(a[i], a[i + 1]);
        }
    }
    vector<int> cycle_a, cycle_b;
    int j = i;
    do {
        cycle_a.push_back(j);
        j = a[j];
    } while (j != i);
    j = i + 1;
    do {
        cycle_b.push_back(j);
        j = a[j];
    } while (j != i + 1);
    show_vector(a);
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

int main() {
    vector<int> a = {3, 1, 4, 5, 9, 2, 6, 8, 7, 0};
    cycle_sort(a);
}