#include <stdio.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using ll = long long;
using namespace std;

const int MAX_T = 100000;
const int MAX_DP = 1e9;

string s, t;
int s_count[26]{}, t_count[26]{};
int dp[27][MAX_T + 1];
int coun[26][26]{};
int max_t;
constexpr int char_size = 26;

string answer = "";
int steps = 0;
ll real_cost = 0;

vector<char> get_next_dp(vector<char>& pre_dp, int al) {
    vector<char> next_dp(max_t, 0);
    for (int i = 0; i < min(s_count[al], max_t); i++) {
        next_dp[i] = pre_dp[i];
    }
    for (int i = s_count[al]; i < max_t; i++) {
        next_dp[i] = (pre_dp[i] + next_dp[i - s_count[al]]) > 0;
    }
    return next_dp;
}

void get_next_dp(vector<char>& pre_dp, vector<char>& next_dp, int al) {
    for (int i = 0; i < min(s_count[al], max_t); i++) {
        next_dp[i] = pre_dp[i];
    }
    for (int i = s_count[al]; i < max_t; i++) {
        next_dp[i] = (pre_dp[i] + next_dp[i - s_count[al]]) > 0;
    }
}

bool check_t(vector<char>& dp) {
    for (int i = 0; i < char_size; i++) {
        if (!dp[t_count[i]]) {
            return false;
        }
    }
    return true;
}

int get_min_s() {
    vector<vector<char>> table(1 << char_size);
    vector<char> used(1 << char_size);
    table[0] = vector<char>(max_t, 0);
    table[0][0] = 1;

    priority_queue<pair<int, int>> pr;
    pr.push(make_pair(-0, 0));
    while (true) {
        int i, cost;
        tie(cost, i) = pr.top();
        pr.pop();

        if (i != 0) {
            int add_bit = i & -i;
            int from = i - add_bit;
            int add;
            for (int j = 0; j < char_size; j++) {
                if (add_bit >> j == 1) {
                    add = j;
                    break;
                }
            }
            table[i].resize(max_t);
            get_next_dp(table[from], table[i], add);
            if (check_t(table[i])) {
                return i;
            }
        }

        for (int j = 0; j < char_size; j++) {
            if (((i >> j) & 1) == 0 && s_count[j] > 0 &&
                used[i | (1 << j)] == 0) {
                pr.push(make_pair(cost - s_count[j], i | (1 << j)));
                used[i | (1 << j)] = 1;
            }
        }
    }
}

void fill_coun(vector<int>& use_s) {
    vector<vector<char>> dp(use_s.size() + 1);
    dp[0] = vector<char>(max_t, 0);
    dp[0][0] = 1;
    for (int i = 0; i < use_s.size(); i++) {
        dp[i + 1].resize(max_t);
        get_next_dp(dp[i], dp[i + 1], use_s[i]);
    }

    for (int al = 0; al < char_size; al++) {
        int i = t_count[al];
        for (int j = use_s.size() - 1; j >= 0; j--) {
            while (true) {
                if (i >= s_count[use_s[j]] &&
                    dp[j + 1][i - s_count[use_s[j]]] == 1) {
                    coun[use_s[j]][al]++;
                    i -= s_count[use_s[j]];
                } else {
                    break;
                }
            }
        }
    }
}

// ダブリングで構築 アルファベットごとに
// というわけではなくて、単純にN倍しています
// 後、この文字が何倍になるかという情報を返している
// 多分倍率が小さいものから代入するほうがコストが低い
// ダブリングで構築 アルファベットごとに
vector<tuple<char, string, bool>> get_insert(int al) {
    vector<tuple<char, string, bool>> ans;
    vector<int> kouho;
    for (int i = 0; i < 26; i++) {
        if (coun[al][i] > 0) kouho.push_back(i);
    }
    if (kouho.size() == 0) return ans;

    for (int i = 0; i < kouho.size(); i++) {
        string string_to;
        string_to += string(coun[al][kouho[i]], 'a' + kouho[i]);

        ans.emplace_back(
            make_tuple('A' + al, string_to, i != kouho.size() - 1));
    }
    return ans;
}

// 改善の余地あり
// ダブリングで構築 アルファベットごとに
// vector<pair<char, string>> get_insert(int al) {
//     vector<pair<char, string>> ans;
//     string string_to = "";
//     int sum = 0;
//     for (int i = 0; i < 26; i++) {
//         if (coun[al][i] > 0) {
//             string_to += std::string(coun[al][i], 'a' + i);
//             sum += coun[al][i];
//         }
//     }
//     ans.emplace_back(make_pair('A' + al, string_to));
//     return ans;
// }

tuple<vector<vector<int>>, vector<vector<int>>> get_char_cost(
    vector<pair<char, string>>& insert) {
    int size = insert.size();
    vector<vector<int>> cost(size + 1,
                             vector<int>(58));  // 58は'z'-'A'=57のため
    vector<vector<int>> length(size + 1, vector<int>(58, 1));
    for (int i = 0; i < 26; i++) {
        cost[size]['a' + i - 'A'] = i;
    }

    for (int i = size - 1; i >= 0; i--) {
        for (int j = 0; j < 58; j++) {
            cost[i][j] = cost[i + 1][j];
            length[i][j] = length[i + 1][j];
        }

        char from;
        string to;
        tie(from, to) = insert[i];
        int from_int = from - 'A';
        cost[i][from_int] = 0;
        length[i][from_int] = 0;
        for (char j : to) {
            cost[i][from_int] += cost[i + 1][j - 'A'];
            length[i][from_int] += length[i + 1][j - 'A'];
        }
    }
    return make_tuple(cost, length);
}

