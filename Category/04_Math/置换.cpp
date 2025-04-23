// ※ 置换不是排序，是映射（可以理解为重命名）

// 离散化
auto f = [&](vector<int>& a) {
    auto tmp = a;
    ranges::sort(tmp);
    for (int& v : a) {
        v = ranges::lower_bound(a, v);
    }
    return a;
}

// 对于离散化后的数组 a，建立值与下标的映射关系
vector<int> p(n);
for (int i = 0; i < n; i++) {
    p[a[i]] = i;
}

// 将映射 p 用到离散化后的数组 b 上（此时数组 a 递增）
for (int i = 0; i < n; i++) {
    b[i] = p[b[i]];
}
