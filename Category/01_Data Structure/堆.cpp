namespace Heap {
    template <typename T>
    using MaxHeap = priority_queue<T>;
    template <typename T>
    using MinHeap = priority_queue<T, vector<T>, greater<>>;
} // namespace Heap
using namespace Heap;


// 构造一个长为 n 的排列 a，对于 0 <= i < n，a[i] 都在 [l[i], r[i]] 范围内，试构造这个排列。
/*
测试样例：
5
3 4
1 4
2 3
1 3
4 5
输出：3 4 2 1 5
*/
void solve() {
    int n;
    cin >> n;

    vector<int> l(n), r(n);
    vector<vector<int>> vec(n);
    for (int i = 0; i < n; ++i) {
        cin >> l[i] >> r[i];
        l[i]--;
        vec[l[i]].push_back(i);
    }

    MinHeap<array<int, 2>> pq;
    vector<int> pos(n);
    for (int x = 0; x < n; ++x) {
        for (int i : vec[x]) {
            pq.push({r[i], i});
        }
        if (pq.empty()) {
            cout << -1 << "\n";
            return;
        }
        auto [r, i] = pq.top();
        pq.pop();
        if (x >= r) {
            cout << -1 << "\n";
            return;
        }
        pos[i] = x;
    }
    for (int i = 0; i < n; ++i) {
        cout << pos[i] + 1 << " \n"[i == n - 1];
    }
}
