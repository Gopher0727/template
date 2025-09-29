type StringHash struct {
    Mod1, Mod2         int
    Base1, Base2       int
    powBase1, powBase2 []int
    preHash1, preHash2 []int
}

// 支持 string 或 []int
func NewStringHash[T ~string | ~[]int](s T) *StringHash {
    var arr []int
    switch v := any(s).(type) {
    case string:
        arr = make([]int, len(v))
        for i := range v {
            arr[i] = int(v[i] - 'a' + 1)
        }
    case []int:
        arr = v
    }

    n := len(arr)
    h := &StringHash{
        Mod1: 1070777777,
        Mod2: 1000000007,
    }
    rng := rand.New(rand.NewSource(time.Now().UnixNano()))
    h.Base1 = rng.Intn(900000000-800000000) + 800000000
    h.Base2 = rng.Intn(900000000-800000000) + 800000000

    h.powBase1 = make([]int, n+1)
    h.powBase2 = make([]int, n+1)
    h.preHash1 = make([]int, n+1)
    h.preHash2 = make([]int, n+1)
    h.powBase1[0], h.powBase2[0] = 1, 1
    for i := 0; i < n; i++ {
        h.powBase1[i+1] = int(((h.powBase1[i]) * (h.Base1)) % (h.Mod1))
        h.powBase2[i+1] = int(((h.powBase2[i]) * (h.Base2)) % (h.Mod2))
        h.preHash1[i+1] = int(((h.preHash1[i])*(h.Base1) + (arr[i])) % (h.Mod1))
        h.preHash2[i+1] = int(((h.preHash2[i])*(h.Base2) + (arr[i])) % (h.Mod2))
    }
    return h
}

// 返回区间 [l, r] 的哈希
func (h *StringHash) Get(l, r int) int {
    h1 := (h.preHash1[r+1] - h.preHash1[l]*h.powBase1[r-l+1]) % h.Mod1
    h2 := (h.preHash2[r+1] - h.preHash2[l]*h.powBase2[r-l+1]) % h.Mod2
    if h1 < 0 {
        h1 += h.Mod1
    }
    if h2 < 0 {
        h2 += h.Mod2
    }
    return h1<<32 | h2
}

// 判断 [l1, r1] 是否为 [l2, r2] 的前缀
func (h *StringHash) IsPre(l1, r1, l2, r2 int) bool {
    len1 := r1 - l1 + 1
    len2 := r2 - l2 + 1
    if len1 > len2 {
        return false
    }
    return h.Get(l1, r1) == h.Get(l2, l2+len1-1)
}
