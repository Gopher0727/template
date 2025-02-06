auto snakeWay(const std::vector<std::string>& g) {
    int n = g.size(), m = g[0].size();
    for (int r = 0; r * 2 <= min(n, m) - 1; ++r) {
        std::string t;
        for (int i = r + 1; i < m - r; ++i) {
            t.push_back(g[r][i]);
        }
        for (int i = r + 1; i < n - r; ++i) {
            t.push_back(g[i][m - 1 - r]);
        }
        for (int i = m - 2 - r; i >= r; --i) {
            t.push_back(g[n - 1 - r][i]);
        }
        for (int i = n - 2 - r; i >= r; --i) {
            t.push_back(g[i][r]);
        }
        std::cout << t << "\n";
    }
}



// 【59.螺旋矩阵】(https://leetcode.cn/problems/spiral-matrix-ii/)

static constexpr int dx[] = {0, 1, 0, -1};
static constexpr int dy[] = {1, 0, -1, 0};

class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector ans(n, vector<int>(n));
        int i = 0, j = 0, k = 0;
        for (int val = 1; val <= n * n; val++) { // 要填入的数
            ans[i][j] = val;
            int x = i + dx[k], y = j + dy[k];
            if (x < 0 || x >= n || y < 0 || y >= n || ans[x][y]) {
                k = (k + 1) % 4;
            }
            i += dx[k], j += dy[k];
        }
        return ans;
    }
};
