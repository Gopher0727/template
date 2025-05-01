struct Node {
    int val; // 节点值
    int count; // 当前节点重复次数
    int size; // 以当前节点为根的子树的节点总数
    long long sum; // 以当前节点为根的子树的节点值之和
    bool color; // 颜色：true为红色，false为黑色
    Node *left, *right, *parent;

    Node(int x) {
        val = x;
        count = 1;
        size = 1;
        sum = x;
        color = true; // 新节点默认为红色
        left = right = parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;
    Node* nil; // 哨兵节点

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;

        // 更新size和sum
        y->size = x->size;
        x->size = x->count + (x->left != nil ? x->left->size : 0) + (x->right != nil ? x->right->size : 0);
        y->sum = x->sum;
        x->sum = (long long) x->val * x->count + (x->left != nil ? x->left->sum : 0) + (x->right != nil ? x->right->sum : 0);
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nil) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nil) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;

        // 更新size和sum
        x->size = y->size;
        y->size = y->count + (y->left != nil ? y->left->size : 0) + (y->right != nil ? y->right->size : 0);
        x->sum = y->sum;
        y->sum = (long long) y->val * y->count + (y->left != nil ? y->left->sum : 0) + (y->right != nil ? y->right->sum : 0);
    }

    void insertFixup(Node* z) {
        while (z->parent->color) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color) {
                    z->parent->color = false;
                    y->color = false;
                    z->parent->parent->color = true;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = false;
                    z->parent->parent->color = true;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color) {
                    z->parent->color = false;
                    y->color = false;
                    z->parent->parent->color = true;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = false;
                    z->parent->parent->color = true;
                    leftRotate(z->parent->parent);
                }
            }
            if (z == root) break;
        }
        root->color = false;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* minimum(Node* x) {
        while (x->left != nil) {
            x = x->left;
        }
        return x;
    }

    void deleteFixup(Node* x) {
        while (x != root && !x->color) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color) {
                    w->color = false;
                    x->parent->color = true;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (!w->left->color && !w->right->color) {
                    w->color = true;
                    x = x->parent;
                } else {
                    if (!w->right->color) {
                        w->left->color = false;
                        w->color = true;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = false;
                    w->right->color = false;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color) {
                    w->color = false;
                    x->parent->color = true;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (!w->right->color && !w->left->color) {
                    w->color = true;
                    x = x->parent;
                } else {
                    if (!w->left->color) {
                        w->right->color = false;
                        w->color = true;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = false;
                    w->left->color = false;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = false;
    }

    void updateSizeAndSum(Node* x) {
        while (x != nil) {
            x->size = x->count + (x->left != nil ? x->left->size : 0) + (x->right != nil ? x->right->size : 0);
            x->sum = (long long) x->val * x->count + (x->left != nil ? x->left->sum : 0) + (x->right != nil ? x->right->sum : 0);
            x = x->parent;
        }
    }

public:
    RedBlackTree() {
        nil = new Node(0);
        nil->color = false;
        nil->size = 0;
        nil->sum = 0;
        root = nil;
    }

    void insert(int val) {
        Node* y = nil;
        Node* x = root;
        while (x != nil) {
            y = x;
            if (val == x->val) {
                x->count++;
                updateSizeAndSum(x);
                return;
            }
            if (val < x->val) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        Node* z = new Node(val);
        z->left = z->right = nil;
        z->parent = y;

        if (y == nil) {
            root = z;
        } else if (val < y->val) {
            y->left = z;
        } else {
            y->right = z;
        }

        updateSizeAndSum(z);
        insertFixup(z);
    }

    void remove(int val) {
        Node* z = root;
        while (z != nil) {
            if (val == z->val) {
                if (z->count > 1) {
                    z->count--;
                    updateSizeAndSum(z);
                    return;
                }
                break;
            }
            if (val < z->val) {
                z = z->left;
            } else {
                z = z->right;
            }
        }
        if (z == nil) return;

        Node* y = z;
        Node* x;
        bool y_original_color = y->color;

        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
            updateSizeAndSum(x);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
            updateSizeAndSum(x);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;

            y->count = z->count;
            updateSizeAndSum(y);
        }

        delete z;
        if (!y_original_color) {
            deleteFixup(x);
        }
    }

    // 查询小于等于x的元素个数
    int countLessEqual(int x) {
        int result = 0;
        Node* current = root;

        while (current != nil) {
            if (current->val == x) {
                result += current->count + (current->left != nil ? current->left->size : 0);
                break;
            } else if (current->val < x) {
                result += current->count + (current->left != nil ? current->left->size : 0);
                current = current->right;
            } else {
                current = current->left;
            }
        }

        return result;
    }

    // 查询小于等于x的元素和
    long long sumLessEqual(int x) {
        long long result = 0;
        Node* current = root;

        while (current != nil) {
            if (current->val == x) {
                result += (long long) current->val * current->count + (current->left != nil ? current->left->sum : 0);
                break;
            } else if (current->val < x) {
                result += (long long) current->val * current->count + (current->left != nil ? current->left->sum : 0);
                current = current->right;
            } else {
                current = current->left;
            }
        }

        return result;
    }
};
