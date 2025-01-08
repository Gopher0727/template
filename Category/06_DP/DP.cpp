// DP



// 背包 DP
/*

01背包：
下标从0开始，外层枚举物品，内层枚举背包容量

分组背包：
下标从0开始，外层枚举组，中层枚举背包容量，内层枚举组内物品
（由于背包容量中层枚举，此时不清楚物品体积，所以应该在内层枚举物品时判断体积大小）


Problems：
> https://atcoder.jp/contests/abc383/tasks/abc383_f

*/



// 区间 DP
/*

枚举区间长度（从小区间到大区间），注意初始化

*/



// 数位 DP
//
vector memo(m, vector<int>(1 << 10, -1));
auto dfs = [&](auto&& dfs, int i, int mask, bool isLimit, bool isNum) -> int {
    if (i == m) {
        return isNum;
    }
    if (!isLimit && isNum && memo[i][mask] != -1) {
        return memo[i][mask];
    }
    int res = 0;
    if (!isNum) { // 可以跳过当前位
        res = dfs(dfs, i + 1, mask, false, false);
    }
    int up = isLimit ? s[i] - '0' : 9;
    for (int k = isNum ? 0 : 1; k <= up; ++k) {
        if (mask >> k & 1) {
            continue;
        }
        res += dfs(dfs, i + 1, mask | (1 << k), isLimit && k == up, true);
    }
    if (!isLimit && isNum) {
        memo[i][mask] = res;
    }
    return res;
};
return dfs(dfs, 0, 0, true, false);



// 划分型 DP
/*

> 判定能否划分
> 最优划分
一般定义 f[i] 表示长为 i 的前缀 a[:i] 在题目约束下，分割出的最少（最多）子数组的个数（或者定义成分割方案数）。
枚举最后一个子数组的左端点，从 `f[i−1][L]` 转移到 `f[i][j]`，并考虑 a[L:j] 对最优解的影响。。
将数组分成（恰好/至多）k 个连续子数组，计算与这些子数组有关的最优值。
一般定义 `f[i][j]` 表示将长为 j 的前缀 a[:j] 分成 i 个连续子数组所得到的最优解。

*/


