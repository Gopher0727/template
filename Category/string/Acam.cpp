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

/*
static constexpr int nodeSize = 26;

struct Node {
    Node* children[nodeSize] {};
    Node* fail;
    Node* last; // 后缀链接（suffix link）
    int idx; // isEnd / 字符串长度（节点深度）
    int cnt; // （子树中）完整字符串的个数
    int nodeId; // 用于构建 fail 树
};

struct gInfo {
    int l, r; // [l, r]
};

struct acam {
public:
    vector<string> patterns; // 额外保存，方便 debug

    Node* root;
    int nodeCnt;

    vector<vector<int>> g; // fail 树
    vector<gInfo> gInfo;
    int dfn;

    map<Node*, int> inDeg; // 求拓扑序

public:

};

acam* newAcam(vector<string> patterns) {
    auto ac = &acam;
    ac.patterns = patterns, ac.root = &Node{}, ac.nodeCnt = 1, ac.inDeg = map<Node*, int>();
    for (int i = 0; i < patterns.size(); ++i) {
        ac.insert(patterns[i], i + 1); // !
    }
    ac.buildFail();
    return ac;
};
*/
