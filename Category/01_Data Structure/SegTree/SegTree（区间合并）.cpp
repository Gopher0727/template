template <class Info, class Merge = std::plus<Info>>
class SegTree {
    const int n;
    const Merge merge;
    vector<Info> info;

    void pull(int p) { info[p] = merge(info[2 * p], info[2 * p + 1]); }

public:
    SegTree(int n) : n(n), merge(Merge()), info(4 << std::__lg(n)) {}
    SegTree(vector<Info> init) : SegTree(init.size()) {
        auto build = [&](auto&& self, int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int m = (l + r) / 2;
            self(self, 2 * p, l, m);
            self(self, 2 * p + 1, m, r);
            pull(p);
        };
        build(build, 1, 0, n);
    }

    void modify(int p, int l, int r, int x, const Info& v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info& v) { modify(1, 0, n, p, v); }

    Info query(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();
        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        return merge(query(2 * p, l, m, x, y), query(2 * p + 1, m, r, x, y));
    }
    Info query(int l, int r) { return query(1, 0, n, l, r); }
};

struct Info {
    int ans;
    vector<array<int, 2>> a;
    Info() : ans(0), a {} {}
    Info(int x) : ans(0) { a.push_back({x, 1}); }
};

void add(Info& c, int x, int l) {
    if (c.a.empty()) {
        c.a.push_back({x, l});
        return;
    }
    auto& last = c.a.back();
    if (l == 0) {
        if (last[0] == x && last[1] == 0) {
            return;
        } else if (last[0] == !x && last[1] == 0) {
            if (c.a.size() > 1) {
                c.ans++;
                c.a.pop_back();
                add(c, x, l);
            } else {
                c.a.push_back({x, l});
            }
        } else if ((last[0] + last[1]) % 2 == x) {
            c.a.push_back({x, l});
        }
    } else {
        if (last[0] == x && last[1] == 0) {
            c.a.pop_back();
            add(c, x, l);
        } else if (last[0] == !x && last[1] == 0) {
            if (c.a.size() > 1) {
                c.ans++;
                c.a.pop_back();
                add(c, x, l);
            } else {
                c.a.push_back({x, l});
            }
        } else if ((last[0] + last[1]) % 2 == x) {
            last[1] += l;
        } else if (l == 1 && last[1] == 1) {
            c.a.pop_back();
            add(c, x, 0);
        } else if (l == 1) {
            last[1]--;
            add(c, x, 0);
        } else if (last[1] == 1) {
            c.a.pop_back();
            add(c, x, 0);
            add(c, !x, l - 1);
        } else {
            int t = min(last[1], l) - 1;
            c.ans += t;
            last[1] -= t;
            add(c, (x + t) % 2, l - t);
        }
    }
}

Info operator+(const Info& a, const Info& b) {
    if (a.a.empty()) {
        return b;
    }
    if (b.a.empty()) {
        return a;
    }
    Info c;
    c.ans = a.ans + b.ans;
    c.a = a.a;
    for (auto [x, l] : b.a) {
        add(c, x, l);
    }
    return c;
}
