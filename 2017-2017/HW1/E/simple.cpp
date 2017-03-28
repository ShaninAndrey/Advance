#include <bits/stdc++.h>

typedef long long ll;
using namespace std;

const long long max_n = 100011;

int n, m;
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
    freopen("input.txt", "r", stdin);
    freopen("output1.txt", "w", stdout);
    while (scanf("%d%d", &n, &m) == 2) {
        for (int i = 0; i < n; ++i) {
            v[i].clear();
        }
        for (int i = 0; i < m; ++i) {
            char arr[4];
            int aa, bb;
            scanf("%s%d%d", arr, &aa, &bb);
            if (arr[0] == 'l') {
                link(aa - 1, bb - 1);
            } else if (arr[0] == 'c') {
                cut(aa - 1, bb - 1);
            } else {
                printf("%d\n", dfs(aa - 1, -1, 0, bb - 1));
            }
        }
    }
    return 0;
}
