class SegTree {
private:
    struct Info {
        Mint sum = 0, add = 0, mul = 1;
        int l, r;
    };

    int n;
    vector<Info> info;

    void pull(int o) { info[o].sum = info[o << 1].sum + info[o << 1 | 1].sum; }

    void push(int o) {
        info[o << 1].sum = info[o << 1].sum * info[o].mul + info[o].add * (info[o << 1].r - info[o << 1].l + 1);
        info[o << 1 | 1].sum = info[o << 1 | 1].sum * info[o].mul + info[o].add * (info[o << 1 | 1].r - info[o << 1 | 1].l + 1);

        info[o << 1].mul = info[o << 1].mul * info[o].mul;
        info[o << 1 | 1].mul = info[o << 1 | 1].mul * info[o].mul;

        info[o << 1].add = info[o << 1].add * info[o].mul + info[o].add;
        info[o << 1 | 1].add = info[o << 1 | 1].add * info[o].mul + info[o].add;

        info[o].add = 0;
        info[o].mul = 1;
    }

    void lazyMul(int o, int l, int r, Mint k) {
        if (l <= info[o].l && info[o].r <= r) {
            info[o].add *= k;
            info[o].mul *= k;
            info[o].sum *= k;
            return;
        }
        push(o);
        int m = info[o].l + (info[o].r - info[o].l) / 2;
        if (l <= m) {
            lazyMul(o << 1, l, r, k);
        }
        if (r > m) {
            lazyMul(o << 1 | 1, l, r, k);
        }
        pull(o);
    }

    void lazyAdd(int o, int l, int r, Mint k) {
        if (l <= info[o].l && info[o].r <= r) {
            info[o].add += k;
            info[o].sum += k * (info[o].r - info[o].l + 1);
            return;
        }
        push(o);
        int m = info[o].l + (info[o].r - info[o].l) / 2;
        if (l <= m) {
            lazyAdd(o << 1, l, r, k);
        }
        if (r > m) {
            lazyAdd(o << 1 | 1, l, r, k);
        }
        pull(o);
    }

    Mint queryPoint(int o, int i) {
        if (info[o].l == info[o].r) {
            return info[o].sum;
        }
        push(o);
        int m = info[o].l + (info[o].r - info[o].l) / 2;
        if (i <= m) {
            return queryPoint(o << 1, i);
        }
        return queryPoint(o << 1 | 1, i);
    }

    Mint queryRange(int o, int l, int r) {
        if (l <= info[o].l && info[o].r <= r) {
            return info[o].sum;
        }
        push(o);
        int m = info[o].l + (info[o].r - info[o].l) / 2;
        if (r <= m) {
            return queryRange(o << 1, l, r);
        }
        if (l > m) {
            return queryRange(o << 1 | 1, l, r);
        }
        return queryRange(o << 1, l, r) + queryRange(o << 1 | 1, l, r);
    }

public:
    SegTree() : n(0) {}
    SegTree(const vector<int>& _init) : n(_init.size()), info(4 << __lg(n)) {
        auto build = [&](auto&& self, int o, int l, int r) -> void {
            info[o].l = l, info[o].r = r;
            if (l == r) {
                info[o].sum += _init[l];
                return;
            }
            int m = l + (r - l) / 2;
            self(self, o << 1, l, m);
            self(self, o << 1 | 1, m + 1, r);
            pull(o);
        };
        build(build, 1, 0, n - 1);
    }

    void lazyAdd(int l, int r, Mint k) { lazyAdd(1, l, r, k); }

    void lazyMul(int l, int r, Mint k) { lazyMul(1, l, r, k); }

    Mint query(int i) { return queryPoint(1, i); }

    Mint query(int l, int r) { return queryRange(1, l, r); }

    Mint queryAll() { return info[1].sum; }
};
