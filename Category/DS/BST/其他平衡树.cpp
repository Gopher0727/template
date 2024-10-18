/*
    二叉树旋转之后，中序遍历的顺序不变。

    无旋：
        > ScapegoatTree (替罪羊树)： 暴力重构
            · https://oi-wiki.org/ds/sgt/
            · https://riteme.site/blog/2016-4-6/scapegoat.html

        （换了一种实现方式）
        > （无旋）fhq Treap： 分裂（按值分裂，按大小分裂） + 合并
            > Treap
            > SegTreap  区间操作（利用懒标记实现区间翻转）

            · 二叉搜索树 + 二叉堆（特点：父节点优先级高）
            · 当作普通平衡树使用时，多采用按值分裂
            · 维护区间信息时多采用按大小分裂，如 '文艺平衡树' SegTreap

    有旋：
        > AVL
        > 红黑树
*/
namespace ScapegoatTree {
    static constexpr double alpha = 0.75;
    static int cnt = 0, root = 0;

    class Scapegoat {
        struct Node {
            int left, right;
            int val;
            int size;
            int realsize; // 以当前节点为根的树的实际大小（未被标记的节点数量）
            bool exist; // 存在标记
        };

        vector<Node> tree;

    public:
        Scapegoat() = default;
        Scapegoat(int size) : tree(size) {}

    private:
        int newNode(int& cur, int val) {
            cur = ++cnt;
            tree[cur].val = val;
            tree[cur].size = tree[cur].realsize = 1;
            tree[cur].exist = true;
            return cur;
        }

        bool imbalance(int cur) {
            return (max(tree[tree[cur].left].size, tree[tree[cur].right].size) > tree[cur].size * alpha //
                    || tree[cur].size - tree[cur].realsize > tree[cur].size * 0.3);
        }

        // 中序遍历
        vector<int> Inorder;
        void InorderTraversal(int cur) {
            if (!cur) {
                return;
            }
            InorderTraversal(tree[cur].left);
            if (tree[cur].exist) {
                Inorder.emplace_back(cur);
            }
            InorderTraversal(tree[cur].right);
        }

        void lift(int l, int r, int& cur) {
            if (l == r) {
                cur = Inorder[l];
                tree[cur].left = tree[cur].right = 0;
                tree[cur].size = tree[cur].realsize = 1;
                return;
            }
            int m = l + (r - l) / 2;
            // 处理重复数据（一般将相同的数放到右子树）
            while (m && l < m && tree[Inorder[m]].val == tree[Inorder[m - 1]].val) {
                m--;
            }
            cur = Inorder[m];
            if (l < m) {
                lift(l, m - 1, tree[cur].left);
            } else {
                tree[cur].left = 0;
            }
            lift(m + 1, r, tree[cur].right);
            tree[cur].size = tree[tree[cur].left].size + tree[tree[cur].right].size + 1;
            tree[cur].realsize = tree[tree[cur].left].realsize + tree[tree[cur].right].realsize + 1;
        }

        // 暴力：中序遍历后分治建树
        void rebuild(int& cur) {
            Inorder.clear();
            InorderTraversal(cur);
            if (Inorder.empty()) {
                cur = 0;
                return;
            }
            lift(0, Inorder.size() - 1, cur);
        }

        void update(int& cur, int end) {
            if (!cur) {
                return;
            }
            if (tree[end].val < tree[cur].val) {
                update(tree[cur].left, end);
            } else {
                update(tree[cur].right, end);
            }
            tree[cur].size = tree[tree[cur].left].size + tree[tree[cur].right].size + 1;
        }

        // 需要重构的条件： 当前节点的左子树或右子树的大小大于当前节点的大小乘上平衡因子 alpha（0.5 <= alpha <= 1）
        void check(int& cur, int end) {
            if (cur == end) {
                return;
            }
            if (imbalance(cur)) {
                rebuild(cur);
                update(root, cur);
                return;
            }
            if (tree[end].val < tree[cur].val) {
                check(tree[cur].left, end);
            } else {
                check(tree[cur].right, end);
            }
        }

