// Reference:
// [SOS DP](https://usaco.guide/plat/dp-sos?lang=cpp#subset-mask-partitioning-with-sx-i)

// bruteforce  O(4^n)
for msk := range 1 << n {
	for i := range 1 << n {
		if msk&i == i {
			f[msk] += a[i]
		}
	}
}

// -> O(3^n)
for msk := range 1 << n {
	f[msk] = a[0]
	for i := msk; i > 0; i = (i - 1) & msk {
		f[msk] += a[i]
	}
}

// SOS DP
sos := make([]int, 1<<n)
dp := make([][]int, 1<<n)
for i := range dp {
	dp[i] = make([]int, n+1)
}

for x := range 1 << n {
	dp[x][0] = a[x]
	for i := range n {
		dp[x][i+1] = dp[x][i]
		if x>>i&1 == 1 {
			dp[x][i+1] += dp[x^(1<<i)][i]
		}
	}
	sos[x] = dp[x][n]
}

// -> Since $dp[x][i]$ only depends on $dp[x][i - 1]$, we can reuse the DP array.
sos = a
for i := range n {
	for x := range 1 << n {
		if x>>i&1 == 1 {
			sos[x] += sos[x^(1<<i)]
		}
	}
}
