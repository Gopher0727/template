def getMu(n):
    """
    · 如果 n = 1, 则 μ(n) = 1。
    · 如果 n 是平方因子自由的（即 n 不被任何平方数除尽）, 且 n 有 k 个不同的素因子，则 μ(n) = (-1)^k。
    · 如果 n 不是平方因子自由的（即 n 被某个平方数除尽）, 则 μ(n) = 0。
    """
    mu = [0] * (n + 1)
    mu[1] = 1
    flg = [0] * (n + 1)
    p = [0] * (n + 1)  # 存储素数
    tot = 0  # 素数计数器
    for i in range(2, n + 1):
        if flg[i] == 0:
            tot = tot + 1
            p[tot] = i
            mu[i] = -1
        j = 1
        while j <= tot and i * p[j] <= n:
            flg[i * p[j]] = 1
            if i % p[j] == 0:
                mu[i * p[j]] = 0
                break
            mu[i * p[j]] = -mu[i]
            j = j + 1
    return mu
