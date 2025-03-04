// v 需要增加多少才能被 r 整除
//
int d1 = (r - (v % r)) % r;
int d2 = (v + r - 1) / r * r - v;


// 给定区间集合，返回最多能选多少个无重叠的区间（端点重合不记作重叠）
auto intervalsCount(vector<pair<int, int>>& intervals) {
    ranges::sort(intervals, {}, &pair<int, int>::second);
    int ans = 0, pre = numeric_limits<int>::min();
    for (auto [l, r] : intervals) {
        if (l >= pre) {
            ans++;
            pre = r;
        }
    }
    return ans;
}

// 给定区间集合，返回合并后的区间
auto intervalsMerge(vector<pair<int, int>>& intervals) {
    vector<pair<int, int>> ans;
    for (auto& p : intervals) {
        if (!ans.empty() && p.first <= ans.back().second) {
            ans.back().second = max(ans.back().second, p.second);
        } else {
            ans.emplace_back(p);
        }
    }
    return ans;
}


// 预处理，求长为 k 的子序列的按位或    O(n * k * 2^k)
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


// 预处理，求集合各子集 LCM, OR    O(2^n)
//
auto findLCMs(const vector<int>& a) {
    int n = a.size();
    vector<i64> lcms(1 << n);
    lcms[0] = 1;
    for (int i = 0; i < n; ++i) {
        int bit = 1 << i;
        for (int mask = 0; mask < bit; ++mask) {
            lcms[bit | mask] = lcm(a[i], lcms[mask]);
        }
    }
    lcms[0] = 0;
    return lcms;
}

auto findORs(const vector<int>& a) {
    int n = a.size();
    vector<i64> ors(1 << n);
    for (int i = 0; i < n; ++i) {
        int bit = 1 << i;
        for (int mask = 0; mask < bit; ++mask) {
            ors[bit | mask] = a[i] | ors[mask];
        }
    }
    return ors;
}


// 预处理，判断子串是否回文    O(n^2)
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


// 预处理右端点为 i 的回文子串的数量
//
auto calc(const string& s) {
    vector<int> sum(n);
    for (int i = 0; i < 2 * n - 1; ++i) {
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


// ----------------------------------------------------------------------------------------------------------------------------------
// 【3171.找到按位或最接近 K 的子数组】(https://leetcode.cn/problems/find-subarray-with-bitwise-or-closest-to-k/)

// LogTrick：
// or: 【3171.找到按位或最接近 K 的子数组】(https://leetcode.cn/problems/find-subarray-with-bitwise-or-closest-to-k/)
// and: https://codeforces.com/contest/1878/problem/E    1400（离线 LogTrick + 二分，其他做法也有很多）

// 考虑记录计算结果，nums[j] 保存 nums[j] 到 nums[i] 的 or 值
// 在记录过程中，当 x = nums[i] 的二进制数对应的集合 Ai 是 nums[j] 对应的集合 Aj 的子集时，即 x | nums[j] == nums[j]，一定也是后续
// 集合的子集，继续更新没有意义了
class Solution {
public:
    int minimumDifference(vector<int>& nums, int k) {
        int ans = INT_MAX;
        for (int i = 0; i < nums.size(); ++i) {
            int x = nums[i];
            ans = min(ans, abs(x - k));
            for (int j = i - 1; j >= 0 && (x | nums[j]) != nums[j]; --j) {
                nums[j] |= x;
                ans = min(ans, abs(nums[j] - k));
            }
        }
        return ans;
    }
};


// 传统的滑动窗口需要考虑维护的运算具有逆运算
// 右端点的运算值更新可以用一个变量维护，当左端点 left 离开窗口时，需要知道 nums[left+1] ... nums[right] 的运算值，
// 可以在传统滑窗的基础上通过 '记录计算结果' 的方式，用栈结构维护
// 当栈空时，重构栈空间（更新栈底），维护右端点的变量清空
class Solution {
public:
    int minimumDifference(vector<int>& nums, int k) {
        int ans = INT_MAX, left = 0, bottom = 0, right_or = 0;
        for (int right = 0; right < nums.size(); ++right) {
            right_or |= nums[right];
            while (left <= right && (nums[left] | right_or) > k) {
                ans = min(ans, (nums[left] | right_or) - k);
                left++;
                if (bottom < left) {
                    for (int i = right - 1; i >= left; --i) {
                        nums[i] |= nums[i + 1];
                    }
                    bottom = right;
                    right_or = 0;
                }
            }
            if (left <= right) {
                ans = min(ans, k - (nums[left] | right_or));
            }
        }
        return ans;
    }
};


// 双栈
class Solution {
public:
    int minimumDifference(vector<int>& nums, int k) {
        int n = nums.size();

        vector<int> ls, rs;
        int l = 0, r = 0;
        auto Push = [&]() {
            if (rs.empty()) {
                rs.push_back(nums[r]);
            } else {
                rs.push_back(rs.back() | nums[r]);
            }
            r++;
        };
        auto Pop = [&]() {
            l++;
            if (!ls.empty()) {
                ls.pop_back();
                return;
            }
            rs.clear();
            if (l < r) {
                ls.resize(r - l);
                ls[0] = nums[r - 1];
                for (int i = 1; i < r - l; ++i) {
                    ls[i] = ls[i - 1] | nums[r - i - 1];
                }
            }
        };
        auto Get = [&]() {
            return (ls.empty() ? 0 : ls.back()) | (rs.empty() ? 0 : rs.back());
        };

        int res = INT_MAX;
        while (r < n) {
            Push();
            res = min(res, abs(k - Get()));
            while (r - l > 1 && Get() > k) {
                Pop();
                res = min(res, abs(k - Get()));
            }
        }
        return res;
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------
