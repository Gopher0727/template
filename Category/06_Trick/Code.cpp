// 预处理，求长为 k 的子序列的按位或    O(n * k * 2^k)
//
auto findORs(const vector<int>& a, int k) {
    vector<set<int>> dp;
    vector<set<int>> pre(k + 1);
    pre[0].insert(0);
    for (int i = 0; i < a.size(); ++i) {
        for (int j = min(k - 1, i + 1); j >= 0; --j) {
            for (int x : pre[j]) {
                pre[j + 1].insert(x | a[i]);
            }
        }
        dp.emplace_back(pre[k]);
    }
    return dp;
}


// 预处理，求集合各子集 LCM, OR    O(2^n)
//
auto findLCMs(const vector<int>& a) {
    int n = a.size();
    vector<i64> lcms(1 << n);
    lcms[0] = 1;
    for (int i = 0; i < n; ++i) {
        int bit = 1 << i;
        for (int mask = 0; mask < bit; ++mask) {
            lcms[bit | mask] = lcm(a[i], lcms[mask]);
        }
    }
    lcms[0] = 0;
    return lcms;
}

auto findORs(const vector<int>& a) {
    int n = a.size();
    vector<i64> ors(1 << n);
    for (int i = 0; i < n; ++i) {
        int bit = 1 << i;
        for (int mask = 0; mask < bit; ++mask) {
            ors[bit | mask] = a[i] | ors[mask];
        }
    }
    return ors;
}


// 预处理，判断子串是否回文    O(n^2)
//
auto isPalindrome(const string& s) {
    int n = s.size();
    vector isPal(n, vector<int>(n));
    for (int l = n - 1; l >= 0; --l) {
        for (int r = l; r < n; ++r) {
            isPal[l][r] = (s[l] == s[r] && (r - l < 3 || isPal[l + 1][r - 1]));
        }
    }
    return isPal;
}


// 前缀和+哈希表 枚举
//
// Problems:
// https://codeforces.com/contest/519/problem/D    1800（远古题，难度分偏高）
