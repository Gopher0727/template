// 珂朵莉树（Chtholly Tree / Old Driver Tree）/ 颜色段均摊
//
// 珂朵莉树核心思想是将值相同的一段区间合并成一个节点处理。
// 相比于传统线段树等结构，对于含有区间覆盖操作的问题，珂朵莉树可以更加方便地维护每个被覆盖的值，
// ⚠ 但要求数据是随机的，或者有大量的随机合并操作，这样才能保证维护的区间个数是一个很小的值。
//
// 在若干次随机合并后，区间个数会骤降至一个稳定的范围，这是理解珂朵莉树的关键。

// 维护结点，使得结点所代表的区间左端点单调增加且两两不交
// 最好可以保证所有区间的“并”是一个极大的连续范围

template <typename T>
struct ODT {
    struct Node_t {
        int l, r; // 闭区间
        mutable T v;

        Node_t(int l, int r, T v) : l(l), r(r), v(v) {}
        bool operator<(const Node_t& o) const { return l < o.l; }
    };

    // 初始化时，可以向 ODT 中插入一个极长区间进行占位，后续进行 assign 操作
    set<Node_t> s;

    const int TODO = 0;

    // 核心操作，将原本包含 x 的区间分裂，并返回指向后者的迭代器
    auto split(int x) {
        auto it = s.lower_bound({x, 0, TODO});
        if (it != s.end() && it->l == x) {
            return it;
        }
        auto [l, r, v] = *(--it);
        s.erase(it);
        s.insert({l, x - 1, v});
        return s.insert({x, r, v}).first;
    }

public:
    void insert(int l, int r, T v) { s.insert({l, r, v}); }

    // 对一段区间 [l, r] 赋值
    void assign(int l, int r, T v) {
        auto itR = split(r + 1);
        auto itL = split(l);
        s.erase(itL, itR);

        // s.insert({l, r, v});

        // 合并操作
        auto it = s.insert({l, r, v}).first;
        if (it != s.begin()) {
            if (auto pre = prev(it); pre->r + 1 == it->l && pre->v == it->v) {
                int nL = pre->l, nR = it->r;
                s.erase(pre);
                s.erase(it);
                it = s.insert({nL, nR, v}).first;
            }
        }
        if (auto nxt = next(it); nxt != s.end() && it->r + 1 == nxt->l && nxt->v == it->v) {
            int nL = it->l, nR = nxt->r;
            s.erase(nxt);
            s.erase(it);
            it = s.insert({nL, nR, v}).first;
        }
    }

    // ⚠
    void add(int l, int r, T x) {
        auto itR = split(r + 1);
        auto itL = split(l);

        for (auto it = itL; it != itR; it++) {
            it->v += x;
        }
    }

    // ⚠
    T kth(int l, int r, T x) {
        auto itR = split(r + 1);
        auto itL = split(l);

        vector<pair<i64, int>> vec;
        for (auto it = itL; it != itR; it++) {
            vec.push_back({it->v, it->r - it->l + 1});
        }
        ranges::sort(vec);
        for (auto [v, c] : vec) {
            if (x <= c) {
                return v;
            }
            x -= c;
        }
        return -1;
    }

    // ⚠
    T perform(int l, int r, T x, T y) {
        auto itR = split(r + 1);
        auto itL = split(l);

        // todo
    }
};
