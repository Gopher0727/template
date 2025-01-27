// 珂朵莉树实质上是一种可以维护区间上的分裂与合并的数据结构
// 但要求数据是随机的，或者有大量的随机合并操作，这样才能保证维护的区间个数是一个很小的值
// 在若干次随机合并后，区间个数会骤降至一个稳定的范围，这是理解珂朵莉树的关键。

// 目前主流的实现是基于 set 来维护节点，但由于平均维护的区间个数很小，set 的优势并不明显。
// 相比之下，链表（或数组）能更简洁地维护分裂与合并操作。
//
// 基于链表实现的珂朵莉树

using i64 = long long;

struct Block {
    Block *next; // 链表下一节点
    int l, r; // 区间范围
    i64 val; // 区间上的值

    Block(Block *next, int l, int r, i64 val): next(next), l(l), r(r), val(val) {}
    bool operator<(const Block &b) const { return val < b.val; }
} *root;


// 分裂区间
// 返回左端点为 mid+1 的区间
Block *split(int mid) {
    for (Block *b = root; b; b = b->next) { // 遍历链表
        if (b->l == mid + 1) { // 左端点为 mid+1
            return b;
        }
        // 寻找能包含 mid 和 mid+1 的区间 [l, r]，将其被拆分成 [l, mid] 和 [mid+1, r]
        if (b->l <= mid && mid + 1 <= b->r) {
            b->next = new Block(b->next, mid + 1, b->r, b->val);
            b->r = mid;
            return b->next;
        }
    }
    return nullptr; // 未找到，返回空
}

// 在操作区间时，由于不能只维护区间的一部分，所以下面的操作进行之前都需要预先分裂区间，再完成相应操作。
Block *lb, *rb;

// 预分裂，保证后续操作在 [l, r] 内部
void prepare(int l, int r) {
    lb = split(l - 1);
    rb = split(r);
}


// 合并区间
void merge(int l, int r, i64 val) {
    prepare(l, r);
    lb->r = r; // 将区间 [lb.l, lb.r] 修改成 [lb.l, r]
    lb->val = val;
    lb->next = rb; // 将 [lb.l, r] 链至其右侧相邻区间
}
// 注：这里没有释放被删除节点的内存，若有需要可自行添加

// 区间修改与计算
// 遍历统计 [l,r] 即可。

// 区间更新
void add(i64 val) {
    for (Block* b = lb; b != rb; b = b->next) {
        b->val += val;
    }
}

// 区间第 k 小
i64 kth(int k) {
    vector<Block> blocks;
    for (Block* b = lb; b != rb; b = b->next) blocks.emplace_back(*b);
    sort(blocks.begin(), blocks.end());

    k--;
    for (Block b : blocks) {
        int cnt = b.r - b.l + 1;
        if (k >= cnt) {
            k -= cnt;
        } else {
            return b.val;
        }
    }
    return -1;
}

// 乘法快速幂，带取模
i64 qpow(i64 a, i64 b, i64 p, i64 res = 1) {
    a = (a % p + p) % p;
    for (; b; b >>= 1, a = a * a % p) {
        if (b & 1) {
            res = a * res % p;
        }
    }
    return res;
}

i64 pow_sum(int n, i64 mod) {
    i64 sum = 0;
    for (Block* b = lb; b != rb; b = b->next) {
        sum += i64(b->r - b->l + 1) * qpow(b->val, n, mod);
    }
    return sum % mod;
}
