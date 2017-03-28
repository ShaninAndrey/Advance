#include <bits/stdc++.h>

typedef long long ll;
using namespace std;

const long long max_n = 10;

int main() {
    srand(time(0));
    ifstream cin("output.txt");
    ifstream cinn("output1.txt");
    int a;
    while (cin >> a) {
        int b;
        cinn >> b;
        if (a == b) {
            cout << "OK\n";
        } else {
            cout << "WA\n";
            return 0;
        }
    }
    return 0;
}
