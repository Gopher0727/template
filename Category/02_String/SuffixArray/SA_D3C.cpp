// 比较函数，用于比较两个后缀
int compare(const vector<int>& sa, const string& s, int a, int b, int n) {
    int len = min(n, (int) s.length() - max(a, b));
    for (int i = 0; i < len; ++i) {
        if (s[a + i] != s[b + i]) {
            return s[a + i] - s[b + i];
        }
    }
    return a - b;
}

// DC3算法构建后缀数组
vector<int> buildSA(const string& s) {
    int n = s.size();
    vector<int> sa(n);
    vector<int> pos = sa, tmp;
    for (int i = 0; i < n; ++i) {
        pos[i] = i, sa[i] = s[i];
    }
    sort(sa.begin(), sa.end(), [&s](int a, int b) { return s[a] < s[b]; });
    for (int cur = 1, k = 0; (1 << k) < n; ++k) {
        sort(pos.begin(), pos.end(), [&](int a, int b) { return sa[a] < sa[b] || (sa[a] == sa[b] && s[(a + (1 << k)) % n] < s[(b + (1 << k)) % n]); });
        tmp[pos[0]] = 0;
        for (int i = 1; i < n; ++i) {
            tmp[pos[i]] = compare(sa, s, pos[i - 1], pos[i], (1 << k) + k);
        }
        swap(tmp, pos);
        if (pos[sa[0]] == 0) {
            cur = 0;
        }
        for (int i = 0; i < n; ++i) {
            if (pos[i] + cur < n) {
                cur++;
            }
        }
        if (cur == n) {
            break;
        }
    }
    return sa;
}
