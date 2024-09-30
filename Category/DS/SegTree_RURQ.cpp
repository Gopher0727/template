static constexpr ll inf = 9e18;

struct Tag {
    ll add = 0;
    void apply(const Tag& t) { add += t.add; }
};
struct Info {
    ll mn = inf, mx = -inf, sum = 0, len = 1;
    void apply(const Tag& t) { mn += t.add, mx += t.add, sum += len * t.add; }
    friend Info operator+(const Info& a, const Info& b) { return Info(min(a.mn, b.mn), max(a.mx, b.mx), a.sum + b.sum, a.len + b.len); }
};

class SegTree {
private:
    ll n;
    vector<Tag> tag;
    vector<Info> info;

    void apply(ll o, const Tag& t) { tag[o].apply(t), info[o].apply(t); }
    void push_up(ll o) { info[o] = info[o << 1] + info[o << 1 | 1]; }
    void push_down(ll o) { apply(o << 1, tag[o]), apply(o << 1 | 1, tag[o]), tag[o] = Tag(); }

    void update(ll todol, ll todor, ll o, ll l, ll r, const Tag& t) {
        if (todor < l || r < todol) return;
        if (todol <= l && r <= todor) {
            apply(o, t);
            return;
        }
        push_down(o);
        ll m = l + (r - l) / 2;
        update(todol, todor, o << 1, l, m, t);
        update(todol, todor, o << 1 | 1, m + 1, r, t);
        push_up(o);
    }

    Info query(ll todol, ll todor, ll o, ll l, ll r) {
        if (todor < l || r < todol) return Info();
        if (todol <= l && r <= todor) return info[o];
        push_down(o);
        ll m = l + (r - l) / 2;
        return query(todol, todor, o << 1, l, m) + query(todol, todor, o << 1 | 1, m + 1, r);
    }

public:
    SegTree() {}
    SegTree(ll n) : n(n), tag(4 * n), info(4 * n) {} // 4 << __lg(n)
    SegTree(const vector<Info>& init) : SegTree(init.size()) {
        auto build = [&](auto&& self, ll o, ll l, ll r) -> void {
            if (l == r) {
                info[o] = init[l];
                return;
            }
            ll m = l + (r - l) / 2;
            self(self, o << 1, l, m);
            self(self, o << 1 | 1, m + 1, r);
            push_up(o);
        };
        build(build, 1, 1, n);
    }

    void update(ll todol, ll todor, const Tag& t) { update(todol, todor, 1, 1, n, t); }
    Info query(ll todol, ll todor) { return query(todol, todor, 1, 1, n); }
};
// Info 下标从 1开始
