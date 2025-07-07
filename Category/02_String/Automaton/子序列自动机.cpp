// 序列自动机
//
// > 判断是否是原字符串的子序列
// > 求子序列个数
// > 求两串的公共子序列个数
// > 求字符串的回文子序列个数
// > 求 A、B 最长公共子序列 S，使得 C 是 S 的子序列

auto create(const string& s) {
    int n = s.size();
    vector nxt(n + 1, array<int, 26> {});
    nxt[n].fill(n);
    for (int i = n - 1; i >= 0; i--) {
        nxt[i] = nxt[i + 1];
        nxt[i][s[i] - 'a'] = i;
    }
    return nxt;
}
