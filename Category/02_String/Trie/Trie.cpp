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
