MOD = 10**18 + 3
base = random.randint(131, 13331)
MX = 10**5
f = [1] * (MX + 1)
for i in range(1, MX + 1):
    f[i] = f[i - 1] * base % MOD
g = [1] * (MX + 1)
g[-1] = pow(f[-1], -1, MOD)
for i in range(MX, 1, -1):
    g[i - 1] = g[i] * base % MOD


class StrHash:
    # 高位在前的写法
    def __init__(self, s: str):
        n = len(s)
        hval = [0] * (n + 1)
        for i, ch in enumerate(s):
            hval[i + 1] = (hval[i] * base + ord(ch)) % MOD
        self.hval = hval

    def substr(self, i: int, l: int):
        # i为起始下标，l为子串长度
        return (self.hval[i + l] - self.hval[i] * f[l]) % MOD


class StrHashInv:
    # 高位在后的写法
    def __init__(self, s: str):
        n = len(s)
        hval = [0] * (n + 1)
        for i, ch in enumerate(s):
            hval[i + 1] = (hval[i] + ord(ch) * f[i]) % MOD
        self.hval = hval

    def substr(self, i: int, l: int):
        # i为起始下标，l为子串长度
        return (self.hval[i + l] - self.hval[i]) * g[i] % MOD
