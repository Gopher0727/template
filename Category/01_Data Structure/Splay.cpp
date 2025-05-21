namespace Splay {
    static constexpr int MAXN = 1E5 + 10;
    int head = 0, cnt = 0;
    int key[MAXN], pa[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];

    void pull(int i) {
        siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
    }

    // 返回是左儿子还是右儿子
    int lr(int i) {
        return rs[pa[i]] == i;
    }

    void rotate(int i) {
        int f = pa[i], g = pa[f], soni = lr(i), sonf = lr(f);
        if (soni == 1) {
            rs[f] = ls[i];
            if (rs[f] != 0) {
                pa[rs[f]] = f;
            }
            ls[i] = f;
        } else {
            ls[f] = rs[i];
            if (ls[f] != 0) {
                pa[ls[f]] = f;
            }
            rs[i] = f;
        }
        if (g != 0) {
            if (sonf == 1) {
                rs[g] = i;
            } else {
                ls[g] = i;
            }
        }
        pa[f] = i;
        pa[i] = g;
        pull(f);
        pull(i);
    }

    // 把 i 节点变为 goal 节点的儿子（goal = 0 则表示把 i 节点变为整棵树的头节点）
    void splay(int i, int goal) {
        int f = pa[i], g = pa[f];
        while (f != goal) {
            if (g != goal) {
                if (lr(i) == lr(f)) {
                    rotate(f);
                } else {
                    rotate(i);
                }
            }
            rotate(i);
            f = pa[i];
            g = pa[f];
        }
        if (goal == 0) {
            head = i;
        }
    }

    /**
        有序表的实现，不做频次压缩
    */
    int find(int rnk) {
        int i = head;
        while (i != 0) {
            if (siz[ls[i]] + 1 == rnk) {
                return i;
            } else if (siz[ls[i]] >= rnk) {
                i = ls[i];
            } else {
                rnk -= siz[ls[i]] + 1;
                i = rs[i];
            }
        }
        return 0;
    }

    void insert(int num) {
        key[++cnt] = num;
        siz[cnt] = 1;
        if (head == 0) {
            head = cnt;
        } else {
            int f = 0, i = head, son = 0;
            while (i != 0) {
                f = i;
                if (key[i] <= num) {
                    son = 1;
                    i = rs[i];
                } else {
                    son = 0;
                    i = ls[i];
                }
            }
            if (son == 1) {
                rs[f] = cnt;
            } else {
                ls[f] = cnt;
            }
            pa[cnt] = f;
            splay(cnt, 0);
        }
    }

    // 返回从 1 开始的排名
    int rnk(int num) {
        int i = head, last = head;
        int ans = 0;
        while (i != 0) {
            last = i;
            if (key[i] >= num) {
                i = ls[i];
            } else {
                ans += siz[ls[i]] + 1;
                i = rs[i];
            }
        }
        splay(last, 0); // 保证复杂度
        return ans + 1;
    }

    void remove(int num) {
        int kth = rnk(num);
        // 确定是否存在 num
        if (kth != rnk(num + 1)) {
            int i = find(kth);
            splay(i, 0);
            if (ls[i] == 0) {
                head = rs[i];
            } else if (rs[i] == 0) {
                head = ls[i];
            } else {
                int j = find(kth + 1);
                splay(j, i); // 提为 i 的右儿子
                ls[j] = ls[i]; // 中序后继节点 j 的左孩子为空
                pa[ls[j]] = j;
                pull(j);
                head = j;
            }
            pa[head] = 0;
        }
    }

    int index(int x) {
        int i = find(x);
        splay(i, 0);
        return key[i];
    }

    int pre(int num) {
        int i = head, last = head;
        int ans = INT_MIN;
        while (i != 0) {
            last = i;
            if (key[i] >= num) {
                i = ls[i];
            } else {
                ans = max(ans, key[i]);
                i = rs[i];
            }
        }
        splay(last, 0);
        return ans;
    }

    int post(int num) {
        int i = head, last = head;
        int ans = INT_MAX;
        while (i != 0) {
            last = i;
            if (key[i] <= num) {
                i = rs[i];
            } else {
                ans = min(ans, key[i]);
                i = ls[i];
            }
        }
        splay(last, 0);
        return ans;
    }

    // 删除小于 x 的所有节点：将某个节点旋转至根，将相应孩子节点置 0 即可
    void del(int x) {
        int i = head, ans = 0;
        while (i != 0) {
            if (key[i] >= x) {
                ans = i;
                i = ls[i];
            } else {
                i = rs[i];
            }
        }
        if (ans == 0) {
            head = 0;
        } else {
            splay(ans, 0);
            ls[head] = 0;
            pull(head);
        }
    }

} // namespace Splay


namespace Splay {
    // 若要修改一个点的点权，应当先将其 splay 到根，然后修改，最后还要调用 pull 维护。
    // 调用完 splay 之后根结点会改变，应该用 splay 的返回值更新根结点。
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

    // 没有 push 操作时，可以直接用 ascend 替换 splay
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

    // 将以 x 为根的子树中的第 k 个结点旋转到根结点
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

    // 分成两个区间 [1, pos-1] 和 [pos, n]
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
        return array {left, mid, right};
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
        traverse(extract(root, 3, 10)[1]);
        cout << "\n";

        return 0;
    }
}; // namespace Splay


namespace SplayTree {
    struct Node {
        Node* l = nullptr;
        Node* r = nullptr;
        int cnt = 0;
        i64 sum = 0;
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

    std::pair<int, i64> get(Node* t, int l, int r, int x, int y) {
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
        if (t->ch[x]) {
            t->ch[x]->p = q;
        }
        t->p = q->p;
        if (q->p) {
            q->p->ch[pos(q)] = t;
        }
        t->ch[x] = q;
        q->p = t;
    }

    void splay(TreeNode* t) {
        vector<TreeNode*> s;
        for (TreeNode* i = t; i->p; i = i->p) {
            s.push_back(i->p);
        }
        while (!s.empty()) {
            push(s.back());
            s.pop_back();
        }
        push(t);
        while (t->p) {
            if (t->p->p) {
                if (pos(t) == pos(t->p)) {
                    rotate(t->p);
                } else {
                    rotate(t);
                }
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
}; // namespace SplayTree
