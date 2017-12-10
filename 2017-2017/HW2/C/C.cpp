#include <iostream>
#include <utility>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <set>
#include <stack>
#include <map>
#include <queue>

typedef long long ll;
typedef long double ld;

const int max_n = 511, max_k = 555, mod = 1000000007, inf = 1111111111;

int used[max_n], n, VV, p[max_n];
std::pair<int, std::pair<int, int> > point[max_n];
std::vector<int> edge[max_n];

int get_time(std::string ss) {
    int aa = (ss[0] - '0') * 10 + (ss[1] - '0');
    int bb = (ss[3] - '0') * 10 + (ss[4] - '0');
    return aa * 60 + bb;
}

long long sqr(int aa) {
    return 1LL * aa * aa;
}

ld dist(std::pair<int, int> xx, std::pair<int, int> yy) {
    return sqrt(sqr(xx.first - yy.first) + sqr(xx.second - yy.second));
}

bool is_crossed(int xx, int yy) {
    int time = - point[xx].first + point[yy].first;
    if (time <= 0) return false;
    ld dd = dist(point[xx].second, point[yy].second);
    return dd * 60 - 1LL * time * VV <= 0.000000001;
}

bool dfs(int xx) {
    used[xx] = 1;

    for (int i = 0; i < edge[xx].size(); ++i) {
        if (used[p[edge[xx][i]]]) continue;
        if (p[edge[xx][i]] == -1 || p[edge[xx][i]] != xx && dfs(p[edge[xx][i]])) {
            p[edge[xx][i]] = xx;
            return true;
        }
    }

    return false;
}

int main() {
    std::cin >> n >> VV;
    for (int i = 0; i < n; ++i) {
        std::string ss;
        std::cin >> ss >> point[i].second.first >> point[i].second.second;
        point[i].first = get_time(ss);
    }
    for (int i = 0; i < n; ++i) {
        for (int q = 0; q < n; ++q) {
            if (i != q && is_crossed(i, q)) {
                edge[i].push_back(q);
            }
        }
    }
    memset(p, -1, sizeof(p));

    for (int i = 0; i < n; ++i) {
        memset(used, 0, sizeof(used));
        dfs(i);
    }

    int ans = 0;
    for (int i = 0; i < n; ++i) {
        if (p[i] == -1) ans++;
    }
    std::cout << ans;
}
