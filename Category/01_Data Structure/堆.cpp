// 对顶堆查询前 K 大
template <typename T = ll>
class DualHeap {
    priority_queue<T, vector<T>, greater<>> p;
    priority_queue<T> q;
    T cur;

public:
    DualHeap() : cur(0) {}

    void add(const T& x) {
        if (!q.size() || x >= q.top()) {
            p.push(x);
            cur += x;
        } else {
            q.push(x);
        }
    }

    int size() const { return p.size() + q.size(); }

    T sum(int k) {
        while (p.size() < k) {
            if (q.empty()) {
                break;
            }
            cur += q.top();
            p.push(q.top());
            q.pop();
        }
        while (p.size() > k) {
            if (p.empty()) {
                break;
            }
            cur -= p.top();
            q.push(p.top());
            p.pop();
        }
        return cur;
    }
};


// 对顶堆查询中位数
template <typename T = ll>
class DualHeap {
private:
    priority_queue<T, vector<T>, greater<>> p;
    priority_queue<T> q;

    void adjust() {
        while (p.size() + 1 < q.size()) {
            p.push(q.top());
            q.pop();
        }
        while (p.size() > q.size()) {
            q.push(p.top());
            p.pop();
        }
    }

public:
    DualHeap() {}

    void add(T num) {
        if (q.empty() || num <= q.top()) {
            q.push(num);
        } else {
            p.push(num);
        }
        adjust(); // 调整堆的大小
    }

    void remove(T num) {
        vector<T> tmp;
        if (!q.empty() && num <= q.top()) { // 从大根堆中移除
            while (!q.empty()) {
                if (q.top() == num) {
                    q.pop();
                    break;
                }
                tmp.push_back(q.top());
                q.pop();
            }
            for (auto& x : tmp) {
                q.push(x);
            }
        } else { // 从小根堆中移除
            while (!p.empty()) {
                if (p.top() == num) {
                    p.pop();
                    break;
                }
                tmp.push_back(p.top());
                p.pop();
            }
            for (auto& x : tmp) {
                p.push(x);
            }
        }
        adjust();
    }

    template <typename U = double>
    U queryMedian() {
        if (p.size() < q.size()) {
            return q.top();
        } else {
            return (p.top() + q.top()) / 2.0L;
        }
    }
};


// 定长滑窗中位数，with 懒删除
class DualHeap_slidingWindow {
private:
    priority_queue<ll, vector<ll>, greater<ll>> MinHeap;
    priority_queue<ll> MaxHeap;
    map<ll, int> delayed; // 记录「延迟删除」的元素
    int MinSize = 0, MaxSize = 0; // 包含的元素个数（需要扣除被「延迟删除」的元素）
    int k = 0; // 窗口大小

private:
    template <typename Heap>
    void prune(Heap& heap) {
        while (!heap.empty()) {
            int num = heap.top();
            if (delayed.count(num)) {
                --delayed[num];
                if (!delayed[num]) {
                    delayed.erase(num);
                }
                heap.pop();
            } else {
                break;
            }
        }
    }

    void makeBalance() {
        if (MaxSize > MinSize + 1) {
            // MaxHeap 比 MinHeap 元素多 2 个
            MinHeap.push(MaxHeap.top());
            MaxHeap.pop();
            --MaxSize;
            ++MinSize;
            prune(MaxHeap);
        } else if (MaxSize < MinSize) {
            // MinHeap 比 MaxHeap 元素多 1 个
            MaxHeap.push(MinHeap.top());
            MinHeap.pop();
            ++MaxSize;
            --MinSize;
            prune(MinHeap);
        }
    }

    template <typename T>
    T getMedian() {
        return k & 1 ? MaxHeap.top() : ((T) MaxHeap.top() + MinHeap.top()) / 2;
    }

public:
    DualHeap_slidingWindow(int _k) : k(_k), MaxSize(0), MinSize(0) {}

    void add(int num) {
        if (MaxHeap.empty() || num <= MaxHeap.top()) {
            MaxHeap.push(num);
            ++MaxSize;
        } else {
            MinHeap.push(num);
            ++MinSize;
        }
        makeBalance();
    }

    void remove(int num) {
        ++delayed[num];
        if (num <= MaxHeap.top()) {
            --MaxSize;
            if (num == MaxHeap.top()) {
                prune(MaxHeap);
            }
        } else {
            --MinSize;
            if (num == MinHeap.top()) {
                prune(MinHeap);
            }
        }
        makeBalance();
    }

    // 大小为 k 的滑动窗口中位数
    template <typename T = double, typename U = int>
    vector<T> medianSlidingWindow(vector<U>& nums) {
        vector<T> medians;
        for (int i = 0; i < nums.size(); i++) {
            add(nums[i]);
            if (i >= k - 1) {
                medians.push_back(getMedian<T>());
                remove(nums[i - k + 1]);
            }
        }
        return medians;
    }
};


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

    priority_queue<array<int, 2>, vector<array<int, 2>>, greater<>> pq;
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
