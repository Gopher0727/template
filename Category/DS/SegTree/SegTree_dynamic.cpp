class Node {
public:
    Node* left = nullptr;
    Node* right = nullptr;
    int l = 0, r = 0, mid = 0;
    int v = 0, add = 0;

    Node(int l, int r) { this->l = l, this->r = r, this->mid = l + (r - l) / 2; }
};

class SegTree {
private:
    Node* root;

    void pull(Node* node) { node->v = max(node->left->v, node->right->v); }

    void push(Node* node) {
        if (node->left == nullptr) {
            node->left = new Node(node->l, node->mid);
        }
        if (node->right == nullptr) {
            node->right = new Node(node->mid + 1, node->r);
        }
        if (node->add) {
            Node* left = node->left;
            Node* right = node->right;
            left->v = node->add, right->v = node->add;
            left->add = node->add, right->add = node->add, node->add = 0;
        }
    }

    void update(int l, int r, int v, Node* node) {
        if (l > r) {
            return;
        }
        if (node->l >= l && node->r <= r) {
            node->v = node->add = v;
            return;
        }
        push(node);
        if (l <= node->mid) {
            update(l, r, v, node->left);
        }
        if (r > node->mid) {
            update(l, r, v, node->right);
        }
        pull(node);
    }

    int query(int l, int r, Node* node) {
        if (l > r) {
            return 0;
        }
        if (node->l >= l && node->r <= r) {
            return node->v;
        }
        push(node);
        int v = 0;
        if (l <= node->mid) {
            v = max(v, query(l, r, node->left));
        }
        if (r > node->mid) {
            v = max(v, query(l, r, node->right));
        }
        return v;
    }

public:
    SegTree() { root = new Node(1, 1e9); }

    void update(int l, int r, int v) { return update(l, r, v, root); }

    int query(int l, int r) { return query(l, r, root); }
};
