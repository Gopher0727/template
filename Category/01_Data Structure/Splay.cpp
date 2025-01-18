namespace mySplay {
    struct Node {
        int s[2] {};
        int pa, val, cnt, size;

        Node() = default;
        Node(int p, int v) : pa(p), val(v) { cnt = size = 1; }
    };

    vector<Node> f;
    int root = 0, idx = 0;

    void pull(int x) {
        f[x].size = f[f[x].s[0]].size + f[f[x].s[1]].size + f[x].cnt;
    }

    void rotate(int x) {
        int y = f[x].pa, z = f[y].pa;
        int k = f[y].s[1] == x;
        // 冲突的子节点
        f[y].s[k] = f[x].s[k ^ 1];
        f[f[x].s[k ^ 1]].pa = y;
        // 调换父子关系
        f[x].s[k ^ 1] = y;
        f[y].pa = x;
        f[z].s[f[z].s[1] == y] = x;
        f[x].pa = z;
        pull(y);
        pull(x);
    }

    // k > 0，把 x 转到 k 下面
    // k = 0，把 x 转到根
    void splay(int x, int k) {
        while (f[x].pa != k) {
            int y = f[x].pa, z = f[y].pa;
            if (z != k) {
                (f[y].s[0] == x) ^ (f[z].s[0] == y) ? rotate(x) : rotate(y);
            }
            rotate(x);
        }
        if (k == 0) {
            root = x;
        }
    }

    // 找到节点并旋转至根
    void find(int v) {
        int x = root;
        while (f[x].s[v > f[x].val] && v != f[x].val) {
            x = f[x].s[v > f[x].val];
        }
        splay(x, 0);
    }

    int getPre(int v) {
        find(v);
        int x = root;
        if (f[x].val < v) {
            return x;
        }
        x = f[x].s[0];
        while (f[x].s[1]) {
            x = f[x].s[1];
        }
        splay(x, 0);
        return x;
    }
    int getNxt(int v) {
        find(v);
        int x = root;
        if (f[x].val > v) {
            return x;
        }
        x = f[x].s[1];
        while (f[x].s[0]) {
            x = f[x].s[0];
        }
        splay(x, 0);
        return x;
    }

    void del(int v) {
        int pre = getPre(v);
        int nxt = getNxt(v);
        splay(pre, 0);
        splay(nxt, pre);
        int d = f[nxt].s[0];
        if (f[d].cnt > 1) {
            f[d].cnt--;
            splay(d, 0);
        } else {
            f[nxt].s[0] = 0;
            splay(nxt, 0);
        }
    }

    void insert(int v) {
        int x = root, p = 0;
        while (x && f[x].val != v) {
            p = x;
            x = f[x].s[v > f[x].val];
        }
        if (x) {
            f[x].cnt++;
        } else {
            x = ++idx;
            f[p].s[v > f[p].val] = x;
            f[x] = Node(p, v);
        }
        splay(x, 0);
    }

    int getRank(int v) {
        // find(v);
        // return f[f[root].s[0]].size;
        insert(v);
        int res = f[f[root].s[0]].size;
        del(v);
        return res;
    }
    int getNum(int k) {
        int x = root;
        while (true) {
            int y = f[x].s[0];
            if (f[y].size + f[x].cnt < k) {
                k -= f[y].size + f[x].cnt;
                x = f[x].s[1];
            } else if (f[y].size >= k) {
                x = y;
            } else {
                break;
            }
        }
        splay(x, 0);
        return f[x].val;
    }

    auto init = []() {
        const int N = 1e6 + 1e5;
        f.resize(N);

        // 哨兵
        insert(-2e9);
        insert(2e9);

        return 0;
    }();
};


namespace Splay {
    // 若要修改一个点的点权，应当先将其splay到根，然后修改，最后还要调用pull维护。
    // 调用完splay之后根结点会改变，应该用splay的返回值更新根结点。
    const int MX = 110000;

    vector<array<int, 2>> ch;
    vector<int> pa, size, key;
    vector<int> stk, rev;
    int cur = 0;