    public:
        void insert(int& cur, int val) {
            if (!cur) {
                newNode(cur, val);
                check(root, cur);
            } else {
                tree[cur].size++;
                tree[cur].realsize++;
                if (val < tree[cur].val) {
                    insert(tree[cur].left, val);
                } else {
                    insert(tree[cur].right, val);
                }
            }
        }
        void del(int cur, int val) { // 惰性删除（要求树中有该元素）
            if (tree[cur].exist && tree[cur].val == val) {
                tree[cur].exist = false;
                tree[cur].realsize--;
                check(root, cur);
            } else {
                tree[cur].realsize--;
                if (val < tree[cur].val) {
                    del(tree[cur].left, val);
                } else {
                    del(tree[cur].right, val);
                }
            }
        }
        // rank：比当前数小的数的个数加 1
        int getRank(int val) {
            int cur = root, rank = 1;
            while (cur) {
                if (val <= tree[cur].val) {
                    cur = tree[cur].left;
                } else {
                    rank += tree[cur].exist + tree[tree[cur].left].realsize;
                    cur = tree[cur].right;
                }
            }
            return rank;
        }
        int getNum(int rank) {
            int cur = root;
            while (cur) {
                if (tree[cur].exist && tree[tree[cur].left].realsize + tree[cur].exist == rank) {
                    break;
                } else if (tree[tree[cur].left].realsize >= rank) {
                    cur = tree[cur].left;
                } else {
                    rank -= tree[tree[cur].left].realsize + tree[cur].exist;
                    cur = tree[cur].right;
                }
            }
            return tree[cur].val;
        }
        int getPre(int x) { return getNum(getRank(x) - 1); }
        int getNxt(int x) { return getNum(getRank(x + 1)); }
    };
};

namespace Treap {
    static int cnt = 0, root = 0;
    static int x = 0, y = 0, z = 0;
    std::mt19937 rnd(233);

    class Treap {
        struct Node {
            int left, right;
            int val, key;
            int size;
        };

        vector<Node> tree;

    public:
        Treap() = default;
        Treap(int size) : tree(size) {}

    private:
        int newNode(int val) {
            cnt++;
            tree[cnt].val = val;
            tree[cnt].key = rnd();
            tree[cnt].size = 1;
            return cnt;
        }
        void update(int cur) { tree[cur].size = tree[tree[cur].left].size + tree[tree[cur].right].size + 1; }

        // 按小于等于某个值分裂
        void split(int cur, int val, int& x, int& y) {
            if (!cur) {
                x = y = 0;
            } else {
                if (tree[cur].val <= val) {
                    x = cur;
                    split(tree[cur].right, val, tree[cur].right, y);
                } else {
                    y = cur;
                    split(tree[cur].left, val, x, tree[cur].left);
                }
                update(cur);
            }
        }

        // x 树中的值不大于 y 树中的值
        int merge(int x, int y) {
            if (!x || !y) {
                return x + y;
            }
            if (tree[x].key > tree[y].key) { // < <= > >=
                tree[x].right = merge(tree[x].right, y);
                update(x);
                return x;
            } else {
                tree[y].left = merge(x, tree[y].left);
                update(y);
                return y;
            }
        }

