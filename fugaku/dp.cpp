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

string s, t;
int s_count[26], t_count[26];
int max_t;
constexpr int char_size = 26;

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

// const int get_min_s_parallel = 2560;
// unsigned int get_min_s() {
//     vector<vector<char>> table(1 << char_size);
//     table[0] = vector<char>(max_t, 0);
//     table[0][0] = 1;

//     vector<pair<int, int>> search;
//     for (int i = 1; i < (1 << char_size); i++) {
//         int cost = 0;
//         bool zero_flag = true;
//         for (int j = 0; j < char_size; j++) {
//             if ((i >> j) & 1 == 1) {
//                 if (s_count[j] == 0) {
//                     zero_flag = false;
//                     break;
//                 }
//                 cost += s_count[j];
//             }
//         }
//         if (zero_flag) {
//             search.emplace_back(make_pair(cost, i));
//         }
//     }
//     cout << "finish initializing search" << endl;
//     sort(search.begin(), search.end(),
//          [](const auto& i, const auto& j) { return i.first < j.first; });
//     cout << "finish initializing search" << endl;
//     int is_end = get_min_s_parallel;
//     int search_index;
//     while (true) {
//         vector<int> search_queue;
//         for (int i = search_index;
//              i < min(search_index + get_min_s_parallel, (int)search.size());
//              i++) {
//             search_queue.push_back(search[i].second);
//             table[search[i].second].resize(max_t);
//         }
//         cout << 123 << endl;
//         for (int i = 0; i < search_queue.size(); i++) {
//             int j = search_queue[i];
//             int add_bit = j & -j;
//             int from = j - add_bit;
//             int add;
//             for (int k = 0; k < char_size; k++) {
//                 if (add_bit >> k == 1) {
//                     add = k;
//                 }
//             }

//             get_next_dp(table[from], table[j], add);
//             if (check_t(table[j])) {
//                 is_end = min(is_end, i);
//             }
//         }
//         cout << 1234 << endl;

//         if (is_end != get_min_s_parallel) {
//             return search_queue[is_end];
//         }

//         search_index += get_min_s_parallel;
//     }
// }

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

template <typename T>
void show_vector(vector<T>& a) {
    for (int i = 0; i < a.size(); i++) {
        printf("%02d ", a[i]);
    }
    printf("\n");
}

int main() {
    cin >> s >> t;
    for (const char& i : s) {
        s_count[i - 'a']++;
    }
    for (const char& i : t) {
        t_count[i - 'a']++;
    }
    max_t = 0;
    for (int i = 0; i < char_size; i++) {
        max_t = max(max_t, t_count[i]);
    }
    max_t++;

    for (int i = 0; i < char_size; i++) {
        printf("%d ", s_count[i]);
    }
    printf("\n");
    for (int i = 0; i < char_size; i++) {
        printf("%d ", t_count[i]);
    }
    printf("\n");

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
    int a = get_min_s();
    cout << a << endl;
}
