var (
	mod = 998244353

	N    = 100
	fac  = make([]int, N+1)
	ifac = make([]int, N+1)
)

func init() {
	fac[0] = 1
	for i := 1; i <= N; i++ {
		fac[i] = fac[i-1] * i % mod
	}
	ifac[N] = qpow(fac[N], mod-2, mod)
	for i := N - 1; i >= 0; i-- {
		ifac[i] = ifac[i+1] * (i + 1) % mod
	}
}

func Comb(n, m int) int {
	if m < 0 || m > n || n < 0 {
		return 0
	}
	return fac[n] * ifac[m] % mod * ifac[n-m] % mod
}
