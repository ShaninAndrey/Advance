#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

const long long max_n = 30, log_n = 32, max_m = 111, mod = 1000000007, inf = 1011111111, p = 1009, p2 = 997;

int n, ans[max_n];
long long hsh, hsh1;

int main() {
    srand(time(0));
    ifstream cin("output.txt");
    ifstream cinn("output1.txt");
    while (cin >> n) {
        hsh = hsh1 = 0;
        for (int i = 0; i < n; ++i) {
            cin >> ans[i];
        }
        int m, n1;
        bool fll = false;
        cinn >> m;
        for (int i = 0; i < m; ++i) {
            cinn >> n1;
            if (n != n1) {
                cout << "WA\n";
                return 0;
            }
            bool fl = false;
            for (int q = 0; q < n; ++q) {
                int a;
                cinn >> a;
                if (a != ans[q]) fl = true;
            }
            if (!fl) {
                fll = true;
            }
        }
        if (fll) {
            cout << "OK\n";
        } else {
            cout << "WA";
            return 0;
        }
    }
    return 0;
}
