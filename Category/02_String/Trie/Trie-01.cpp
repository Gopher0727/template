static constexpr i64 inf = 1E18;

struct TrieNode {
    array<int, 2> son {-1, -1};
    int cnt = 0;
    i64 mn = numeric_limits<i64>::max(); // 子树里的最小值
};

class Trie {
    vector<TrieNode> trie;
    int N = 0;

public:
    Trie(int n) : N(n) { trie.push_back(TrieNode()); }

    // n: 预分配的节点数
    void reserve(int n) { trie.reserve(n * (N + 1)); }

    int newNode() {
        trie.push_back(TrieNode());
        return trie.size() - 1;
    }

    void insert(i64 x) {
        int p = 0;
        trie[p].cnt++;
        trie[p].mn = min(trie[p].mn, x);
        for (int i = N - 1; i >= 0; i--) {
            int o = x >> i & 1;
            if (trie[p].son[o] == -1) {
                trie[p].son[o] = newNode();
            }
            p = trie[p].son[o];
            trie[p].cnt++;
            trie[p].mn = min(trie[p].mn, x);
        }
    }

    // ! remove 中没有维护 mn
    void remove(i64 x) {
        int p = 0;
        trie[p].cnt--;
        for (int i = N - 1; i >= 0; i--) {
            int o = x >> i & 1;
            if (trie[p].son[o] == -1) {
                trie[p].son[o] = newNode();
            }
            p = trie[p].son[o];
            trie[p].cnt--;
        }
    }

    // 也可以用哈希表
    i64 maxXor(i64 x) const {
        if (trie.size() == 1) {
            return -inf;
        }
        i64 ans = 0;
        int p = 0;
        for (int i = N - 1; i >= 0; i--) {
            int o = x >> i & 1;
            if (trie[p].son[o ^ 1] != -1 && trie[trie[p].son[o ^ 1]].cnt > 0) {
                ans |= 1ll << i;
                o ^= 1;
            }
            p = trie[p].son[o];
        }
        return ans;
    }

    i64 minXor(i64 x) const {
        if (trie.size() == 1) {
            return inf;
        }
        i64 ans = 0;
        int p = 0;
        for (int i = N - 1; i >= 0; i--) {
            int o = x >> i & 1;
            if (trie[p].son[o] == -1 || trie[trie[p].son[o]].cnt <= 0) {
                ans |= 1ll << i;
                o ^= 1;
            }
            p = trie[p].son[o];
        }
        return ans;
    }

    // 返回 x 与 trie 上所有数的第 k 大异或值
    // k 从 1 开始，超过总数则返回 0
    i64 kth_maxXor(i64 x, int k) const {
        i64 ans = 0;
        int p = 0;
        for (int i = N - 1; i >= 0; i--) {
            int o = x >> i & 1;
            int cnt = (trie[p].son[o ^ 1] == -1 ? 0 : trie[trie[p].son[o ^ 1]].cnt);
            if (k <= cnt) {
                ans |= 1ll << i;
                o ^= 1;
            } else {
                k -= cnt;
            }
            p = trie[p].son[o];
            if (p == -1) {
                break;
            }
        }
        return ans;
    }

    // 统计与 x 异或值 ≤ limit 的元素个数
    // 也可以用哈希表
    i64 countXorWithLimit(i64 x, i64 limit) const {
        // 核心原理是，当 limit+1 的某一位是 1 的时候，若该位异或值取 0，则后面的位是可以取任意数字的
        // 如果在 limit 上而不是 limit+1 上讨论，就要单独处理走到叶子的情况了（恰好等于 limit）
        limit++;
        i64 ans = 0;
        int p = 0;
        for (int i = N - 1; i >= 0 && p != -1; i--) {
            int o = x >> i & 1;
            if (limit >> i & 1) {
                if (trie[p].son[o] != -1) {
                    ans += trie[trie[p].son[o]].cnt;
                }
                o ^= 1;
            }
            p = trie[p].son[o];
        }
        return ans;
    }

    // x 与 trie 上所有 ≤ limit 的数的最大异或值
    // 不存在时返回 -1
    i64 maxXorWithLimit(i64 x, i64 limit) const {
        if (trie[0].mn > limit) {
            return -1;
        }
        i64 ans = 0;
        int p = 0;
        for (int i = N - 1; i >= 0; --i) {
            int o = (x >> i) & 1;
            if (trie[p].son[o ^ 1] != -1 && trie[trie[p].son[o ^ 1]].cnt > 0 && trie[trie[p].son[o ^ 1]].mn <= limit) {
                ans |= 1ll << i;
                o ^= 1;
            }
            p = trie[p].son[o];
        }
        return ans;
    }

    // x 与 trie 上所有数的异或 <= limit 的最大异或值
    // 不存在时返回 -1
    i64 maxXorWithLimitXor(i64 x, i64 limit) const {
        limit++;
        i64 ans = 0;
        int p = 0;
        // 记录最后一次我们“仍能走 0 分支”，但 limit 那位是 1 的情况
        int last_p = -1, last_i = -1;
        i64 last_ans = 0;
        for (int i = N - 1; i >= 0 && p != -1; i--) {
            int o = x >> i & 1;
            if (limit >> i & 1) {
                // 分两种：如果走 son[o]，当前位异或 0，依然 < limit 在这位的 1
                if (trie[p].son[o] != -1) {
                    last_p = trie[p].son[o];
                    last_i = i - 1;
                    last_ans = ans;
                }
                // 如果走 son[o^1]，当前位异或 1
                if (trie[p].son[o ^ 1] != -1) {
                    ans |= 1ll << i;
                }
                // 实际走的还是 o^1
                o ^= 1;
            }
            p = trie[p].son[o];
        }
        if (last_p == -1) {
            return -1;
        }
        ans = last_ans;
        p = last_p;
        for (int i = last_i; i >= 0; i--) {
            int o = x >> i & 1;
            if (trie[p].son[o ^ 1] != -1 && trie[trie[p].son[o ^ 1]].cnt > 0) {
                ans |= 1ll << i;
                o ^= 1;
            }
            p = trie[p].son[o];
        }
        return ans;
    }

    // 完全图，边权为 a[u]^a[v]，求 MST
    // Boruvka 算法，分治连边
    template <integral T>
    auto xorMST(const vector<T>& a) {
        T ans {};
        auto dfs = [&](auto&& dfs, auto& a, int p) {
            if (a.empty() || p < 0) {
                return;
            }
            vector<T> b[2];
            b[0].reserve(a.size());
            b[1].reserve(a.size());
            for (auto& v : a) {
                b[v >> p & 1].push_back(v);
            }
            if (!b[0].empty() && !b[1].empty()) {
                if (b[0].size() > b[1].size()) {
                    swap(b[0], b[1]);
                }
                Trie t(30); // todo
                t.reserve(b[0].size());
                for (auto& x : b[0]) {
                    t.insert(x);
                }
                T minXor = numeric_limits<T>::max();
                for (auto& x : b[1]) {
                    minXor = min(minXor, t.minXor(x));
                }
                ans += minXor;
            }
            dfs(dfs, b[0], p - 1);
            dfs(dfs, b[1], p - 1);
        };
        dfs(dfs, a, N - 1);
        return ans;
    }
};