    public:
        void insert(int val) {
            split(root, val, x, y);
            root = merge(merge(x, newNode(val)), y);
        }
        void del(int val) {
            split(root, val, x, z);
            split(x, val - 1, x, y);
            y = merge(tree[y].left, tree[y].right);
            root = merge(merge(x, y), z);
        }
        int getRank(int val) {
            split(root, val - 1, x, y);
            int ans = tree[x].size + 1;
            root = merge(x, y);
            return ans;
        }
        int getNum(int rank) {
            int cur = root;
            while (cur) {
                if (tree[tree[cur].left].size + 1 == rank) {
                    break;
                } else if (tree[tree[cur].left].size >= rank) {
                    cur = tree[cur].left;
                } else {
                    rank -= tree[tree[cur].left].size + 1;
                    cur = tree[cur].right;
                }
            }
            return tree[cur].val;
        }
        int getPre(int val) {
            split(root, val - 1, x, y);
            int cur = x;
            while (tree[cur].right) {
                cur = tree[cur].right;
            }
            int ans = tree[cur].val;
            root = merge(x, y);
            return ans;
        }
        int getNxt(int val) {
            split(root, val, x, y);
            int cur = y;
            while (tree[cur].left) {
                cur = tree[cur].left;
            }
            int ans = tree[cur].val;
            root = merge(x, y);
            return ans;
        }
    };
};

namespace SegTreap {
    static int cnt = 0, root = 0;
    static int x = 0, y = 0, z = 0;
    std::mt19937 rnd(233);

    class SegTreap {
        struct Node {
            int left, right;
            int val, key;
            int size;
            bool isReverse;
        };

        vector<Node> tree;

    public:
        SegTreap() = default;
        SegTreap(int size) : tree(size) {}

    private:
        void update(int cur) { tree[cur].size = tree[tree[cur].left].size + tree[tree[cur].right].size + 1; }

        void push(int now) {
            swap(tree[now].left, tree[now].right);
            tree[tree[now].left].isReverse ^= 1;
            tree[tree[now].right].isReverse ^= 1;
            tree[now].isReverse = false;
        }

        void split(int cur, int size, int& x, int& y) {
            if (!cur) {
                x = y = 0;
            } else {
                if (tree[cur].isReverse) {
                    push(cur);
                }
                if (tree[tree[cur].left].size < size) {
                    x = cur;
                    split(tree[cur].right, size - tree[tree[cur].left].size - 1, tree[cur].right, y);
                } else {
                    y = cur;
                    split(tree[cur].left, size, x, tree[cur].left);
                }
                update(cur);
            }
        }

        // 中序遍历
        vector<int> Inorder;
        void InorderTraversal(int cur) {
            if (!cur) {
                return;
            }
            if (tree[cur].isReverse) {
                push(cur);
            }
            InorderTraversal(tree[cur].left);
            Inorder.emplace_back(tree[cur].val);
            InorderTraversal(tree[cur].right);
        }

    public:
        int newNode(int val) {
            cnt++;
            tree[cnt].val = val;
            tree[cnt].key = rnd();
            tree[cnt].size = 1;
            return cnt;
        }
        // x 树中的值不大于 y 树中的值
        int merge(int x, int y) {
            if (!x || !y) {
                return x + y;
            }
            if (tree[x].key < tree[y].key) {
                if (tree[x].isReverse) {
                    push(x);
                }
                tree[x].right = merge(tree[x].right, y);
                update(x);
                return x;
            } else {
                if (tree[y].isReverse) {
                    push(y);
                }
                tree[y].left = merge(x, tree[y].left);
                update(y);
                return y;
            }
        }
        // [l >= 1, r]
        void reverse(int l, int r) {
            int x, y, z;
            split(root, l - 1, x, y);
            split(y, r - l + 1, y, z);
            tree[y].isReverse ^= 1;
            root = merge(merge(x, y), z);
        }

        vector<int> snapshot(int cur = root) {
            Inorder.clear();
            InorderTraversal(cur);
            return Inorder;
        }
    };
};

namespace AVLTree {
    struct Node {
        Node* ch[2] {};
        int height = 0, val = 0;

    public:
        Node() : height(0), val(0), ch {} {}

        void maintain() { height = max(ch[0]->height, ch[1]->height) + 1; }
    };

    Node* nil = new Node;

