static constexpr int MX = 2E5 + 10;

int trie[26][MX] {};
int cnt[MX] {};
int tot {};
bool isEnd[MX] {};

int f(char ch) {
    return ch - 'a';
}

void insert(const string& s) {
    int p = 0;
    for (char ch : s) {
        int o = f(ch);
        if (!trie[o][p]) {
            trie[o][p] = ++tot;
        }
        p = trie[o][p];
        cnt[p]++;
    }
    isEnd[p] = true;
}

bool find(const string& s) {
    int p = 0;
    for (char ch : s) {
        int o = f(ch);
        if (!trie[o][p]) {
            return false;
        }
        p = trie[o][p];
    }
    return isEnd[p];
}

bool startsWith(const string& s) {
    int p = 0;
    for (char ch : s) {
        int o = f(ch);
        if (!trie[o][p]) {
            return false;
        }
        p = trie[o][p];
    }
    return true;
}

// 以 s（非空）为前缀的模式串的个数
int getCnt(const string& s) {
    int p = 0;
    for (char ch : s) {
        int o = f(ch);
        if (!trie[o][p]) {
            return 0;
        }
        p = trie[o][p];
    }
    return cnt[p];
}
