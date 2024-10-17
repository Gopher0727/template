/*
    0-1 BFS 本质是对 Dijkstra 算法的优化。
    因为边权只有 0 和 1，我们可以把最小堆换成双端队列，遇到 0 边权就加入队首，遇到 1 边权就加入队尾，
    这样可以保证队首总是最小的，就不需要最小堆了。
*/
static constexpr int inf = INT_MAX / 2;

void solve() {
    int n = grid.size(), m = grid[0].size();

    vector dis(n, vector<int>(m, inf));
    dis[0][0] = grid[0][0];

    deque<pair<int, int>> q;
    q.emplace_front(0, 0);

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop_front();

        for (auto& [dx, dy] : dir) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                int g = grid[nx][ny];
                if (dis[x][y] + g < dis[nx][ny]) {
                    dis[nx][ny] = dis[x][y] + g;
                    g == 0 ? q.emplace_front(nx, ny) : q.emplace_back(nx, ny);
                }
            }
        }
    }
}
