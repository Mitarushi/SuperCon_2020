#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const int MAX_T = 100000;
const int MAX_DP = 1e9;

string s, t;
int s_count[26]{}, t_count[26]{};
int dp[27][MAX_T + 1];
int coun[26][26]{};

// ナップザックの復元
void nap_count(int i, int alphabet) {
    for (int j = 25; j >= 0; j--) {
        while (1) {
            if (i < s_count[j] ||
                dp[j + 1][i] != dp[j + 1][i - s_count[j]] + 1) {
                break;
            } else {
                i -= s_count[j];
                coun[j][alphabet]++;
            }
        }
    }
}

// ダブリングで構築 アルファベットごとに
vector<pair<char, string>> get_insert(int al) {
    vector<pair<char, string>> ans;
    while (1) {
        bool zero = true;
        bool one = true;

        string string_to = "";
        for (int i = 0; i < 26; i++) {
            if (coun[al][i] % 2 == 1) {
                string_to += i + 'a';
            }
            if (coun[al][i] > 0) {
                zero = false;
                if (coun[al][i] > 1) {
                    one = false;
                }
            }
            coun[al][i] /= 2;
        }
        if (!one) {
            string_to = string_to + (char)(al + 'A') + (char)(al + 'A');
        }
        if (zero) {
            if (s_count[al] > 0) {
                ans.push_back(make_pair(al + 'A', ""));
            }
            return ans;
        } else {
            ans.push_back(make_pair(al + 'A', string_to));
            if (one) {
                return ans;
            }
        }
    }
}

// カルダノの代入
string substitute(string s, char x, string v) {
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
    cin >> s >> t;

    for (auto i : s) {
        s_count[i - 'a']++;
    }
    for (auto i : t) {
        t_count[i - 'a']++;
    }

    // 和が最小となるDP
    fill(dp[0], dp[27], MAX_DP);
    dp[0][0] = 0;
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j <= MAX_T; j++) {
            dp[i + 1][j] = dp[i][j];
            if (j - s_count[i] >= 0) {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i + 1][j - s_count[i]] + 1);
            }
        }
    }

    // 全てで復元可能かチャック
    for (int i = 0; i < 26; i++) {
        if (dp[26][t_count[i]] == MAX_DP) {
            cout << "NO" << endl;
            return 0;
        } else {
            nap_count(t_count[i], i);
        }
    }
    cout << "YES" << endl;

    string s_change = s;

    // 小文字を大文字に変換
    for (int i = 0; i < 26; i++) {
        if (s_count[i] > 0) {
            printf("1 %c%c\n", i + 'a', i + 'A');
            s_change = substitute(s_change, i + 'a', string({(char)(i + 'A')}));
        }
    }
    // 大文字->小文字にして代入
    for (int i = 0; i < 26; i++) {
        for (auto j : get_insert(i)) {
            printf("1 %c%s\n", j.first, j.second.c_str());
            s_change = substitute(s_change, j.first, j.second);
        }
    }

    cout << s_change << endl;
}
