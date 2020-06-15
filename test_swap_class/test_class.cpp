#include <random>

#include "../swap_class.cpp"

int main() {
    string s, t;
    cin >> s >> t;
    min_cost = simple_cost(t);
    t_length = t.size();

    vector<pair<char, string>> insert;
    while (true) {
        int i;
        cin >> i;
        if (i == 0) break;
        string tmp;
        cin >> tmp;
        insert.push_back(make_pair(tmp[0], tmp.substr(1)));
    }

    auto test = swap_calc(s, insert);

    random_device rnd;
    mt19937 mt(rnd());

    for (int i = 0; i < insert.size(); i++) {
        test.init_BIT();
        int s_length = test.s.size();
        for (int j = 0; j < 10000; j++) {
            int a = mt() % s_length, b = mt() % s_length;
            if (a != b) test.swap_string(min(a, b), max(a, b));
        }
        test.update_s();
    }
    cout << test.cost_estimate << endl;
    cout << test.swap_cost << endl;
    cout << test.s << endl;
}