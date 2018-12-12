#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

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
    for (int i = 0; i < n; i++) {
        if (used[i]) {
            vertices.push_back(i);
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
vector<vector<int>> buildGraph(string &s, string &c, unordered_map<int,int> &str, unordered_map<int,int> &example, int &ans) {
    int n = s.size();
    int m = c.size();
    int k = 1;
    for (int i = 0; i < n; i++) {
        if (s[i] == '?') {
            str[i] = k++;
        }
    }
    for (int i = 0; i < m; i++) {
        if (c[i] == '?') {
            example[i] = k++;
        }
    }
    int ng = k + 1;
    vector<vector<int>> g(ng, vector<int> (ng, 0));
    for (int i = 0; i < m; i++)
        for (int j = i; j <= n - m + i; j++) {
            if (c[i] == '?') {
                if (s[j] == '?') {
                    g[example[i]][str[j]]++;
                    g[str[j]][example[i]]++;
                }
                else if (s[j] == '1') {
                    g[example[i]][ng - 1]++;
                }
                else if (s[j] == '0') {
                    g[0][example[i]]++;
                }
            }
            else if (s[j] == '?') {
                if (c[i] == '0'){
                    g[0][str[j]]++;
                }
                else if (c[i] == '1') {
                    g[str[j]][ng - 1]++;
                }
            }
            else if (s[j] != c[i])
                ans++;
        }
    return g;
}
void makeStringsGreatAgain(vector<vector<int>> &g, string &s, string &c, unordered_map<int,int> &str, unordered_map<int,int> &example){
    vector<int> vert;
    bfs(g, vert);
    int it = 1;
    vert.push_back(-1);
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '?') {
            if (str[i] == vert[it]){
                it++;
                s[i] = '0';
            }
            else{
                s[i] = '1';
            }
        }
    }
    for (int i = 0; i < c.size(); i++) {
        if (c[i] == '?') {
            if (example[i] == vert[it]){
                it++;
                c[i] = '0';
            }
            else{
                c[i] = '1';
            }
        }
    }
}
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    string s,c;
    cin >> s >> c;
    n = s.size();
    m = c.size();
    int ans = 0;
    unordered_map<int,int> str;
    unordered_map<int,int> example;
    vector<vector<int>> g = buildGraph(s, c, str, example, ans);
    ans += maxFlow(g);
    makeStringsGreatAgain(g, s, c, str, example);
    cout << ans << endl << s << endl << c;
}