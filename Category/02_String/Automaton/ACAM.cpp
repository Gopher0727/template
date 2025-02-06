// AC 自动机    Aho–Corasick Automaton (ACAM) / Deterministic Finite Automaton (DFA)
// trie + KMP
// 如果我们既知道前缀信息（trie），又知道后缀信息（fail），就可以做字符串匹配：
// 前缀的后缀就是子串，只要遍历到所有前缀，对每个前缀做「后缀匹配」，就完成了字符串匹配（统计子串出现次数）

// 视频：
// https://www.bilibili.com/video/BV1oAPyeWEBs/
// https://www.bilibili.com/video/BV1Sy421a78C/

// 可视化：
// https://wiomoc.github.io/aho-corasick-viz/


// 从根到 node 的字符串，也是某个 words[i] 的前缀
// 从根到 node 的字符串的长度，也是 node 在 trie 中的深度
// 第一层的失配指针 fail，都指向根节点 ∅
// 当 o.son[i] 不能匹配文本串 s 中的某个字符时，o.fail.son[i] 即为下一个待匹配节点（等于 root 则表示没有匹配）
// 虚拟子节点 o.son[i]，和 o.fail.son[i] 是同一个，方便失配时直接跳到下一个可能匹配的位置（但不一定是某个 words[k] 的最后一个字母）

// cnt 表示（子树中）完整字符串的个数


// 非指针实现
//
static constexpr int ALPHABET = 26;
struct Node {
    int len = 0;
    int fail = 0;
    int cnt = 0;
    array<int, ALPHABET> next {};
};
vector<Node> t;
class AhoCorasick {
public:
    AhoCorasick() { init(); }

    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
    }

    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    int put(const string& a) {
        int p = 1;
        for (auto c : a) {
            int x = c - 'a';
            if (t[p].next[x] == 0) {
                t[p].next[x] = newNode();
                t[t[p].next[x]].len = t[p].len + 1;
            }
            p = t[p].next[x];
        }
        t[p].cnt++;
        return p;
    }

    void work() {
        queue<int> q;
        q.push(1);
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (int i = 0; i < ALPHABET; i++) {
                if (t[x].next[i] == 0) {
                    t[x].next[i] = t[t[x].fail].next[i];
                } else {
                    t[t[x].next[i]].fail = t[t[x].fail].next[i];
                    q.push(t[x].next[i]);
                }
            }
        }
    }

    // 返回在文本串 s 中出现的模式串的数量（编号不同，则模式串不同）
    int query(const string& s) {
        int ans = 0;
        int p = 1;
        for (char ch : s) {
            p = t[p].next[ch - 'a'];
            for (int f = p; f != 1 && t[f].cnt >= 0; f = t[f].fail) {
                ans += t[f].cnt;
                t[f].cnt = -1; // visited
            }
        }
        return ans;
    }
} ac;


// 指针实现
//
static constexpr int ALPHABET = 26;
struct Node {
    Node* son[ALPHABET] {};
    Node* fail = nullptr;
    Node* last = nullptr;
    int len;
    int cnt;

    explicit Node(int len = 0, int cnt = 0) : len(len), cnt(cnt) {}
};
struct AhoCorasick {
    Node* root = new Node();

    void put(string& s) {
        auto o = root;
        for (char ch : s) {
            if (o->son[ch - 'a'] == nullptr) {
                o->son[ch - 'a'] = new Node(o->len + 1);
            }
            o = o->son[ch - 'a'];
        }
        o->cnt++;
    }

    void work() {
        root->fail = root;
        root->last = root;
        queue<Node*> q;
        for (int i = 0; i < ALPHABET; i++) {
            auto& son = root->son[i];
            if (son == nullptr) {
                son = root;
            } else {
                son->fail = root;
                son->last = root;
                q.push(son);
            }
        }
        while (!q.empty()) {
            auto o = q.front();
            q.pop();
            for (int i = 0; i < ALPHABET; i++) {
                auto& son = o->son[i];
                if (son == nullptr) {
                    son = o->fail->son[i];
                    continue;
                }
                son->fail = o->fail->son[i]; // 计算失配位置
                if (son->fail->cnt > 0) {
                    son->last = son->fail;
                } else {
                    son->last = son->fail->last;
                }
                q.push(son);
            }
        }
    }

    // 返回在文本串 s 中出现的模式串的数量（编号不同，则模式串不同）
    int query(const string& s) {
        auto o = root;
        int ans = 0;
        for (char ch : s) {
            o = o->son[ch - 'a'];
            for (auto f = o; f != root && f->cnt >= 0; f = f->fail) {
                ans += f->cnt;
                f->cnt = -1; // 访问标记
            }
        }
        return ans;
    }
};


static constexpr int ALPHABET = 26;
struct Node {
    Node* son[ALPHABET] {};
    Node* fail = nullptr;
    int cnt = 0;
    int idx = 0;
};
struct AhoCorasick {
    Node* root = new Node();
    map<Node*, int> inDeg;

    int ord(char ch) { return ch - 'a'; }

    void put(const string& s, int idx) {
        auto o = this->root;
        for (char ch : s) {
            int i = ord(ch);
            if (o->son[i] == nullptr) {
                o->son[i] = new Node();
                this->inDeg[o->son[i]] = 0;
            }
            o = o->son[i];
        }
        o->idx = ++idx;
    }

    void work() {
        this->root->fail = this->root;
        queue<Node*> q;
        for (int i = 0; i < ALPHABET; i++) {
            auto& son = this->root->son[i];
            if (son == nullptr) {
                son = this->root;
            } else {
                son->fail = this->root;
                q.push(son);
            }
        }
        while (!q.empty()) {
            auto o = q.front();
            q.pop();
            for (int i = 0; i < ALPHABET; i++) {
                auto& son = o->son[i];
                if (son == nullptr) {
                    son = o->fail->son[i];
                    continue;
                }
                son->fail = o->fail->son[i]; // 计算失配位置
                this->inDeg[son->fail]++;
                q.push(son);
            }
        }
    }

    // 返回一个 ans 列表，其中 ans[i] 表示 patterns[i] 的在文本串 text 的出现次数（未找到时为 0）
    //! 不存在相同的模式串
    auto SearchCount(int n, const string& s) {
        auto o = this->root;
        for (char ch : s) {
            o = o->son[ord(ch)];
            // 本来应该像上面那样一路找到 t.root，但这样太慢了
            // 可以先打个标记，然后在 fail 树上跑拓扑序一起统计
            o->cnt++;
        }

        vector<int> ans(n);
        auto deg = this->inDeg;
        queue<Node*> q;
        for (auto& [v, d] : deg) {
            if (d == 0) {
                q.push(v);
            }
        }
        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            if (v->idx > 0) {
                ans[v->idx - 1] = v->cnt;
            }
            auto w = v->fail;
            w->cnt += v->cnt;
            if (--deg[w] == 0) {
                q.push(w);
            }
        }
        return ans;
    }
};
