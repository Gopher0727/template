template <class Info, class Tag>
class Node {
public:
    Node* left = nullptr;
    Node* right = nullptr;
    int l = 0, r = 0, mid = 0;
    Info info;
    Tag tag;

    Node(int l, int r) : l(l), r(r), mid(l + (r - l) / 2) { info.len = r - l + 1; }
};
template <class Info, class Tag, int N>
class DynamicSegTree {
    using _Node = Node<Info, Tag>;

private:
    _Node* root;

    void apply(_Node* node, const Tag& t) { node->tag.apply(t), node->info.apply(t); }

    void pull(_Node* node) {
        if (node->left && node->right) {
            node->info = node->left->info + node->right->info;
        }
    }

    void push(_Node* node) {
        if (node->left == nullptr) {
            node->left = new _Node(node->l, node->mid);
        }
        if (node->right == nullptr) {
            node->right = new _Node(node->mid + 1, node->r);
        }
        apply(node->left, node->tag), apply(node->right, node->tag), node->tag = Tag();
    }

public:
    DynamicSegTree(int L = 0, int R = N) { root = new _Node(L, R); }

    void modify(int L, int R, const Tag& v, _Node* node) {
        if (L <= node->l && node->r <= R) {
            apply(node, v);
            return;
        }
        push(node);
        if (L <= node->mid) {
            modify(L, R, v, node->left);
        }
        if (R > node->mid) {
            modify(L, R, v, node->right);
        }
        pull(node);
    }
    void modify(int L, int R, const Tag& v) { modify(L, R, v, root); }

    Info query(int L, int R, _Node* node) {
        if (L <= node->l && node->r <= R) {
            return node->info;
        }
        push(node);
        Info res;
        if (L <= node->mid) {
            res = res + query(L, R, node->left);
        }
        if (R > node->mid) {
            res = res + query(L, R, node->right);
        }
        return res;
    }
    Info query(int L, int R) { return query(L, R, root); }
    Info queryAll() { return query(0, N); }
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
    i64 sum = 0, len = 0, mx = 0;
    void apply(const Tag& t) {
        if (t.add) {
            sum += len * t.add;
            mx += t.add;
        }
    }
};
Info operator+(const Info& a, const Info& b) {
    Info info;
    info.sum = a.sum + b.sum;
    info.len = a.len + b.len;
    info.mx = max(a.mx, b.mx);
    return info;
}

static constexpr int N = 1E9 + 10;
using SegTree = DynamicSegTree<Info, Tag, N>;
