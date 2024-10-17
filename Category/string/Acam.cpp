// Aho–Corasick Automaton (ACAM) / Deterministic Finite Automaton (DFA)

// 可视化：
// https://wiomoc.github.io/aho-corasick-viz/

/*
    如果我们既知道前缀信息（trie），又知道后缀信息（fail），就可以做字符串匹配：
    前缀的后缀就是子串，只要遍历到所有前缀，对每个前缀做「后缀匹配」，就完成了字符串匹配（统计子串出现次数）
*/

// 从根到 node 的字符串，也是某个 words[i] 的前缀
struct Node {
    Node* son[26]{};
    Node* fail; // 当 cur.son[i] 不能匹配 target 中的某个字符时，cur.fail.son[i] 即为下一个待匹配节点（等于 root 则表示没有匹配）
    int len; // 从根到 node 的字符串的长度，也是 node 在 trie 中的深度

    Node(int len) : len(len) {}
};

struct AhoCorasick {
    Node* root = new Node(0);

    void put(string& s) {
        auto cur = root;
        for (char b : s) {
            b -= 'a';
            if (cur->son[b] == nullptr) {
                cur->son[b] = new Node(cur->len + 1);
            }
            cur = cur->son[b];
        }
    }

    void build_fail() {
        root->fail = root;
        queue<Node*> q;
        for (auto& son : root->son) {
            if (son == nullptr) {
                son = root;
            } else {
                son->fail = root; // 第一层的失配指针，都指向根节点 ∅
                q.push(son);
            }
        }
        // BFS
        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            for (int i = 0; i < 26; i++) {
                auto& son = cur->son[i];
                if (son == nullptr) {
                    // 虚拟子节点 cur.son[i]，和 cur.fail.son[i] 是同一个
                    // 方便失配时直接跳到下一个可能匹配的位置（但不一定是某个 words[k] 的最后一个字母）
                    son = cur->fail->son[i];
                    continue;
                }
                son->fail = cur->fail->son[i]; // 计算失配位置
                q.push(son);
            }
        }
    }
};

// 0x3f template
static constexpr int nodeSize = 26;

struct acamNode {
    acamNode* son[acamNodeSize];
    acamNode* fail;
    acamNode* last;
    int idx;
    int cnt;
    int nodeID;
};

struct gInfo {
    int l, r;
};

class acam {
public:
    vector<string> patterns;
    acamNode* root;
    int nodeCnt;
    vector<vector<int>> g;
    vector<gInfo> gInfo;
    int dfn;
    map<acamNode*, int> inDeg;

public:
    acam(const vector<string>& patterns) : patterns(patterns) {
        root = new acamNode();
        root->fail = root;
        root->last = root;
        root->nodeID = nodeCnt++;
        for (int i = 0; i < patterns.size(); ++i) {
            put(patterns[i], i + 1);
        }
        buildFail();
    }

    ~acam() {
        // Free memory
        queue<acamNode*> q;
        q.push(root);
        while (!q.empty()) {
            acamNode* node = q.front();
            q.pop();
            for (int i = 0; i < acamNodeSize; ++i) {
                if (node->son[i]) q.push(node->son[i]);
            }
            delete node;
        }
    }

public:
    void put(const string& s, int idx) {
        acamNode* o = root;
        for (char b : s) {
            int i = b - 'a';
            if (!o->son[i]) {
                o->son[i] = new acamNode();
                o->son[i]->nodeID = nodeCnt++;
            }
            o = o->son[i];
        }
        o->cnt++;
        o->idx = idx;
    }

    void buildFail() {
        queue<acamNode*> q;
        for (int i = 0; i < acamNodeSize; ++i) {
            if (root->son[i]) {
                root->son[i]->fail = root;
                root->son[i]->last = root;
                g[root->nodeID].push_back(root->son[i]->nodeID);
                q.push(root->son[i]);
            } else {
                root->son[i] = root;
            }
        }
        while (!q.empty()) {
            acamNode* o = q.front();
            q.pop();
            for (int i = 0; i < acamNodeSize; ++i) {
                if (o->son[i]) {
                    acamNode* f = o->fail->son[i];
                    o->son[i]->fail = f;
                    o->son[i]->last = f->cnt > 0 ? f : f->last;
                    g[f->nodeID].push_back(o->son[i]->nodeID);
                    q.push(o->son[i]);
                } else {
                    o->son[i] = o->fail->son[i];
                }
            }
        }
    }

