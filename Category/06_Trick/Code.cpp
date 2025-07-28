// 将字符矩阵顺时针旋转 90 度
auto f = [&](vector<string>& s) {
    int n = s.size(), m = s[0].size();
    vector<string> ns(m);
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < m; j++) {
            ns[j].push_back(s[i][j]);
        }
    }
    s = std::move(ns);

    // int n = s.size(), m = s[0].size();
    // for (int i = 0; i < n; i++) {
    //     for (int j = i; j < m; ++j) {
    //         swap(s[i][j], s[j][i]);
    //     }
    // }
    // for (int i = 0; i < n; i++) {
    //     reverse(s[i].begin(), s[i].end());
    // }
};


// v 需要增加多少才能被 r 整除
int d1 = (r - (v % r)) % r;
int d2 = (v + r - 1) / r * r - v;


// 给定区间集合，返回最多能选多少个无重叠的区间（端点重合不记作重叠）
auto intervalsCount(vector<pair<int, int>>& intervals) {
    ranges::sort(intervals, {}, &pair<int, int>::second);
    int ans = 0, pre = -1E9;
    for (auto [l, r] : intervals) {
        if (l >= pre) {
            ans++;
            pre = r;
        }
    }
    return ans;
}

// 给定区间集合，返回合并后的区间
auto intervalsMerge(const vector<array<int, 2>>& intervals) {
    vector<array<int, 2>> ans;
    for (auto [l, r] : intervals) {
        if (!ans.empty() && l <= ans.back()[1]) {
            ans.back()[1] = max(ans.back()[1], r);
        } else {
            ans.push_back({l, r});
        }
    }
    return ans;
}


// 预处理，求长为 k 的子序列的按位或    O(n * k * 2^k)
auto findORs(const vector<int>& a, int k) {
    vector<set<int>> dp;
    vector<set<int>> pre(k + 1);
    pre[0].insert(0);
    for (int i = 0; i < a.size(); i++) {
        for (int j = min(k - 1, i + 1); j >= 0; j--) {
            for (int x : pre[j]) {
                pre[j + 1].insert(x | a[i]);
            }
        }
        dp.push_back(pre[k]);
    }
    return dp;
}


// 预处理，求集合各子集 LCM, AND, OR, XOR    O(2^n)
auto findLCMs(const vector<int>& a) {
    int n = a.size();
    vector<i64> lcms(1 << n);
    lcms[0] = 1;
    for (int i = 0; i < n; i++) {
        int bit = 1 << i;
        for (int msk = 0; msk < bit; msk++) {
            lcms[bit | msk] = lcm(a[i], lcms[msk]);
        }
    }
    lcms[0] = 0;
    return lcms;
}

auto findANDs(const vector<int>& a) {
    int n = a.size();
    vector<int> And(1 << n);
    And[0] = -1;
    for (int i = 0; i < n; i++) {
        int bit = 1 << i;
        for (int msk = 0; msk < bit; msk++) {
            And[bit | msk] = a[i] & And[msk];
        }
    }
    And[0] = 0;
    return And;
}

auto findORs(const vector<int>& a) {
    int n = a.size();
    vector<int> Or(1 << n);
    for (int i = 0; i < n; i++) {
        int bit = 1 << i;
        for (int msk = 0; msk < bit; msk++) {
            Or[bit | msk] = a[i] | Or[msk];
        }
    }
    return Or;
}

auto findXORs(const vector<int>& a) {
    int n = a.size();
    vector<int> Xor(1 << n);
    for (int i = 0; i < n; i++) {
        int bit = 1 << i;
        for (int msk = 0; msk < bit; msk++) {
            Xor[bit | msk] = a[i] ^ Xor[msk];
        }
    }
    return Xor;
}

// 预处理，判断子串是否回文    O(n^2)
auto isPalindrome(const string& s) {
    int n = s.size();
    vector isPal(n, vector<int>(n));
    for (int l = n - 1; l >= 0; l--) {
        for (int r = l; r < n; r++) {
            isPal[l][r] = (s[l] == s[r] && (r - l < 3 || isPal[l + 1][r - 1]));
        }
    }
    return isPal;
}


// 预处理右端点为 i 的回文子串的数量
auto calc(const string& s) {
    vector<int> sum(n);
    for (int i = 0; i < 2 * n - 1; i++) {
        for (int l = i / 2, r = (i + 1) / 2; l >= 0 && r < n && s[l] == s[r]; l--, r++) {
            sum[r]++;
        }
    }
    return sum;
}



// 前缀和+哈希表 枚举
//
// Problems:
// https://codeforces.com/contest/519/problem/D    1800（远古题，难度分偏高）
