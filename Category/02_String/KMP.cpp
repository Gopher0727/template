// 前缀函数 pi 表示 s[0:i]（闭区间）的相等的真前缀与真后缀的最大长度

// ※ 在线算法
auto calcPi(const string& s) {
    int n = s.size();
    vector<int> pi(n);
    int match = 0;
    for (int i = 1; i < n; i++) {
        while (match > 0 && s[match] != s[i]) {
            match = pi[match - 1];
        }
        if (s[match] == s[i]) {
            match++;
        }
        pi[i] = match;
    }
    return pi;
}

// KMP 利用 PMT 加速下一次匹配，并且原串指针不会进行回溯。
// 实际编码过程中，把 PMT 右移一位，第 0 位的值设成了 -1，得到 nxt 数组
// nxt 数组表示不含当前字符的前后缀最大匹配长度（前后缀小于子串长度）

auto nextArray(const string& s) {
    int n = s.size();
    vector<int> nxt(n + 1);
    nxt[0] = -1;
    for (int i = 2, j = 0; i <= n; i++) {
        while (j != -1 && s[i - 1] != s[j]) {
            j = nxt[j];
        }
        nxt[i] = ++j;
    }
    return nxt;
}

// 返回文本串 s 中，模式串 p 首次出现的下标（p[0] 在 s 中的下标）
int KMP(const string& s, const string& p) {
    vector<int> nxt(nextArray(p));
    int n = s.size(), m = p.size();
    int x = 0, y = 0;
    while (x < n && y < m) {
        if (s[x] == p[y]) {
            x++, y++;
        } else if (y == 0) {
            x++;
        } else {
            y = nxt[y];
        }
    }
    return y == m ? x - y : -1;
}

// 返回文本串 s 中，模式串 p 所有成功匹配的下标（p[0] 在 s 中的下标）
auto kmpSearch(const string& s, const string& p) {
    int n = s.size();
    vector<int> pi(n);
    vector<int> pos;
    int match = 0;
    for (int i = 0; i < n; i++) {
        while (match > 0 && p[match] != s[i]) {
            match = pi[match - 1];
        }
        if (p[match] == s[i]) {
            match++;
        }
        if (match == p.size()) {
            pos.push_back(i - p.size() + 1);
            // 不允许重叠则 match = 0
            match = pi[match - 1];
        }
    }
    return pos;
}
