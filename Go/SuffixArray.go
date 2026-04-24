func SuffixArray(s string) (sa []int32, rnk []int, lcp []int) {
	n := len(s)

	// sa[k] = i 表示字典序第 k 小的后缀的起始位置为 i
	index := suffixarray.New([]byte(s))
	sa = *(*[]int32)(unsafe.Pointer(reflect.ValueOf(index).Elem().FieldByName("sa").Field(0).UnsafeAddr()))

	// rnk[i] 表示后缀 s[i:] 按字典序的排名
	rnk = make([]int, n)
	for i, x := range sa {
		rnk[x] = i
	}

	// lcp[i] = k 表示 s[i] 与 s[i-1] 的 LCP 长度为 k
	lcp = make([]int, n)
	h := 0
	for i, rk := range rnk {
		if h > 0 {
			h--
		}
		if rk > 0 {
			j := int(sa[rk-1])
			for i+h < n && j+h < n && s[i+h] == s[j+h] {
				h++
			}
		}
		lcp[rk] = h
	}
	return
}
