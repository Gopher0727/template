template <class Info, class Tag, class DynNode>
class DynamicSegTree {
private:
    DynNode* root;
    int L, R;

    void apply(DynNode* node, const Tag& t) { node->tag.apply(t), node->info.apply(t); }

    void pull(DynNode* node) {
        if (node->left && node->right) {
            node->info = node->left->info + node->right->info;
        }
    }

    void push(DynNode* node) {
        if (node->left == nullptr) {
            node->left = new DynNode(node->l, node->mid);
        }
        if (node->right == nullptr) {
            node->right = new DynNode(node->mid + 1, node->r);
        }
        apply(node->left, node->tag), apply(node->right, node->tag), node->tag = Tag();
    }

public:
    DynamicSegTree(int L, int R) : L(L), R(R) { root = new DynNode(L, R); }

    void modify(int L, int R, const Tag& v, DynNode* node) {
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

    Info query(int L, int R, DynNode* node) {
        if (L <= node->l && node->r <= R) {
            return node->info;
        }
        push(node);
        Info res {};
        // todo
        if (L <= node->mid) {
            res = res + query(L, R, node->left);
        }
        if (R > node->mid) {
            res = res + query(L, R, node->right);
        }
        return res;
    }
    Info query(int L, int R) { return query(L, R, root); }

    Info queryAll() { return query(L, R); }
};
struct Tag {
    i64 add = 0;
    Tag(i64 v = 0) : add(v) {}
    void apply(const Tag& t) {
        if (t.add) {
            add += t.add;
        }
    }
};
struct Info {
    i64 sum, len;
    // todo
    void apply(const Tag& t) {
        if (t.add) {
            sum += len * t.add;
            // todo
        }
    }
};
Info operator+(const Info& a, const Info& b) {
    Info info;
    info.sum = a.sum + b.sum;
    info.len = a.len + b.len;
    // todo
    return info;
}
class DynNode {
public:
    DynNode* left = nullptr;
    DynNode* right = nullptr;
    int l = 0, r = 0, mid = 0;
    Info info;
    Tag tag;

    DynNode(int l, int r) : l(l), r(r), mid(l + (r - l) / 2) {
        info.sum = 0;
        info.len = r - l + 1;
        // todo
    }
};

static constexpr int N = 1E9 + 10;
using SegTree = DynamicSegTree<Info, Tag, DynNode>;
