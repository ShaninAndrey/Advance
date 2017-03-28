#include <bits/stdc++.h>
#include <vector>
#include <utility>

typedef long long ll;
typedef long double ld;

const int max_n = 111, log_n = 32, mod = 1000000007, inf = 1011111111;

int color[max_n], n, m, used[max_n];
std::pair<int, int> edge[max_n * 4];
std::vector<int> res;
std::vector<std::pair<int, std::pair<int, int> > > vv[max_n];

bool dfs(int xx, int uu) {
    if (xx == n - 1) return 1;
    used[xx] = 1;
    for (int i = 0; i < vv[xx].size(); ++i) {
        if (used[vv[xx][i].second.first] || vv[xx][i].first < uu) continue;
        int kk = dfs(vv[xx][i].second.first, uu);
        if (kk) {
            vv[xx][i].first -= uu;
            vv[vv[xx][i].second.first][vv[xx][i].second.second].first += uu;
            return 1;
        }
    }
    return 0;
}

void dsfs(int xx) {
    used[xx] = 1;
    color[xx] = 1;
    for (int i = 0; i < vv[xx].size(); ++i) {
        if (!used[vv[xx][i].second.first] && vv[xx][i].first) dsfs(vv[xx][i].second.first);
    }
}

int main() {
    // srand(time(NULL));
    // freopen("input.txt", "r", stdin);
    // freopen("stdin", "r", stdin);
    // freopen("stdout", "w", stdout);
    std::cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int ff, g, h;
        std::cin >> ff >> g >> h;
        ff--;
        g--;
        edge[i] = std::make_pair(ff, g);
        vv[ff].push_back(std::make_pair(h, std::make_pair(g, vv[g].size())));
        vv[g].push_back(std::make_pair(h, std::make_pair(ff, vv[ff].size() - 1)));
    }
    int uu = inf;
    ll ans = 0;
    while (uu > 0) {
        while (dfs(0, uu)) {
            ans += uu;
            memset(used, 0, sizeof(used));
        }
        memset(used, 0, sizeof(used));
        uu /= 2;
    }
    dsfs(0);
    for (int i = 0; i < m; ++i) {
        if (color[edge[i].first] ^ color[edge[i].second]) {
            res.push_back(i + 1);
        }
    }
    std::cout << res.size() << " " << ans << std::endl;
    for (int i =0 ; i < res.size(); ++i) {
        std::cout << res[i] << " ";
    }
    return 0;
}
