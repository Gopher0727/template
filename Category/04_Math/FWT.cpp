// FWT（快速沃尔什变换）
//
// 资料：
// https://oi-wiki.org/math/poly/fwt/
// https://www.cnblogs.com/ying-xue/p/17687332.html
// https://www.cnblogs.com/yijan/p/12387352.html
//
// 题目：
// https://codeforces.com/contest/2070/problem/F


// FMT 能解决的问题是 FWT 的子集，两者用来求位运算卷积，时间复杂度 O(n logn)
auto FMT(const vector<int>& a, int Op = 1) {
    int n = a.size();
    vector<int> b(a);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (1 << n); ++j) {
            if (j & (1 << i)) {
                b[j] += b[j ^ (1 << i)] * Op; // Op = -1，则表示 IFMT
            }
        }
    }
    return b;
}

// 位运算卷积：
// 给定长为 n 的序列 A，B，求它们的卷积 C[i] = Σ(A[j]·B[k])，其中 Op(j, k) = i，Op 可为 and / or / xor
// 如果把 Op 换成 +，就是多项式乘法

// Or：O(n 2^n)  fast zeta transform
// 令 fwt_i(A) = Σ A[j]，其中 j|i = i，那么有 fwt_i(C) = fwt_i(A)·fwt_i(B)
auto fwtOr(vector<int> a, int Op) {
    int n = a.size();
    for (int len = 2, k = 1; len <= n; len <<= 1, k <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < k; ++j) {
                a[i + j + k] += a[i + j] * Op;
            }
        }
    }
    return a;
}

// And：与 Or 本质相同  fast mobius transform
auto fwtAnd(vector<int> a, int Op) {
    int n = a.size();
    for (int len = 2, k = 1; len <= n; len <<= 1, k <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < k; ++j) {
                a[i + j] += a[i + j + k] * Op;
            }
        }
    }
    return a;
}

// Xor
auto fwtXor(vector<int> a, int Op) {
    int n = a.size();
    for (int len = 2, k = 1; len <= n; len <<= 1, k <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < k; ++j) {
                int x = a[i + j], y = a[i + j + len];
                a[i + j] = (x + y) * Op; // % mod
                a[i + j + len] = (x - y) * Op; // % mod
            }
        }
    }
    return a;
}

auto fwtXor(vector<i64>& a, bool invert) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                i64 u = a[i + j], v = a[i + j + len];
                a[i + j] = u + v;
                a[i + j + len] = u - v;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

// 子集卷积
// todo


