/*
    二叉树旋转之后，中序遍历的顺序不变。

    无旋：
        ScapegoatTree (替罪羊树)： 暴力重构
        fhq Treap
    有旋：
        AVL
        Splay
        红黑树
*/
namespace ScapegoatTree {
    // https://oi-wiki.org/ds/sgt/
    // https://riteme.site/blog/2016-4-6/scapegoat.html

    struct Node {
        int left, right;
        int val;
        int size;
        int realsize; // 以当前节点为根的树的实际大小（未被标记的节点数量）
        bool exist; // 存在标记
    };

    vector<Node> tree;
    int cnt = 0, root = 0;
    const double alpha = 0.75;

    void newNode(int& now, int val) {
        now = ++cnt;
        tree[now].val = val;
        tree[now].size = tree[now].realsize = 1;
        tree[now].exist = true;
    }

    bool imbalance(int now) {
        return (max(tree[tree[now].left].size, tree[tree[now].right].size) > tree[now].size * alpha || tree[now].size - tree[now].realsize > tree[now].size * 0.3);
    }

    // 中序遍历
    vector<int> Inorder;
    void InorderTraversal(int now) {
        if (!now) {
            return;
        }
        InorderTraversal(tree[now].left);
        if (tree[now].exist) {
            Inorder.emplace_back(now);
        }
        InorderTraversal(tree[now].right);
    }

    void lift(int l, int r, int& now) {
        if (l == r) {
            now = Inorder[l];
            tree[now].left = tree[now].right = 0;
            tree[now].size = tree[now].realsize = 1;
            return;
        }
        int m = l + (r - l) / 2;
        // 处理重复数据（一般将相同的数放到右子树）
        while (m && l < m && tree[Inorder[m]].val == tree[Inorder[m - 1]].val) {
            m--;
        }
        now = Inorder[m];
        if (l < m) {
            lift(l, m - 1, tree[now].left);
        } else {
            tree[now].left = 0;
        }
        lift(m + 1, r, tree[now].right);
        tree[now].size = tree[tree[now].left].size + tree[tree[now].right].size + 1;
        tree[now].realsize = tree[tree[now].left].realsize + tree[tree[now].right].realsize + 1;
    }

    // 暴力：中序遍历后分治建树
    void rebuild(int& now) {
        Inorder.clear();
        InorderTraversal(now);
        if (Inorder.empty()) {
            now = 0;
            return;
        }
        lift(0, Inorder.size() - 1, now);
    }

    void update(int& now, int end) {
        if (!now) {
            return;
        }
        if (tree[end].val < tree[now].val) {
            update(tree[now].left, end);
        } else {
            update(tree[now].right, end);
        }
        tree[now].size = tree[tree[now].left].size + tree[tree[now].right].size + 1;
    }

    // 需要重构的条件： 当前节点的左子树或右子树的大小大于当前节点的大小乘上平衡因子 alpha（0.5 <= alpha <= 1）
    void check(int& now, int end) {
        if (now == end) {
            return;
        }
        if (imbalance(now)) {
            rebuild(now);
            update(root, now);
            return;
        }
        if (tree[end].val < tree[now].val) {
            check(tree[now].left, end);
        } else {
            check(tree[now].right, end);
        }
    }

    void insert(int& now, int val) {
        if (!now) {
            newNode(now, val);
            check(root, now);
        } else {
            tree[now].size++;
            tree[now].realsize++;

            if (val < tree[now].val) {
                insert(tree[now].left, val);
            } else {
                insert(tree[now].right, val);
            }
        }
    }

    void del(int now, int val) { // 惰性删除（要求树中有该元素）
        if (tree[now].exist && tree[now].val == val) {
            tree[now].exist = false;
            tree[now].realsize--;
            check(root, now);
        } else {
            tree[now].realsize--;
            if (val < tree[now].val) {
                del(tree[now].left, val);
            } else {
                del(tree[now].right, val);
            }
        }
    }

    int getRank(int val) { // 比当前数小的数的个数加 1
        int now = root, rank = 1;
        while (now) {
            if (val <= tree[now].val) {
                now = tree[now].left;
            } else {
                rank += tree[now].exist + tree[tree[now].left].realsize;
                now = tree[now].right;
            }
        }
        return rank;
    }

