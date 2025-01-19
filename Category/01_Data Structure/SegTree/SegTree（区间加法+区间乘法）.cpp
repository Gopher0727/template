struct Info {
    ll sum = 0, add = 0, mul = 1;
    int l, r;
};

class SegTree {
private:
    int n, m;
    vector<Info> info;

    void pull(int o) { info[o].sum = info[o << 1].sum + info[o << 1 | 1].sum; }
    void push(int o) {
        info[o << 1].sum = info[o << 1].sum * info[o].mul + info[o].add * (info[o << 1].r - info[o << 1].l + 1);
        info[o << 1 | 1].sum = info[o << 1 | 1].sum * info[o].mul + info[o].add * (info[o << 1 | 1].r - info[o << 1 | 1].l + 1);

        info[o << 1].mul = info[o << 1].mul * info[o].mul % m;
        info[o << 1 | 1].mul = info[o << 1 | 1].mul * info[o].mul % m;

        info[o << 1].add = info[o << 1].add * info[o].mul + info[o].add;
        info[o << 1 | 1].add = info[o << 1 | 1].add * info[o].mul + info[o].add;

        info[o].add = 0, info[o].mul = 1;
    }

public:
    SegTree() {}
    SegTree(const vector<int>& _init, int m) : n(_init.size()), m(m), info(4 * n) {
        auto build = [&](auto&& self, ll o, ll l, ll r) -> void {
            info[o].l = l, info[o].r = r;
            if (l == r) {
                info[o].sum += _init[l] % m;
                return;
            }
            int mid = l + (r - l) / 2;
            self(self, o << 1, l, mid);
            self(self, o << 1 | 1, mid + 1, r);
            pull(o);
        };
        build(build, 1, 1, n);
    }

    void lazyMul(int o, int l, int r, ll k) {
        if (l <= info[o].l && info[o].r <= r) {
            info[o].add *= k;
            info[o].mul *= k;
            info[o].sum *= k;
            return;
        }
        push(o);
        int mid = info[o].l + (info[o].r - info[o].l) / 2;
        if (l <= mid) {
            lazyMul(o << 1, l, r, k);
        }
        if (r > mid) {
            lazyMul(o << 1 | 1, l, r, k);
        }
        pull(o);
    }

    void lazyAdd(int o, int l, int r, ll k) {
        if (l <= info[o].l && info[o].r <= r) {
            info[o].add += k;
            info[o].sum += k * (info[o].r - info[o].l + 1);
            return;
        }
        push(o);
        ll mid = info[o].l + (info[o].r - info[o].l) / 2;
        if (l <= mid) {
            lazyAdd(o << 1, l, r, k);
        }
        if (r > mid) {
            lazyAdd(o << 1 | 1, l, r, k);
        }
        pull(o);
    }

    ll query(int o, int l, int r) {
        if (l <= info[o].l && info[o].r <= r) {
            return info[o].sum;
        }
        push(o);
        ll val = 0, mid = info[o].l + (info[o].r - info[o].l) / 2;
        if (l <= mid) {
            val += query(o << 1, l, r);
        }
        if (r > mid) {
            val += query(o << 1 | 1, l, r);
        }
        return val;
    }
};
// Info 下标从 1开始
