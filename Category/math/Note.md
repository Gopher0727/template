## 错排

D(n) 表示 n 个元素的错排，易得 D(1) = 0, D(2) = 1, D(3) = 2, D(4) = 9, D(5) = 44

$D(n) = n!\sum_{k=0}^{n}\frac{(-1)^k}{k!}$\


## 第一类斯特林数




## 第二类斯特林数

定义： 表示把 n 个不同的数划分为 m 个集合的方案数，要求不能为空集，写作 S(n, m).
和第一类斯特林数不同，划分集合不必考虑排列次序。
根据加法原理，S(n, m) = S(n-1, m-1) + S(n-1, m) * m

```cpp []
const int MOD = 1'000'000'007;
const int MX = 1001;
int s[MX][MX];

auto init = [] {
    s[0][0] = 1;
    for (int i = 1; i < MX; i++) {
        for (int j = 1; j <= i; j++) {
            s[i][j] = (s[i - 1][j - 1] + j * 1ll * s[i - 1][j]) % MOD;
        }
    }
    return 0;
}();
```

## 约瑟夫环

ans = (ans + k - 1) % n + 1
