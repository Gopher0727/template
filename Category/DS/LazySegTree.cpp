static constexpr ll inf = 9e18;

template <class Info, class Tag>
class LazySegmentTree {
public:
    int n;
    vector<Info> info;
    vector<Tag> tag;

public:
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n_, Info v_ = Info()) { init(n_, v_); }
    void init(int n_, Info v_ = Info()) { init(vector(n_, v_)); }

    template <class T>
    LazySegmentTree(vector<T> init_) {
        init(init_);
    }
    template <class T>
    void init(vector<T> init_) {
        n = init_.size();
        info.assign(4 << __lg(n), Info());
        tag.assign(4 << __lg(n), Tag());

        function<void(int, int, int)> build = [&](int o, int l, int r) {
            if (l + 1 == r) {
                info[o] = init_[l];
                return;
            }
            int m = l + (r - l) / 2;
            build(o << 1, l, m);
            build(o << 1 | 1, m, r);
            pull(o);
        };
        build(1, 0, n);
    }

private:
    void apply(int o, const Tag& v) {
        info[o].apply(v);
        tag[o].apply(v);
    }

    void pull(int o) { info[o] = info[o << 1] + info[o << 1 | 1]; }
    void push(int o) {
        apply(2 * o, tag[o]);
        apply(2 * o + 1, tag[o]);
        tag[o] = Tag();
    }

public:
    // 赋值式更新
    void modify(int o, int l, int r, int x, const Info& v) {
        if (l + 1 == r) {
            info[o] = v;
            return;
        }
        int m = l + (r - l) / 2;
        push(o);
        if (x < m) {
            modify(o << 1, l, m, x, v);
        } else {
            modify(o << 1 | 1, m, r, x, v);
        }
        pull(o);
    }
    void modify(int o, const Info& v) { modify(1, 0, n, o, v); }

    // [l, r)
    Info rangeQuery(int o, int l, int r, int x, int y) {
        if (r <= x || l >= y) {
            return Info();
        }
        if (x <= l && r <= y) {
            return info[o];
        }
        int m = l + (r - l) / 2;
        push(o);
        return rangeQuery(o << 1, l, m, x, y) + rangeQuery(o << 1 | 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r) { return rangeQuery(1, 0, n, l, r); }

    void rangeModify(int o, int l, int r, int x, int y, const Tag& v) {
        if (r <= x || l >= y) {
            return;
        }
        if (x <= l && r <= y) {
            apply(o, v);
            return;
        }
        int m = l + (r - l) / 2;
        push(o);
        rangeModify(o << 1, l, m, x, y, v);
        rangeModify(o << 1 | 1, m, r, x, y, v);
        pull(o);
    }
    void rangeModify(int l, int r, const Tag& v) { return rangeModify(1, 0, n, l, r, v); }

    template <class F>
    int findFirst(int o, int l, int r, int x, int y, F& pred) {
        if (l >= y || r <= x || !pred(info[o])) {
            return -1;
        }
        if (l + 1 == r) {
            return l;
        }
        int m = l + (r - l) / 2;
        push(o);
        int res = findFirst(o << 1, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(o << 1 | 1, m, r, x, y, pred);
        }
        return res;
    }
    template <class F>
    int findFirst(int l, int r, F& pred) {
        return findFirst(1, 0, n, l, r, pred);
    }

    template <class F>
    int findLast(int o, int l, int r, int x, int y, F& pred) {
        if (l >= y || r <= x || !pred(info[o])) {
            return -1;
        }
        if (l + 1 == r) {
            return l;
        }
        int m = l + (r - l) / 2;
        push(o);
        int res = findLast(o << 1 | 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(o << 1, l, m, x, y, pred);
        }
        return res;
    }
    template <class F>
    int findLast(int l, int r, F& pred) {
        return findLast(1, 0, n, l, r, pred);
    }

public:
    void maintainL(int p, int l, int r, int pre) {
        if (info[p].difl > 0 && info[p].maxlowl < pre) {
            return;
        }
        if (r - l == 1) {
            info[p].max = info[p].maxlowl;
            info[p].maxl = info[p].maxr = l;
            info[p].maxlowl = info[p].maxlowr = -inf;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        maintainL(2 * p, l, m, pre);
        pre = max(pre, info[2 * p].max);
        maintainL(2 * p + 1, m, r, pre);
        pull(p);
    }
    void maintainL() { maintainL(1, 0, n, -1); }
    void maintainR(int p, int l, int r, int suf) {
        if (info[p].difr > 0 && info[p].maxlowr < suf) {
            return;
        }
        if (r - l == 1) {
            info[p].max = info[p].maxlowl;
            info[p].maxl = info[p].maxr = l;
            info[p].maxlowl = info[p].maxlowr = -inf;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        maintainR(2 * p + 1, m, r, suf);
        suf = max(suf, info[2 * p + 1].max);
        maintainR(2 * p, l, m, suf);
        pull(p);
    }
    void maintainR() { maintainR(1, 0, n, -1); }
};
// 下标从 0 开始

struct Tag {
    int add = 0;
    void apply(const Tag& t) { add += t.add; }
};

struct Info {
    int mx = 0;
    void apply(const Tag& t) { mx += t.add; }
};
Info operator+(const Info& a, const Info& b) {
    return Info(max(a.mx, b.mx));
}