    int getNum(int rank) {
        int now = root;
        while (now) {
            if (tree[now].exist && tree[tree[now].left].realsize + tree[now].exist == rank) {
                break;
            } else if (tree[tree[now].left].realsize >= rank) {
                now = tree[now].left;
            } else {
                rank -= tree[tree[now].left].realsize + tree[now].exist;
                now = tree[now].right;
            }
        }
        return tree[now].val;
    }

    int getPre(int x) {
        return getNum(getRank(x) - 1);
    }
    int getPost(int x) {
        return getNum(getRank(x + 1));
    }

    auto init = []() {
        const int MX = 1e5 + 5;
        return 0;
    }();
    tree.resize(MX);
};

namespace Treap {
    const int maxn = 5000000;
    struct Node {
        Node *ch[2]; // 左右子树
        int r; // 随机优先级
        int v; // 值
        int s; // 结点总数
        int cmp(int x) const {
            if (x == v) return -1;
            return x < v ? 0 : 1;
        }
        void maintain() {
            s = ch[0]->s + ch[1]->s + 1;
        }
    }Nodes[maxn], *cur, *nil;
    Node *newNode(int x)
    {
        Node *o = cur++;
        o->ch[0] = o->ch[1] = nil;
        o->r = rand();
        o->v = x;
        o->s = 1;
        return o;
    }
    void init()
    {
        cur = Nodes;
        nil = newNode(0);
        nil->s = 0;
    }
    void rotate(Node* &o, int d) {
        Node* k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
        o->maintain(); k->maintain(); o = k;
    }
    void insert(Node* &o, int x) {
        if(o == nil) o = newNode(x);
        else {
            int d = (x < o->v ? 0 : 1); // 不要用cmp函数，因为可能会有相同结点
            insert(o->ch[d], x);
            if(o->ch[d]->r > o->r) rotate(o, d^1);
        }
        o->maintain();
    }
    Node *find(Node* o, int x) {
        while (o != nil)
        {
            int d = o->cmp(x);
            if (d == -1) return o;
            else o = o->ch[d];
        }
        return nil;
    }
    // 要确保结点存在
    void remove(Node* &o, int x) {
        int d = o->cmp(x);
        if (d == -1)
        {
            if (o->ch[0] == nil)
                o = o->ch[1];
            else if (o->ch[1] == nil)
                o = o->ch[0];
            else
            {
                int d2 = (o->ch[0]->r > o->ch[1]->r ? 1 : 0);
                rotate(o, d2); remove(o->ch[d2], x);
            }
        }
        else
            remove(o->ch[d], x);
        if(o != nil) o->maintain();
    }
    int kth(Node* o, int k) {
        if(o == nil || k <= 0 || k > o->s) return 0;
        if(k == o->ch[0]->s+1) return o->v;
        else if(k <= o->ch[0]->s) return kth(o->ch[0], k);
        else return kth(o->ch[1], k - o->ch[0]->s - 1);
    }
    // 返回在以o为根的子树中x的排名，没有x则返回x的后继的排名
    int rank(Node* o, int x) {
        if(o == nil) return 1;
        if(x <= o->v) return rank(o->ch[0], x);
        else return rank(o->ch[1], x) + o->ch[0]->s + 1;
    }
    // 返回在以o为根的子树中x的前驱的排名
    int rank1(Node *o, int x)
    {
        if (o == nil)
            return 0;
        if (x <= o->v)
            return rank1(o->ch[0], x);
        else return rank1(o->ch[1], x) + o->ch[0]->s + 1;
    }
    // 返回在以o为根的子树中x的后继的排名
    int rank2(Node *o, int x)
    {
        if (o == nil)
            return 1;
        if (x >= o->v)
            return rank2(o->ch[1], x) + o->ch[0]->s + 1;
        else return rank2(o->ch[0], x);
    }
    /*
    对于rank、rank1、rank2的理解：
    给定序列A = {1, 2, 3, 3, 4, 4, 5}
    下标(排名):  1  2  3  4  5  6  7
    将序列A插入到平衡树root中。
    rank(root, 3):  因为只要x<=o->v就一直往左走，所以会走到排名2~3的位置，到达nil结点返回1，所以最外层rank返回值为3
    rank1(root, 3): 因为只要x<=o->v就一直往左走，所以会走到排名2~3的位置，到达nil结点返回0，所以最外层rank返回值为2
    rank2(root, 3): 因为只要x>=o->v就一直往右走，所以会走到排名4~5的位置，到达nil结点返回1，所以最外层rank返回值为5
    */

