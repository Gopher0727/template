// 笛卡尔树
//
// Link：
// https://oi-wiki.org/ds/cartesian-tree/
//
// 笛卡尔树是一种二叉树，每一个节点由一个键值二元组 (k,w) 构成。
// 要求 k 满足二叉搜索树的性质，而 w 满足堆的性质。
// 如果笛卡尔树的 k,w 键值确定，且 k 互不相同，w 也互不相同，那么这棵笛卡尔树的结构是唯一的。
//
// 把数组元素值当作键值 w，而把数组下标当作键值 k。
// (※) 可以发现这种特殊的笛卡尔树满足一棵子树内的下标是一个连续区间。
//
// Problems:
// https://www.luogu.com.cn/problem/P5854
//


// 构建笛卡尔树并返回根节点指针
struct CtNode {
    int id, val;
    CtNode* lr[2];

    CtNode(int _id, int _val) : id(_id), val(_val), lr {nullptr, nullptr} {}
};
CtNode* buildCartesianTree(const vector<int>& a) {
    if (a.empty()) {
        return nullptr;
    }
    vector<CtNode*> s;
    for (int i = 0; i < a.size(); ++i) {
        CtNode* o = new CtNode(i, a[i]);
        while (!s.empty()) {
            CtNode* top = s.back();
            if (top->val < o->val) {
                top->lr[1] = o;
                break;
            }
            o->lr[0] = top;
            s.pop_back();
        }
        s.push_back(o);
    }
    return s.empty() ? nullptr : s[0];
}

// 非指针版，返回每个节点的左右儿子的编号
vector<pair<int, int>> buildCartesianTree2(const vector<int>& a) {
    vector<pair<int, int>> lr(a.size(), {-1, -1});
    vector<int> s;
    for (int i = 0; i < a.size(); ++i) {
        while (!s.empty()) {
            int top = s.back();
            if (a[top] < a[i]) {
                lr[top].second = i;
                break;
            }
            lr[i].first = top;
            s.pop_back();
        }
        s.push_back(i);
    }
    return lr;
}
