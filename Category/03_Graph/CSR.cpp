template <class E>
struct csr {
    vector<int> start; // start 表示每个顶点的第一条出边在 elist 中的位置
    vector<E> elist; // elist 包含了所有边的目标顶点
    explicit csr(int n, const vector<pair<int, E>>& edges) : start(n + 1), elist(edges.size()) {
        for (auto [u, _] : edges) {
            start[u + 1]++;
        }
        for (int i = 1; i <= n; i++) {
            start[i] += start[i - 1];
        }
        auto counter = start;
        for (auto [u, v] : edges) {
            elist[counter[u]++] = v;
        }
    }
};

void print_csr(const csr<int>& g) {
    cout << "start: ";
    for (int i = 0; i < g.start.size(); ++i) {
        cout << g.start[i] << ' ';
    }
    cout << "\nelist: ";
    for (auto& e : g.elist) {
        cout << e << ' ';
    }
    cout << '\n';
}

int main() {
    vector<pair<int, int>> edges = {
        {0, 1},
        {0, 2},
        {1, 4},
        {1, 2},
        {1, 3},
        {3, 4},
        {3, 0},
    };

    csr<int> graph(5, edges);
    print_csr(graph);

    return 0;
}
