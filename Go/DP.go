// 给你两个字符串 s 和 t ，统计并返回在 s 的 子序列 中 t 出现的个数。
func get(s string, t string) int {
	n, m := len(s), len(t)
	if n < m {
		return 0
	}
	f := make([]int, m+1)
	f[0] = 1
	for i := range s {
		for j := min(i, m-1); j >= max(m-n+i, 0); j-- {
			if s[i] == t[j] {
				f[j+1] += f[j]
			}
		}
	}
	return f[m]
}