    void rotate(Node*& o, int d) {
        Node* k = o->ch[d ^ 1];
        o->ch[d ^ 1] = k->ch[d];
        k->ch[d] = o;
        o->maintain();
        k->maintain();
        o = k;
    }
    Node* Insert(int X, Node* T) {
        if (T == nil) {
            T = new Node;
            T->val = X;
            T->height = 0;
            T->ch[0] = T->ch[1] = nil;
        } else if (X < T->val) {
            T->ch[0] = Insert(X, T->ch[0]);
            if (T->ch[0]->height - T->ch[1]->height == 2) {
                if (X >= T->ch[0]->val) {
                    rotate(T->ch[0], 0);
                }
                rotate(T, 1);
            }
        } else if (X > T->val) {
            T->ch[1] = Insert(X, T->ch[1]);
            if (T->ch[1]->height - T->ch[0]->height == 2) {
                if (X <= T->ch[1]->val) {
                    rotate(T->ch[1], 1);
                }
                rotate(T, 0);
            }
        }
        T->height = max(T->ch[0]->height, T->ch[1]->height) + 1;
        return T;
    }

    int main() {
        int n;
        cin >> n;

        Node* root = nil;
        for (int i = 0; i < n; ++i) {
            int val;
            cin >> val;
            root = Insert(val, root);
        }
        cout << root->val << "\n";

        return 0;
    }
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

namespace SplayJiangly {
    struct Node {
        Node *l = nullptr;
        Node *r = nullptr;
        int cnt = 0;
        i64 sum = 0;
    };

    Node *add(Node *t, int l, int r, int p, int v) {
        Node *x = new Node;
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

    int find(Node *tl, Node *tr, int l, int r, int x) {
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

    std::pair<int, i64> get(Node *t, int l, int r, int x, int y) {
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

    struct Tree {
        int add = 0;
        int val = 0;
        int id = 0;
        Tree *ch[2] = {};
        Tree *p = nullptr;
    };

    int pos(Tree *t) {
        return t->p->ch[1] == t;
    }

    void add(Tree *t, int v) {
        t->val += v;
        t->add += v;
    }

    void push(Tree *t) {
        if (t->ch[0]) {
            add(t->ch[0], t->add);
        }
        if (t->ch[1]) {
            add(t->ch[1], t->add);
        }
        t->add = 0;
    }

    void rotate(Tree *t) {
        Tree *q = t->p;
        int x = !pos(t);
        q->ch[!x] = t->ch[x];
        if (t->ch[x]) t->ch[x]->p = q;
        t->p = q->p;
        if (q->p) q->p->ch[pos(q)] = t;
        t->ch[x] = q;
        q->p = t;
    }

    void splay(Tree *t) {
        std::vector<Tree *> s;
        for (Tree *i = t; i->p; i = i->p) s.push_back(i->p);
        while (!s.empty()) {
            push(s.back());
            s.pop_back();
        }
        push(t);
        while (t->p) {
            if (t->p->p) {
                if (pos(t) == pos(t->p)) rotate(t->p);
                else rotate(t);
            }
            rotate(t);
        }
    }

    void insert(Tree *&t, Tree *x, Tree *p = nullptr) {
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

    void dfs(Tree *t) {
        if (!t) {
            return;
        }
        push(t);
        dfs(t->ch[0]);
        std::cerr << t->val << " ";
        dfs(t->ch[1]);
    }

    std::pair<Tree *, Tree *> split(Tree *t, int x) {
        if (!t) {
            return {t, t};
        }
        Tree *v = nullptr;
        Tree *j = t;
        for (Tree *i = t; i; ) {
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

        Tree *u = v->ch[0];
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

    Tree *merge(Tree *l, Tree *r) {
        if (!l) {
            return r;
        }
        if (!r) {
            return l;
        }
        Tree *i = l;
        while (i->ch[1]) {
            i = i->ch[1];
        }
        splay(i);
        i->ch[1] = r;
        r->p = i;
        return i;
    }
};
