#include <bits/stdc++.h>

using namespace std;
using ll = long long;

string substitute(string s, string k) {
    char x = k[0];
    string v = k.substr(1);
    string ns = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == x) {
            ns += v;
        } else {
            ns += s[i];
        }
    }
    return ns;
}

int main() {
    string y, s, t;
    cin >> s >> t >> y;
    if (y == "NO") {
        puts("Impossible");
        return 0;
    }
    ll n, res = 0;
    cin >> n;
    cout << n << endl;
    for (ll _ = 0; _ < n; _++) {
        int a;
        cin >> a;
        if (a == 1) {
            string k;
            cin >> k;
            res += k.length();
            s = substitute(s, k);
        } else {
            ll i, j;
            cin >> i >> j;
            swap(s[i - 1], s[j - 1]);
            res += j - i;
        }
    }
    if (s != t) {
        puts("unmatch");
        cout << s << endl;
    } else {
        printf("%lld\n", res);
    }
}