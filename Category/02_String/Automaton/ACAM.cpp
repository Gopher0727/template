// AC 自动机    Aho–Corasick Automaton (ACAM)
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


// 以下为数组实现：

static constexpr int ALPHABET = 26;
struct Node {
    array<int, ALPHABET> next {};
    int fail = 0;
    int len = 0;
    int cnt = 0; // （子树中）完整字符串的个数
};

vector<Node> t;
vector<int> endIdx;

struct AhoCorasick {
public:
    AhoCorasick() { init(); }

    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
        endIdx.clear();
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
        endIdx.push_back(p);
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

    // 返回模式串在文本串 s 中的出现次数（未找到时为 0）
    auto query(const string& s) {
        int p = 1;
        for (char ch : s) {
            // 本来应该像上面那样一路找到 t.root，但这样太慢了
            // 可以先打个标记，然后在 fail 树上跑拓扑序一起统计
            p = t[p].next[ch - 'a'];
            t[p].cnt++;
        }

        int n = t.size();
        vector<int> inDeg(n);
        for (int u = 0; u < n; u++) {
            inDeg[t[u].fail]++;
        }

        queue<int> q;
        for (int u = 0; u < n; u++) {
            if (inDeg[u] == 0) {
                q.push(u);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            int v = t[u].fail;
            t[v].cnt += t[u].cnt;
            if (--inDeg[v] == 0) {
                q.push(v);
            }
        }

        vector<int> ans;
        for (int& v : endIdx) {
            ans.push_back(t[v].cnt);
        }
        return ans;
    }
};
