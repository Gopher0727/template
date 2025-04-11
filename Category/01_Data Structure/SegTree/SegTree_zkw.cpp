template <class Info, class Tag>
class ZKWSegTree {
private:
    int size;
    int height;
    vector<Info> info;
    vector<Tag> tag;

    void apply(int p, const Tag& t) {
        info[p].apply(t, 1 << (height - (31 - __builtin_clz(p))));
        tag[p].apply(t);
    }

    void push(int p) {
        apply(p << 1, tag[p]);
        apply(p << 1 | 1, tag[p]);
        tag[p] = Tag();
    }

    void pull(int p) { info[p] = info[p << 1] + info[p << 1 | 1]; }

public:
    ZKWSegTree() = delete;
    ZKWSegTree(int n, Info v = Info {}) : ZKWSegTree(vector(n, Info {})) {}
    ZKWSegTree(const vector<Info>& _init) {
        init(_init.size());
        build(_init);
    }

    void init(int n) {
        size = 1;
        height = 0;
        while (size < n) {
            size <<= 1;
            height++;
        }
        info.assign(size << 1, Info());
        tag.assign(size << 1, Tag());
    }

    void build(const vector<Info>& data) {
        int n = data.size();
        for (int i = 0; i < n; i++) {
            info[i + size] = data[i];
        }
        for (int i = size - 1; i > 0; i--) {
            pull(i);
        }
    }

public:
    void modify(int l, int r, const Tag& t) {
        l += size;
        r += size + 1;
        int l0 = l, r0 = r;
        for (int i = height; i > 0; i--) {
            if ((l0 >> i << i) != l0) {
                push(l0 >> i);
            }
            if ((r0 >> i << i) != r0) {
                push((r0 - 1) >> i);
            }
        }
        for (int tl = l, tr = r; tl < tr; tl >>= 1, tr >>= 1) {
            if (tl & 1) {
                apply(tl++, t);
            }
            if (tr & 1) {
                apply(--tr, t);
            }
        }
        for (int i = 1; i <= height; i++) {
            if ((l0 >> i << i) != l0) {
                pull(l0 >> i);
            }
            if ((r0 >> i << i) != r0) {
                pull((r0 - 1) >> i);
            }
        }
    }

    Info query(int l, int r) {
        l += size;
        r += size + 1;
        for (int i = height; i > 0; i--) {
            if ((l >> i << i) != l) {
                push(l >> i);
            }
            if ((r >> i << i) != r) {
                push((r - 1) >> i);
            }
        }
        Info resL {}, resR {};
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                resL = resL + info[l++];
            }
            if (r & 1) {
                resR = info[--r] + resR;
            }
        }
        return resL + resR;
    }

    // 查找满足条件的最左侧索引，范围在 [pos, size)
    int findFirst(int pos, const function<bool(const Info&)>& check) {
        int l = pos - 1, r = size;
        while (l + 1 < r) {
            int m = l + (r - l) / 2;
            if (check(query(l + 1, m))) {
                r = m;
            } else {
                l = m;
            }
        }
        if (r == size) {
            r = -1;
        }
        return r;
    }

    // 查找满足条件的最右侧索引，范围在 [0, pos)
    int findLast(int pos, const function<bool(const Info&)>& check) {
        int l = -1, r = pos;
        while (l + 1 < r) {
            int m = l + (r - l) / 2;
            if (check(query(m, pos - 1))) {
                l = m;
            } else {
                r = m;
            }
        }
        return l;
    }
};

struct Tag {
    i64 add = 0;
    Tag() {}
    Tag(i64 v) : add(v) {}
    void apply(const Tag& t) {
        if (t.add) {
            add += t.add;
        }
    }
};

struct Info {
    i64 sum = 0, len = 0;
    void apply(const Tag& t, int len) {
        if (t.add) {
            sum += len * t.add;
        }
    }
};
Info operator+(const Info& a, const Info& b) {
    Info info;
    info.sum = a.sum + b.sum;
    info.len = a.len + b.len;
    return info;
}
