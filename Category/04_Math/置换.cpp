// ※ 置换不是排序，是映射（可以理解为重命名）

// 1. 离散化

// 2. 对于离散化后的数组 a，建立值与下标的映射关系
vector<int> p(n);
for (int i = 0; i < n; i++) {
    p[a[i]] = i;
}

// 3. 将映射 p 用到离散化后的数组 b 上（此时数组 a 递增）
for (int i = 0; i < n; i++) {
    b[i] = p[b[i]];
}


// 通过交换，把一个排列变成单位排列（非单位排列可以先进行置换）：
// 1> 相邻交换，操作次数等于排列的逆序数
// 2> 任意交换，操作次数等于 N - C，
//              其中 N 是排列长度，C 是将排列分解为 "不动点+若干循环" 时的循环个数
// 两种交换，操作次数的奇偶性相同

auto calc(const vector<int>& a) {
    vector<bool> vis(n);
    int cyc = 0;
    for (int i = 0; i < n; i++) {
        if (vis[i]) {
            continue;
        }
        int j = i;
        while (!vis[j]) {
            vis[j] = true;
            j = a[j];
        }
        cyc++;
    }
    return cyc;
}
