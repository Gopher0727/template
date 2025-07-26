// 单调栈 (Monotone stack)
//
// 举例如：返回每个元素两侧严格大于它的元素位置（不存在则为 -1 或 n）
//
// 技巧：事先压入一个边界元素到栈底，这样保证循环时栈一定不会为空，从而简化逻辑
// 一些转换：
//     若区间 [l,r] 的最大值等于 a[r]，则 l 必须 > left[r]
//     若区间 [l,r] 的最大值等于 a[l]，则 r 必须 < right[l]
//     这一结论可以用于思考一些双变量的题目
//
// Link：
// 【图解单调栈】(https://leetcode.cn/problems/next-greater-node-in-linked-list/solutions/2217563/tu-jie-dan-diao-zhan-liang-chong-fang-fa-v9ab/)
//

static constexpr int inf = 1E9;

auto monotoneStack(vector<int>& a) {
    // 求左侧严格小于 a[i] 的最近位置 left[i]，这样 a[i] 就是区间 [left[i]+1, i] 内最小的元素（之一）
    // 如果改成求左侧小于等于，那么 a[i] 就是区间 [left[i]+1, i] 内唯一的最小元素
    // 不存在时 left[i] = -1
    {
        int n = a.size();
        vector<int> left(n), stk {-1};
        for (int i = 0; i < n; i++) {
            while (stk.size() > 1 && a[stk.back()] >= a[i]) {
                stk.pop_back();
            }
            left[i] = stk.back();
            stk.push_back(i);
        }
    }

    // 求右侧严格小于 a[i] 的最近位置 right[i]，这样 a[i] 就是区间 [i, right[i]-1] 内最小的元素（之一）
    // 如果改成求右侧小于等于，那么 a[i] 就是区间 [i, right[i]-1] 内唯一的最小元素
    // 不存在时 right[i] = n
    {
        int n = a.size();
        vector<int> right(n), stk {n};
        for (int i = n - 1; i >= 0; i--) {
            while (stk.size() > 1 && a[stk.back()] >= a[i]) {
                stk.pop_back();
            }
            right[i] = stk.back();
            stk.push_back(i);
        }
    }

    // TIPS: 如果有一侧定义成小于等于，还可以一次遍历求出 left 和 right
    {
        int n = a.size();
        vector<int> left(n); // a[left[i]] < a[i]
        vector<int> right(n, n); // a[right[i]] <= a[i]
        vector<int> stk {-1};
        for (int i = 0; i < n; i++) {
            while (stk.size() > 1 && a[i] <= a[stk.back()]) {
                right[stk.back()] = i;
                stk.pop_back();
            }
            left[i] = stk.back();
            stk.push_back(i);
        }
    }
    // 不需要栈的写法
    {
        int n = a.size();

        // left[i] 为左侧严格小于 a[i] 的最近元素位置（不存在时为 -1）
        vector<int> left(n);
        for (int i = 0; i < n; i++) {
            int j = i - 1;
            while (j >= 0 && a[j] >= a[i]) {
                j = left[j];
            }
            left[i] = j;
        }
        // right[i] 为右侧小于等于 a[i] 的最近元素位置（不存在时为 n）
        vector<int> right(n);
        for (int i = n - 1; i >= 0; i--) {
            int j = i + 1;
            while (j < n && a[j] > a[i]) {
                j = right[j];
            }
            right[i] = j;
        }
    }
    // 合并的写法
    {
        int n = a.size();
        vector<int> left(n), right(n, n);
        for (int i = 0; i < n; i++) {
            int j = i - 1;
            while (j >= 0 && a[j] >= a[i]) {
                right[j] = i;
                j = left[j];
            }
            left[i] = j;
        }
    }

    // 求出 left, right
    // EXTRA: 求所有长为 i 的子区间的最小值的最大值
    vector<int> left, right;
    {
        int n = a.size();
        vector<int> ans(n + 1, -inf);
        for (int i = 0; i < n; i++) {
            int sz = right[i] - left[i] - 1;
            ans[sz] = max(ans[sz], a[i]);
        }
        for (int i = n - 1; i > 0; i--) {
            ans[i] = max(ans[i], ans[i + 1]);
        }
        // ans[1:]
    }
}

// 求右边第二个更大元素的下标
// ! 注意不是下一个更大元素的下一个更大元素
// 如果没有，那么结果为 n
auto find2Greater(vector<int>& a) {
    int n = a.size();
    vector<int> right(n, n), right2(n, n), s, t;
    for (int i = 0; i < n; i++) {
        while (!t.empty() && a[t.back()] < a[i]) {
            right2[t.back()] = i;
            t.pop_back();
        }
        while (!s.empty() && a[s.back()] < a[i]) {
            right[s.back()] = i;
            t.push_back(s.back());
            s.pop_back();
        }
        s.push_back(i);
    }
    return pair(right, right2);
}

