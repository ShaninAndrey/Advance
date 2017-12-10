#include <bits/stdc++.h>
#include <vector>

const int max_n = 5050; // число вершин
const int inf = 1000000011; // константа-бесконечность
const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};

struct edges {
    int aa, bb, cap, flow, ind;
};

int nn, s, t, d[max_n], ptr[max_n], q[max_n], m, ans[max_n];
std::vector<edges> edge;
std::vector<int> g[max_n];

bool is_in(int x, int y) {
    return x >= 0 && y >= 0 && x < nn && y < m;
}

int get_num(int x, int y) {
    return x * m + y;
}

void add_edge (int aa, int bb, int cap, int ind) {
    edges edgee = { aa, bb, cap, 0, ind};
    edges edgeee = { bb, aa, 0, 0, ind};
    g[aa].push_back (edge.size());
    edge.push_back (edgee);
    g[bb].push_back (edge.size());
    edge.push_back (edgeee);
}

bool bfs() {
    int qh = 0, qt = 0;
    q[qt++] = s;
    memset (d, -1, nn * sizeof d[0]);
    d[s] = 0;
    while (qh < qt && d[t] == -1) {
        std::cout << "2\n";
        int vv = q[qh++];
        for (size_t i = 0; i < g[vv].size(); ++i) {
            int id = g[vv][i],
                to = edge[id].bb;
            if (d[to] == -1 && edge[id].flow < edge[id].cap) {
                q[qt++] = to;
                d[to] = d[vv] + 1;
            }
        }
    }
    return d[t] != -1;
}

int dfs (int vv, int flow) {
    if (!flow)  return 0;
    if (vv == t)  return flow;
    for (; ptr[vv] < g[vv].size(); ++ptr[vv]) {
        int id = g[vv][ptr[vv]],
            to = edge[id].bb;
        if (d[to] != d[vv] + 1)  continue;
        int pushed = dfs (to, std::min (flow, edge[id].cap - edge[id].flow));
        if (pushed) {
            edge[id].flow += pushed;
            edge[id^1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

long long dinic() {
    long long flow = 0;
    while (bfs()) {
        std::cout << "3\n";
        memset (ptr, 0, nn * sizeof ptr[0]);
        while (int pushed = dfs (s, inf)) {
            std::cout<<"1\n";
            flow += pushed;
        }
    }
    return flow;
}

int main() {
    std::cin >> nn >> m;
    std::string cc;
    s = nn * m * 2;
    t = nn * m * 2 + 1;
    int sum = 0;
    for (int i =0 ; i < nn; ++i) {
        std::cin >> cc;
        for (int q =0 ; q < m; ++q) {
            sum += (cc[q] - '0');
            add_edge(s, get_num(i, q), cc[q] - '0', 0);
            add_edge(get_num(i, q) + nn * m, t, cc[q] - '0', 0);
            for (int w =0; w < 4; ++w) {
                if (is_in(i + dx[w], q + dy[w])) {
                    add_edge(get_num(i, q), get_num(i + dx[w], q + dy[w]) + nn * m, 1, 0);
                }
            }
        }
    }
    if (dinic() == sum) {
        std::cout << "Valid\n";
    } else {
        std::cout << "Invalid\n";
    }
}
