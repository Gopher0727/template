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
};

struct SegTreap {
#define size(_) (_ == nullptr ? 0 : _->size)
    Node* root = nullptr;

    pair<Node*, Node*> split(Node* cur, int size) { // 按照树的大小划分
        if (cur == nullptr) return {nullptr, nullptr};
        cur->check_tag();
        if (size <= size(cur->ch[0])) {
            auto temp = split(cur->ch[0], size);
            cur->ch[0] = temp.second;
            cur->update_size();
            return {temp.first, cur};
        } else {
            auto temp = split(cur->ch[1], size - size(cur->ch[0]) - 1);
            cur->ch[1] = temp.first;
            cur->update_size();
            return {cur, temp.second};
        }
    }

    Node* merge(Node* sm, Node* bg) {
        if (sm == nullptr && bg == nullptr) return nullptr;
        if (sm != nullptr && bg == nullptr) return sm;
        if (sm == nullptr && bg != nullptr) return bg;
        sm->check_tag(), bg->check_tag();
        if (sm->key < bg->key) {
            sm->ch[1] = merge(sm->ch[1], bg);
            sm->update_size();
            return sm;
        } else {
            bg->ch[0] = merge(sm, bg->ch[0]);
            bg->update_size();
            return bg;
        }
    }

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

    void reverse(int l, int r) { // 这里的 less 和 more 是相对于 l 的
        auto less = split(root, l - 1);
        // 所有小于等于 l - 1 的会在 less 的左边
        auto more = split(less.second, r - l + 1);
        // 拿出从 l 开始的前 r - l + 1 个
        more.first->isReverse = true;
        root = merge(less.first, merge(more.first, more.second));
    }

    void print(Node* cur) {
        if (cur == nullptr) return;
        cur->check_tag();
        print(cur->ch[0]);
        cout << cur->val << " ";
        print(cur->ch[1]);
    }
#undef size
};