// 最大全 1 矩形
auto maximalRectangleArea(vector<vector<int>>& mat) {
    const int target = 1;

    int n = mat.size(), m = mat[0].size();
    // heights[i][j] 表示从 (i,j) 往上看的高度（连续 1 的长度），mat[i][j] = 0 时为 0
    vector heights(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mat[i][j] == target) {
                if (i == 0) {
                    heights[i][j] = 1;
                } else {
                    heights[i][j] = heights[i - 1][j] + 1;
                }
            }
        }
    }

    int ans = 0;
    for (auto& hs : heights) {
        vector<int> left(m);
        vector<array<int, 2>> s; // h, i
        s.push_back({-1, -1});
        for (int j = 0; j < m; j++) {
            while (true) {
                if (s.back()[0] < hs[j]) {
                    left[j] = s.back()[1];
                    break;
                }
                s.pop_back();
            }
            s.push_back({hs[j], j});
        }

        vector<int> right(m);
        vector<array<int, 2>> t;
        t.push_back({-1, m});
        for (int j = m - 1; j >= 0; j--) {
            while (true) {
                if (t.back()[0] < hs[j]) {
                    right[j] = t.back()[1];
                    break;
                }
                t.pop_back();
            }
            t.push_back({hs[j], j});
        }

        for (int j = 0; j < m; j++) {
            ans = max(ans, (right[j] - left[j] - 1) * hs[j]);
        }
    }
    return ans;
}

// 全 1 矩形个数
auto cntSubmat(vector<vector<int>>& mat) {
    int m = mat[0].size();
    int ans = 0;
    vector<int> heights(m);
    for (auto& row : mat) {
        vector<int> sum(m);
        vector<array<int, 2>> stk;
        stk.push_back({-1, -1});
        for (int j = 0; j < m; j++) {
            int& h = heights[j];
            if (row[j] == 0) {
                h = 0;
            } else {
                h++;
            }
            while (true) {
                if (auto [th, pre] = stk.back(); th < h) {
                    if (pre < 0) {
                        sum[j] = (j + 1) * h;
                    } else {
                        sum[j] = sum[pre] + (j - pre) * h;
                    }
                    ans += sum[j];
                    break;
                }
                stk.pop_back();
            }
            stk.push_back({h, j});
        }
    }
    return ans;
}

// 字典序最小的无重复字符的子序列，包含原串所有字符
auto removeDuplicateLetters(string s) {
    vector<int> left(26);
    for (char ch : s) {
        left[ch - 'a']++;
    }

    string t;
    vector<int> in(26);
    for (char ch : s) {
        left[ch - 'a']--;
        if (in[ch - 'a']) {
            continue;
        }
        while (!t.empty() && ch < t.back() && left[t.back() - 'a']) {
            // t.back() > ch，且 t.back() 后面还有，那么可以重新加进来
            in[t.back() - 'a'] = 0;
            t.pop_back();
        }
        t.push_back(ch);
        in[ch - 'a'] = 1;
    }
    return t;
}

// 求 a 的最长的子数组，其元素和大于 lowerSum
// 返回任意一个符合要求的子数组的左右端点（闭区间）
// 如果不存在，返回 [-1,-1]
auto longestSubarrayWithLowerSum(vector<int>& a, int lowerSum) {
    int n = a.size();
    vector<int> stk {0};

    vector<int> pre(n + 1);
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i] + a[i];
        if (pre[i + 1] < pre[stk.back()]) {
            stk.push_back(i + 1);
        }
    }

    int l = -1, r = 0;
    for (int i = n; i > 0; i--) {
        while (!stk.empty() && pre[i] - pre[stk.back()] > lowerSum) {
            int j = stk.back();
            stk.pop_back();
            if (l < 0 || i - j < r - l) {
                l = j;
                r = i;
            }
        }
    }
    return pair(l, r - 1); // 闭区间
}

// 静态区间离线查询最值
auto rangeMaxWithSt(vector<int>& a, vector<pair<int, int>>& queries) {
    int n = a.size(), m = queries.size();

    vector<vector<array<int, 2>>> qs(n);
    for (int i = 0; i < m; i++) {
        auto [l, r] = queries[i];
        qs[r].push_back({l, i});
    }

    vector<int> maxAns(m), minAns(m), maxSt, minSt;
    for (int r = 0; r < n; r++) {
        while (!maxSt.empty() && a[r] >= a[maxSt.back()]) {
            maxSt.pop_back();
        }
        maxSt.push_back(r);

        while (!minSt.empty() && a[r] <= a[minSt.back()]) {
            minSt.pop_back();
        }
        minSt.push_back(r);

        // 处理所有以当前元素为右端点的查询
        for (auto& [l, i] : qs[r]) {
            if (auto iter = ranges::lower_bound(maxSt, l); iter != maxSt.end()) {
                maxAns[i] = a[*iter];
            }
            if (auto iter = ranges::lower_bound(minSt, l); iter != minSt.end()) {
                minAns[i] = a[*iter];
            }
        }
    }
    return pair(maxAns, minAns);
}

