#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;

const double cost_param[] = {
    -0.0000000000008402,     0.0000004798000799,    -0.0002000268185879,
    0.0044613262787533,      -0.0000001652505149,   0.2437505930922388,
    -7.7352385687962633,     25.8180687932298483,   0.0000356935469576,
    -5.5227029004491737,     1456.5673308701468613, -47616.8398519225956989,
    -0.0145529321527654,     224.8752720847048465,  -54970.5081915919436142,
    2140886.3656231933273375};

ll simple_cost(ll n, ll cost) {
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

int main() {
    const int line = 10000;
    for (int i = 0; i < line; i++) {
        int n;
        ll simple;
        ll quick;
        ll tmp;
        cin >> n >> simple >> tmp >> quick;
        cout << simple_cost(n, simple) << " " << quick << "\n";
    }
}