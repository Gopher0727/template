class SegTree {
private:
    struct Node {
        int l, r;
        int min_cover_len = 0; // 区间内被矩形覆盖次数最少的底边长之和
        int min_cover = 0; // 区间内被矩形覆盖的最小次数
        int todo = 0; // 子树内的所有节点的 min_cover 需要增加的量，注意这可以是负数
    };

    vector<Node> t;

    void pull(int o) {
        int mn = min(t[o << 1].min_cover, t[o << 1 | 1].min_cover);
        t[o].min_cover = mn;
        t[o].min_cover_len = (t[o << 1].min_cover == mn ? t[o << 1].min_cover_len : 0) + (t[o << 1 | 1].min_cover == mn ? t[o << 1 | 1].min_cover_len : 0);
    }

    void _do(int o, int v) { // 更新节点信息，不下传懒标记
        t[o].min_cover += v;
        t[o].todo += v;
    }

    void push(int o) {
        int& v = t[o].todo;
        if (v != 0) {
            _do(o << 1, v);
            _do(o << 1 | 1, v);
            v = 0;
        }
    }

    void build(const vector<int>& pos, int o, int l, int r) {
        t[o].l = l, t[o].r = r;
        if (l == r) {
            t[o].min_cover_len = pos[l + 1] - pos[l];
            return;
        }
        int m = l + (r - l) / 2;
        build(pos, o << 1, l, m);
        build(pos, o << 1 | 1, m + 1, r);
        pull(o);
    }

    void modify(int o, int l, int r, int v) {
        if (l <= t[o].l && t[o].r <= r) {
            _do(o, v);
            return;
        }
        push(o);
        int m = t[o].l + (t[o].r - t[o].l) / 2;
        if (l <= m) {
            modify(o << 1, l, r, v);
        }
        if (m < r) {
            modify(o << 1 | 1, l, r, v);
        }
        pull(o);
    }

public:
    SegTree(const vector<int>& pos) {
        unsigned n = pos.size() - 1;
        t.resize(2 << bit_width(n - 1));
        build(pos, 1, 0, n - 1);
    }

    void modify(int l, int r, int v) { modify(1, l, r, v); }

    int get_uncovered_length() { return t[1].min_cover ? 0 : t[1].min_cover_len; }
};

struct Event {
    int delta, L, R, Op;
};

void solve() {
    int n;
    cin >> n;

    vector<int> pos;
    vector<Event> events;
    for (int i = 0; i < n; ++i) {
        // int x1, y1, x2, y2;
        // cin >> x1 >> y1 >> x2 >> y2;

        pos.push_back(y1);
        pos.push_back(y2);
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
    }

    ranges::sort(pos);
    pos.erase(unique(pos.begin(), pos.end()), pos.end());

    ranges::sort(events, {}, &Event::delta);

    SegTree t(pos);

    i64 tot = 0;
    for (int i = 0; i + 1 < events.size(); ++i) {
        auto& [delta, L, R, Op] = events[i];
        int l = ranges::lower_bound(pos, L) - pos.begin();
        int r = ranges::lower_bound(pos, R) - pos.begin() - 1; // r 对应着 pos[r] 与 pos[r+1]=R 的差值
        t.modify(l, r, Op);
        i64 sum = pos.back() - pos[0] - t.get_uncovered_length();
        tot += sum * (events[i + 1].delta - delta);
    }
    cout << tot << "\n";
}
