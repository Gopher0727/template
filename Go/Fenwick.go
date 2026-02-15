type Fenwick []int

func (f Fenwick) Add(k, v int) {
	for i := k + 1; i <= len(f); i += i & -i {
		f[i-1] += v
	}
}

// Query [0, k)
func (f Fenwick) Query(k int) (ans int) {
	for i := k; i > 0; i &= i - 1 {
		ans += f[i-1]
	}
	return
}

// RangeQuery [l, r)
func (f Fenwick) RangeQuery(l, r int) int {
	if l > r {
		panic("invalid range")
	}
	return f.Query(r) - f.Query(l)
}

func (f Fenwick) Select(k int) (x int) {
	n := len(f)
	cur := 0
	for i := 1 << bits.Len(uint(n-1)); i > 0; i >>= 1 {
		if x+i <= n && cur+f[x+i-1] <= k {
			x += i
			cur += f[x-1]
		}
	}
	return
}
