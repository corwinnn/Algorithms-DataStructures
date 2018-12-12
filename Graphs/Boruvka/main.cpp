#include <bits/stdc++.h>
using namespace std;

struct edge {
    int number;
    int from;
    int  to;
    int weight;
    edge(int number, int from , int to, int weight):
            number(number),
            from(from),
            to(to),
            weight(weight)
    {}
};


vector<int> findcomp(int n, vector<vector<int>> &g) {
    vector<bool> visited(n,false);
    vector<int> comp(n + 1, -1);
    int cur = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            queue<int> q;
            q.push(i);
            while(!q.empty()) {
                int v = q.front();
                q.pop();
                visited[v] = true;
                comp[v] = cur;
                for (int j = 0; j < g[v].size(); j++) {
                    if (!visited[g[v][j]])
                        q.push(g[v][j]);
                }
            }
            cur++;
        }

    }
    comp[n] = cur;
    return comp;
}

vector<edge> boruvka(int n, vector<edge> &edges) {
    int m = edges.size();
    edges.push_back(edge(0,0,0,numeric_limits<int>::max()));
    vector<vector<int>> g(n, vector<int> (0));
    vector<edge> T(0, edge(0,0,0,0));
    vector<bool> visited(m, false);
    vector<int> comp(n + 1, 0);
    while(T.size() < n - 1) {
        comp = findcomp(n, g);
        int kol = comp[n];
        vector<int> minEdge(kol, m);
        for (int i = 0; i < edges.size(); i++) {
            int comp1 = comp[edges[i].from], comp2 = comp[edges[i].to];
            if (comp1 != comp2) {
                if (edges[minEdge[comp1]].weight > edges[i].weight || 
                        edges[minEdge[comp1]].weight == edges[i].weight && edges[minEdge[comp1]].number > edges[i].number) {
                    minEdge[comp1] = i;
                }
                if (edges[minEdge[comp2]].weight > edges[i].weight ||
                    edges[minEdge[comp2]].weight == edges[i].weight && edges[minEdge[comp2]].number > edges[i].number) {
                    minEdge[comp2] = i;
                }
            }
        }
        for (int i = 0; i < kol; i++) {
            if (!visited[minEdge[i]]) {
                visited[minEdge[i]] = true;
                T.push_back(edges[minEdge[i]]);
                g[edges[minEdge[i]].from].push_back(edges[minEdge[i]].to);
                g[edges[minEdge[i]].to].push_back(edges[minEdge[i]].from);
            }
        }
    }
    return  T;
}


int main() {
    int n,m;
    int u, v, w;
    cin >> n >> m;
    edge nll(0,0,0,0);
    vector<edge> edges(m, nll),mst;
    vector<vector<edge>> lst(n, vector<edge> (0, nll));
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        v--;
        u--;
        edges[i] = edge(i, u, v, w);
        lst[u].push_back(edge(i, u, v, w));
        lst[v].push_back(edge(i, u, v, w));
    }
    mst = boruvka(n, edges);
    int s = 0;
    for (int i = 0; i < mst.size(); i++)
        s += mst[i].weight;
    cout << s;
}
/*
7 11
1 2 7
1 4 4
2 4 9
2 3 11
2 5 10
3 5 5
4 5 15
4 6 6
5 6 12
5 7 8
6 7 13
 */