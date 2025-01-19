// [从集合论到位运算，常见位运算技巧分类总结！](https://leetcode.cn/circle/discuss/CaOJ45/)

//* Gosper's Hack
// 查找下一个置位数与 num 相等的数
int Gosper(int num) {
    int lb = num & -num;
    int x = num + lb;
    int y = (num ^ x) / lb >> 2;
    return x | y;
}

// 题目：
// https://codeforces.com/contest/1918/problem/C    1400
// https://atcoder.jp/contests/abc147/tasks/abc147_d


// 一些结论：
// x + y = x|y + x&y
