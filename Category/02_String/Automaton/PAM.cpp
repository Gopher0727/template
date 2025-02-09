static constexpr int ALPHABET_SIZE = 26;
struct Node {
    array<int, ALPHABET_SIZE> next {};
    int fail = 0;
    int len = 0;
    int cnt = 0;
};

vector<Node> t;
string s;
int endIdx;

struct PAM {
public:
    PAM() { init(); }

    void init() {
        t.assign(2, Node());
        t[0].len = -1;
        s.clear();
        endIdx = 1;
    }

    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    bool insert(char ch) {
        int pos = s.size();
        s.push_back(ch);
        int x = ch - 'a';
        int cur = endIdx, curlen = 0;
        while (true) {
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                break;
            }
            cur = t[cur].fail;
        }
        if (t[cur].next[x]) {
            endIdx = t[cur].next[x];
            return false;
        }
        int num = newNode();
        endIdx = num;
        t[cur].next[x] = num;
        t[num].len = t[cur].len + 2;
        if (t[num].len == 1) {
            t[num].fail = 1;
            t[num].cnt = 1;
            return true;
        }
        while (true) {
            cur = t[cur].fail, curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                t[num].fail = t[cur].next[x];
                break;
            }
        }
        t[num].cnt = t[t[num].fail].cnt + 1;
        return true;
    }
};
