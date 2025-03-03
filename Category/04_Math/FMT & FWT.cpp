// FMT（快速莫比乌斯变换）与 FWT（快速沃尔什变换）
//
// 资料：
// https://oi-wiki.org/math/poly/fwt/
// https://www.cnblogs.com/ying-xue/p/17687332.html
// https://www.cnblogs.com/yijan/p/12387352.html
//
// 题目：
// https://codeforces.com/contest/2070/problem/F


// FMT 能解决的问题是 FWT 的子集，两者用来求位运算卷积，时间复杂度 O(n logn)
auto FMT(const vector<int>& a, int tp = 1) {
    int n = a.size();
    vector<int> b(a);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (1 << n); ++j) {
            if (j & (1 << i)) {
                b[j] += b[j ^ (1 << i)] * tp; // tp = -1，则表示 IFMT
            }
        }
    }
    return b;
}

// 位运算卷积：
// 给定长为 n 的序列 A，B，求它们的卷积 C[i] = Σ(A[j]·B[k])，其中 Op(j, k) = i，Op 可为 and / or / xor
// 如果把 Op 换成 +，就是多项式乘法

// Or：O(n 2^n)
// 令 fwt_i(A) = Σ A[j]，其中 j|i = i，那么有 fwt_i(C) = fwt_i(A)·fwt_i(B)
auto Or(const vector<int>& a, int tp) {
    int n = a.size();
    vector<int> b(a);
    for (int len = 1; len < (1 << n); len <<= 1) {
        for (int i = 0; i < (1 << n); i += (len << 1)) {
            for (int j = 0; j < len; j++) {
                b[i + j + len] += b[i + j] * tp;
            }
        }
    }
    return b;
}

// And：与 Or 本质相同
auto And(const vector<int>& a, int tp) {
    int n = a.size();
    vector<int> b(a);
    for (int len = 1; len < (1 << n); len <<= 1) {
        for (int i = 0; i < (1 << n); i += (len << 1)) {
            for (int j = 0; j < len; j++) {
                b[i + j] += b[i + j + len] * tp;
            }
        }
    }
    return b;
}

// Xor
auto Xor(const vector<int>& a, int tp) {
    int n = a.size();
    vector<int> b(a);
    for (int len = 1; len < (1 << n); len <<= 1) {
        for (int i = 0; i < (1 << n); i += (len << 1)) {
            for (int j = 0; j < len; j++) {
                int x = b[i + j], y = b[i + j + len];
                b[i + j] = (x + y) * tp; // % mod
                b[i + j + len] = (x - y) * tp; // % mod
            }
        }
    }
    return b;
}

// 子集卷积
// todo


