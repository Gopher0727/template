class Combinatorics:
    def __init__(self, MX) -> None:
        self.f = [1] * (MX + 1)
        self.g = [1] * (MX + 1)
        for i in range(2, MX + 1):
            self.f[i] = self.f[i - 1] * i % MOD
        self.g[-1] = pow(self.f[-1], -1, MOD)
        for i in range(MX, 1, -1):
            self.g[i - 1] = self.g[i] * i % MOD

    def fact(self, n) -> int:
        return self.f[n]

    def fact_inv(self, n) -> int:
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


C = Combinatorics(2 * 10**5)
