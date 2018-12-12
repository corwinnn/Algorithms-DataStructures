#include <vector>
#include <iostream>
#include <limits>

using namespace std;
template <typename T>
struct vert {
    vert() = default;
    vert(T max, T sum, T update):
        max(max),
        sum(sum),
        update(update)
    {}
    int max;
    int sum;
    int update;
};
template <typename T>
vert<T> vertUnion(vert<T> a, vert<T> b){
    vert<T> c = vert<T>();
    c.max = max(a.max, b.max);
    c.sum = a.sum + b.sum;
    c.update = 0;
    return c;
}
template <typename T>
class SegmentTree {
public:
    void TreeBuild(int v, int tl, int tr, const vector<T> &a) {
        if (tl == tr) {
            t[v] = vert<T>(a[tl], a[tl], 0);
        }
        else {
            int tm = (tl + tr) / 2;
            TreeBuild(v*2 + 1, tl, tm, a);
            TreeBuild(v*2 + 2, tm + 1, tr, a);
            t[v] = vertUnion(t[v*2 + 1], t[v*2 + 2]);
        }
    }
    SegmentTree(int n, vector<T> a) {
        t.resize(4 * a.size());
        size = a.size();
        TreeBuild(0, 0, a.size() - 1, a);
    }
    void print() {
        /*for (int i = 0; i < t.size(); i++) {
            cout << t[i].sum << " ";
        }

        cout << endl;*/
        for (int i = 0; i < t.size(); i++) {
            cout << t[i].max << " ";
        }
        cout << endl;
        for (int i = 0; i < t.size(); i++) {
            cout << t[i].update << " ";
        }
        cout << endl;
    }
    T getmax(int l, int r) {
        return _getmax(0, 0, size - 1, l, r);
    }
    T getsum(int l, int r) {
        return _getsum(0, 0, size - 1, l, r);
    }
    void update(int l, int r, int val) {
        _upd(0, 0, size - 1, l, r, val);
    }


private:
    int size;
    vector<vert<T>> t;
    T _getmax(int v, int tl, int tr, int l, int r) {
        //cout << "getmax " << v << " " << tl << " " << tr << " " << l << " " << r << endl;
        if (l > r)
            return numeric_limits<T>::min();
        if (l == tl && r == tr)
            return t[v].max;
        int tm = (tr + tl) / 2;
        return max(_getmax(v*2 + 1, tl, tm, l, min(r, tm)), _getmax(v*2 + 2, tm + 1, tr, max(l, tm + 1), r)) +
                t[v].update;
    }
    T _getsum(int v, int tl, int tr, int l, int r) {
        if (l > r)
            return 0;
        if (l == tl && r == tr)
            return t[v].sum;
        int tm = (tr + tl) / 2;
        return _getsum(v*2 + 1, tl, tm, l, min(r, tm)) + _getsum(v*2 + 2, tm + 1, tr, max(l, tm + 1), r) + t[v].update;
    }
    void _upd(int v, int tl, int tr, int l, int r, int val) {
        //cout << "upd " << v << " " << tl << " " << tr << " " << l << " " << r << " " << val << endl;
        if (l > r)
            return;
        if (l == tl && r == tr) {
            t[v].update += val;
            t[v].max += val;
            return;
        }
        int tm = (tr + tl) / 2;
        _upd(v*2 + 1, tl, tm, l, min(r, tm), val);
        _upd(v*2 + 2, tm + 1, tr, max(l, tm + 1), r, val);
        t[v].max = max(t[v*2 + 1].max, t[v*2 + 2].max) + t[v].update;
    }
};

int main() {
    int n;
    cin >> n;
    n--;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    SegmentTree<int> *t = new SegmentTree<int>(n, a);
    int kol;
    cin >> kol;
    int q;
    cin >> q;
    int l, r, k;
    //t->print();
    for (int i = 0; i < q; i++){
        cin >> l >> r >> k;
        r--;
        int d = t->getmax(l,r) + k;
        //cout << "d" << " " << d << endl;
        if (d <= kol) {
            t->update(l, r, k);
        }
        else {
            cout << i << " ";
        }
        //t->print();
    }
    return 0;
}
// 2 3 2 2 1