// 高效静态区间第 K 大查询

using u64 = unsigned long long;
struct Wavelet {
    vector<int> vec, sum;
    vector<u64> bit;
    int t, n;

    int getSum(int i) { return sum[i >> 6] + __builtin_popcountll(bit[i >> 6] & ((1ULL << (i & 63)) - 1)); }

    Wavelet(vector<int> v) : vec(v), n(v.size()) {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());

        int n_ = vec.size();
        t = std::__lg(2 * n_ - 1);
        bit.resize((t * n + 64) >> 6);
        sum.resize(bit.size());

        vector<int> cnt(n_ + 1);
        for (auto& x : v) {
            x = lower_bound(vec.begin(), vec.end(), x) - vec.begin();
            cnt[x + 1]++;
        }
        for (int i = 1; i < n_; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int j = 0; j < t; ++j) {
            for (int i : v) {
                int tmp = i >> (t - 1 - j);
                int pos = (tmp >> 1) << (t - j);
                auto setBit = [&](int i, u64 v) {
                    bit[i >> 6] |= (v << (i & 63));
                };
                setBit(j * n + cnt[pos], tmp & 1);
                cnt[pos]++;
            }
            for (int i : v) {
                cnt[(i >> (t - j)) << (t - j)]--;
            }
        }
        for (int i = 1; i < sum.size(); ++i) {
            sum[i] = sum[i - 1] + __builtin_popcountll(bit[i - 1]);
        }
    }

    int small(int l, int r, int k) {
        r++;
        for (int j = 0, x = 0, y = n, res = 0;; ++j) {
            if (j == t) return vec[res];
            int A = getSum(n * j + x), B = getSum(n * j + l);
            int C = getSum(n * j + r), D = getSum(n * j + y);
            int ab_zeros = r - l - C + B;
            if (ab_zeros > k) {
                res = res << 1;
                y -= D - A;
                l -= B - A;
                r -= C - A;
            } else {
                res = (res << 1) | 1;
                k -= ab_zeros;
                x += y - x - D + A;
                l += y - l - D + B;
                r += y - r - D + C;
            }
        }
    }
    int large(int l, int r, int k) { return small(l, r, r - l - k); }
};
