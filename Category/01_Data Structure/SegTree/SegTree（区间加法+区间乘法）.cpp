struct Info {
    ll sum = 0, add = 0, mul = 1;
    int l, r;
};

class SegTree {
private:
    int n, m;
    vector<Info> t;

    void pull(int o) { t[o].sum = t[o << 1].sum + t[o << 1 | 1].sum; }
    void push(int o) {
        t[o << 1].sum = t[o << 1].sum * t[o].mul + t[o].add * (t[o << 1].r - t[o << 1].l + 1);
        t[o << 1 | 1].sum = t[o << 1 | 1].sum * t[o].mul + t[o].add * (t[o << 1 | 1].r - t[o << 1 | 1].l + 1);

        t[o << 1].mul = t[o << 1].mul * t[o].mul % m;
        t[o << 1 | 1].mul = t[o << 1 | 1].mul * t[o].mul % m;

        t[o << 1].add = t[o << 1].add * t[o].mul + t[o].add;
        t[o << 1 | 1].add = t[o << 1 | 1].add * t[o].mul + t[o].add;

        t[o].add = 0, t[o].mul = 1;
    }

public:
    SegTree() {}
    SegTree(const vector<int>& vec, int m) : n(vec.size()), m(m), t(4 * n) {
        auto build = [&](auto&& self, ll o, ll l, ll r) -> void {
            t[o].l = l, t[o].r = r;
            if (l == r) {
                t[o].sum += vec[l] % m;
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
        if (l <= t[o].l && t[o].r <= r) {
            t[o].add *= k;
            t[o].mul *= k;
            t[o].sum *= k;
            return;
        }
        push(o);
        int mid = t[o].l + (t[o].r - t[o].l) / 2;
        if (l <= mid) {
            lazyMul(o << 1, l, r, k);
        }
        if (r > mid) {
            lazyMul(o << 1 | 1, l, r, k);
        }
        pull(o);
    }

    void lazyAdd(int o, int l, int r, ll k) {
        if (l <= t[o].l && t[o].r <= r) {
            t[o].add += k;
            t[o].sum += k * (t[o].r - t[o].l + 1);
            return;
        }
        push(o);
        ll mid = t[o].l + (t[o].r - t[o].l) / 2;
        if (l <= mid) {
            lazyAdd(o << 1, l, r, k);
        }
        if (r > mid) {
            lazyAdd(o << 1 | 1, l, r, k);
        }
        pull(o);
    }

    ll query(int o, int l, int r) {
        if (l <= t[o].l && t[o].r <= r) {
            return t[o].sum;
        }
        push(o);
        ll val = 0, mid = t[o].l + (t[o].r - t[o].l) / 2;
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
