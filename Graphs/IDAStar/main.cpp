#include <bits/stdc++.h>

using namespace std;

map<char,char> dict;
class Table {
public:
    int* table;
    static int size;
    Table(int n) {
        table = new int [n*n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                int x;
                cin >> x;
                table[(i << 2) +j] = x;
            }
    }
    ~Table() {
        delete table;
    }

    int getHeuristic(){
        int k = 0, p, p1,ii;
        for (int i = 0; i < size; i++) {
            p = 0;
            p1 = 0;
            ii = i << 2;
            for (int j =  0; j < size; j++) {
                if (i * size + j + 1 != table[ii + j])
                    k += ( !table[ii + j]) ? 0 :abs(i - ((table[ii + j] - 1)>> 2)) + abs(j - ((table[ii + j]-1) & 3));

                for (int q = j + 1; q < size; q++) {
                    if (((table[ii + j] - 1) >> 2) == i && ((table[ii + q] - 1) >> 2) == i)
                        if (table[ii + q] && table[ii + j] > table[ii + q])
                            p += 2;
                    if (((table[(j << 2) + i] - 1)  & 3) == i && ((table[(q << 2) + i] - 1)  & 3) == i)
                        if (table[(q << 2) + i] && table[(j << 2) + i] > table[(q << 2) + i])
                            p1 += 2;
                }
            }
            k += p + p1;
        }
        return k;
    }

    pair<int,int> getZeroPlace(){
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++)
                if (table[(i << 2) + j] == 0)
                    return make_pair(i, j);
        }
    };

    bool isSolve() {
        int a[size*size];
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                a[i*size+j] = table[(i << 2) +j];
        int inv = 0;
        for (int i=0; i<size*size; ++i)
            if (a[i])
                for (int j=0; j<i; ++j)
                    if (a[j] > a[i])
                        ++inv;
        if (!(size%2)) {
            for (int i = 0; i < size * size; ++i)
                if (a[i] == 0)
                    inv += 1 + (i >> 2);
        }
        return (inv & 1) ? false : true;
    }

    void show() {
        for (int i = 0;i < size; i++) {
            for (int j = 0;j < size; j++)
                cout << table[(i << 2) +j] << " ";
            cout << endl;
        }
        cout << endl;
    }

    bool isMove(char s, int x, int y){
        if (s == 'L')
            return y == 0 ? false : true;
        if (s == 'U')
            return x == 0 ? false : true;
        if (s == 'D')
            return x == size - 1 ? false : true;
        if (s == 'R')
            return y == size - 1 ? false : true;


    }

    int getLinearStr(int x1, int y1) {
        int linear = 0;
        int xx = x1 << 2;
        int el = table[xx + y1];
        if(((el - 1) >> 2) == x1) {
            for (int j = 0; j < size; j++)
                if (table[xx +j] && ((table[xx + j] - 1) >> 2) == x1)
                    if ((el < table[xx +j] && j < y1) || (el > table[xx +j] && j > y1))
                        linear += 2;
        }
        return linear;
    }

    int getLinearCol(int x1, int y1) {
        int linear = 0;
        if(((table[(x1 << 2) +y1] - 1)  & 3) == y1) {
            for (int j = 0; j < size; j++)
                if (table[(j << 2) +y1] && ((table[(j << 2) +y1] - 1)  & 3) == y1)
                    if ((table[(x1 << 2) +y1] < table[(j << 2) +y1] && j < x1) || (table[(x1 << 2) +y1] > table[(j << 2) +y1] && j > x1))
                        linear += 2;
        }
        return linear;
    }

    int changeHevristic(int x1, int y1, int x2, int y2) {
        int change = 0;
        change -= getLinearStr(x2, y2) + getLinearCol(x2, y2);
        swap(table[(x1 << 2) + y1], table[(x2 << 2) + y2]);
        int el = table[(x1 << 2) +y1];
        int m1 = abs(x2 - ((el - 1) >> 2)) + abs(y2 - ((el - 1) & 3));
        int m2 = abs(x1 - ((el - 1) >> 2)) + abs(y1 - ((el - 1) & 3));
        change += m2 - m1;
        change += getLinearStr(x1, y1) + getLinearCol(x1, y1);
        //swap(table[(x1 << 2) + y1], table[(x2 << 2) + y2]);

        //swap(table[(x1 << 2) +y1], table[(x2 << 2) +y2]);
        return change;
    }
};
int Table::size = 0;
void Search(Table* b, int g, int ev, string &s, int x, int y, int f);
inline void Moving(Table* b, int g, int ev, string &s, int x, int y, int f, char direction) {
    if (b->isMove(direction, x, y) && s[g - 1] != direction) {
        int newX, newY;
        if (direction == 'L') {
            newX = x;
            newY = y - 1;
        }

        else if (direction == 'U') {
            newX = x - 1;
            newY = y;
        }
        else if (direction == 'D') {
            newX = x + 1;
            newY = y;
        }
        else if (direction == 'R') {
            newX = x;
            newY = y + 1;
        }

        //swap(b->table[(x << 2) + y], b->table[(newX << 2) + newY]);
        s[g] = dict[direction];
        int d = b->changeHevristic(x, y, newX, newY);
        Search(b, g + 1, ev, s, newX, newY, f + d);
        swap(b->table[(x << 2) + y], b->table[(newX << 2) + newY]);
    }
    //return Search(b, g, ev, s, x, y, f);
}
void Search(Table* b, int g, int ev, string &s, int x, int y, int f) {
    if (f == 0) {
        cout << s.size() << endl << s;
        exit(0);
    }
    if (f + g > ev)
        return;
    Moving(b, g, ev, s, x, y, f, 'L');
    Moving(b, g, ev, s, x, y, f, 'D');
    Moving(b, g, ev, s, x, y, f, 'U');
    Moving(b, g, ev, s, x, y, f, 'R');



    return;
}

int IDAstar(Table* begin) {
    if (!begin->isSolve()) {
        cout << -1 << endl;
        return 0;
    }
    int e = begin->getHeuristic();
    int e0 = e;
    pair<int,int> zp = begin->getZeroPlace();
    register string s;
    while(true)
    {   s.resize(e);
        Search(begin, 0, e, s, zp.first, zp.second, e0);
        e+=2;
    }
}
int main() {
    dict['R'] = 'L';
    dict['L'] = 'R';
    dict['D'] = 'U';
    dict['U'] = 'D';
    int n = 4;
    Table::size = n;
    register Table* t0 = new Table(n);
    IDAstar(t0);
    delete t0;
    return 0;
}