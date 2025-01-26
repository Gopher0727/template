// 小波矩阵树（WaveletMatrix）
//
// https://kopricky.github.io/code/DataStructure_Advanced/wavelet_matrix.html
// https://zhuanlan.zhihu.com/p/590974585

struct WaveletTree {
    using iter = vector<int>::iterator;
    vector<vector<int>> c;
    const int SIGMA;

    WaveletTree(vector<int>& a, int sigma) : c(sigma * 2), SIGMA(sigma) { build(a.begin(), a.end(), 0, SIGMA, 1); }

    void build(iter begin, iter end, int l, int r, int u) {
        if (r - l == 1) {
            return;
        }
        int m = (l + r) / 2;
        c[u].reserve(end - begin + 1);
        c[u].push_back(0);
        for (auto it = begin; it != end; ++it) {
            c[u].push_back(c[u].back() + (*it < m));
        }
        auto p = stable_partition(begin, end, [=](int i) { return i < m; });
        build(begin, p, l, m, 2 * u);
        build(p, end, m, r, 2 * u + 1);
    }

    // occurrences of val in position[0, i)
    int rank(int i, int val) const {
        if (val < 0 or val >= SIGMA) {
            return 0;
        }
        int l = 0, r = SIGMA, u = 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (val < m) {
                i = c[u][i], r = m;
                u = u * 2;
            } else {
                i -= c[u][i], l = m;
                u = u * 2 + 1;
            }
        }
        return i;
    }

    // The kth(>= 1) smallest val in position[i, j)
    int quantile(int i, int j, int k) const {
        assert(k > 0);
        k = min(k, j - i);
        int l = 0, r = SIGMA, u = 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            int ni = c[u][i], nj = c[u][j];
            if (k <= nj - ni) {
                i = ni, j = nj, r = m;
                u = 2 * u;
            } else {
                k -= nj - ni;
                i -= ni, j -= nj, l = m;
                u = 2 * u + 1;
            }
        }
        return l;
    }
};
