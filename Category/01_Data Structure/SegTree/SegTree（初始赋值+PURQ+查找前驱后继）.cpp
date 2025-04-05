static constexpr i64 inf = 9E18;

template <class Info>
class SegTree {
private:
    int n;
    vector<Info> info;

    void pull(int o) { info[o] = info[o << 1] + info[o << 1 | 1]; }

public:
    SegTree() : n(0) {}
    SegTree(int n, Info v = Info {}) { init(vector(n, v)); }
    template <typename T>
    SegTree(const vector<T>& _init) {
        init(_init);
    }

    template <typename T>
    void init(const vector<T>& _init) {
        n = _init.size();
        info.assign(4 << std::__lg(n), Info {});

        auto build = [&](auto&& self, int o, int l, int r) -> void {
            if (l == r) {
                info[o] = _init[l];
                return;
            }
            int m = l + (r - l) / 2;
            self(self, o << 1, l, m);
            self(self, o << 1 | 1, m + 1, r);
            pull(o);
        };
        build(build, 1, 0, n - 1);
    }

public:
    void modify(int id, const Info& _init, int o, int l, int r) {
        if (l == r) {
            info[o] = _init;
            return;
        }
        int m = l + (r - l) / 2;
        if (id <= m) {
            modify(id, _init, o << 1, l, m);
        } else {
            modify(id, _init, o << 1 | 1, m + 1, r);
        }
        pull(o);
    }
    void modify(int id, const Info& _init) { modify(id, _init, 1, 0, n - 1); }

    Info query(int L, int R, int o, int l, int r) {
        if (L <= l && r <= R) {
            return info[o];
        }
        int m = l + (r - l) / 2;
        if (R <= m) {
            return query(L, R, o << 1, l, m);
        }
        if (m < L) {
            return query(L, R, o << 1 | 1, m + 1, r);
        }
        return query(L, R, o << 1, l, m) + query(L, R, o << 1 | 1, m + 1, r);
    }
    Info query(int L, int R) { return query(L, R, 1, 0, n - 1); }

    Info queryAll() { return info[1]; }

    int findFirst(int L, int R, auto&& pred, int o, int l, int r) {
        if (l > R || r < L || !pred(info[o])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        int m = l + (r - l) / 2;
        int idx = findFirst(L, R, pred, o << 1, l, m);
        if (idx == -1) {
            idx = findFirst(L, R, pred, o << 1 | 1, m + 1, r);
        }
        return idx;
    }
    int findFirst(int l, int r, auto&& pred) { return findFirst(l, r, pred, 1, 0, n - 1); }

    int findLast(int L, int R, auto&& pred, int o, int l, int r) {
        if (l > R || r < L || !pred(info[o])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        int m = l + (r - l) / 2;
        int idx = findLast(L, R, pred, o << 1 | 1, m + 1, r);
        if (idx == -1) {
            idx = findLast(L, R, pred, o << 1, l, m);
        }
        return idx;
    }
    int findLast(int l, int r, auto&& pred) { return findLast(l, r, pred, 1, 0, n - 1); }
};

struct Info {
    i64 mx = 0;
};
Info operator+(const Info& a, const Info& b) {
    Info info;
    info.mx = max(a.mx, b.mx);
    return info;
}
