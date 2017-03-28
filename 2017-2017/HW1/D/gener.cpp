#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

const long long max_n = 15, log_n = 32, max_m = 111, mod = 1000000007, inf = 101111111, p = 1000000009, p2 = 997;

int n, rnd = 11;

int main() {
    srand(time(0));
    freopen("input.txt", "w", stdout);
    int z = 1000;
    cout << z << endl;
    while (z--) {
        n = rand() % (max_n - 1) + 1;
        cout << n << endl;
        for (int i = 0; i < n; ++i) {
            int a = rand() % rnd - rnd / 2;
            int b = rand() % rnd - rnd / 2;
            int c = rand() % rnd - rnd / 2;
            while (1LL * a * a + 1LL * b * b == 0) {
                a = rand() % rnd - rnd / 2;
                b = rand() % rnd - rnd / 2;
            }
            cout << a << " " << b << " " << c << endl;
        }
    }
    return 0;
}
