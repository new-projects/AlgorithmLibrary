#include <cstdio>
#include <cstring>
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;

typedef int T;
const int MAX_N = 1000000 + 10;
const int SEG_LEN = 16;
const int MAX_M = MAX_N / SEG_LEN + 2;
const int MAX_K = 18; // MAX_K = log(MAX_M)

int d[MAX_K][MAX_M];
int int_log[MAX_M];

T a[MAX_N];


// Time Complexity: O(N/SEG_LEN * log(N/SEG_LEN)) ~ O(N)
// Space Complexity: O(N/SEG_LEN * log(N/SEG_LEN)) ~ O(N)
template <class Compare>
void RmqBuild(int m, const Compare& comp) {
    int n = 0;
    for (int i = 0, j = 0; i < m; i += SEG_LEN) {
        d[0][n++] = min_element(a + i,
                (i + SEG_LEN <= m ? a + i + SEG_LEN : a + m), comp) - a;
        while ((1 << j) <= n) ++j;
        int_log[n] = j - 1;
    }

    for (int j = 1; (1 << j) <= n; ++j) {
        for (int i = 0; i + (1 << j) <= n; ++i) {
            if (comp(a[d[j - 1][i + (1 << (j - 1))]], a[d[j - 1][i]])) {
                d[j][i] = d[j - 1][i + (1 << (j - 1))];
            } else {
                d[j][i] = d[j - 1][i];
            }
        }
    }
}


// Time Complexity: O(1) + O(SEG_LEN)
template <class Compare>
int RmqQuery(int i, int j, const Compare& comp) { // i <= j
    int x = i / SEG_LEN, y = j / SEG_LEN;
    if (x == y) return min_element(a + i, a + j + 1, comp) - a;
    int l = min_element(a + i, a + (x + 1) * SEG_LEN, comp) - a;
    int r = min_element(a + y * SEG_LEN, a + j + 1, comp) - a;
    if (y - 1 < x + 1) return comp(a[r], a[l]) ? r : l;
    int k = int_log[y - 1 - x];
    int m = comp(a[d[k][y - (1 << k)]], a[d[k][x + 1]]) ?
        d[k][y - (1 << k)] : d[k][x + 1];
    if (comp(a[r], a[l])) return comp(a[r], a[m]) ?  r : m;
    else return comp(a[m], a[l]) ? m : l;
}


int main() {
    ios_base::sync_with_stdio(false);
    int n, k;
    cin >> n >> k;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    RmqBuild(n, less<int>());
    for (int i = 0; i + k <= n; ++i) {
        if (i > 0) cout << ' ';
        cout << (a[RmqQuery(i, i + k - 1, less<int>())]);
    }
    cout << endl;

    RmqBuild(n, greater<int>());
    for (int i = 0; i + k <= n; ++i) {
        if (i > 0) cout << ' ';
        cout << (a[RmqQuery(i, i + k - 1, greater<int>())]);
    }
    cout << endl;
    return 0;
}

