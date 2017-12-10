#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <queue>

typedef long long ll;

const long long max_n = 505, log_n = 32, max_m = 111;

std::vector<int> edge[max_n];
std::queue<int> qu;
int match[max_n], parent[max_n], base[max_n], n;
bool used[max_n], bl[max_n], usedd[max_n];

int lca(int aa, int bb) {
    memset(usedd, 0, sizeof(used));
    for (int i = 0; i < 1; ) {
        aa = base[aa];
        usedd[aa] = 1;
        if (match[aa] == -1) {
            break;
        }
        aa = parent[match[aa]];
    }
    for (int i = 0; i < 1; ) {
        bb = base[bb];
        if (usedd[bb]) {
            return bb;
        }
        bb = parent[match[bb]];
    }
}

void mark_path(int cur, int anc, int child) {
    while (base[cur] != anc) {
        bl[base[cur]] = bl[base[match[cur]]] = 1;
        parent[cur] = child;
        child = match[cur];
        cur = parent[match[cur]];
    }
}

void operate_blossom(int cur, int to) {
    int ll = lca(cur, to);
    memset(bl, 0, sizeof(bl));
    mark_path(cur, ll, to);
    mark_path(to, ll, cur);
    for (int i = 0; i < n; ++i) {
        if (bl[base[i]]) {
            base[i] = ll;
            if (!used[i]) {
                used[i] = 1;
                qu.push(i);
            }
        }
    }
}

int find_path(int root) {
    while (qu.size()) qu.pop();
    qu.push(root);
    memset(used, 0, sizeof(used));
    memset(parent, -1, sizeof(parent));
    for (int i = 0; i < n; ++i) {
        base[i] = i;
    }
    used[root] = 1;
    while (qu.size()) {
        int cur = qu.front();
        qu.pop();
        for (int ii = 0; ii < edge[cur].size(); ++ii) {
            int to = edge[cur][ii];
            if (base[cur] == base[to] || match[cur] == to) continue;
            if (to == root || match[to] != -1 && parent[match[to]] != -1) {
                operate_blossom(cur, to);
            } else if (parent[to] == -1) {
                parent[to] = cur;
                if (match[to] == -1) {
                    return to;
                }
                used[match[to]] = 1;
                qu.push(match[to]);
            }
        }
    }
    return -1;
}

int main() {
    // freopen("input.txt", "r", stdin);
    std::cin >> n;
    int aaa, bbb;
    while (scanf("%d%d", &aaa, &bbb) == 2) {
        aaa--;
        bbb--;
        edge[aaa].push_back(bbb);
        edge[bbb].push_back(aaa);
    }

    memset (match, -1, sizeof match);
    for (int i = 0; i < n; ++i) {
        if (match[i] == -1) {
            int nxt = find_path(i);
            int cnt = 0;
            while (nxt != -1) {
                int pnxt = parent[nxt], mpnxt = match[pnxt];
                match[nxt] = pnxt;
                match[pnxt] = nxt;
                nxt = mpnxt;
            }
        }
    }

    std::vector<std::pair<int, int> > ans;
    memset(used, 0, sizeof(used));
    for (int i = 0; i < n; ++i) {
        if (used[i]) continue;
        if (match[i] != -1) {
            used[i] = used[match[i]] = 1;
            ans.push_back(std::make_pair(i, match[i]));
        }
    }
    std::cout << ans.size() << std::endl;

    return 0;
}
