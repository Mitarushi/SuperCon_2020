#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <iostream>
// #include <random>
#include <string>
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

string answer = "";
int steps = 0;

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

ll min_cost;  // tのコスト
ll t_length;
const double cost_param[] = {
    -0.0000000000008402,     0.0000004798000799,    -0.0002000268185879,
    0.0044613262787533,      -0.0000001652505149,   0.2437505930922388,
    -7.7352385687962633,     25.8180687932298483,   0.0000356935469576,
    -5.5227029004491737,     1456.5673308701468613, -47616.8398519225956989,
    -0.0145529321527654,     224.8752720847048465,  -54970.5081915919436142,
    2140886.3656231933273375};

ll simple_cost(const string& s) {
    ll cost = 0;
    for (int i = 0; i < s.size(); i++) {
        cost += i * (26 - (int)(s[i] - 'a'));
    }
    return cost;
}

ll fix_cost(ll n, ll cost) {
    double a[] = {n * n, n, pow(n, 0.5), 1};
    double b[] = {cost, pow(cost, 0.5), pow(cost, 0.25), 1};

    int param_count = 0;
    double correct = 0;
    for (double j : b) {
        for (double i : a) {
            correct += i * j * cost_param[param_count];
            param_count++;
        }
    }
    return (ll)round(correct);
}

// 0-indexedに改造して、テンプレートをいろいろしたやつ
template <typename T>
class BIT {
   private:
    vector<T> bit;
    int size;

   public:
    BIT() {}

    BIT(int n) {
        bit.resize(n + 1);
        size = n;
    }
    T sum(int i) {
        i++;
        T res = 0;
        while (i > 0) {
            res += bit[i];
            i -= i & -i;
        }
        return res;
    }
    void add(int i, T x) {
        i++;
        while (i <= size) {
            bit[i] += x;
            i += i & -i;
        }
    }

    // [i,j)の和
    T section(int i, int j) { return sum(j - 1) - sum(i - 1); }
};

class swap_calc {
   public:
    vector<pair<char, string>> insert;
    // vector<vector<pair<int, int>>> swap_item;
    string s;
    ll cost_estimate;
    ll swap_cost;
    int s_length;

    vector<vector<int>> letter_cost;
    vector<vector<int>> letter_length;
    int insert_index;

    vector<int> string_cost;
    vector<int> string_length;
    BIT<int> cost_bit;
    BIT<int> length_bit;

    // 初期化
    swap_calc(string s, const vector<pair<char, string>>& insert) {
        this->insert = insert;
        this->s = s;
        s_length = s.size();
        // swap_item.resize(insert.size());
        cost_estimate = calc_first_cost();
        swap_cost = 0;
        insert_index = 0;

        calc_letter_cost();
    }

    // 現状のswapアルゴリズムだと差分しか計算できないので、代入後のコストを前もって計算しておく
    ll calc_first_cost() {
        string t = s;
        for (auto ins : insert) {
            string nt = "";
            for (auto i : t) {
                if (i == ins.first) {
                    nt += ins.second;
                } else {
                    nt += i;
                }
            }
            t = nt;
        }
        return simple_cost(t) - min_cost;
    }

    // それぞれの文字がどれだけのコスト寄与分を持っているか計算する
    void calc_letter_cost() {
        letter_cost.emplace_back(vector<int>(58, 0));  // 58は'z'-'A'=57のため
        letter_length.emplace_back(vector<int>(58, 1));
        for (int i = 'a'; i <= 'z'; i++) {
            letter_cost[0][i - 'A'] = 26 - (i - 'a');
        }

        for (int i = insert.size() - 1; i >= 0; i--) {
            vector<int> tmp1 = letter_cost[letter_cost.size() - 1];
            vector<int> tmp2 = letter_length[letter_length.size() - 1];
            int s = 0;
            int t = 0;
            for (char j : insert[i].second) {
                s += tmp1[j - 'A'];
                t += tmp2[j - 'A'];
            }
            tmp1[insert[i].first - 'A'] = s;
            tmp2[insert[i].first - 'A'] = t;
            letter_cost.emplace_back(tmp1);
            letter_length.emplace_back(tmp2);
        }
        reverse(letter_cost.begin(), letter_cost.end());
        reverse(letter_length.begin(), letter_length.end());
    }

    // insert_index番目の代入を行う (insert_indexも増やす)
    void update_s() {
        string ns = "";
        auto ins = insert[insert_index];
        for (char i : s) {
            if (i == ins.first) {
                ns += ins.second;
            } else {
                ns += i;
            }
        }
        s = ns;
        s_length = s.size();
        insert_index++;
    }

    // insert_index番目の前にあるswapを計算するときの、bitとかの初期化
    void init_BIT() {
        string_cost.resize(s_length);
        string_length.resize(s_length);
        cost_bit = BIT<int>(s_length);
        length_bit = BIT<int>(s_length);
        for (int i = 0; i < s_length; i++) {
            string_cost[i] = letter_cost[insert_index][s[i] - 'A'];
            string_length[i] = letter_length[insert_index][s[i] - 'A'];

            cost_bit.add(i, letter_cost[insert_index][s[i] - 'A']);
            length_bit.add(i, letter_length[insert_index][s[i] - 'A']);
        }
    }

