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
};

struct Treap {
    Node* root = nullptr;

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
