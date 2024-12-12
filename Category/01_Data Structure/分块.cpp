// 分块，是一种优雅的暴力，它通过对数列分段，完成对数列一些区间操作和区间查询的操作，是一种根号算法。
// Link:
// https://www.luogu.com.cn/problem/P2801


// block: 块的大小
// n: 被分块的数列的大小
// Lx，Rx: 第 x 号块的左右边界
// tot: 块的数量
// belong_x: 第 x 号元素所属的块

// 建块：
// 1. 确定块的大小: block = floor(sqrt(n))
int block = sqrt(n);

// 2. 确定块的数量:
int tot = (n + block - 1) / block;

// 3. 标记每个块的左右边界: Lx = (x - 1) * block + 1, Rx = x * block (Specially, Rtot= n)
vector<int> L(tot + 1), R(tot + 1);
for (int i = 1; i <= tot; ++i) {
    L[i] = (i - 1) * block + 1;
    R[i] = i * block;
}
R[tot] = n;

// 4. 标记每个元素所属的块:
vector<int> belong(n + 1);
for (int i = 1; i <= n; ++i) {
    belong[i] = (i - 1) / block + 1;
}

// 5. 对每个块的数据进行初始化。







// 整除分块： sum(f(i) * (n/i))

// 性质 1：分块的块数不大于 2*floor(sqrt(n))
// 性质 2：i 所在的块的右端点为 n/(n/i)

// 循环： l = 1, r = 0; l <= n; l = r + 1