    void buildDFN() {
        gInfo.resize(g.size());
        dfn = 0;
        function<void(int)> dfs = [&](int v) {
            dfn++;
            gInfo[v].l = dfn;
            for (int w : g[v]) {
                dfs(w);
            }
            gInfo[v].r = dfn;
        };
        dfs(root->nodeID);
    }

    int sumCountAllPatterns(const string& text) {
        int cnt = 0;
        acamNode* o = root;
        for (char b : text) {
            o = o->son[b - 'a'];
            for (acamNode* match = o; match != root; match = match->last) {
                if (match->cnt != -1) {
                    cnt += match->cnt;
                    match->cnt = -1;
                }
            }
        }
        return cnt;
    }

    vector<vector<int>> acSearch(const string& text) {
        vector<vector<int>> pos(patterns.size());
        acamNode* o = root;
        for (int i = 0; i < text.size(); ++i) {
            o = o->son[text[i] - 'a'];
            for (acamNode* match = o; match != root; match = match->last) {
                if (match->idx > 0) {
                    pos[match->idx - 1].push_back(i - patterns[match->idx - 1].size() + 1);
                }
            }
        }
        return pos;
    }

    vector<int> acSearchCount(const string& text) {
        acamNode* o = root;
        for (char b : text) {
            o = o->son[b - 'a'];
            o->cnt++;
        }

        vector<int> cnt(patterns.size(), 0);
        queue<acamNode*> q;
        for (auto& p : inDeg) {
            if (p.second == 0) q.push(p.first);
        }
        while (!q.empty()) {
            acamNode* v = q.front();
            q.pop();
            if (v->idx > 0) {
                cnt[v->idx - 1] = v->cnt;
            }
            acamNode* w = v->fail;
            w->cnt += v->cnt;
            if (--inDeg[w] == 0) {
                q.push(w);
            }
        }
        return cnt;
    }

    void debug(const string& text) {
        cout << "text: " << text << endl;
        cout << "patterns (下面简称 p)" << endl;
        for (int i = 0; i < patterns.size(); ++i) {
            cout << i << ": " << patterns[i] << endl;
        }

        acamNode* o = root;
        for (int i = 0; i < text.size(); ++i) {
            o = o->son[text[i] - 'a'];
            int cnt = 0;
            for (acamNode* f = o; f != root; f = f->fail) {
                if (f->idx > 0) cnt++;
            }
            if (cnt == 0) continue;

            cout << endl;
            cout << text << endl;
            cout << string(i, ' ');
            cout << "^ i=" << i << endl;
            cout << "找到 " << cnt << " 个模式串" << endl;

            for (acamNode* f = o; f != root; f = f->fail) {
                if (f->idx == 0) continue;
                int pIdx = f->idx - 1;
                cout << "p[" << pIdx << "]=" << patterns[pIdx] << endl;
            }
        }
    }
};



// jiangly template
struct AhoCorasick {
    static constexpr int ALPHABET = 26;
    struct Node {
        int len;
        int link;
        std::array<int, ALPHABET> next;
        Node() : len{0}, link{0}, next{} {}
    };

    std::vector<Node> t;

    AhoCorasick() {
        init();
    }

    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
    }

    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    int add(const std::string &a) {
        int p = 1;
        for (auto c : a) {
            int x = c - 'a';
            if (t[p].next[x] == 0) {
                t[p].next[x] = newNode();
                t[t[p].next[x]].len = t[p].len + 1;
            }
            p = t[p].next[x];
        }
        return p;
    }

    void work() {
        std::queue<int> q;
        q.push(1);

        while (!q.empty()) {
            int x = q.front();
            q.pop();

            for (int i = 0; i < ALPHABET; i++) {
                if (t[x].next[i] == 0) {
                    t[x].next[i] = t[t[x].link].next[i];
                } else {
                    t[t[x].next[i]].link = t[t[x].link].next[i];
                    q.push(t[x].next[i]);
                }
            }
        }
    }

    int next(int p, int x) {
        return t[p].next[x];
    }

    int link(int p) {
        return t[p].link;
    }

    int len(int p) {
        return t[p].len;
    }

    int size() {
        return t.size();
    }
};
