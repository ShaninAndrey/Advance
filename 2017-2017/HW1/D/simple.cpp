#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#define F first
#define S second

using namespace std;
typedef long long ll;

const long long max_n = 3011, log_n = 32, max_m = 111, mod = 1000000007, inf = 1011111111, p = 1009, p2 = 997;

struct line {
    int a, b, c;
};

int n, mx;
line l[max_n];
std::vector<vector<int> > ans;
vector<int> cur;

void read() {
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> l[i].a >> l[i].b >> l[i].c;
    }
}

void solve(int c) {
    if (c == n) {
        for (int i = 0; i < cur.size(); ++i) {
            for (int q = i + 1; q < cur.size(); ++q) {
                if (1LL * l[cur[i]].a * l[cur[q]].b == 1LL * l[cur[i]].b * l[cur[q]].a ||
                    1LL * l[cur[i]].b * l[cur[q]].c == 1LL * l[cur[i]].c * l[cur[q]].b) {
                        return;
                }
            }
        }
        if (mx < cur.size()) {
            ans.clear();
            mx = cur.size();
            ans.push_back(cur);
        } else if (mx == cur.size()) {
            ans.push_back(cur);
        }
        return;
    }
    cur.push_back(c);
    solve(c + 1);
    cur.pop_back();
    solve(c + 1);
}

void write() {
    std::cout << ans.size() << std::endl;
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i].size() << endl;
        sort(ans[i].begin(), ans[i].end());
        for (int q = 0; q < ans[i].size(); ++q) {
            std::cout << ans[i][q] + 1 << " ";
        }
        cout << endl;
    }
}

void clr() {
    ans.clear();
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output1.txt", "w", stdout);
    int z;
    std::cin >> z;
    while (z--) {
        mx = 0;
        read();
        solve(0);
        write();
        clr();
    }
    return 0;
}
