// 后缀自动机 [https://www.bilibili.com/video/BV1Eqxye2EG1]
// 把一个子串 s 在母串中的所有结束位置构成的集合，叫做该子串的结束位置集合 endpos

// SAM 的每个节点代表的字符串，其 endpos 都相同（划分出一个等价类）
// 从失配树往下走，对应字符串越长，对应的 endpos 集合越小
// 任何一条状态转移 u-(c)->v，都要满足 len[v] = len[u] + 1。

static constexpr int ALPHABET_SIZE = 26;
struct Node {
    array<int, ALPHABET_SIZE> next {};
    int fail = 0;
    int len = 0; // 从初始态(空串)到该状态代表的最长子串长度
    i64 cnt = 0; // 该状态对应的 endpos 大小 (出现次数)
};

vector<Node> t;

struct SAM {
    SAM() {
        t.assign(2, Node {});
        // 状态 0 是 "虚拟根"
        t[0].next.fill(1);
        t[0].len = -1;
    }

    int newNode() {
        t.push_back({});
        return t.size() - 1;
    }

    int insert(const string& s) {
        int p = 1;
        for (char ch : s) {
            int c = ch - 'a';
            p = insert(p, c);
        }
        return p;
    }

    // 以状态 p 为当前尾部状态，插入字符 c，并返回插入后新的尾部状态
    int insert(int p, int c) {
        assert(0 <= c && c < 26);
        // 已经存在一条 p-(c)->q 的边
        if (t[p].next[c]) {
            // 复用节点 q
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1) {
                return q;
            }
            int r = newNode();
            t[r].next = t[q].next;
            t[r].fail = t[q].fail;
            t[r].len = t[p].len + 1;
            t[r].cnt = 0;
            // 沿 fail 链向上，将所有从 p 沿 c 到 q 的转移改成到 r
            t[q].fail = r;
            while (t[p].next[c] == q) {
                t[p].next[c] = r;
                p = t[p].fail;
            }
            return r;
        }
        // 不存在从 p 出发的字符 c 边，需要创建新状态 cur
        int cur = newNode();
        t[cur].len = t[p].len + 1;
        t[cur].cnt = 1;
        // 沿 fail 链向上，为所有缺少 c 边的状态添加指向 cur 的转移
        while (!t[p].next[c]) {
            t[p].next[c] = cur;
            p = t[p].fail;
        }
        t[cur].fail = insert(p, c);
        return cur;
    }

    // 统计每个状态对应子串出现次数
    // ! 只能运行一次
    void getOccurrences() {
        int maxLen = 0;
        for (auto& node : t) {
            maxLen = max(maxLen, node.len);
        }
        // 计数排序
        int sz = t.size();
        vector<int> bucket(maxLen + 1);
        vector<int> order(sz); // 将下标排序
        for (int i = 1; i < sz; i++) {
            bucket[t[i].len]++;
        }
        for (int i = 1; i <= maxLen; i++) {
            bucket[i] += bucket[i - 1];
        }
        for (int i = sz - 1; i > 0; i--) {
            order[--bucket[t[i].len]] = i;
        }
        // 反向遍历 (len 从大到小)，把 cnt 累加到 fail
        for (int i = sz - 1; i > 0; i--) {
            int v = order[i], f = t[v].fail;
            t[f].cnt += t[v].cnt;
        }
    }

    // 返回包含本质不同的字符串的个数
    int getDistinctString() {
        i64 diff = 0;
        for (int i = 1; i < t.size(); i++) {
            diff += t[i].len - t[t[i].fail].len;
        }
        return diff;
    }

    // 查询模式串出现次数
    // ! 在 getOccurrences() 调用之后
    i64 countOccurrences(const string& p) const {
        int u = 1;
        for (char ch : p) {
            int c = ch - 'a';
            if (!t[u].next[c]) {
                return 0;
            }
            u = t[u].next[c];
        }
        return t[u].cnt;
    }
};
