class Combinatorics:
    __slots__ = "f", "g"

    def __init__(self, N) -> None:
        N += 1
        self.f = [1] * N
        self.g = [1] * N
        for i in range(1, N):
            self.f[i] = self.f[i - 1] * i % MOD
        self.g[-1] = pow(self.f[-1], MOD - 2, MOD)
        for i in range(N - 2, -1, -1):
            self.g[i] = self.g[i + 1] * (i + 1) % MOD

    def fac(self, n) -> int:
        return self.f[n]

    def ifac(self, n) -> int:
        return self.g[n]

    def inv(self, n) -> int:
        return self.f[n - 1] * self.g[n] % MOD

    def perm(self, n, m) -> int:
        if m < 0 or n < 0 or n < m:
            return 0
        return self.f[n] * self.g[n - m] % MOD

    def comb(self, n, m) -> int:
        if m < 0 or n < 0 or n < m:
            return 0
        return self.f[n] * self.g[m] * self.g[n - m] % MOD

    def catalan(self, n):
        if n <= 0:
            return 0
        return (self.comb(2 * n, n) - self.comb(2 * n, n - 1)) % MOD


C = Combinatorics(2 * 10**5)


# 预处理
MOD = 1_000_000_007
N = 41

fac = [0] * N  # f[i] = i!
fac[0] = 1
for i in range(1, N):
    fac[i] = fac[i - 1] * i % MOD

ifac = [0] * N  # ifac[i] = i!^-1
ifac[-1] = pow(fac[-1], -1, MOD)
for i in range(N - 1, 0, -1):
    ifac[i - 1] = ifac[i] * i % MOD
