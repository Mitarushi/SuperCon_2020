// #include <bits/stdc++.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;

// メモ:
// stringに破壊的操作を行うので値渡しにしたが、組みこむときは参照渡しにすること

const int line_n = 10000;

ll simple_cost(string s) {
    ll cost = 0;
    for (int i = 0; i < s.size(); i++) {
        cost += i * (26 - (int)(s[i] - 'a'));
    }

    // ここの下は定数になるので前もって計算しておくこと(入力のTから計算できる)
    sort(s.begin(), s.end());
    for (int i = 0; i < s.size(); i++) {
        cost -= i * (26 - (int)(s[i] - 'a'));
    }

    return cost;
}

class BIT {
   private:
    vector<ll> bit;
    ll size;

   public:
    BIT(ll n) {
        bit.resize(n + 1);
        size = n;
    }
    ll sum(ll i) {
        ll res = 0;
        while (i > 0) {
            res += bit[i];
            i -= i & -i;
        }
        return res;
    }
    void add(ll i, ll x) {
        while (i <= size) {
            bit[i] += x;
            i += i & -i;
        }
    }
};

ll inversion_number(string s) {
    ll n = s.length(), res = 0;
    BIT cnt(27);  // 文字の種類が少ないので26で足りる
    for (ll i = n - 1; i >= 0; i--) {
        res += cnt.sum(s[i] - 'a');
        cnt.add(s[i] - 'a' + 1, 1);
    }
    return res;
}

char med3(char x, char y, char z) {
    if (x < y) {
        if (y < z)
            return y;
        else if (z < x)
            return x;
        else
            return z;
    } else {
        if (z < y)
            return y;
        else if (x < z)
            return x;
        else
            return z;
    }
}

ll quicksort(string a, int left, int right) {
    ll cost = 0;
    if (left < right) {
        int i = left, j = right;
        char pivot = med3(a[i], a[i + (j - i) / 2], a[j]);
        while (true) {
            while (a[i] < pivot) i++;
            while (pivot < a[j]) j--;
            if (i >= j) break;
            if (a[i] != a[j]) {
                swap(a[i], a[j]);
                cost += abs(j - i);
            }
            i++;
            j--;
        }
        cost += quicksort(a, left, i - 1);
        cost += quicksort(a, j + 1, right);
    }
    return cost;
}

int main() {
    cin.tie(0);
    // ios::sync_with_stdio(false);
    string s;
    for (int i = 0; i < line_n; i++) {
        cin >> s;
        ll simple = simple_cost(s);
        ll inver = inversion_number(s);
        ll quick = quicksort(s, 0, s.length() - 1);
        printf("%d %lld %lld %lld\n", s.size(), simple, inver, quick);
    }
}