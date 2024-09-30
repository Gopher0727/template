/**
 * manacher:
 *
 * 扩展串中的回文半径和真实回文长度的对应：真实长度 = p[i] - 1
 * 扩展回文串结尾下标和真实回文串终止位置的对应：真实回文串终止位置 = 扩展回文串结尾下标 / 2
 *      （真实回文串终止位置指不能满足条件的第一个字符）
 *
 * · 求回文子串数量
 */

vector<int> manacher(string& s) {
    int n = s.size() * 2 + 1;
    string t(n, ' ');
    for (int i = 0, j = 0; i < n; ++i) {
        t[i] = i & 1 ? s[j++] : '#';
    }

    vector<int> vec(n);
    for (int i = 0, c = 0, r = 0; i < n; ++i) {
        int len = r > i ? min(vec[2 * c - i], r - i) : 1;
        while (i + len <= n && i - len >= 0 && t[i - len] == t[i + len]) {
            len++;
        }
        if (i + len > r) {
            r = i + len;
            c = i;
        }
        vec[i] = len;
    }
    return vec;
} // 返回值为真实长度 +1
