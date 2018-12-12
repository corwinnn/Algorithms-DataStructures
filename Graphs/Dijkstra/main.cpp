#include <bits/stdc++.h>
using namespace std;
int main() {
    ios_base::sync_with_stdio(0);
    int n,m;
    cin >> n;
    vector< vector < pair <int, long long>>> g(n);
    cin >> m;
    int v, w;
    long long l;
    while(m--) {
        cin >> v >> w >> l;
        g[v].push_back(make_pair(w,l));
        g[w].push_back(make_pair(v,l));
    }
    long long MAXLENGTH = 1000000000000;
    vector<long long> d(n, MAXLENGTH);
    int from, to;
    cin >> from >> to;
    d[from] = 0;
    set<pair <long long, int>> way;
    way.insert(make_pair(d[from], from));
    while(!way.empty()) {
        v = way.begin()->second;
        way.erase(way.begin());
        for (int i = 0; i < g[v].size(); i++) {
            if (d[g[v][i].first] > d[v] + g[v][i].second) {
                way.erase(make_pair(d[g[v][i].first], g[v][i].first));
                d[g[v][i].first] = d[v] + g[v][i].second;
                way.insert(make_pair(d[g[v][i].first], g[v][i].first));
            }
        }
    }
    cout << d[to] << endl;
}