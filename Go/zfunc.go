func zfunc(s string) []int {
    n := len(s)
    z := make([]int, n)
    for i, l, r := 1, 0, 0; i < n; i++ {
        if i <= r {
            z[i] = min(z[i-l], r-i+1)
        }
        for i+z[i] < n && s[z[i]] == s[i+z[i]] {
            l = i
            r = i + z[i]
            z[i]++
        }
    }
    return z
}
