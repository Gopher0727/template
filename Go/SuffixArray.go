func SuffixArray(s string) ([]int32, []int, []int) {
    index := suffixarray.New([]byte(s))

    // 找到 sa
    sa := *(*[]int32)(unsafe.Pointer(reflect.ValueOf(index).Elem().FieldByName("sa").Field(0).UnsafeAddr()))

    // rank 数组
    n := len(s)
    rank := make([]int, n)
    for i := range n {
        rank[sa[i]] = i
    }

    // height 数组
    height := make([]int, n)
    h := 0
    for i, rk := range rank {
        if h > 0 {
            h--
        }
        if rk > 0 {
            j := int(sa[rk-1])
            for i+h < n && j+h < n && s[i+h] == s[j+h] {
                h++
            }
        }
        height[rk] = h
    }

    return sa, rank, height
}
