# 暴力判断一个数是否是质数
def isPrime(n: int):
    for i in range(2, isqrt(n) + 1):
        if n % i == 0:
            return False
    return n >= 2


# 记录一个数的所有因数
def factorize(v: int):
    fac = []
    for i in range(1, isqrt(v) + 1):
        if v % i == 0:
            fac.append(i)
            if i * i != v:
                fac.append(v // i)
    # fac.sort()
    return fac


# 记录范围内的所有数的因数  2e5
def sieve(n: int):
    factors = [[] for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(i, n + 1, i):
            factors[j].append(i)
    return factors


# 找到一个数的所有质因数
def factorize(x: int):
    res = []
    for i in rnage(2, isqrt(x) + 1):
        cnt = 0
        while x % i == 0:
            x //= i
            cnt += 1
        if cnt > 0:
            res.append((i, cnt))
    if x > 1:
        res.append((x, 1))
    return res


# EXP[x] 为 x 分解质因数后，每个质因数的指数
def sieve(n: int):
    EXP = [[] for _ in range(n + 1)]
    for x in range(2, n + 1):
        a = x
        for i in range(2, isqrt(a) + 1):
            e = 0
            while a % i == 0:
                a //= i
                e += 1
            if e > 0:
                EXP[x].append((i, e))
        if a > 1:
            EXP[x].append((a, 1))
    return EXP


# 按从小到大的顺序生成回文数（1 ~ 1e9 之间）
def generate_palindromes():
    palindrome = [0]  # 哨兵
    for base in [1, 10, 100, 1000, 10000]:
        # 生成奇数长度的回文数
        for i in range(base, base * 10):
            x = i
            t = i // 10
            while t:
                x = x * 10 + t % 10
                t //= 10
            palindrome.append(x)
        # 生成偶数长度的回文数
        if base <= 1000:
            for i in range(base, base * 10):
                x = i
                t = i
                while t:
                    x = x * 10 + t % 10
                    t //= 10
                palindrome.append(x)
    palindrome.append(10**9 + 1)  # 哨兵
    return palindrome


# 埃氏筛  ErlichScreen
def sieve(n: int):
    isPrime = [True] * (n + 1)
    isPrime[0] = isPrime[1] = False
    primes = []
    for i in range(2, n + 1):
        if isPrime[i]:
            primes.append(i)
            for j in range(i * i, n + 1, i):
                isPrime[j] = False
    return primes, isPrime


# 线性筛  LinearScreen / EulerScreen
# 让每个合数只被标记一次
def sieve(n: int):
    isPrime = [True] * (n + 1)
    isPrime[0] = isPrime[1] = False
    primes = []
    for i in range(2, n + 1):
        if isPrime[i]:
            primes.append(i)
        for p in primes:
            if i * p > n:
                break
            isPrime[i * p] = False
            if i % p == 0:
                break
    return primes, isPrime


# 线性筛 + 最小质因数  1e8
# 最小质因数 * 最大真因数
def sieve(n: int):
    minp = [0] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if minp[i] == 0:
            minp[i] = i
            primes.append(i)
        for p in primes:
            if i * p > n:
                break
            minp[i * p] = p
            if p == minp[i]:
                break
    return primes, minp
# isPrime(n): n >= 0 and minp[n] == n


# 欧拉函数（单个数）
# pow(a, b) = pow(a, (b%phi[n])) (mod n)
def phi(n):
    res = n
    for i in range(2, isqrt(n) + 1):
        if n % i == 0:
            while n % i == 0:
                n //= i
            res = res // i * (i - 1)
    if n > 1:
        res = res // n * (n - 1)
    return res


# 线性筛 + 欧拉函数（从 1 到 i 中与 i 互质的数的个数）
def sieve(n):
    isPrime = [True] * (n + 1)
    isPrime[0] = isPrime[1] = False
    primes = []
    phi = [0] * (n + 1)
    phi[1] = 1
    for i in range(2, n + 1):
        if isPrime[i]:
            primes.append(i)
            phi[i] = i - 1
        for p in primes:
            if i * p > n:
                break
            isPrime[i * p] = False
            if i % p == 0:
                phi[i * p] = phi[i] * p
                break
            phi[i * p] = phi[i] * (p - 1)
    return isPrime, primes, phi


# 线性筛 + 最小质因数 + 欧拉函数（从 1 到 i 中与 i 互质的数的个数）
def sieve(n):
    primes = []
    minp = [0] * (n + 1)
    phi = [0] * (n + 1)
    phi[1] = 1
    for i in range(2, n + 1):
        if minp[i] == 0:
            primes.append(i)
            minp[i] = i
            phi[i] = i - 1
        for p in primes:
            if i * p > n:
                break
            minp[i * p] = p
            if p == minp[i]:
                phi[i * p] = phi[i] * p
            phi[i * p] = phi[i] * (p - 1)
    return primes, minp, phi
# isPrime(n): n >= 0 and minp[n] == n


# 块筛选  ——  计算不大于 n 的质数的总数量
def count_primes_block(n):
    S = 10000

    nsqrt = isqrt(n)
    isPrime = [1] * (nsqrt + 1)
    primes = []
    for i in range(2, nsqrt + 1):
        if isPrime[i]:
            primes.append(i)
            for j in range(i * i, nsqrt + 1, i):
                isPrime[j] = 0

    res = 0
    for k in range(0, n // S + 1):
        block = [1] * S
        s = k * S
        for p in primes:
            for j in range(fmax(p, (s + p - 1) // p) * p - s, S, p):
                block[j] = 0
        if k == 0:
            block[0] = block[1] = 0
        res += sum(block[i] for i in range(min(S, n - s + 1)))
    return res
