// 求解 SA：（倍增，基数排序/桶排序）
//    先把以每个位置开始的长度为 1 的子串排序，在此基础上再把长度为 2 的子串排序，以此类推，直到子串的末尾。

struct suffixArray {
    int n;
    // sa —— 后缀数组（sa[i] 表示排序为 i 的后缀编号）
    // 把字符串 str 的所有后缀按字典序排序，sa[i] 表示排名为 i 的后缀的开头下标
    // rk —— 名次数组（rk[i] 表示后缀 i 的排名）
    // rk[i] 表示当前长度下，i 开始的后缀的排名。
    vector<int> sa, rk, lc;

    explicit suffixArray(const string& s) : n(s.size()), sa(n), lc(n - 1), rk(n) {
        iota(sa.begin(), sa.end(), 0);
        // 按所有后缀的第一个字符排序
        sort(sa.begin(), sa.end(), [&](int a, int b) { return s[a] < s[b]; });
        rk[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
        }

        vector<int> cnt(n), tmp;
        tmp.reserve(n);
        for (int k = 1; rk[sa[n - 1]] < n - 1; k <<= 1) {
            // 首先添加长度为 k 的后缀，然后添加其余后缀的下一半
            tmp.clear();
            for (int i = 0; i < k; i++) {
                tmp.push_back(n - k + i);
            }
            for (int i : sa) {
                if (i >= k) {
                    tmp.push_back(i - k);
                }
            }

            // 使用计数排序对 tmp 中的后缀进行排序。
            // 首先计数每个排名的出现次数，然后计算排名的累积计数。
            fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; i++) {
                cnt[rk[i]]++;
            }
            for (int i = 1; i < n; i++) {
                cnt[i] += cnt[i - 1];
            }

            // 根据计数数组 cnt 将 tmp 中的后缀按排名顺序放入 sa。
            for (int i = n - 1; i >= 0; i--) {
                sa[--cnt[rk[tmp[i]]]] = tmp[i];
            }

            // tmp 现在存储旧的排名，重新计算排名
            swap(rk, tmp);
            rk[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                rk[sa[i]] = rk[sa[i - 1]] + (tmp[sa[i - 1]] < tmp[sa[i]] || sa[i - 1] + k == n || tmp[sa[i - 1] + k] < tmp[sa[i] + k]);
            }
        }

        // 最长公共前缀（LCP）
        for (int i = 0, j = 0; i < n; i++) {
            if (rk[i] == 0) {
                j = 0;
            } else {
                j -= j > 0;
                while (i + j < n && sa[rk[i] - 1] + j < n && s[i + j] == s[sa[rk[i] - 1] + j]) {
                    j++;
                }
                lc[rk[i] - 1] = j;
            }
        }
    }
};


void solve() {
    int n = nums.size();
    suffixArray sa(nums);

    static constexpr int K = 21;
    vector st(K, vector<int>(n - 1));
    st[0] = sa.lc;
    for (int j = 0; j < K - 1; j++) {
        for (int i = 0; i + (2 << j) <= n - 1; i++) {
            st[j + 1][i] = min(st[j][i], st[j][i + (1 << j)]);
        }
    }

    auto rmq = [&](int l, int r) {
        int k = __lg(r - l);
        return min(st[k][l], st[k][r - (1 << k)]);
    };

    auto lcp = [&](int i, int j) {
        if (i == j || i == n || j == n) {
            return min(n - i, n - j);
        }
        int a = sa.rk[i];
        int b = sa.rk[j];
        if (a > b) {
            swap(a, b);
        }
        return min({n - i, n - j, rmq(a, b)});
    };

    auto lcs = [&](int i, int j) {
        if (i == j || i == 0 || j == 0) {
            return min(i, j);
        }
        int a = sa.rk[n + n - i];
        int b = sa.rk[n + n - j];
        if (a > b) {
            swap(a, b);
        }
        return min({i, j, rmq(a, b)});
    };
}


