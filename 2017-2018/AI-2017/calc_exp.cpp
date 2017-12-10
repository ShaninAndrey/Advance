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
char c[max_n];
pair<double, string> ans;

void fll(int i, int n, double num, double sum) {
    if (i == n) {
        if (fabs(ans.F - n) > fabs(sum + num - n)) {
            ans = make_pair(sum + num, string(c));
        }
        return;
    }
    c[i - 1] = '+';
    fll(i + 1, n, tn[i + 1], sum + num);
    c[i - 1] = '-';
    fll(i + 1, n, -tn[i + 1], sum + num);
    c[i - 1] = '*';
    fll(i + 1, n, num * tn[i + 1], sum);
}

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    ofstream out("outexp.txt");
    for (int i = 1; i < max_n; ++i) {
        tn[i] = tan(i);
    }
    for (int i = 5; i < max_n; ++i) {
        cout << i << endl;
        ans = make_pair(-inf, "");
        fll(1, i, tn[1], 0);
        out << i << " " << ans.S << endl;
        //out << ans.F << endl;
    }
    return 0;
}
