// 小波矩阵树（WaveletMatrix）
//
// 高效查询静态区间第 K 大、第 K 小、区间某个数的频次、区间在某个值域中的最值
//
// https://kopricky.github.io/code/DataStructure_Advanced/wavelet_matrix.html
// https://zhuanlan.zhihu.com/p/590974585

class WaveletMatrix {
    using u32 = unsigned;
    using u64 = unsigned long long;
    struct BitRank {
        // block 管理一行一行的 bit
        std::vector<u64> block;
        std::vector<u32> count;
        BitRank() {}
        // 位向量长度
        void resize(u32 num) {
            block.resize(((num + 1) >> 6) + 1, 0);
            count.resize(block.size(), 0);
        }
        // 设置i位bit
        void set(u32 i, u64 val) { block[i >> 6] |= (val << (i & 63)); }
        void build() {
            for (u32 i = 1; i < block.size(); i++) {
                count[i] = count[i - 1] + __builtin_popcountll(block[i - 1]);
            }
        }
        // [0, i) 1 的个数
        u32 rank1(u32 i) { return count[i >> 6] + __builtin_popcountll(block[i >> 6] & ((1ULL << (i & 63)) - 1ULL)); }
        // [i, j) 1 的个数
        u32 rank1(u32 i, u32 j) { return rank1(j) - rank1(i); }
        // [0, i) 0 的个数
        u32 rank0(u32 i) { return i - rank1(i); }
        // [i, j) 0 的个数
        u32 rank0(u32 i, u32 j) { return rank0(j) - rank0(i); }
    };

private:
    u32 height;
    std::vector<BitRank> B;
    std::vector<int> pos;

private:
    void init(std::vector<int> vec, u32 sigma) {
        height = (sigma == 1) ? 1 : (64 - __builtin_clzll(sigma - 1));
        B.resize(height), pos.resize(height);
        for (u32 i = 0; i < height; ++i) {
            B[i].resize(vec.size());
            for (u32 j = 0; j < vec.size(); ++j) {
                B[i].set(j, get(vec[j], height - i - 1));
            }
            B[i].build();
            auto it = stable_partition(vec.begin(), vec.end(), [&](int c) { return !get(c, height - i - 1); });
            pos[i] = it - vec.begin();
        }
    }

    int get(int val, int i) { return val >> i & 1; }

    int rangefreq(int i, int j, int a, int b, int l, int r, int x) {
        if (i == j || r <= a || b <= l) {
            return 0;
        }
        int mid = (l + r) >> 1;
        if (a <= l && r <= b) {
            return j - i;
        }
        return rangefreq(B[x].rank0(i), B[x].rank0(j), a, b, l, mid, x + 1) + rangefreq(pos[x] + B[x].rank1(i), pos[x] + B[x].rank1(j), a, b, mid, r, x + 1);
    }

    int rangeMin(int i, int j, int a, int b, int l, int r, int x, int val) {
        if (i == j || r <= a || b <= l) {
            return -1;
        }
        if (r - l == 1) {
            return val;
        }
        int mid = (l + r) >> 1;
        int left = rangeMin(B[x].rank0(i), B[x].rank0(j), a, b, l, mid, x + 1, val);
        if (left >= 0) {
            return left;
        }
        return rangeMin(pos[x] + B[x].rank1(i), pos[x] + B[x].rank1(j), a, b, mid, r, x + 1, val + (1 << (height - x - 1)));
    }

    int rangeMax(int i, int j, int a, int b, int l, int r, int x, int val) {
        if (i == j || r <= a || b <= l) {
            return -1;
        }
        if (r - l == 1) {
            return val;
        }
        int mid = (l + r) >> 1;
        int right = rangeMax(pos[x] + B[x].rank1(i), pos[x] + B[x].rank1(j), a, b, mid, r, x + 1, val + (1 << (height - x - 1)));
        if (right >= 0) {
            return right;
        }
        return rangeMax(B[x].rank0(i), B[x].rank0(j), a, b, l, mid, x + 1, val);
    }

public:
    WaveletMatrix() {}
    WaveletMatrix(const std::vector<int>& vec) : WaveletMatrix(vec, *std::max_element(vec.begin(), vec.end()) + 1) {}
    // sigma: 字母表大小(字符串的话)，数字序列的话是数的种类
    WaveletMatrix(const std::vector<int>& vec, u32 sigma) { init(vec, sigma); }

    // [0,i) 中 val 出现的频次
    int rank(int i, int val) {
        int p = 0;
        for (u32 j = 0; j < height; ++j) {
            if (get(val, height - j - 1)) {
                p = pos[j] + B[j].rank1(p);
                i = pos[j] + B[j].rank1(i);
            } else {
                p = B[j].rank0(p);
                i = B[j].rank0(i);
            }
        }
        return i - p;
    }
    // [l,r) 中 val 出现的频次
    int rank(int l, int r, int val) { return rank(r, val) - rank(l, val); }

    // [l,r) 中 k (>= 0) 小
    int kthMin(int l, int r, int k) {
        int res = 0;
        for (u32 i = 0; i < height; ++i) {
            int j = B[i].rank0(l, r);
            if (j > k) {
                l = B[i].rank0(l);
                r = B[i].rank0(r);
            } else {
                l = pos[i] + B[i].rank1(l);
                r = pos[i] + B[i].rank1(r);
                k -= j;
                res |= (1 << (height - i - 1));
            }
        }
        return res;
    }

    // [l,r) 中 k (>= 0) 大
    int kthMax(int l, int r, int k) { return kthMin(l, r, r - l - k - 1); }

    // [l,r) 在 [a,b) 值域的数字个数
    int rangefreq(int l, int r, int a, int b) { return rangefreq(l, r, a, b, 0, 1 << height, 0); }

    // [l,r) 在 [a,b) 值域内存在的最小值是什么，不存在返回 -1
    int rangeMin(int l, int r, int a, int b) { return rangeMin(l, r, a, b, 0, 1 << height, 0, 0); }

    // [l,r) 在 [a,b) 值域内存在的最大值是什么，不存在返回 -1
    int rangeMax(int l, int r, int a, int b) { return rangeMax(l, r, a, b, 0, 1 << height, 0, 0); }
};