    auto init = []() {
        ch.resize(MX);
        pa.resize(MX);
        size.resize(MX);
        key.resize(MX);
        stk.resize(MX);
        rev.resize(MX);
        return 0;
    }();

    int newNode(int val) {
        int x = ++cur;
        ch[x][0] = ch[x][1] = pa[x] = rev[x] = 0;
        size[x] = 1;
        key[x] = val;
        return x;
    }
    bool son(int x) {
        return ch[pa[x]][1] == x;
    }
    void pull(int x) {
        size[x] = size[ch[x][0]] + size[ch[x][1]] + 1;
    }
    void push(int x) {
        if (rev[x]) {
            rev[x] = 0;
            swap(ch[x][0], ch[x][1]);
            rev[ch[x][0]] ^= 1;
            rev[ch[x][1]] ^= 1;
        }
    }
    void rotate(int x) {
        int y = pa[x], z = pa[y], c = son(x);
        if (pa[y]) {
            ch[z][son(y)] = x;
        }
        pa[x] = z;
        ch[y][c] = ch[x][!c];
        pa[ch[y][c]] = y;
        ch[x][!c] = y;
        pa[y] = x;
        pull(y);
    }
    void ascend(int x) {
        for (int y = pa[x]; y; rotate(x), y = pa[x]) {
            if (pa[y]) {
                son(x) ^ son(y) ? rotate(x) : rotate(y);
            }
        }
        pull(x);
    }
    // 没有push操作时，可以直接用ascend替换splay
    int splay(int x) {
        int top = 0;
        for (int i = x; i; i = pa[i]) {
            stk[++top] = i;
        }
        while (top) {
            push(stk[top--]);
        }
        ascend(x);
        return x;
    }
    // 将以x为根的子树中的第k个结点旋转到根结点
    int splay(int x, int k) {
        while (push(x), k != size[ch[x][0]] + 1) {
            if (k <= size[ch[x][0]])
                x = ch[x][0];
            else
                k -= size[ch[x][0]] + 1, x = ch[x][1];
        }
        if (x) ascend(x);
        return x;
    }
    template <typename... T>
    int merge(int x, int y, T... args) {
        if constexpr (sizeof...(args) == 0) {
            // swap(x, y);
            if (x == 0) {
                return y;
            }
            x = splay(x, size[x]);
            ch[x][1] = y;
            pa[y] = x;
            pull(x);
            return x;
        } else {
            return merge(merge(x, y), args...);
        }
    }
    // 分成两个区间[1, pos - 1]和[pos, n]
    auto split(int x, int pos) {
        if (pos == size[x] + 1) {
            return make_pair(x, 0);
        }
        x = splay(x, pos);
        int y = ch[x][0];
        pa[y] = ch[x][0] = 0;
        pull(x);
        return make_pair(y, x);
    }
    auto extract(int x, int L, int R) {
        auto [left, y] = split(x, L);
        auto [mid, right] = split(y, R - L + 2);
        return make_tuple(left, mid, right);
    }
    void traverse(int x) {
        if (x != 0) {
            traverse(ch[x][0]);
            cout << key[x] << " ";
            // cout << format("{} (left: {}, right: {})  size: {}  key: {}\n", x, ch[x][0], ch[x][1], size[x], key[x]);
            traverse(ch[x][1]);
        }
    }

    int main() {
        vector<int> vec(50);
        int root = 0;
        for (int i = 1; i <= 10; ++i) {
            vec[i] = newNode(i);
            root = merge(root, vec[i]);
        }
        traverse(get<1>(extract(root, 3, 10)));
        cout << "\n";
        return 0;
    }
};

namespace SplayTree {
    struct Node {
        Node* l = nullptr;
        Node* r = nullptr;
        int cnt = 0;
        ll sum = 0;
    };

    Node* add(Node* t, int l, int r, int p, int v) {
        Node* x = new Node;
        if (t) {
            *x = *t;
        }
        x->cnt += 1;
        x->sum += v;
        if (r - l == 1) {
            return x;
        }
        int m = (l + r) / 2;
        if (p < m) {
            x->l = add(x->l, l, m, p, v);
        } else {
            x->r = add(x->r, m, r, p, v);
        }
        return x;
    }

