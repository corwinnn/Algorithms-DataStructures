#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;
template <typename T>
class DTree{
public:
    DTree<T> *left, *right;
    T key;
    int prior, size;

    explicit DTree<T> (T &x) {
        key = x;
        prior = rand();
        left = right = nullptr;
        size = 1;
    }

    int sizeOf() const {
        return this ? this->size : 0;
    }

    void updSize() {
        if(this)
            size = left->sizeOf() + right->sizeOf() + 1;
    }

    void show() const {
        if (left)
            left->show();
        cout << key << ", size: " << size << endl;
        if (right)
            right->show();
    }

};

template <typename T>
void split(DTree<T> *root, DTree<T>* &left, DTree<T>* &right, int key) {
    if (root == nullptr) {
        left = right = nullptr;
        return;
    }
    int len = root->left->sizeOf();
    if (len >=  key) {
        split(root->left, left, root->left, key);
        right = root;
    }
    else
    {   split(root->right, root->right, right, key - len - 1);
        left = root;
    }
    root->updSize();
}

template <typename T>
DTree<T>* merge(DTree<T> *left, DTree<T> *right)
{
    if (left == nullptr || right == nullptr)
        return right == nullptr ? left : right;
    if (left->prior> right->prior) {
        left->right = merge(left->right, right);
        left->updSize();
        return left;
    }
    else {
        right->left = merge(left, right->left);
        right->updSize();
        return right;
    }
}

template <typename T>
void insert_t(DTree<T>* &root, T key, int pos) {
    DTree<T> *q1, *q2;
    split(root, q1, q2, pos);
    q1 = merge(q1, new DTree<T>(key));
    root = merge(q1, q2);
}

template <typename T>
void erase_t(DTree<T>* &root, int pos) {
    DTree<T> *q1, *q2, *q3, *q4;
    split(root, q1, q2, pos);
    split(q2, q3, q4, 1);
    root = merge(q1, q4);
    root->updSize();
}

template <typename T>
T& get_t(DTree<T>* &root, int pos) {
    int len = root->left->sizeOf();
    if (pos == len) {
        return root->key;
    }
    if (pos < len) {
        return get_t(root->left, pos);
    }
    if (pos > len) {
        return get_t(root->right, pos - len - 1);
    }

}
template <typename T>
class Vector {
public:
    Vector() {
        t = nullptr;
    }
    void insert(T key, int pos) {
        insert_t(t, key, pos);
    }
    void erase(int pos) {
        erase_t(t, pos);
    }
    T& operator[] (int i) { return get_t(t, i); }

private:
    DTree<T>* t;
};
int main() {
    srand(time(0));
    int n, p;
    cin >> n;
    Vector<string> a;
    string s;
    char c;
    for (int i = 0; i < n; i++) {
        cin >> c;
        if (c == '+') {
            cin >> p >> s;
            a.insert(s, p);
        }
        if (c == '-') {
            cin >> p;
            a.erase(p);
        }
        if (c == '?') {
            cin >> p;
            cout << a[p] << endl;
        }
    }
    return 0;
}