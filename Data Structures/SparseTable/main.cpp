#include <bits/stdc++.h>
using namespace std;
template <typename T>

class SparseTable {
public:
    vector<vector<int>> st;
    vector<T> values;

    SparseTable(const vector<T> &a) {
        size = a.size();
        values = a;
        int k = 0;
        int p = 1;
        pows.resize(0);
        while (p <= a.size()) {
            pows.push_back(p);
            p *= 2;
        }
        pows.push_back(p);
        st.resize(a.size());
        for (int i = 0; i < a.size(); i++) {
            int j = 0;
            while (pows[j] < a.size() - i) {
                j++;
            }
            st[i].resize(j + 1);
        }
        rows.resize(st[0].size());
        rows.assign(rows.size(), 0);
        for (int i = 0; i < st.size(); i++) {
            for (int j = 0; j < st[i].size(); j++) {
                rows[j]++;
            }
        }
        for (int j = 0; j < st[0].size(); j++) {
            for (int i = 0; i < rows[j]; i++) {
                if (j == 0) {
                    st[i][j] = i;
                }
                else {
                    int t = min(j - 1, (int) st[i + pows[j - 1]].size() - 1);
                    set<T> s;
                    st[i][j] = (a[st[i][j - 1]] < a[st[i + pows[j - 1]][t]]) ? st[i][j - 1] : st[i + pows[j - 1]][t];
                }
            }
        }
        log2down.resize(a.size());
        for (int i = 0; i < a.size(); i++) {
            int j = 0;
            while (pows[j] <= i + 1) {
                j++;
            }
            log2down[i] = j - 1;
        }
    }
    T getSMin(int l, int r) {
        int p = log2down[r - l];
        int ans;
        int i1 = st[l][p];
        int i2 = st[r - pows[p] + 1][p];
        ans = (values[i1] < values[i2]) ? i1 : i2;
        return ans;
    }
    void print() {
        for (int i = 0; i < st.size();i++) {
            for (int j = 0; j < st[i].size(); j++) {
                cout << st[i][j] << " ";
            }
            cout << endl;
        }
    }
private:
    vector<int> pows;
    vector<int> log2down;
    vector<int> rows;
    int size;

};
class Tree {
public:
    vector<int> depth;
    vector<vector<int>> g;
    vector<int> depth_dfs;
    vector<int> vrt_dfs;
    vector<int> ind_dfs;
    vector<int> values;
    int size;
    int countDeapth(int k) {
        if (depth[k] != -1)
            return depth[k];
        if (k == 0) {
            depth[k] = 0;
            return 0;
        }
        return depth[k] = countDeapth(values[k]) + 1;
    }
    void dfs(int v) {
        depth_dfs.push_back(depth[v]);
        vrt_dfs.push_back(v);
        if (ind_dfs[v] == -1) {
            ind_dfs[v] = vrt_dfs.size() - 1;
        }
        for (int i = 0; i < g[v].size(); i++) {
            dfs(g[v][i]);
            depth_dfs.push_back(depth[v]);
            vrt_dfs.push_back(v);
        }
    }
    Tree(vector<int> a) {
        size = a.size();
        depth.resize(size);
        depth.assign(size, -1);
        values = a;
        for (int i = 0; i < depth.size(); i++) {
            countDeapth(i);
        }
        g.resize(size);
        for (int i = 1; i < size; i++) {
            g[a[i]].push_back(i);
        }
        ind_dfs.resize(size);
        ind_dfs.assign(size, -1);
        depth_dfs.resize(0);
        vrt_dfs.resize(0);
        dfs(0);
    }

    void print() {
        for (int i = 0; i < depth_dfs.size(); i++) {
            cout << depth_dfs[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < ind_dfs.size(); i++) {
            cout << ind_dfs[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < vrt_dfs.size(); i++) {
            cout << vrt_dfs[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < depth.size(); i++) {
            cout << depth[i] << " ";
        }
        cout << endl;
    }
};
int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    a[0] = 0;
    for (int i = 1; i < n; i++) {
        cin >> a[i];
    }
    Tree *t = new Tree(a);
    SparseTable<int> *st = new SparseTable<int>(t->depth_dfs);
    //t->print();
    long long a1, a2;
    cin >> a1 >> a2;
    int x, y, z;
    cin >> x >> y >> z;
    int ans = 0;
    long long a0 = a1, b0 = a2;
    for (int i = 0; i < m; i++) {
        int i1 = t->ind_dfs[a1];
        int i2 = t->ind_dfs[a2];
        if (i1 > i2) {
            swap(i1, i2);
        }
        int ians = st->getSMin(i1, i2);
        long long d = t->vrt_dfs[ians];
        ans += d;
        a0 = (x * a0 + y * b0 + z) % n;
        b0 = (x * b0 + y * a0 + z) % n;
        a1 = (a0 + d) % n;
        a2 = b0;
    }
    cout << ans;
    return 0;
}