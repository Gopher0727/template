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
