#include <bits/stdc++.h>
#include <utility>
#include <string>
#include <vector>

const int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};

bool is_in(const int &x, const int &y, const int &n, const int &m) {
    return x >= 0 && y >= 0 && x < n && y < m;
}

// Kuhn's algorithm based on matrix
bool dfs(const int &x, const int &y,
         const int &n, const int &m,
         const std::vector<std::string> &arr,
         std::vector<std::vector<std::pair<int, int> > > &pair,
         std::vector<std::vector<bool> > &used) {

    used[x][y] = 1;
    for (int i = 0; i < 4; ++i) {
        const int tox = x + dx[i];
        const int toy = y + dy[i];
        if (is_in(tox, toy, n, m) &&
            arr[tox][toy] == '*' &&
            pair[tox][toy].first == -1) {

            pair[tox][toy] = std::make_pair(x, y);
            pair[x][y] = std::make_pair(tox, toy);
            return true;
        }
    }
    for (int i = 0; i < 4; ++i) {
        const int tox = x + dx[i];
        const int toy = y + dy[i];
        if (is_in(tox, toy, n, m) &&
            arr[tox][toy] == '*' &&
            !used[pair[tox][toy].first][pair[tox][toy].second] &&
            dfs(pair[tox][toy].first, pair[tox][toy].second, n, m, arr, pair, used)) {

            pair[tox][toy] = std::make_pair(x, y);
            pair[x][y] = std::make_pair(tox, toy);
            return true;
        }
    }
    return false;
}

int solve(const int &n,
          const int &m,
          const int &a,
          const int &b,
          const std::vector<std::string> &arr) {

    // count numbers of empty points and zeroing arrays
    int cnt = 0;
    std::vector<std::vector<std::pair<int, int> > > pair(n);
    for (int i = 0; i < n; ++i) {
        pair[i].resize(m, std::make_pair(-1, -1));
        for (int q = 0; q < m; ++q) {
            cnt += arr[i][q] == '*';
        }
    }

    // if single tiles are better than double
    if (a >= b * 2) {
        return b * cnt;
    }

    /*
    * find maximum matches with
    * similar to Hopcroft-Karp's algorithm
    * optimization
    */
    for (int run = 1; run; ) {
        run = 0;
        std::vector<std::vector<bool> > used(n, std::vector<bool>(m));
        for (int i = 0; i < n; ++i) {
            for (int q = 0; q < m; ++q) {
                if (arr[i][q] == '*' &&
                    (i + q) % 2 &&
                    pair[i][q].first == -1 &&
                    dfs(i, q, n, m, arr, pair, used)) {

                    run = 1;
                }
            }
        }
    }

    // numbers of matches
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        for (int q = 0; q < m; ++q) {
            if ((i + q) % 2 && pair[i][q].first != -1) {
                sum++;
            }
        }
    }
    return sum * a + (cnt - sum * 2) * b;
}

void read(std::istream &in,
          int &n, int &m, int &a, int &b,
          std::vector<std::string> &arr) {

    in >> n >> m >> a >> b;
    arr.resize(n);
    for (int i = 0; i < n; ++i) {
        in >> arr[i];
    }
}

void write(std::ostream &out, const int &ans) {
    out << ans << std::endl;
}

int main() {
    int n, m, a, b;
    std::vector<std::string> arr;
    std::vector<std::vector<bool> > used;
    read(std::cin, n, m, a, b, arr);
    const int ans = solve(n, m, a, b, arr);
    write(std::cout, ans);
    return 0;
}
