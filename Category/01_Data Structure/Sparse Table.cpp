template <typename T>
class SparseTable { // 用于解决可重复贡献问题，需要空间较大
private:
    int n, len;
    vector<vector<T>> g;
    function<T(T, T)> Op; // max, min, gcd, 按位 &, 按位 | -> [](int a, int b) { return max(a, b); }

public:
    SparseTable(const vector<T>& a, auto&& op) : n(a.size()), len(__lg(n)), g(len + 1, vector<T>(n)), Op(Op) {
        for (int i = 0; i < n; i++) {
            g[0][i] = a[i];
        }
        for (int i = 1; i <= len; i++) {
            for (int j = 0; j + (1 << i) - 1 < n; j++) {
                g[i][j] = Op(g[i - 1][j], g[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    // [l, r]
    T query(int l, int r) {
        int s = __lg(r - l + 1);
        return Op(g[s][l], g[s][r - (1 << s) + 1]);
    }
};


template <typename T>
class SparseTable {
private:
    int n, m, K, L;
    vector<int> logRow, logCol;
    vector<vector<vector<vector<T>>>> g;
    function<T(T, T)> Op;

public:
    SparseTable(const vector<vector<T>>& a, auto&& Op) : n(a.size()), m(a[0].size()), K(__lg(n)), L(__lg(m)), Op(Op) {
        logRow.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            logRow[i] = logRow[i / 2] + 1;
        }

        logCol.assign(m + 1, 0);
        for (int j = 2; j <= m; j++) {
            logCol[j] = logCol[j / 2] + 1;
        }

        g.assign(K + 1, vector(L + 1, vector(n, vector<T>(m))));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                g[0][0][i][j] = a[i][j];
            }
        }
        for (int k = 1; k <= K; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                for (int j = 0; j < m; j++) {
                    g[k][0][i][j] = Op(g[k - 1][0][i][j], g[k - 1][0][i + (1 << (k - 1))][j]);
                }
            }
        }
        for (int l = 1; l <= L; ++l) {
            for (int k = 0; k <= K; k++) {
                for (int i = 0; i + (1 << k) <= n; i++) {
                    for (int j = 0; j + (1 << l) <= m; j++) {
                        g[k][l][i][j] = Op(g[k][l - 1][i][j], g[k][l - 1][i][j + (1 << (l - 1))]);
                    }
                }
            }
        }
    }

    // [x1..x2] x [y1..y2]
    T query(int x1, int y1, int x2, int y2) const {
        int k = logRow[x2 - x1 + 1];
        int l = logCol[y2 - y1 + 1];
        int dx = (x2 - x1 + 1) - (1 << k);
        int dy = (y2 - y1 + 1) - (1 << l);
        return Op(Op(g[k][l][x1][y1], g[k][l][x1 + dx][y1]), Op(g[k][l][x1][y1 + dy], g[k][l][x1 + dx][y1 + dy]));
    }
};
