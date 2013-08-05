#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

// Time Complexity: O(MN/w), w = bits of int or long long
// Space Complexity: O(N)
// Tested by:  whu_OJ 1210  code forces 335B  SPOJ12076  SPOJ452
typedef unsigned int T; // [USER] T could be unsigned int or unsigned long long
typedef char Key;       // [USER]
const int ALPHA_SIZE = 128; // [USER]
const int W_LEN = sizeof(T) * 8;
const int SHIFT_LEN = (W_LEN == 32 ? 5 : 6);
const int MAX_N = 50000 + 10; // [USER]


// LcsBit() could be called directly if you don't need the LCS path
// LcsBit() is ONE time faster than Lcs()
// |LCS| = res[m - 1]
void LcsBit(int m, int n, const Key* a, const Key* b, int* res) {
    static T bit[ALPHA_SIZE][(MAX_N >> SHIFT_LEN) + 2];
    static T dp[(MAX_N >> SHIFT_LEN) + 2];

    int nbits = (m + W_LEN - 1) >> SHIFT_LEN;
    for (int i = 0; i < m; i++)
        bit[a[i]][i >> SHIFT_LEN] |= static_cast<T>(1) << (i & (W_LEN - 1));
    
    fill(dp, dp + nbits, 0);
    T top_bit, t, carry, x, y, *bit_s;
    for (int i = 0; i < n; i++) {
        top_bit = 1; bit_s = bit[b[i]];
        for (int j = carry = 0; j < nbits; j++) {
            x = dp[j] | bit_s[j];
            t = dp[j] >> (W_LEN - 1);
            y = (dp[j] << 1) | top_bit;
            top_bit = t;
            dp[j] = x & ((x - y - carry) ^ x);
            carry = (x < y + carry) ? 1 : 0;
        }
    }
    int sum = 0;
    for (int i = 0; i < m; i++) {
        if (dp[i >> SHIFT_LEN] & 
            (static_cast<T>(1) << (i & (W_LEN - 1)))) ++sum;
        res[i] = sum;
        bit[a[i]][i >> SHIFT_LEN] = 0;
    }
}


// m = length of array a
// n = length of array b
// c contains one of LCS paths(NOT necessary the lexicographically smallest)
// return the |LCS|
int Lcs(int m, int n, const Key* a, const Key* b, Key* c) {
    if (n <= 0 || m <= 0) {
        return 0;
    } else if (m == 1) {
        for (int i = 0; i < n; ++i) {
            if (a[0] == b[i]) { c[0] = a[0]; return 1; }
        }
        return 0;
    }

    static Key ra[MAX_N], rb[MAX_N];
    static int res1[MAX_N], res2[MAX_N];
    int i = (m - 1) >> 1;
    reverse_copy(a, a + m, ra);
    reverse_copy(b, b + n, rb);
    LcsBit(n, i + 1, b, a, res1);
    LcsBit(n, m - i - 1, rb, ra, res2);

    int len1 = 0, len2 = res2[n - 1], k = -1;
    for (int j = 0; j < n - 1; ++j) {
        if (res1[j] + res2[n - j - 2] > len1 + len2) {
            len1 = res1[j]; len2 = res2[n - j - 2]; k = j;
        }
    }
    if (res1[n - 1] > len1 + len2) { len1 = res1[n - 1]; len2 = 0; k = n - 1; }

    if (len1 > 0) Lcs(i + 1, k + 1, a, b, c);
    if (len2 > 0) Lcs(m - i - 1, n - k - 1, a + i + 1, b + k + 1, c + k + 1);
    copy(c + k + 1, c + k + 1 + len2, c + len1);
    return len1 + len2;
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

