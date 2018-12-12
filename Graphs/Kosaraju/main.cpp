#include <bits/stdc++.h>
using namespace std;
vector <vector <int>> g, gt;
vector<bool> used;
vector<int> order, component, componentId;
vector<vector<int>> components;
int n,k;
void dfsSort(int v) {
    used[v] = true;
    for (int i = 0; i < g[v].size(); i++)
        if (!used[g[v][i]])
            dfsSort(g[v][i]);
    order.push_back(v);
}

void dfsCond(int v) {
    used[v] = true;
    componentId[v] = k;
    component.push_back(v);
    for (int i = 0; i < gt[v].size(); i++) {
        if (!used[gt[v][i]])
            dfsCond(gt[v][i]);
    }
}
int main() {
    int e;
    cin >> n >> e;
    k = 0;
    g.resize(n);
    gt.resize(n);
    int v1, v2;
    while (e--) {
        cin >> v1 >> v2;
        v1--;
        v2--;
        g[v1].push_back(v2);
        gt[v2].push_back(v1);
    }
    used.assign(n, false);
    for (int i = 0; i < n; i++)
        if (!used[i]) {
            dfsSort(i);
        }
    used.assign(n, false);
    componentId.resize(n);
    for (int i = 0; i < n; i++)
        if (!used[order[n - i - 1]]) {
            component.clear();
            dfsCond(order[n - i - 1]);
            components.push_back(component);
            k++;
            for (int j = 0; j < component.size(); j++) {
                cout << component[j] << " ";
            }
            cout << endl;
        }
    int countWithoutFrom = 0, countWithoutTo = 0;
    if (components.size() == 1){
        cout << 0;
        return 0;
    }
    for (int i = 0; i < components.size(); i++) {
        bool to = false;
        bool from = false;
        for (int j = 0; j < components[i].size(); j++) {
            for (int q = 0; q < g[components[i][j]].size(); q++)
                if (componentId[g[components[i][j]][q]] != componentId[components[i][j]]) {
                    from = true;
                }
            for (int q = 0; q < gt[components[i][j]].size(); q++)
                if (componentId[gt[components[i][j]][q]] != componentId[components[i][j]]) {
                    to = true;
                }
        }
        if (!to) countWithoutTo++;
        if (!from) countWithoutFrom++;
    }
    cout << max(countWithoutFrom, countWithoutTo);
    return 0;
}