/*
1.  rank[i]表示下标i的排名（排名从0开始）。
2.  sa[i]表示第i小的后缀的下标（i从0开始）。
3.  height[i]表示sa[i - 1]与sa[i]的最长公共前缀。
*/
const int maxn = 210000; //maxn应当开到最大字符串长度的两倍，否则(1)处下标访问可能越界。
const int maxlog = 20;
struct Suffix_Array {
    pair<int, int> locate(int l, int r) {
        //返回一个最长的区间[L, R]使得sa中下标从L到R的所有后缀都以s[l, r]为前缀。
        int pos = rank[l], length = r - l + 1;
        int L = 0, R = pos;
        while (L < R) {
            int M = L + (R - L) / 2;
            if (lcp(l, sa[M]) >= length) R = M;
            else L = M + 1;
        }
        int tmp = L;
        L = pos, R = n - 1;
        while (L < R) {
            int M = L + (R - L + 1) / 2;
            if (lcp(l, sa[M]) >= length) L = M;
            else R = M - 1;
        }
        return make_pair(tmp, L);
    }
    pair<int, int> locate(vector<pair<int, int>> ranges) {
        //将ranges中的所有下标区间对应的子串拼接到一起，得到字符串T，
        //返回一个最长的区间[L, R]使得sa中下标从L到R的所有后缀都以T为前缀，
        //无解返回{ 0, -1 }。
        int l = 0, r = n - 1, pos = 0;
        for (auto [x, y] : ranges) {
            int L = l, R = r, len = y - x + 1;
            while (L < R) {
                int M = L + (R - L) / 2;
                int pre = sa[M] + pos < n ? lcp(sa[M] + pos, x) : 0;
                int ch1 = s[sa[M] + pos + pre], ch2 = s[x + pre];
                if (pre >= len || ch1 > ch2) R = M;
                else L = M + 1;
            }
            int left = L;
            L = l, R = r;
            while (L < R) {
                int M = L + (R - L + 1) / 2;
                int pre = sa[M] + pos < n ? lcp(sa[M] + pos, x) : 0;
                int ch1 = s[sa[M] + pos + pre], ch2 = s[x + pre];
                if (pre >= len || ch1 < ch2) L = M;
                else R = M - 1;
            }
            int right = L;
            if (sa[left] + pos > n || lcp(sa[left] + pos, x) < len)
                return make_pair(0, -1);
            else
                l = left, r = right;
            pos += len;
        }
        return make_pair(l, r);
    }
    pair<int, int> go(int x, int y, int h, char c) {
        //设sa数组中区间[x, y]的最长公共前缀至少为h，则返回区间中的后缀i满足s[i + h] == c
        //若要识别s中所有以c开头的后缀，则调用go(0, n - 1, 0, c)
        int L = x, R = y;
        while (L < R) {
            int M = L + (R - L) / 2;
            if (sa[M] + h < n && s[sa[M] + h] >= c) R = M;
            else L = M + 1;
        }
        int tmp = L;
        R = y;
        while (L < R) {
            int M = L + (R - L + 1) / 2;
            if (s[sa[M] + h] == c) L = M;
            else R = M - 1;
        }
        if (sa[L] + h >= n || s[sa[L] + h] != c)
            return { 0, -1 }; //空区间
        return { tmp, L };
    }
    pair<int, int> go(int x, int y, int h, const string &str) {
        //设sa数组中区间[x, y]的最长公共前缀至少为h，则返回区间中的后缀i满足s[i + h + j] == str[j]
        //若要识别s中所有以str开头的后缀，则调用go(0, n - 1, 0, str)
        for (auto c : str) {
            int L = x, R = y;
            while (L < R) {
                int M = L + (R - L) / 2;
                if (sa[M] + h < n && s[sa[M] + h] >= c) R = M;
                else L = M + 1;
            }
            x = L, R = y;
            while (L < R) {
                int M = L + (R - L + 1) / 2;
                if (s[sa[M] + h] == c) L = M;
                else R = M - 1;
            }
            if (sa[L] + h >= n || s[sa[L] + h] != c)
                return { 0, -1 }; //空区间
            y = L;
            h += 1;
        }
        return { x, y };
    }
    pair<int, int> go_rev(int x, int y, char c) {
        //设区间[x, y]表示sa中的一个区间，将[x, y]中的每个后缀都往前走一个字符c，
        //得到一个新的连续区间并返回，注意这个新区间不一定是[x, y]的子区间
        //注意：因为sa[0, n-1]不包含空串，所以go(0, n - 1, 0, c) != go_rev(0, n - 1, c)
        int L = 0, R = n - 1;
        while (L < R) {
            int M = L + (R - L) / 2;
            if (s[sa[M]] < c || s[sa[M]] == c && (sa[M] == n - 1 || rank[sa[M] + 1] < x))
                L = M + 1;
            else
                R = M;
        }
        int tmp = L;
        R = n - 1;
        while (L < R) {
            int M = L + (R - L + 1) / 2;
            if (s[sa[M]] > c || s[sa[M]] == c && rank[sa[M] + 1] > y)
                R = M - 1;
            else
                L = M;
        }
        if (s[sa[L]] != c || rank[sa[L] + 1] < x || rank[sa[L] + 1] > y)
            return { 0, -1 }; //空区间
        return { tmp, L };
    }
};
