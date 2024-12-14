// 字典树（前缀树、单词查找树）

// Trie 是一棵 “非典型” 的多叉树模型
// TrieNode 中并没有直接保存字符值的数据成员，而是通过字母映射表 next 和一个父节点预知所有子节点

// 一般多叉树节点设计
template <typename T>
struct TreeNode {
    T value; // 节点值
    TreeNode* children[NUM]; // 指向子节点
};

// Trie 节点设计
struct TrieNode {
    bool isEnd; // 该节点是否是一个字符串的末尾
    TrieNode* next[26]; // 子节点映射（假定26个小写英文字母）
};

// 字典树应用：
//   · 检索字符串
//   · AC自动机（trie 是AC自动机的一部分）
//   · 维护异或极值 ———— 将数的二进制看作一个字符串，建构 01-trie
//   · 维护异或和

// 练习题目：
// https://atcoder.jp/contests/abc377/tasks/abc377_g

class Trie {
private:
    Trie* children[26] {};
    bool isEnd = false;

public:
    Trie() = default;

    void insert(string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[ch - 'a'] == nullptr) {
                node->children[ch - 'a'] = new Trie();
            }
            node = node->children[ch - 'a'];
        }
        node->isEnd = true;
    }
    bool find(string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[ch - 'a'] == nullptr) {
                return false;
            }
            node = node->children[ch - 'a'];
        }
        return node->isEnd;
    }
    bool startsWith(string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[ch - 'a'] == nullptr) {
                return false;
            }
            node = node->children[ch - 'a'];
        }
        return true;
    }
};

// 01-Trie
class Trie {
private:
    Trie* children[2] {};

public:
    Trie() = default;

    void insert(int val) {
        Trie* node = this;
        for (int k = 30; k >= 0; --k) {
            if (node->children[val >> k & 1] == nullptr) {
                node->children[val >> k & 1] = new Trie();
            }
            node = node->children[val >> k & 1];
        }
    }
    int query(int val) {
        Trie* node = this;
        int res = 0;
        for (int k = 30; k >= 0; --k) {
            int b = val >> k & 1;
            if (node->children[b ^ 1] == nullptr) {
                node = node->children[b];
            } else {
                node = node->children[b ^ 1];
                res += 1 << k;
            }
        }
        return res;
    }
};
