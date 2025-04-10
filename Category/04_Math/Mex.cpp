// MEX（The minimum excluded）
//
// 性质：
// 对于排列，如果 0~x-1 都在 x 的左侧或右侧，那么一定有子数组的 mex 是 x，否则一定没有。

// 多考虑 0 这个特殊点

// 题目：
// https://codeforces.com/contest/2049/problem/C    1500  贪心，构造
// https://codeforces.com/contest/1944/problem/C    1300
// https://codeforces.com/contest/1935/problem/B    1200
// https://codeforces.com/contest/2059/problem/C
// https://codeforces.com/contest/2067/problem/E


// 求解 suf-mex
vector<int> vis(n), suf(n);
int mex = 0;
for (int i = n - 1; i >= 0; i--) {
    if (a[i] < n) {
        vis[a[i]] = 1;
    }
    while (mex < n && vis[mex]) {
        mex++;
    }
    suf[i] = mex;
}
