// 可持久化线段树：
//
// 作用：
// > 查询任意区间第 k 大值，k 小值，权值和

class PersistentSegTree {
private:
    struct Node {
        int cnt = 0;
        i64 sum = 0;
        shared_ptr<Node> left = nullptr, right = nullptr;
    };
    using NodePtr = shared_ptr<Node>;

    // 离散化后的结果
    int N;
    vector<int> sortedVals;

    // 持久化树版本，roots[0] 为空树，roots[i] 表示前 i 个元素插入后的状态
    vector<NodePtr> roots;

    // 动态开点构造一个空树，区间为 [L, R]
    NodePtr build(int L, int R) {
        if (L > R) {
            return nullptr;
        }
        if (L == R) {
            return make_shared<Node>(0, 0);
        }
        int mid = L + (R - L) / 2;
        return make_shared<Node>(0, 0, build(L, mid), build(mid + 1, R));
    }

    // update 函数：在前一版本树 prev 的基础上，
    // 在区间 [L,R] 内的离散化下标 pos 处增加一个新的元素（次数+1, sum 增加 value）
    NodePtr update(NodePtr prev, int L, int R, int pos, i64 value) {
        if (L > R) {
            return nullptr;
        }
        if (L == R) {
            int cnt = (prev ? prev->cnt : 0) + 1;
            i64 sum = (prev ? prev->sum : 0) + value;
            return make_shared<Node>(cnt, sum);
        }
        int mid = L + (R - L) / 2;
        NodePtr newLeft {}, newRight {};
        if (pos <= mid) {
            newLeft = update(prev ? prev->left : nullptr, L, mid, pos, value);
            newRight = (prev ? prev->right : nullptr);
        } else {
            newLeft = (prev ? prev->left : nullptr);
            newRight = update(prev ? prev->right : nullptr, mid + 1, R, pos, value);
        }
        int cnt = (newLeft ? newLeft->cnt : 0) + (newRight ? newRight->cnt : 0);
        i64 sum = (newLeft ? newLeft->sum : 0) + (newRight ? newRight->sum : 0);
        return make_shared<Node>(cnt, sum, newLeft, newRight);
    }

    // kth 查询函数：在两棵树 u 与 v（它们分别为前缀 r+1 与前缀 l 的树）差分上查询第 k 小元素的离散化下标
    // u 表示区域 [0, r] 的版本，v 表示区域 [0, l-1] 的版本，二者差分正好表示区间 [l, r]
    int kth(NodePtr u, NodePtr v, int L, int R, int k) {
        if (L == R) {
            return L;
        }
        int mid = L + (R - L) / 2;
        int cntLeft = (u->left ? u->left->cnt : 0) - (v->left ? v->left->cnt : 0);
        if (k <= cntLeft) {
            return kth(u->left, v->left, L, mid, k);
        } else {
            return kth(u->right, v->right, mid + 1, R, k - cntLeft);
        }
    }

public:
    // 构造函数直接接收一个数组（原始数据类型为 i64）
    // 内部会对所有数值离散化，建立一个域 sortedVals（数组中所有不同数值按升序排列）
    // 同时建立持久化线段树前缀版本：roots[0] 表示空树，
    // roots[i] 表示将前 i 个元素插入后的树（i 从 1 到 n）
    explicit PersistentSegTree(const vector<int>& arr) {
        int n = arr.size();

        // 离散化：复制一份数组，然后排序去重
        auto vals = arr;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        this->sortedVals = vals;
        this->N = this->sortedVals.size();

        // 建立空树（域为 [0, N-1]，即离散化后的下标区间）
        // 对每个位置依次插入，对应版本编号 1..n
        roots.resize(n + 1);
        roots[0] = build(0, this->N - 1);
        for (int i = 0; i < n; i++) {
            // 将 arr[i] 映射为离散化下标
            int pos = lower_bound(this->sortedVals.begin(), this->sortedVals.end(), arr[i]) - this->sortedVals.begin();
            roots[i + 1] = update(roots[i], 0, this->N - 1, pos, arr[i]);
        }
    }

    // [l, r] 0-indexed 权值之和
    i64 query(int l, int r) {
        // 区间 [l, r] 对应的差分持久化树为：根指针差 = roots[r + 1] - roots[l]
        return roots[r + 1]->sum - roots[l]->sum;
    }

    i64 kth_smallest(int l, int r, int k) {
        // 利用差分树：区间 [l, r] = roots[r+1] - roots[l]
        // kth 查询返回离散化下标，再映射为实际数值
        int idx = kth(roots[r + 1], roots[l], 0, this->N - 1, k);
        return this->sortedVals[idx];
    }

    i64 kth_largest(int l, int r, int k) {
        int total = r - l + 1;
        // k 大 = (total - k + 1) 小
        return kth_smallest(l, r, total - k + 1);
    }
};
