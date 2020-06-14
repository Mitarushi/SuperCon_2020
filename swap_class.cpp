#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using ll = long long;
using namespace std;

ll min_cost;
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
    T section(int i, int j) { return sum(j) - sum(i); }
};

class swap_calc {
   public:
    vector<pair<char, string>> insert;
    vector<vector<pair<int, int>>> swap_item;
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
        swap_item.resize(insert.size());
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
};

//テスト
int main() {
    string s = "abcde", t = "aahhiiiimmrrssttuu";
    min_cost = simple_cost(t);  // ココ大切
    t_length = t.size();        // ココ大切

    vector<pair<char, string>> insert = {
        make_pair('a', "A"),  make_pair('b', "B"),         make_pair('c', "C"),
        make_pair('d', "D"),  make_pair('e', "E"),         make_pair('A', ""),
        make_pair('B', ""),   make_pair('C', ""),          make_pair('D', ""),
        make_pair('E', "EE"), make_pair('E', "ahmrstuEE"), make_pair('E', "i")};

    vector<vector<pair<int, int>>> swaps = {
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {make_pair(12, 16), make_pair(12, 11), make_pair(12, 15)},
        {make_pair(15, 16)}};

    auto test = swap_calc(s, insert);

    for (int i = 0; i < insert.size(); i++) {
        test.update_s();
        test.init_BIT();
        for (auto j : swaps[i]) {
            test.swap_string(j.first, j.second);
        }

        printf("%s\nestimate: %lld swap: %lld\n", test.s.c_str(),
               test.cost_estimate, test.swap_cost);
    }
}