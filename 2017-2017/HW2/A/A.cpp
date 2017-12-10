#include <bits/stdc++.h>
#include <vector>

const int max_n = 505; // число вершин
const int inf = 1000000011; // константа-бесконечность

struct edges {
    int aa, bb, cap, flow, ind;
};

int nn, s, t, d[max_n], ptr[max_n], q[max_n], m, ans[max_n];
std::vector<edges> edge;
std::vector<int> g[max_n];

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
        memset (ptr, 0, nn * sizeof ptr[0]);
        while (int pushed = dfs (s, inf))
            flow += pushed;
    }
    return flow;
}

int main() {
    std::cin >> nn >> m;
    for (int i =0 ; i < m; ++i) {
        int aa, b, c;
        scanf("%d%d%d", &aa, &b, &c);
        add_edge(aa - 1, b - 1, c, i);
    }
    s = 0;
    t = nn - 1;
    std::cout << dinic() << std::endl;
    for (int i = 0; i < edge.size(); i += 2) {
        printf("%d\n", edge[i].flow);
    }
}
