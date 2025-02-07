// 字典树（前缀树、单词查找树）

// Trie 是一棵 “非典型” 的多叉树模型
// TrieNode 中并没有直接保存字符值的数据成员，而是通过字母映射表 next 和一个父节点预知所有子节点

// 字典树应用：
//   · 检索字符串
//   · AC自动机（trie 是AC自动机的一部分）
//   · 维护异或极值 ———— 将数的二进制看作一个字符串，建构 01-trie
//   · 维护异或和

// 练习题目：
// https://atcoder.jp/contests/abc377/tasks/abc377_g

class Trie {
private:
    Trie* children[70] {}; // todo 根据 |Σ| 调整
    bool isEnd = false;
    int cnt = 0;

    int get(char x) {
        if (x >= 'A' && x <= 'Z') {
            return x - 'A';
        } else if (x >= 'a' && x <= 'z') {
            return x - 'a' + 26;
        }
        return x - '0' + 52;
    }

public:
    Trie() = default;

    void insert(const string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[get(ch)] == nullptr) {
                node->children[get(ch)] = new Trie();
            }
            node = node->children[get(ch)];
            node->cnt++;
        }
        node->isEnd = true;
    }

    // 查询 s 是否在模式串中出现
    bool find(const string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[get(ch)] == nullptr) {
                return false;
            }
            node = node->children[get(ch)];
        }
        return node->isEnd;
    }

    // 查询 s 是否是某个模式串的前缀
    bool startsWith(const string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[get(ch)] == nullptr) {
                return false;
            }
            node = node->children[get(ch)];
        }
        return true;
    }

    // 以 s 为前缀的模式串的个数
    int getCnt(const string& s) {
        Trie* node = this;
        for (char ch : s) {
            if (node->children[get(ch)] == nullptr) {
                return 0;
            }
            node = node->children[get(ch)];
        }
        return node->cnt;
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
