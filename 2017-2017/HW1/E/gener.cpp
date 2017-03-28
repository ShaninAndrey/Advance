#include <bits/stdc++.h>

typedef long long ll;
using namespace std;

const long long max_n = 10, max_m = 20;

int n = 5, m = 10, mm = 2, z = 1000;
vector<int> v[max_n];

int dfs(int x, int p, int h, int pos) {
    if (x == pos) return h;
    for (int i = 0; i < v[x].size(); ++i) {
        if (v[x][i] == p) continue;
        int k = dfs(v[x][i], x, h + 1, pos);
        if (k != -1) return k;
    }
    return -1;
}
void link(int aa, int bb) {
    v[aa].push_back(bb);
    v[bb].push_back(aa);
}
void cut(int aa, int bb) {
    for (int i = 0; i < v[aa].size(); ++i) {
        if (v[aa][i] == bb) {
            v[aa].erase(v[aa].begin() + i);
            break;
        }
    }
    for (int i = 0; i < v[bb].size(); ++i) {
        if (v[bb][i] == aa) {
            v[bb].erase(v[bb].begin() + i);
            break;
        }
    }
}

int main() {
    srand(time(0));
    freopen("input.txt", "w", stdout);
    while (z--) {
        n = rand() % max_n + 1;
        m = rand() % max_m;
        for (int i = 0; i < n; ++i) {
            v[i].clear();
        }
        cout << n << " " << m << endl;
        for (int i = 0; i < m; ++i) {
            int st = rand() % n;
            int fn = rand() % n;
            if (dfs(st, -1, 0, fn) == -1) {
                cout << "link " << st + 1 << " " << fn + 1 << endl;
                link(st, fn);
                continue;
            }
            if (dfs(st, -1, 0, fn) == 1 && rand() % mm) {
                cout << "cut " << st + 1 << " " << fn + 1 << endl;
                cut(st, fn);
                continue;
            }
            cout << "get " << st + 1 << " " << fn + 1 << endl;
        }
    }
    return 0;
}
