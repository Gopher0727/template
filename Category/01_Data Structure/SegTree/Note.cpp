// 线段树有两个下标，一个是线段树底层数组的下标，一个是线段树维护的区间的下标（这个其实无所谓，从 0 从 1 开始都可以）。
// 底层数组的下标从 1 开始。
// 线段树维护的区间的下标从 0 开始。
// 线段树区间修改一般需要附加懒信息。
// 函数调用的左右端点为闭区间形式。



// 参考链接
//
// https://www.acwing.com/blog/content/1684/    较为详细
//
// https://www.luogu.com.cn/article/s2aiq49p    权值线段树
// https://www.luogu.com.cn/article/3euserch    主席树
// https://www.luogu.com/article/r1mp3hga   进阶
// https://www.luogu.com/article/aentaeud   进阶
// https://www.luogu.com.cn/article/tqelf691    线段树分裂
//
// https://www.cnblogs.com/dyhaohaoxuexi/p/14046275.html   维护区间加、乘、赋值、平方和、立方和
//



// 题目链接
//
// 模板
// > 单点修改、区间查询（增量式更新可以用覆盖式更新代替）
//      https://www.luogu.com.cn/problem/P2068
//
// 维护平方和
//      https://leetcode.cn/problems/subarrays-distinct-element-sum-of-squares-ii/
//      https://www.luogu.com.cn/problem/P1471
//
//
//
// 【多个更新操作复合】
// = + max https://www.luogu.com.cn/problem/P1253
//      代码 https://www.luogu.com.cn/record/138265877
//
// * + ∑ https://www.luogu.com.cn/problem/P3373
//      LC1622 https://leetcode.cn/problems/fancy-sequence/
//
// = + ∑ https://codeforces.com/edu/course/2/lesson/5/4/practice/contest/280801/problem/A
// 注：区间赋值（=x）可以看成是先 *0 再 +x
//
// 【单个更新操作复合】
//  - ∑ https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/D
//    https://www.luogu.com.cn/problem/P2068
//    https://www.luogu.com.cn/problem/P3372
//  - + ∑ https://atcoder.jp/contests/abc357/tasks/abc357_f
//
// | & https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/C
// = min https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/E
// = ∑ https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/F
//



// 技巧
//
// > 区间加等比数列（q 恒定）
//      提出首项后，剩余的是固定的 1 + q^1 + q^2 + ... + q^(线段树区间长度-1)
//      可以预处理 1 + q^1 + q^2 + ... 的前缀和
//      线段树维护首项的和
//
// > 区间加等差数列（差分法）
//      https://www.luogu.com.cn/problem/P1438
//
//
//
// 如果一个题目可以用分治解决，那么这个题目的带修改版本可以用线段树解决
//
// > 带修最长连续相同子串
//      https://leetcode.cn/problems/longest-substring-of-one-repeating-character/
// > 带修最大子段和
//      https://www.luogu.com.cn/problem/P4513
//      - https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/A
// > 带修最大子段和+按位或
//      https://www.luogu.com.cn/problem/P7492 https://www.luogu.com.cn/contest/42328
// > 带修打家劫舍
//      https://www.luogu.com.cn/problem/P3097
//      - https://leetcode.cn/problems/maximum-sum-of-subsequence-with-non-adjacent-elements/
//
//
//
//
// 势能线段树：
// 区间开方、区间取模、区间 GCD 一个数，都是可以暴力更新的
//



// 维护最大子段和
//
// https://leetcode.cn/problems/maximize-subarray-sum-after-removing-all-occurrences-of-one-element/
//
struct Info {
    i64 sum, lval, rval, ans;
    Info() {}
    Info(int v) : sum(v), lval(v), rval(v), ans(v) {}
};
Info operator+(const Info& p, const Info& q) {
    Info info;
    info.sum = p.sum + q.sum;
    info.lval = max(p.lval, p.sum + q.lval);
    info.rval = max(p.rval + q.sum, q.rval);
    info.ans = max({p.ans, q.ans, p.rval + q.lval});
    return info;
}


// 维护最长重复子串的长度
//
// https://leetcode.cn/problems/longest-substring-of-one-repeating-character/
//
struct Info {
    int pre = 0, suf = 0, most = 0, len = 0;
    char lch = 0, rch = 0;
};
Info operator+(const Info& a, const Info& b) {
    if (a.lch == 0) {
        return Info();
    }
    Info info;
    info.pre = a.pre;
    if (a.lch == b.lch && a.pre == a.len) {
        info.pre += b.pre;
    }
    info.suf = b.suf;
    if (a.rch == b.rch && b.suf == b.len) {
        info.suf += a.suf;
    }
    info.most = max(a.most, b.most);
    if (a.rch == b.lch) {
        info.most = max(info.most, a.suf + b.pre);
    }
    info.len = a.len + b.len;
    info.lch = a.lch;
    info.rch = b.rch;
    return info;
}


// 实现一个区间赋值
//
// https://atcoder.jp/contests/abc382/tasks/abc382_f
//
struct Tag {
    int s = 0;
    void apply(const Tag& t) {
        if (t.s != -1) {
            s = max(s, t.s);
        }
    }
};
struct Info {
    int mx = 0;
    void apply(const Tag& t) {
        if (t.s != -1) {
            mx = max(mx, t.s);
        }
    }
};
Info operator+(const Info& a, const Info& b) {
    return Info(max(a.mx, b.mx));
}



// 扫描线（矩形面积并）
//
// 直接维护的困难：一次操作对于同一节点内的每个元素可能有不同的影响。