template <typename T>
void cycle_sort(vector<T>& a, vector<int>& cycle) {
    if (cycle.size() <= 1) return;

    int n = cycle.size();
    int i;
    for (i = 0; i < n - 1; i++) {
        if (a[cycle[i]] > cycle[i] && a[cycle[i + 1]] < cycle[i + 1]) {
            swap(a[cycle[i]], a[cycle[i + 1]]);

            char tmp_s[100];
            sprintf(tmp_s, "2 %d %d\n", cycle[i] + 1, a[cycle[i + 1]] + 1);
            answer += tmp_s;
            steps++;
            real_cost += abs(cycle[i] - cycle[i + 1]);
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

pair<vector<int>, string> sorted_index(string& s, vector<int>& cost,
                                       vector<int>& length) {
    int n = s.size();
    vector<int> from(n);
    for (int i = 0; i < n; i++) {
        from[i] = i;
    }
    stable_sort(from.begin(), from.end(), [&](const int& i, const int& j) {
        int a = s[i] - 'A', b = s[j] - 'A';
        return cost[a] * length[b] < cost[b] * length[a];
    });

    string new_s = "";
    vector<int> to(n);
    for (int i = 0; i < n; i++) {
        new_s += s[from[i]];
        to[from[i]] = i;
    }
    return {to, new_s};
}

string insert_string(string& s, pair<char, string>& insert) {
    string new_s = "";
    for (char i : s) {
        if (i == insert.first) {
            new_s += insert.second;
        } else {
            new_s += i;
        }
    }
    char tmp_s[1000];
    sprintf(tmp_s, "1 %c%s\n", insert.first, insert.second.c_str());
    answer += tmp_s;
    steps++;
    real_cost += insert.second.size();

    return new_s;
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

template <typename T>
void show_vector(vector<T>& a) {
    for (int i = 0; i < a.size(); i++) {
        printf("%02d ", a[i]);
    }
    printf("\n");
}

int main() {
    cin >> s >> t;

    // sとtの文字のカウント
    for (auto i : s) {
        s_count[i - 'a']++;
    }
    for (auto i : t) {
        t_count[i - 'a']++;
    }
    max_t = 0;
    for (int i = 0; i < char_size; i++) {
        max_t = max(max_t, t_count[i]);
    }
    max_t++;

    vector<char> dp(max_t, 0);
    dp[0] = 1;
    for (int i = 0; i < char_size; i++) {
        dp = get_next_dp(dp, i);
    }
    if (!check_t(dp)) {
        cout << "NO" << endl;
        return 0;
    } else {
        cout << "YES" << endl;
    }

    int use_s_int = get_min_s();
    vector<int> use_s;
    for (int i = 0; i < char_size; i++) {
        if ((use_s_int >> i) & 1 == 1) {
            use_s.push_back(i);
        }
    }

    vector<pair<char, string>> insert;

    fill_coun(use_s);

    // それぞれのSの文字が何倍になっているかを見て、小さいものから代入する
    vector<int> sum(26, 0);
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            sum[i] += coun[i][j];
        }
    }
    vector<int> dainyuu;
    for (int i = 0; i < 26; i++) {
        if (sum[i]) {
            dainyuu.push_back(i);
        } else {
            insert.emplace_back(make_pair('a' + i, ""));
        }
    }
    sort(dainyuu.begin(), dainyuu.end(),
         [&](const int& i, const int& j) { return sum[i] < sum[j]; });

    // // 小文字を大文字に変換
    for (int i = 0; i < 26; i++) {
        string tmp{(char)(i + 'A')};
        insert.emplace_back(make_pair(i + 'a', tmp));
    }

    vector<vector<tuple<char, string, bool>>> insert_alphabet(char_size);
    // 代入の操作を追加
    for (auto i : dainyuu) {
        for (auto j : get_insert(i)) {
            insert_alphabet[get<1>(j)[0] - 'a'].emplace_back(j);
        }
    }
    char t_last = t[t.size() - 1];
    for (int i = 0; i < char_size; i++) {
        for (auto& j : insert_alphabet[i]) {
            if (!get<2>(j)) {
                insert.emplace_back(make_pair(get<0>(j), get<1>(j)));
            } else {
                insert.emplace_back(
                    make_pair(get<0>(j), string({t_last, get<0>(j)})));
                insert.emplace_back(make_pair(t_last, get<1>(j)));
            }
        }
    }

    random_device rnd;
    // mt19937 mt(rnd());
    init_xor128(rnd());

    vector<vector<int>> cost_insert, lenth_insert;
    tie(cost_insert, lenth_insert) = get_char_cost(insert);
    for (int i = 0; i < insert.size(); i++) {
        s = insert_string(s, insert[i]);

        if (insert[i].second.size() != 0) {
            vector<int> to;
            string new_s;
            tie(to, new_s) = sorted_index(s, cost_insert[i], lenth_insert[i]);
            cycle_sort(to);
            s = new_s;
        }
    }

    printf("%d\n%s", steps, answer.c_str());

    // ここを消し忘れてはいけない
    cout << real_cost << endl;
    cout << s.size() << " " << t.size() << endl;
    cout << (s == t) << endl;
}