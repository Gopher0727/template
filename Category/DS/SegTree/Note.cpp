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
// 势能线段树：区间开方、区间取模、区间 GCD 一个数，都是可以暴力更新的


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


// 当维护的信息较多，且传入构造的参数有条件时，
// 该函数可以用需要传入的参数新建节点（solve 函数内部使用）
//
// https://codeforces.com/contest/1609/problem/E
//
Info newInfo(char ch) {
    Info o;
    //
    return o;
}
