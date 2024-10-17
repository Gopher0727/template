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
