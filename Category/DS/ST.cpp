class ST { // 用于解决可重复贡献问题，需要空间较大
private:
    int n, len;
    function<int(int, int)> Op; // max, min, gcd, 按位 &, 按位 | -> [](int a, int b) { return max(a, b); }
    vector<vector<int>> st;

public:
    explicit ST(int n, function<int(int, int)> Op) : n(n), len(__lg(n)), Op(Op), st(n, vector<int>(len + 1)) {}

    void init(vector<int>& vec) {
        for (int i = 0; i < n; ++i) {
            st[i][0] = vec[i];
        }
        for (int i = 1; i <= len; ++i) {
            for (int j = 0; j + (1 << i) - 1 < n; ++j) {
                st[j][i] = Op(st[j][i - 1], st[j + (1 << (i - 1))][i - 1]);
            }
        }
    }

    int query(int l, int r) { // [l, r]
        int s = __lg(r - l + 1);
        return Op(st[l][s], st[r - (1 << s) + 1][s]);
    }
};


// 二维 ST 表

// 初始化 log 表
vector<vector<int>> log_;
void initlog_(int n) {
    log_.resize(n + 1, vector<int>(n + 1, 0));
    for (int i = 2; i <= n; ++i) {
        for (int j = 2; j <= n; ++j) {
            log_[i][j] = log_[i / 2][j] + 1;
        }
    }
}

// 初始化ST表
vector<vector<vector<vector<int>>>> st;

// 初始化ST表
void initST(const vector<vector<int>>& arr, int n) {
    st.resize(n, vector<vector<vector<int>>>(n, vector<vector<int>>(log_[n][n] + 1, vector<int>(log_[n][n] + 1))));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            st[i][j][0][0] = arr[i][j];
        }
    }
    for (int k = 1; (1 << k) <= n; ++k) {
        for (int i = 0; i + (1 << k) - 1 < n; ++i) {
            for (int j = 0; j + (1 << k) - 1 < n; ++j) {
                st[i][j][k][0] = min(st[i][j][k-1][0], st[i + (1 << (k-1))][j][k-1][0]);
                st[i][j][0][k] = min(st[i][j][0][k-1], st[i][j + (1 << (k-1))][0][k-1]);
            }
        }
    }
    for (int k = 1; (1 << k) <= n; ++k) {
        for (int l = 1; (1 << l) <= n; ++l) {
            for (int i = 0; i + (1 << k) - 1 < n; ++i) {
                for (int j = 0; j + (1 << l) - 1 < n; ++j) {
                    int min1 = min(st[i][j][k-1][l], st[i + (1 << (k-1))][j][k-1][l]);
                    int min2 = min(st[i][j][k][l-1], st[i][j + (1 << (l-1))][k][l-1]);
                    st[i][j][k][l] = min(min1, min2);
                }
            }
        }
    }
}

// 查询矩形区域的最小值
int query(int x1, int y1, int x2, int y2) {
    int k = log_[x2 - x1 + 1][y2 - y1 + 1];
    int l = log_[y2 - y1 + 1][x2 - x1 + 1];
    int min1 = min(st[x1][y1][k][l], st[x2 - (1 << k) + 1][y1][k][l]);
    int min2 = min(st[x1][y2 - (1 << l) + 1][k][l], st[x2 - (1 << k) + 1][y2 - (1 << l) + 1][k][l]);
    return min(min1, min2);
}
