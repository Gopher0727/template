template <class Info>
class SegTree {
private:
    int n;
    vector<Info> tree;

private:
    void pull(int o) { tree[o] = tree[o << 1] + tree[o << 1 | 1]; }

public:
    SegTree() = delete;
    SegTree(const vector<Info>& init) : n(init.size() - 1), tree(4 << __lg(n)) {
        auto build = [&](auto&& self, int o, int l, int r) {
            if (l == r) {
                tree[o] = init[l];
                return;
            }
            int m = l + (r - l) / 2;
            self(self, o << 1, l, m);
            self(self, o << 1 | 1, m + 1, r);
            pull(o);
        };
        build(build, 1, 1, n);
    }

public:
    void modify(int id, const Info& info, int o, int l, int r) {
        if (l == r) {
            tree[o] = info;
            return;
        }
        int m = l + (r - l) / 2;
        if (id <= m) {
            modify(id, info, o << 1, l, m);
        } else {
            modify(id, info, o << 1 | 1, m + 1, r);
        }
        pull(o);
    }
    void modify(int L, int R) { modify(L, R, 1, 1, n); }

    Info query(int L, int R, int o, int l, int r) {
        if (R < l || r < L) {
            return Info();
        }
        if (L <= l && r <= R) {
            return tree[o];
        }
        int m = l + (r - l) / 2;
        return query(L, R, o << 1, l, m) + query(L, R, o << 1 | 1, m + 1, r);
    }
    Info query(int L, int R) { return query(L, R, 1, 1, n); }
};

struct Info {
    ll sum = 0, mx = 0;
};
Info operator+(const Info& a, const Info& b) {
    return Info(a.sum + b.sum, max(a.mx, b.mx));
}
