// 给定一个数组 a，返回长度至少为 k 且中位数最大的子数组
// 1 <= ai <= n
func get(a []int) int {
	n := len(a)

	check := func(x int) bool {
		pre := make([]int, n+1)
		for i := 0; i < n; i++ {
			pre[i+1] = pre[i]
			if a[i] >= x {
				pre[i+1]++
			} else {
				pre[i+1]--
			}
		}
		mn := 0
		for i := k; i <= n; i++ {
			if pre[i-k] < mn {
				mn = pre[i-k]
			}
			if pre[i] > mn {
				return true
			}
		}
		return false
	}

	lo, hi := 0, n+1
	for lo+1 < hi {
		mid := lo + (hi-lo)/2
		if check(mid) {
			lo = mid
		} else {
			hi = mid
		}
	}
	return lo
}