    // sのi番目とj番目をswapしてestimate_costを更新する
    void swap_string(int i, int j) {
        swap_cost += abs(i - j);
        cost_estimate +=
            -length_bit.section(i, j) * string_cost[j] +
            length_bit.section(i + 1, j + 1) * string_cost[i] +
            cost_bit.section(i + 1, j) * (string_length[j] - string_length[i]);

        cost_bit.add(i, -string_cost[i] + string_cost[j]);
        cost_bit.add(j, -string_cost[j] + string_cost[i]);
        length_bit.add(i, -string_length[i] + string_length[j]);
        length_bit.add(j, -string_length[j] + string_length[i]);
        swap(s[i], s[j]);
        swap(string_cost[i], string_cost[j]);
        swap(string_length[i], string_length[j]);
    }
    void swap_string(int i, int j, ll estimate_diff) {
        swap_cost += abs(i - j);
        cost_estimate += estimate_diff;

        cost_bit.add(i, -string_cost[i] + string_cost[j]);
        cost_bit.add(j, -string_cost[j] + string_cost[i]);
        length_bit.add(i, -string_length[i] + string_length[j]);
        length_bit.add(j, -string_length[j] + string_length[i]);
        swap(s[i], s[j]);
        swap(string_cost[i], string_cost[j]);
        swap(string_length[i], string_length[j]);
    }

    pair<ll, ll> swap_diff(int i, int j) {
        ll swap_diff = abs(i - j);
        ll estimate_diff =
            -length_bit.section(i, j) * string_cost[j] +
            length_bit.section(i + 1, j + 1) * string_cost[i] +
            cost_bit.section(i + 1, j) * (string_length[j] - string_length[i]);

        return make_pair(swap_diff - fix_cost(t_length, cost_estimate) +
                             fix_cost(t_length, cost_estimate + estimate_diff),
                         estimate_diff);
    }

    // 挿入されるcharのindexを返す
    vector<int> insert_char_index() {
        vector<int> res;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == insert[insert_index].first) {
                res.push_back(i);
            }
        }
        return res;
    }
};

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

// ここにswapの出力あるよ
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

                char tmp_s[100];
                sprintf(tmp_s, "2 %d %d\n", i + 1, j + 1);
                answer += tmp_s;
                steps++;
            }
            i++;
            j--;
        }
        cost += quicksort(a, left, i - 1);
        cost += quicksort(a, j + 1, right);
    }
    return cost;
}

// //乱数
// unsigned int seed[4];
// void init_xor128(unsigned int s) {
//     for (unsigned int i = 0; i < 4; ++i)
//         seed[i] = s = 1812433253U * (s ^ (s >> 30)) + i;
// }
// unsigned int xor128() {
//     unsigned int t = (seed[0] ^ (seed[0] << 11));
//     seed[0] = seed[1];
//     seed[1] = seed[2];
//     seed[2] = seed[3];
//     return (seed[3] = (seed[3] ^ (seed[3] >> 19)) ^ (t ^ (t >> 8)));
// }

int main() {
    cin >> s >> t;

    min_cost = simple_cost(t);  // ココ大切
    t_length = t.size();        // ココ大切

    // sとtの文字のカウント
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

    vector<pair<char, string>> insert;

    // 小文字を大文字に変換
    for (int i = 0; i < 26; i++) {
        if (s_count[i] > 0) {
            string tmp{(char)(i + 'A')};
            insert.emplace_back(make_pair(i + 'a', tmp));
        }
    }
    // 大文字->小文字にして代入
    for (int i = 0; i < 26; i++) {
        for (auto j : get_insert(i)) {
            insert.emplace_back(make_pair(j.first, j.second));
        }
    }

    auto swap_cost_calc = swap_calc(s, insert);
    // random_device rnd;
    // // mt19937 mt(rnd());
    // init_xor128(rnd());

    for (int i = 0; i < insert.size(); i++) {
        swap_cost_calc.init_BIT();
        int s_length = swap_cost_calc.s.size();

        // auto char_index = swap_cost_calc.insert_char_index();
        // int index_len = char_index.size();

        //ここの回数、調整の余地あり コムソートベースにした
        int h = s_length * 10 / 15;
        for (int j = 0; j < 25; j++) {
            for (int a = 0; a < s_length - h; a++) {
                int b = a + h;
                auto diff = swap_cost_calc.swap_diff(a, b);
                if (diff.first < 0) {
                    swap_cost_calc.swap_string(a, b, diff.second);

                    char tmp_s[100];
                    sprintf(tmp_s, "2 %d %d\n", a + 1, b + 1);
                    answer += tmp_s;
                    steps++;
                }
            }

            if (h != 1) h = h * 10 / 15;
        }
        swap_cost_calc.update_s();
        char tmp_s[100];
        sprintf(tmp_s, "1 %c%s\n", insert[i].first, insert[i].second.c_str());
        answer += tmp_s;
        steps++;
    }
    /*cout << "estimate"
         << fix_cost(t_length, swap_cost_calc.cost_estimate) +
                swap_cost_calc.swap_cost
         << endl;*/

    ll real_cost = swap_cost_calc.swap_cost;

    s = swap_cost_calc.s;
    real_cost += quicksort(s, 0, s.size() - 1);

    printf("%d\n%s", steps, answer.c_str());

    // ここを消し忘れてはいけない
    cout << real_cost << endl;
}