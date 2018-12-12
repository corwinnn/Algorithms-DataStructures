#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
bool bfs(vector<vector<int>> &g, vector<int> &vertices) {
    vertices.resize(0);
    int n = g.size();
    int v = 0, w;
    queue<int> q;
    vector<bool> used(n, false);
    vector<int> parents(n, -1);
    q.push(v);
    used[v] = true;
    while(!q.empty()) {
        w = q.front();
        q.pop();
        if (w == n - 1) {
            int p = n - 1;
            while (p != -1) {
                vertices.push_back(p);
                p = parents[p];
            }
            reverse(vertices.begin(), vertices.end());
            return true;
        }
        for (int i = 0; i < n; i++) {
            if (g[w][i] && !used[i]) {
                used[i] = true;
                q.push(i);
                parents[i] = w;
            }
        }
    }
    return false;
}
int maxFlow(vector<vector<int>> &g) {
    int ans = 0;
    int n = g.size();
    vector<int> vertices;
    int c;
    while (bfs(g, vertices)) {
        int m = vertices.size();
        c = numeric_limits<int>::max();
        for (int i = 1; i < m; i++) {
            c = min(c, g[vertices[i - 1]][vertices[i]]);
        }
        ans += c;
        for (int i = 1; i < m; i++) {
            g[vertices[i - 1]][vertices[i]] -= c;
            g[vertices[i]][vertices[i - 1]] += c;
        }
    }
    return ans;
}
bool bfsd(vector<vector<int>> &g, vector<int> &slices) {
    int n = g.size();
    slices.resize(n);
    fill(slices.begin(), slices.end(), -1);
    int v = 0, w;
    queue<int> q;
    vector<bool> used(n, false);
    q.push(v);
    used[v] = true;
    slices[v] = 0;
    while(!q.empty()) {
        w = q.front();
        q.pop();
        if (w == n - 1) {
            return true;
        }
        for (int i = 0; i < n; i++) {
            if (g[w][i] && !used[i]) {
                used[i] = true;
                q.push(i);
                slices[i] = slices[w] + 1;
            }
        }
    }
    return false;
}
int dfsd(vector<vector<int>> &g, vector<int> &slices,int v, int flow, vector<int> &edges) {
    if (!flow) return 0;
    int n = g.size();
    if (v == n - 1) return flow;
    for (int &i = edges[v]; i < n; i++)
        if (slices[i] == slices[v] + 1 && g[v][i]) {
            int push = dfsd(g, slices, i, min(flow, g[v][i]), edges);
            if (push) {
                g[v][i] -= push;
                g[i][v] += push;
                return push;
            }
        }
    return 0;
}
int dinic(vector<vector<int>> &g) {
    int ans = 0, t;
    int n = g.size();
    vector<int>slices;
    vector<int> edges(n);
    while(bfsd(g, slices)) {
        t = 1;
        fill(edges.begin(), edges.end(),0);
        while(t) {
            t = dfsd(g, slices, 0, numeric_limits<int>::max(), edges);
            ans += t;
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n, vector<int>(n,0));
    int u, v, p;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> p;
        g[u - 1][v - 1] = p;
    }
    int c = dinic(g);
    cout << c << endl;

    return 0;
}
/*
6 9
1 2 10
1 3 10
2 3 2
2 4 4
2 5 8
3 5 9
4 6 10
5 6 10
5 4 6
*/
/*
3 2
1 2 5
2 3 6
*/