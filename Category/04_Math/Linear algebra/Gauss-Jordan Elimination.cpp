// 高斯消元 Gaussian elimination O(n^3)   列主元消去法
// 求解 Ax=B，A 为方阵，返回解（无解或有无穷多组解）
pair<vector<double>, bool> gaussJordanElimination(vector<vector<int>>& A, vector<int>& B) {
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
