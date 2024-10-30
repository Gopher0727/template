// manacher:
//
// 扩展串中的回文半径和真实回文长度的对应：真实长度 = p[i] - 1
// 扩展回文串结尾下标和真实回文串终止位置的对应：真实回文串终止位置 = 扩展回文串结尾下标 / 2
// （真实回文串终止位置指不能满足条件的第一个字符）
//  · 求回文子串数量
//
// 练习题目：
// https://leetcode.cn/problems/check-if-dfs-strings-are-palindromes/  （dfs时间戳）
//
vector<int> manacher(string& s) {
    int m = s.size() * 2 + 1;
    string t(m, ' ');
    for (int i = 0, j = 0; i < m; ++i) {
        t[i] = i & 1 ? s[j++] : '#';
    }

    vector<int> Len(m);
    for (int i = 0, c = 0, r = 0; i < m; ++i) {
        int len = r > i ? min(Len[2 * c - i], r - i) : 1;
        while (i + len <= m && i - len >= 0 && t[i - len] == t[i + len]) {
            len++;
        }
        if (i + len > r) {
            r = i + len;
            c = i;
        }
        Len[i] = len;
    }
    return Len;
} // 返回值为真实回文串长度 +1
