// 高斯消元 Gaussian elimination O(n^3)   列主元消去法
// 求解 Ax=B，A 为方阵，返回解（无解或有无穷多组解）
pair<vector<double>, bool> gaussJordanElimination(vector<vector<int>>& A, vector<int>& B) {
    constexpr double eps = 1e-8;

    int n = A.size();

    // 增广矩阵
    vector a(n, vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        copy(A[i].begin(), A[i].end(), a[i].begin());
        a[i][n] = B[i];
    }

    int row = 0;
    for (int col = 0; col < n; ++col) {
        // 把正在处理的未知数系数的绝对值最大的式子换到第 row 行
        int pivot = row;
        for (int i = row; i < n; ++i) {
            if (abs(a[i][col]) > abs(a[pivot][col])) {
                pivot = i;
            }
        }
        if (abs(a[pivot][col]) < eps) { // 这一列全为 0，表明无解或有无穷多解
            continue;
        }
        swap(a[pivot], a[row]);

        // 初等行变换
        for (int j = col + 1; j <= n; ++j) {
            a[row][j] /= a[row][col];
        }
        for (int i = 0; i < n; ++i) {
            if (i != row) {
                for (int j = col + 1; j <= n; ++j) {
                    a[i][j] -= a[i][col] * a[row][j];
                }
            }
        }
        row++;
    }

    if (row < n) {
        for (int i = row; i < n; ++i) {
            if (a[i][n] > eps) {
                return pair(vector<double>(), true); // 无解
            }
        }
        return pair(vector<double>(), true); // 无穷多解
    }

    vector<double> res(n);
    for (int i = 0; i < n; ++i) {
        res[i] = a[i][n];
    }
    return pair(res, false); // 唯一解
}

// 求行列式
int determinant(vector<vector<int>> a) {
    int n = a.size();
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (a[i][i] != 0) {
                int div = a[j][i] / a[i][j];
                for (int k = i; k < n; ++k) {
                    a[j][k] = (a[j][k] - a[i][k] * div % MOD + MOD) % MOD;
                }
                swap(a[i], a[j]);
                sign *= -1;
            }
        }
    }

    int res = 1;
    for (int i = 0; i < n; ++i) {
        res = res * a[i][i] % MOD;
    }
    res = (res * sign + MOD) % MOD;
    return res;
}

// 线性基（异或空间的极大线性无关子集）
// 可以用来解决「子序列异或和」相关问题
class xorBasis {
private:
    vector<int> b;

    int num, or_;
    bool canBeZero; // minXor, kthXor
    vector<int> basis; // initOnce
    vector<int> rightMost;

    bool insert(int v) {
        or_ |= v;
        for (int i = b.size() - 1; i >= 0; --i) {
            if ((v >> i & 1) == 0) {
                continue;
            }
            if (b[i] == 0) {
                b[i] = v;
                num++;
                return true;
            }
            v ^= b[i];
        }
        canBeZero = true; // 没有找到线性基，但说明了可以选择一些数使得异或和为 0
        return false;
    }

    bool insertRightMost(int idx, int v) { // 遇到线性相关的基，保留位置最靠右的
        for (int i = b.size() - 1; i >= 0; --i) {
            if ((v >> i & 1) == 0) {
                continue;
            }
            if (b[i] == 0) {
                b[i] = v;
                rightMost[i] = idx;
                num++;
                return true;
            }
            if (idx >= rightMost[i]) {
                swap(idx, rightMost[i]);
                swap(v, b[i]);
            }
            v ^= b[i];
        }
        canBeZero = true; // 没有找到线性基，但说明了可以选择一些数使得异或和为 0
        return false;
    }

    void initOnce() {
        if (basis.empty()) {
            return;
        }
        vector<int> tmp(b);
        for (int i = 0; i < tmp.size(); ++i) {
            if (tmp[i] == 0) {
                continue;
            }
            for (int j = i - 1; j >= 0; --j) {
                if (tmp[i] >> j & 1) {
                    tmp[i] ^= tmp[j];
                }
            }
            basis[i] = tmp[i];
        }
    }

    void merge(xorBasis& other) {
        for (int i = other.b.size() - 1; i >= 0; --i) {
            int x = other.b[i];
            if (x > 0) {
                insert(x);
            }
        }
    }

public:
    xorBasis() = default;
    xorBasis(vector<int>& a) : b(64), rightMost(b.size()) {
        for (int& v : a) {
            insert(v);
        }
    }

    int maxXor(int val = 0) {
        int res = 0;
        for (int i = b.size() - 1; i >= 0; --i) {
            // (res >> i & 1) == 0 && rightMost[i] >= lowerIndex && res ^ b[i] > res
            if ((res ^ b[i]) > res) {
                res ^= b[i];
            }
        }
        return res;
    }

    int minXor() {
        if (canBeZero) {
            return 0;
        }
        for (int i = 0;; ++i) {
            if (b[i] > 0) {
                return b[i];
            }
        }
    }

    // 从 1 开始，第 k 小值（不允许空）
    int kthXor(int k) {
        initOnce();
        if (canBeZero) {
            k--;
        }
        if (k >= (1 << basis.size())) {
            return -1;
        }

        int res = 0;
        for (int i = 0; i < basis.size(); ++i) {
            if (k >> i & 1) {
                res ^= basis[i];
            }
        }
        return res;
    }

    // v 能否由线性基表出
    bool decompose(int v) {
        for (int i = b.size() - 1; i >= 0; --i) {
            if ((v >> i & 1) == 0) {
                continue;
            }
            if (b[i] == 0) {
                return false;
            }
            v ^= b[i];
        }
        return true;
    }
};
