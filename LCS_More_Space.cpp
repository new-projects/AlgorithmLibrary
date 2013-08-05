#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

// Time Complexity: O(MN/w), w = bits of int or long long
// Space Complexity: O(MN/w)
// Tested by:  whu_OJ 1210  SPOJ452
typedef unsigned int T; // [USER] T could be unsigned int or unsigned long long
typedef char Key;       // [USER]
const int ALPHA_SIZE = 128; // [USER]
const int W_LEN = sizeof(T) * 8;
const int SHIFT_LEN = (W_LEN == 32 ? 5 : 6);
const int MAX_N = 10000 + 10; // [USER]


// m = length of array a
// n = length of array b
// res contains one of LCS paths(NOT necessary the lexicographically smallest)
// return the |LCS|
int Lcs(int m, int n, const Key* a, const Key* b, Key* res) {
    static T bit[ALPHA_SIZE][(MAX_N >> SHIFT_LEN) + 2];
    static T mat[MAX_N][(MAX_N >> SHIFT_LEN) + 2];

    int nbits = (m + W_LEN - 1) >> SHIFT_LEN;
    for (int i = 0; i < m; i++)
        bit[a[i]][i >> SHIFT_LEN] |= static_cast<T>(1) << (i & (W_LEN - 1));

    fill(mat[0], mat[0] + nbits, 0);
    T top_bit, t, carry, x, y, *bit_s, *old, *dp;
    for (int i = 0; i < n; ++i) {
        top_bit = 1; bit_s = bit[b[i]];
        if (i == 0) { dp = old = mat[0]; }
        else        { dp = mat[i]; old = mat[i - 1]; }
        for (int j = carry = 0; j < nbits; j++) {
            x = old[j] | bit_s[j];
            t = old[j] >> (W_LEN - 1);
            y = (old[j] << 1) | top_bit;
            top_bit = t;
            dp[j] = x & ((x - y - carry) ^ x);
            carry = (x < y + carry) ? 1 : 0;
        }
    }
    int r = n - 1, c = m - 1, len = 0;
    while (r >= 0 && c >= 0) {
        if (b[r] == a[c]) { 
            res[len++] = b[r];
            --r;
            --c;
        } else if (mat[r][c >> SHIFT_LEN] & 
                (static_cast<T>(1) << (c & (W_LEN - 1)))) {
            --r;
        } else {
            --c;
        }
    }
    for (int i = 0; i < m; i++) bit[a[i]][i >> SHIFT_LEN] = 0;
    reverse(res, res + len);
    return len; 
}


Key c[MAX_N];
string a, b;

int main() {
    ios_base::sync_with_stdio(false);
    while (cin >> a >> b) {
        cout << Lcs(a.size(), b.size(), a.c_str(), b.c_str(), c) << endl;
    }
    return 0;
}

