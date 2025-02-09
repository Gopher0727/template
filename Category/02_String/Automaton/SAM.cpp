static constexpr int ALPHABET_SIZE = 26;
struct Node {
    array<int, ALPHABET_SIZE> next {};
    int fail = 0;
    int len = 0;
    int cnt = 0;
};

vector<Node> t;

struct SAM {
public:
    SAM() { init(); }

    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
    }

    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    int insert(int p, int c) {
        if (t[p].next[c]) {
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1) {
                return q;
            }
            int r = newNode();
            t[r].next = t[q].next;
            t[r].fail = t[q].fail;
            t[r].len = t[p].len + 1;
            t[r].cnt = 0;
            t[q].fail = r;
            while (t[p].next[c] == q) {
                t[p].next[c] = r;
                p = t[p].fail;
            }
            return r;
        }
        int cur = newNode();
        t[cur].len = t[p].len + 1;
        t[cur].cnt = 1;
        while (!t[p].next[c]) {
            t[p].next[c] = cur;
            p = t[p].fail;
        }
        t[cur].fail = insert(p, c);
        return cur;
    }
};
