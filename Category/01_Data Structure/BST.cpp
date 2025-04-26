//---------------------------------------------------------------------------------------
// 普通二叉搜索树
namespace BST {
    class BST {
        const int inf = 2147483647; // 标志

        struct Node {
            Node* left = 0;
            Node* right = 0;
            int val = 0, lcnt = 0, rcnt = 0;
            Node(int val) : val(val) {}
        }* root = nullptr;

    private:
        vector<int> Inorder;
        void dfs(Node* now) {
            if (!now) return;
            dfs(now->left);
            Inorder.push_back(now->val);
            dfs(now->right);
        }

        void insert(Node*& now, int key) {
            if (!now) {
                now = new Node(key);
                return;
            }
            if (now->val <= key) {
                insert(now->right, key);
                now->rcnt++;
            } else {
                insert(now->left, key);
                now->lcnt++;
            }
        }
        bool findKey(Node* now, int key) {
            if (!now) return false;
            if (now->val == key) return true;
            if (now->val < key) {
                return findKey(now->right, key);
            }
            return findKey(now->left, key);
        }
        int getNum(Node* now, int rank) {
            if (!now) return inf;
            int tmp = now->lcnt + 1;
            if (tmp == rank) return now->val;
            if (tmp < rank) {
                return getNum(now->right, (rank - tmp));
            }
            return getNum(now->left, rank);
        }
        int getRank(Node* now, int x, int K = 0) {
            if (!now) return K;
            if (now->val == x) return K + now->lcnt + 1;
            if (now->val < x) {
                return getRank(now->right, x, K + (now->lcnt + 1));
            }
            return getRank(now->left, x, K);
        }
        vector<int> snapshot(Node* now) {
            Inorder.clear();
            dfs(now);
            return Inorder;
        }

    public:
        void insert(int key) { insert(root, key); }
        bool findKey(int key) { return findKey(root, key); }
        int getNum(int x) { return getNum(root, x); }
        int getRank(int x) { return getRank(root, x); }
        int getPre(int key) {
            int ret = -inf;
            Node* p = root;
            while (p) {
                if (key > (p->val)) {
                    ret = p->val;
                    p = p->right;
                } else {
                    p = p->left;
                }
            }
            return ret;
        }
        int getNxt(int key) {
            int ret = inf;
            Node* p = root;
            while (p) {
                if (key < p->val) {
                    ret = p->val;
                    p = p->left;
                } else {
                    p = p->right;
                }
            }
            return ret;
        }
        vector<int> snapshot() { return snapshot(root); }
    };
};

//---------------------------------------------------------------------------------------
// 替罪羊树： 暴力重构
// 无旋，数组实现）
//    [link] https://oi-wiki.org/ds/sgt/
//    [link] https://riteme.site/blog/2016-4-6/scapegoat.html
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

//---------------------------------------------------------------------------------------
// fhq Treap： 分裂（按值分裂，按大小分裂） + 合并
//（无旋）
//   · 二叉搜索树 + 二叉堆（特点：父节点优先级高）
//   · 当作普通平衡树使用时，多采用按值分裂，维护区间信息时多采用按大小分裂
// SegTreap 区间操作（利用懒标记实现区间翻转）

// 数组实现
namespace Treap {
    static int cnt = 0, root = 0;
    static int x = 0, y = 0, z = 0;
    // std::mt19937 rnd(233);
    mt19937_64 rnd{random_device{}()};

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
    // std::mt19937 rnd(233);
    mt19937_64 rnd{random_device{}()};

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