    int main() {
        init();
        Node *root = nil;
        int A[] = {1, 2, 3, 3, 4, 4, 5};
        for (unsigned i = 0; i < sizeof(A) / sizeof(*A); ++i)
            insert(root, A[i]);
        printf("%d\n", rank(root, 3));
        printf("%d\n", rank1(root, 3));
        printf("%d\n", rank2(root, 3));
        return 0;
    }
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
    const int maxn = 110000;
    //若要修改一个点的点权，应当先将其splay到根，然后修改，最后还要调用pushup维护。
    //调用完splay之后根结点会改变，应该用splay的返回值更新根结点。
    namespace splay_tree {
        int ch[maxn][2], fa[maxn], stk[maxn], rev[maxn], sz[maxn], key[maxn], cur;
        void init() {
            cur = 0;
        }
        int newNode(int val) {
            int x = ++cur;
            ch[x][0] = ch[x][1] = fa[x] = rev[x] = 0;
            sz[x] = 1;
            key[x] = val;
            return x;
        }
        inline bool son(int x) {
            return ch[fa[x]][1] == x;
        }
        inline void pushup(int x) {
            sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1;
        }
        inline void pushdown(int x) {
            if (rev[x]) {
                rev[x] = 0;
                swap(ch[x][0], ch[x][1]);
                rev[ch[x][0]] ^= 1;
                rev[ch[x][1]] ^= 1;
            }
        }
        void rotate(int x) {
            int y = fa[x], z = fa[y], c = son(x);
            if (fa[y])
                ch[z][son(y)] = x;
            fa[x] = z;
            ch[y][c] = ch[x][!c];
            fa[ch[y][c]] = y;
            ch[x][!c] = y;
            fa[y] = x;
            pushup(y);
        }
        void ascend(int x) {
            for (int y = fa[x]; y; rotate(x), y = fa[x]) if (fa[y])
                son(x) ^ son(y) ? rotate(x) : rotate(y);
            pushup(x);
        }
        int splay(int x) { //没有pushdown操作时，可以直接用ascend替换splay
            int top = 0;
            for (int i = x; i; i = fa[i])
                stk[++top] = i;
            while (top)
                pushdown(stk[top--]);
            ascend(x);
            return x;
        }
        int splay(int x, int k) { //将以x为根的子树中的第k个结点旋转到根结点
            while (pushdown(x), k != sz[ch[x][0]] + 1) {
                if (k <= sz[ch[x][0]])
                    x = ch[x][0];
                else
                    k -= sz[ch[x][0]] + 1, x = ch[x][1];
            }
            if (x) ascend(x);
            return x;
        }
        template<typename ...T> int merge(int x, int y, T... args) {
            if constexpr (sizeof...(args) == 0) {
                if (x == 0) return y; //swap(x, y);
                x = splay(x, sz[x]);
                ch[x][1] = y; fa[y] = x;
                pushup(x);
                return x;
            }
            else {
                return merge(merge(x, y), args...);
            }
        }
        auto split(int x, int pos) { //分成两个区间[1, pos - 1]和[pos, n]
            if (pos == sz[x] + 1)
                return make_pair(x, 0);
            x = splay(x, pos);
            int y = ch[x][0];
            fa[y] = ch[x][0] = 0;
            pushup(x);
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
                printf("%d ", key[x]);
                //printf("%d (left: %d, right: %d) sz(%d) key(%d)\n", x, ch[x][0], ch[x][1], sz[x], key[x]);
                traverse(ch[x][1]);
            }
        }
    }

    using namespace splay_tree;
    int main() {
        init();
        int nd[50], root = 0;
        for (int i = 1; i <= 10; ++i) {
            nd[i] = newNode(i);
            root = merge(root, nd[i]);
        }
        traverse(get<1>(extract(root, 3, 10))); printf("\n");
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
