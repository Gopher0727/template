|                 Link                  | Introduce |
| :-----------------------------------: | :-------: |
| https://zhuanlan.zhihu.com/p/66925097 |           |


# 错排

D(n) 表示 n 个元素的错排，易得 D(1) = 0, D(2) = 1, D(3) = 2, D(4) = 9, D(5) = 44

$D(n) = n!\sum_{k=0}^{n}\frac{(-1)^k}{k!}$


# 组合数恒等式

## Hockey-stick identity

$\overset{n}{\underset{i=r}{\sum}}\binom{i}{r} = \binom{n+1}{r+1}$ $(n \ge r)$


## 范德蒙恒等式

$\sum{\binom{n}{i} \cdot \binom{m}{i}} = \binom{n+m}{k}$


<!-- [小红的数组回文值](https://ac.nowcoder.com/acm/contest/89860/F)    利用公式减少一层枚举 -->



# 斯特林数

## 第一类斯特林数


## 第二类斯特林数

> 定义： 表示把 n 个不同的数划分为 m 个集合的方案数，要求不能为空集，写作 S(n, m).
> 和第一类斯特林数不同，划分集合不必考虑排列次序。
> 根据加法原理，$S(n, m) = S(n-1, m-1) + m \cdot S(n-1, m)$


```cpp
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


# 卡特兰数 (Catalan)

> 通项公式： $\binom{2n}{n} - \binom{2n}{n-1}$ 或者 $\frac{\binom{2n}{n}}{n+1}$。
>
> 递推式： $f(n) = \overset{n-1}{\underset{k=0}{\sum}}f(k)f(n-1-k)$ 或者 $f(n)=\frac{f(n-1)\cdot(4n-2)}{n+1}$。
>
> 已知一个栈（无穷大）的进栈序列，有多少种不同的出栈序列？
> n 个节点的无标号二叉树有多少种形态？
> 一个凸 n 边形，每次连接两个顶点可以将其划分成三角形，有多少种不同的剖分方法？
> n 对括号的合法括号匹配的数量。


```cpp
// 前 10 项
int Catalan[] {
    1,
    2,
    5,
    14,
    42,
    132,
    429,
    1430,
    4862,
    16796
};
```