    public:
        void insert(int i) {
            root = merge(root, newNode(i));
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

// 指针实现
namespace Treap {
    class Treap {
        struct Node {
            Node* ch[2] {};
            int val = 0;
            int key = rand();
            int cnt = 1;
            int size = 1;

            Node(int val) : val(val) {}
            Node(Node* node) : val(node->val), key(node->key), cnt(node->cnt), size(node->size) {}

            void update_size() {
                size = cnt;
                if (ch[0] != nullptr) {
                    size += ch[0]->size;
                }
                if (ch[1] != nullptr) {
                    size += ch[1]->size;
                }
            }
        }* root = nullptr;

    private:
        pair<Node*, Node*> split(Node* cur, int key) {
            if (cur == nullptr) return {nullptr, nullptr};
            if (cur->val <= key) {
                auto temp = split(cur->ch[1], key);
                cur->ch[1] = temp.first;
                cur->update_size();
                return {cur, temp.second};
            } else {
                auto temp = split(cur->ch[0], key);
                cur->ch[0] = temp.second;
                cur->update_size();
                return {temp.first, cur};
            }
        }

        tuple<Node*, Node*, Node*> split_rank(Node* cur, int rk) {
            if (cur == nullptr) return {nullptr, nullptr, nullptr};
            int ls_siz = cur->ch[0] == nullptr ? 0 : cur->ch[0]->size;
            if (rk <= ls_siz) {
                Node *l = nullptr, *mid = nullptr, *r = nullptr;
                tie(l, mid, r) = split_rank(cur->ch[0], rk);
                cur->ch[0] = r;
                cur->update_size();
                return {l, mid, cur};
            } else if (rk <= ls_siz + cur->cnt) {
                Node* lt = cur->ch[0];
                Node* rt = cur->ch[1];
                cur->ch[0] = cur->ch[1] = nullptr;
                return {lt, cur, rt};
            } else {
                Node *l = nullptr, *mid = nullptr, *r = nullptr;
                tie(l, mid, r) = split_rank(cur->ch[1], rk - ls_siz - cur->cnt);
                cur->ch[1] = l;
                cur->update_size();
                return {cur, mid, r};
            }
        }
        Node* merge(Node* u, Node* v) {
            if (u == nullptr && v == nullptr) return nullptr;
            if (u != nullptr && v == nullptr) return u;
            if (v != nullptr && u == nullptr) return v;
            if (u->key < v->key) {
                u->ch[1] = merge(u->ch[1], v);
                u->update_size();
                return u;
            } else {
                v->ch[0] = merge(u, v->ch[0]);
                v->update_size();
                return v;
            }
        }
        int getRank(Node* cur, int val) {
            auto temp = split(cur, val - 1);
            int ret = (temp.first == nullptr ? 0 : temp.first->size) + 1;
            root = merge(temp.first, temp.second);
            return ret;
        }
        int getVal(Node* cur, int rk) {
            Node *l, *mid, *r;
            tie(l, mid, r) = split_rank(cur, rk);
            int ret = mid->val;
            root = merge(merge(l, mid), r);
            return ret;
        }

    public:
        void insert(int val) {
            auto temp = split(root, val);
            auto l_tr = split(temp.first, val - 1);
            Node* new_node;
            if (l_tr.second == nullptr) {
                new_node = new Node(val);
            } else {
                l_tr.second->cnt++;
                l_tr.second->update_size();
            }
            Node* l_tr_combined = merge(l_tr.first, l_tr.second == nullptr ? new_node : l_tr.second);
            root = merge(l_tr_combined, temp.second);
        }
        void del(int val) {
            auto temp = split(root, val);
            auto l_tr = split(temp.first, val - 1);
            if (l_tr.second->cnt > 1) {
                l_tr.second->cnt--;
                l_tr.second->update_size();
                l_tr.first = merge(l_tr.first, l_tr.second);
            } else {
                if (temp.first == l_tr.second) {
                    temp.first = nullptr;
                }
                delete l_tr.second;
                l_tr.second = nullptr;
            }
            root = merge(l_tr.first, temp.second);
        }
        int getRank(int val) { return getRank(root, val); }
        int getVal(int rank) { return getVal(root, rank); }
        int getPre(int val) {
            auto temp = split(root, val - 1);
            int ret = getVal(temp.first, temp.first->size);
            root = merge(temp.first, temp.second);
            return ret;
        }
        int getNxt(int val) {
            auto temp = split(root, val);
            int ret = getVal(temp.second, 1);
            root = merge(temp.first, temp.second);
            return ret;
        }
    };
}
namespace SegTreap {
    class SegTreap {
    private:
        struct Node {
            Node* ch[2] {};
            int val = 0, key = rand(), cnt = 1, size = 1;
            bool isReverse = false; // 需要把这个子树下的每一个节点都翻转过来

            Node(int val) : val(val) {}
            Node(Node* node) : val(node->val), key(node->key), cnt(node->cnt), size(node->size) {}

            void update_size() {
                size = cnt;
                if (ch[0] != nullptr) size += ch[0]->size;
                if (ch[1] != nullptr) size += ch[1]->size;
            }
            void push() {
                swap(ch[0], ch[1]);
                if (ch[0] != nullptr) ch[0]->isReverse ^= 1;
                if (ch[1] != nullptr) ch[1]->isReverse ^= 1;
                isReverse = false;
            }
            void check_tag() {
                if (isReverse) push();
            }
        }* root = nullptr;

    private:
        pair<Node*, Node*> split(Node* cur, int size) { // 按照树的大小划分
            if (cur == nullptr) return {nullptr, nullptr};
            cur->check_tag();
            int s = cur->ch[0] == nullptr ? 0 : cur->ch[0]->size;
            if (size <= s) {
                auto temp = split(cur->ch[0], size);
                cur->ch[0] = temp.second;
                cur->update_size();
                return {temp.first, cur};
            } else {
                auto temp = split(cur->ch[1], size - s - 1);
                cur->ch[1] = temp.first;
                cur->update_size();
                return {cur, temp.second};
            }
        }
        Node* merge(Node* x, Node* y) {
            if (x == nullptr && y == nullptr) return nullptr;
            if (x != nullptr && y == nullptr) return x;
            if (x == nullptr && y != nullptr) return y;
            x->check_tag(), y->check_tag();
            if (x->key < y->key) {
                x->ch[1] = merge(x->ch[1], y);
                x->update_size();
                return x;
            } else {
                y->ch[0] = merge(x, y->ch[0]);
                y->update_size();
                return y;
            }
        }

        vector<int> Inorder;
        void dfs(Node* cur) {
            if (!cur) return;
            dfs(cur->ch[0]);
            Inorder.push_back(cur->val);
            dfs(cur->ch[1]);
        }
        vector<int> snapshot(Node* cur) {
            Inorder.clear();
            dfs(cur);
            return Inorder;
        }

    public:
        void insert(int val) {
            auto temp = split(root, val);
            auto l_tr = split(temp.first, val - 1);
            Node* new_node;
            if (l_tr.second == nullptr) {
                new_node = new Node(val);
            }
            Node* l_tr_combined = merge(l_tr.first, l_tr.second == nullptr ? new_node : l_tr.second);
            root = merge(l_tr_combined, temp.second);
        }
        void reverse(int l, int r) {
            auto less = split(root, l - 1);
            auto more = split(less.second, r - l + 1);
            more.first->isReverse = true;
            root = merge(less.first, merge(more.first, more.second));
        }
        vector<int> snapshot() { return snapshot(root); }
    };
}

//---------------------------------------------------------------------------------------
// 有旋：二叉树旋转之后，中序遍历的顺序不变。
//    > AVL （以下代码只实现了基础的插入操作）
//    > Splay
//    > 红黑树
namespace AVLTree {
    struct Node {
        Node* ch[2] {};
        int height = 0, val = 0;

        void update() { height = max(ch[0]->height, ch[1]->height) + 1; }
    };

    Node* nil = new Node;

    void rotate(Node*& o, int d) {
        Node* k = o->ch[d ^ 1];
        o->ch[d ^ 1] = k->ch[d];
        k->ch[d] = o;
        o->update();
        k->update();
        o = k;
    }
    Node* Insert(Node* T, int X) {
        if (T == nil) {
            T = new Node;
            T->val = X;
            T->height = 0;
            T->ch[0] = T->ch[1] = nil;
        } else if (X < T->val) {
            T->ch[0] = Insert(T->ch[0], X);
            if (T->ch[0]->height - T->ch[1]->height == 2) {
                if (X >= T->ch[0]->val) {
                    rotate(T->ch[0], 0);
                }
                rotate(T, 1);
            }
        } else if (X > T->val) {
            T->ch[1] = Insert(T->ch[1], X);
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

    void solve() {
        int n;
        cin >> n;

        Node* root = nil;
        for (int i = 0; i < n; ++i) {
            int val;
            cin >> val;
            root = Insert(root, val);
        }
        cout << root->val << "\n";
    }
};
