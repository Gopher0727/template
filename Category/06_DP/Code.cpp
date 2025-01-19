// 长为 k 的子序列按位或
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

// O(n^2) 求子串（子数组）是否回文
// 一般用于 DP 预处理
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
