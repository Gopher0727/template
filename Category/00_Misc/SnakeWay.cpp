std::vector<string> snakeWay(const std::vector<std::string>& g) {
    int n = g.size(), m = g[0].size();
    for (int r = 0; r * 2 <= min(n, m) - 1; ++r) {
        sdt::string t;
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
