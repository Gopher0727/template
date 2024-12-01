struct Tree {
    int n; // 边数
    vector<vector<int>> g;

public:
    Tree(int n) : n(n), g(n + 1) {}

    void addEdge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }


    // 获取子树大小（包括当前节点并以当前节点为根）
    vector<int> subTreeSize(int x) {
        vector<int> _size(n + 1, 1);
        vector<int> vis(n + 1);
        stack<pair<int, int>> stk;
        stk.emplace(x, -1); // 子节点，父节点
        while (!stk.empty()) {
            auto [x, pa] = stk.top();
            if (vis[x] == 0) {
                vis[x] = 1;
                for (int y : g[x]) {
                    if (y != pa && vis[y] == 0) {
                        stk.emplace(y, x);
                    }
                }
            } else { // 当前节点及其子树已经完全访问，处理子树大小
                for (int y : g[x]) {
                    if (y != pa) {
                        _size[x] += _size[y];
                    }
                }
                stk.pop();
            }
        }
        return _size;
    }
};
/*
    获取子树大小（包括当前节点并以当前节点为根）
    递归写法：
*/
// vector<int> subTreeSize(int x) {
//     vector<int> _size(n + 1, 1);
//     auto dfs = [&](auto&& dfs, int x, int pa = -1) -> void {
//         for (int y : g[x]) {
//             if (y == pa) {
//                 continue;
//             }
//             dfs(dfs, y, x);
//             _size[x] += _size[y];
//         }
//     };
//     dfs(dfs, x);
//     return _size;
// }