    int find(Node* tl, Node* tr, int l, int r, int x) {
        if (r <= x) {
            return -1;
        }
        if (l >= x) {
            int cnt = (tr ? tr->cnt : 0) - (tl ? tl->cnt : 0);
            if (cnt == 0) {
                return -1;
            }
            if (r - l == 1) {
                return l;
            }
        }
        int m = (l + r) / 2;
        int res = find(tl ? tl->l : tl, tr ? tr->l : tr, l, m, x);
        if (res == -1) {
            res = find(tl ? tl->r : tl, tr ? tr->r : tr, m, r, x);
        }
        return res;
    }

    std::pair<int, ll> get(Node* t, int l, int r, int x, int y) {
        if (l >= y || r <= x || !t) {
            return {0, 0LL};
        }
        if (l >= x && r <= y) {
            return {t->cnt, t->sum};
        }
        int m = (l + r) / 2;
        auto [cl, sl] = get(t->l, l, m, x, y);
        auto [cr, sr] = get(t->r, m, r, x, y);
        return {cl + cr, sl + sr};
    }

    struct TreeNode {
        int add = 0;
        int val = 0;
        int id = 0;
        TreeNode* ch[2] = {};
        TreeNode* p = nullptr;
    };

    int pos(TreeNode* t) {
        return t->p->ch[1] == t;
    }

    void add(TreeNode* t, int v) {
        t->val += v;
        t->add += v;
    }

    void push(TreeNode* t) {
        if (t->ch[0]) {
            add(t->ch[0], t->add);
        }
        if (t->ch[1]) {
            add(t->ch[1], t->add);
        }
        t->add = 0;
    }

    void rotate(TreeNode* t) {
        TreeNode* q = t->p;
        int x = !pos(t);
        q->ch[!x] = t->ch[x];
        if (t->ch[x]) t->ch[x]->p = q;
        t->p = q->p;
        if (q->p) q->p->ch[pos(q)] = t;
        t->ch[x] = q;
        q->p = t;
    }

    void splay(TreeNode* t) {
        std::vector<TreeNode*> s;
        for (TreeNode* i = t; i->p; i = i->p) s.push_back(i->p);
        while (!s.empty()) {
            push(s.back());
            s.pop_back();
        }
        push(t);
        while (t->p) {
            if (t->p->p) {
                if (pos(t) == pos(t->p))
                    rotate(t->p);
                else
                    rotate(t);
            }
            rotate(t);
        }
    }

    void insert(TreeNode*& t, TreeNode* x, TreeNode* p = nullptr) {
        if (!t) {
            t = x;
            x->p = p;
            return;
        }

        push(t);
        if (x->val < t->val) {
            insert(t->ch[0], x, t);
        } else {
            insert(t->ch[1], x, t);
        }
    }

    void dfs(TreeNode* t) {
        if (!t) {
            return;
        }
        push(t);
        dfs(t->ch[0]);
        std::cerr << t->val << " ";
        dfs(t->ch[1]);
    }

    std::pair<TreeNode*, TreeNode*> split(TreeNode* t, int x) {
        if (!t) {
            return {t, t};
        }
        TreeNode* v = nullptr;
        TreeNode* j = t;
        for (TreeNode* i = t; i;) {
            push(i);
            j = i;
            if (i->val >= x) {
                v = i;
                i = i->ch[0];
            } else {
                i = i->ch[1];
            }
        }

        splay(j);
        if (!v) {
            return {j, nullptr};
        }

        splay(v);

        TreeNode* u = v->ch[0];
        if (u) {
            v->ch[0] = u->p = nullptr;
        }
        // std::cerr << "split " << x << "\n";
        // dfs(u);
        // std::cerr << "\n";
        // dfs(v);
        // std::cerr << "\n";
        return {u, v};
    }

    TreeNode* merge(TreeNode* l, TreeNode* r) {
        if (!l) {
            return r;
        }
        if (!r) {
            return l;
        }
        TreeNode* i = l;
        while (i->ch[1]) {
            i = i->ch[1];
        }
        splay(i);
        i->ch[1] = r;
        r->p = i;
        return i;
    }
};
