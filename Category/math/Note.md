# 排列组合

## 错排

D(n) 表示 n 个元素的错排，易得 D(1) = 0, D(2) = 1, D(3) = 2, D(4) = 9, D(5) = 44

$D(n) = n!\sum_{k=0}^{n}\frac{(-1)^k}{k!}$


## 第一类斯特林数




## 第二类斯特林数

定义： 表示把 n 个不同的数划分为 m 个集合的方案数，要求不能为空集，写作 S(n, m).
和第一类斯特林数不同，划分集合不必考虑排列次序。
根据加法原理，$S(n, m) = S(n-1, m-1) + m \cdot S(n-1, m)$

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




# 不等式

## 闵可夫斯基不等式

$\sqrt{a^2 + b^2} + \sqrt{c^2 + d^2} \ge \sqrt{(a+c)^2 + (b+d)^2}$

## 柯西不等式

$(a_1^2 + a_2^2 + \cdots + a_n^2) (b_1^2 + b_2^2 + \cdots + b_n^2) \ge (a_1b_1 + a_2b_2 + \cdots + a_nb_n)^2$

## Jesen(琴生)不等式

$\overset{n}{\underset{i=1}{\sum}} \lambda_i f(x_i) \ge f(\overset{n}{\underset{i=1}{\sum}} \lambda_i x_i)$


# 数论

## 约瑟夫环
```cpp []
ans = (ans + k - 1) % n + 1 // ans 表示旧环编号，n 表示旧环大小
```
