#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <utility>

typedef long long ll;

const long long max_n = 3011, inf = 1011111111;

int gcd(int aa, int bb) {
    return aa == 0 ? bb : gcd(bb % aa, aa);
}

struct fract {
    int aa, bb;

    fract(int aaa, int bbb) {
        aa = aaa;
        bb = bbb;
        if (bb < 0) {
            aa *= -1;
            bb *= -1;
        }
        update();
    }
    void update() {
        if (aa == 0) {
            bb = 1;
        }
        if (bb == 0) {
            aa = 1;
        }
        // std::cout << a << " " << b << "   ";
        int gg = gcd(std::min(abs(aa), bb), std::max(abs(aa), bb));
        // std::cout << gg << std::endl;
        if (gg == 0) return;
        aa /= gg;
        bb /= gg;
    }
    bool operator < (const fract &other) const {
        if (aa == other.aa) return bb < other.bb;
        return aa < other.aa;
    }
};

int used[max_n], n, F_last, S_last, res = -1;
std::pair<int, int> pr[max_n];
std::vector<std::pair<int, int> > v[max_n];
std::vector<int> ans;
std::map<fract, int> F_ptr, S_ptr;

bool dfs(int cur, int parent) {
    used[cur] = 1;
    for (int i = 0; i < v[cur].size(); ++i) {
        if (v[cur][i].first == parent) continue;
        if (pr[v[cur][i].first].first == -1 ||
            !used[pr[v[cur][i].first].first] && dfs(pr[v[cur][i].first].first, v[cur][i].first)) {
            pr[v[cur][i].first] = std::make_pair(cur, v[cur][i].second);
            return true;
        }
    }
    return false;
}

void read() {
    std::cin >> n;
    int f_val, s_val, t_val;
    for (int ii = 0; ii < n; ++ii) {
        std::cin >> f_val >> s_val >> t_val;
        if (s_val == t_val && s_val == 0) {
            res = ii;
            continue;
        }
        fract f_cur(f_val, s_val), s_cur(t_val, s_val);
        if (!F_ptr.count(f_cur)) {
            F_ptr[f_cur] = F_last++;
        }
        if (!S_ptr.count(s_cur)) {
            S_ptr[s_cur] = S_last++;
        }
        v[F_ptr[f_cur]].push_back(std::make_pair(S_ptr[s_cur], ii));
    }
}

void write() {
    if (ans.size() == 0) {
        std::cout << "1\n" << res + 1 << std::endl;
        return;
    }
    std::cout << ans.size() << std::endl;
    sort(ans.begin(), ans.end());
    for (int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i] + 1 << " ";
    }
    std::cout << std::endl;
}

void clr() {
    res = -1;
    F_ptr.clear();
    S_ptr.clear();
    ans.clear();
    for (int i = 0; i < F_last; ++i) {
        used[i] = 0;
        v[i].clear();
    }
    for (int i = 0; i < S_last; ++i) {
        pr[i] = std::make_pair(-1, -1);
    }
    F_last = S_last = n = 0;
}

int main() {
    for (int i = 0; i < max_n; ++i) {
        pr[i] = std::make_pair(-1, -1);
    }
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int tests;
    std::cin >> tests;
    while (tests--) {
        read();
        for (int i = 0; i < F_last; ++i) {
            for (int q = 0; q < F_last; ++q) {
                used[q] = 0;
            }
            dfs(i, -1);
        }
        for (int i = 0; i < S_last; ++i) {
            if (pr[i].first != -1) {
                ans.push_back(pr[i].second);
            }
        }
        write();
        clr();
    }
    return 0;
}
