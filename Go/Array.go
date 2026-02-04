type Number interface {
	~int | ~int8 | ~int16 | ~int32 | ~int64 |
		~uint | ~uint8 | ~uint16 | ~uint32 | ~uint64 |
		~float32 | ~float64 |
		~complex64 | ~complex128
}

// RangeSum [l, r] 闭区间求和
func RangeSum[T Number](a []T, l, r int) (ans T) {
	for l <= r {
		ans += a[l]
		l++
	}
	return
}

// Reverse [l, r] 闭区间翻转
func Reverse[T any](a []T, l, r int) {
	for l < r {
		a[l], a[r] = a[r], a[l]
		l++
		r--
	}
}

// PrintArray 在一行打印数组元素，并按空格分开
func PrintArray[T any](a []T) {
	for _, x := range a {
		fmt.Fprint(out, x, " ")
	}
	fmt.Fprintln(out)
}
