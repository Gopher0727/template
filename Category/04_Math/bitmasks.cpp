//* Gosper's Hack
// 查找下一个置位数与 num 相等的数
int Gosper(int num) {
    int lb = num & -num;
    int x = num + lb;
    int y = (num ^ x) / lb >> 2;
    return x | y;
}

// 题目：
// https://codeforces.com/contest/1918/problem/C
