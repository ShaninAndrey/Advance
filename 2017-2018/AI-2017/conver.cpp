#include <bits/stdc++.h>

#define F first
#define S second
#define x1 privet1
#define x2 privet2
#define y1 privet3
#define y2 privet4
#define left privet6
#define prev ppppp

using namespace std;
typedef long long ll;

const int max_n = 366, log_n = 32, max_m = 111, mod = 1000000007, inf = 1000111222;

double tn[max_n];
map<int, string> m;

double get_res(int a, string b) {
    double last = tn[1], res = 0;
    for (int i = 0; i < b.length(); ++i) {
        if (b[i] != '*') {
            res += last;
            last = tn[i + 2];
            if (b[i] == '-') last *= -1;
        } else {
            last *= tn[i + 2];
        }
    }
    res += last;
    return res;
}

void update(int a, string b) {
    if (!m.count(a) || fabs(get_res(a, b) - a) < fabs(get_res(a, m[a]) - a)) {
        m[a] = b;
    }
}

void update_ans(const string path) {
    ifstream in(path);
    int a;
    string b;
    while (in >> a) {
        in >> b;
        update(a, b);
    }
}

int main() {
    freopen("output.txt", "w", stdout);
    for (int i = 1; i <max_n; ++i) {
        tn[i] = tan(i);
        /*vector<double> v;
        for (int q = 1; q <= i; ++q) {
            v.push_back(fabs(tn[q]));
        }
        sort(v.begin(), v.end());
        double sum = 0;
        for (int q = 0; q < i; ++q) {
            if (sum >= fabs(v[q]) || v[q] <= 0.5) {
                sum += fabs(v[q]);
            }
        }
        cout << i << " " << tn[i] << "        " << sum << endl;*/
    }
    //return 0;
    update_ans("outexp.txt");
    update_ans("outcnt.txt");
    update_ans("outpow.txt");
    update_ans("outpow0.txt");
    update_ans("outpow1.txt");
    update_ans("newout.txt");
    update_ans("newout1.txt");
    update_ans("newout2.txt");
    for (map<int, string>::iterator it = m.begin(); it != m.end(); ++it) {
        cout << "ans[" << it->F << "] = \"" << it->S << "\";";
        //cout << " " << get_res(it->F, it->S);
        cout << endl;
    }
    return 0;
}
