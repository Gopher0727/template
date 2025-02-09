//* 矩阵的遍历

// 对角线遍历  （对角线从左上到右下）
auto diagonalTraversal(const vector<vector<int>>& g) {
    int n = g.size(), m = g[0].size();

    // 右上角 k = 1，左下角 k = n+m-1
    vector<vector<int>> ans;
    for (int k = 1; k < n + m; ++k) {
        int mx_j = min(n - k, 0) + m - 1;
        int mn_j = max(m - k, 0);

        // mn_j > 0 右上角

        vector<int> a;
        for (int j = mn_j; j <= mx_j; ++j) {
            a.push_back(g[k + j - m][j]);
        }
        ans.push_back(a);
    }
    return ans;
}

// 【59.螺旋矩阵】(https://leetcode.cn/problems/spiral-matrix-ii/)
// 蛇形遍历
auto snakeTraversal(const vector<vector<int>>& g) {
    int n = g.size(), m = g[0].size();
    vector vis(n, vector<int>(m));
    for (int i = 0, j = 0, k = 0, step = 0; step < n * m; ++step) {
        // do something
        cout << g[i][j] << " ";

        vis[i][j] = 1;
        if (int x = i + dx[k], y = j + dy[k]; x < 0 || x >= n || y < 0 || y >= m || vis[x][y]) {
            k = (k + 1) % 4;
        }
        i += dx[k], j += dy[k];
    }
}

// 围道遍历
auto enclosureTraversal(const vector<vector<int>>& g) {
    int n = g.size(), m = g[0].size();
    vector<vector<int>> ans;
    for (int top = 0, bottom = n - 1, left = 0, right = m - 1; top <= bottom && left <= right; ++top, --bottom, ++left, --right) {
        vector<int> layer;
        for (int j = left; j <= right; ++j) {
            layer.push_back(g[top][j]);
        }
        if (top < bottom) {
            for (int i = top + 1; i <= bottom; ++i) {
                layer.push_back(g[i][right]);
            }
            for (int j = right - 1; j >= left; --j) {
                layer.push_back(g[bottom][j]);
            }
            if (left < right) {
                for (int i = bottom - 1; i > top; --i) {
                    layer.push_back(g[i][left]);
                }
            }
        }
        ans.push_back(layer);
    }
    return ans;
}


//* 求行列式
auto determinant(vector<vector<int>> a) {
    int n = a.size();
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (a[i][i]) {
                int div = a[j][i] / a[i][i];
                for (int k = i; k < n; ++k) {
                    a[j][k] = (a[j][k] - a[i][k] * 1ll * div % MOD + MOD) % MOD;
                }
                swap(a[i], a[j]);
                sign *= -1;
            }
            swap(a[i], a[j]);
            sign *= -1;
        }
    }

    int res = 1;
    for (int i = 0; i < n; ++i) {
        res = res * 1ll * a[i][i] % MOD;
    }
    res = (res * sign + MOD) % MOD;
    return res;
}


//* 矩阵的逆
auto inversion(const vector<vector<int>>& A) {
    int n = A.size(), m = n * 2;

    vector<vector<int>> a(A);
    for (int i = 0; i < n; ++i) {
        a[i].resize(m);
        a[i][n + i] = 1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (a[j][i]) {
                swap(a[i], a[j]);
                break;
            }
        }
        if (a[i][i] == 0) { // 矩阵不是满秩的
            cout << "No Solution\n";
            return vector<vector<int>> {};
        }
        int inv = qpow(a[i][i], MOD - 2, MOD);
        for (int j = i; j < m; ++j) {
            a[i][j] = a[i][j] * 1ll * inv % MOD;
        }
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                int inv = a[j][i];
                for (int k = i; k < m; ++k) {
                    a[j][k] = (a[j][k] - inv * 1ll * a[i][k] % MOD + MOD) % MOD;
                }
            }
        }
    }

    vector ans(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ans[i][j] = a[i][j + n];
        }
    }
    return ans;
}


//* 高斯消元 Gauss-Jordan elimination O(n^3)
// 列主元消去法
// 求解 Ax=B，A 为方阵，返回解（无解或有无穷多组解）
auto gaussJordanElimination(vector<vector<int>>& A, vector<int>& B) {
    constexpr double eps = 1e-8;

    int n = A.size();
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
