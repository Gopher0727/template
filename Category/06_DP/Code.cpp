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
