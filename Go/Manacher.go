func Manacher(s string) (ans []int) {
    m := len(s)*2 + 1
    t := make([]rune, m)
    for i, j := 0, 0; i < m; i++ {
        if i%2 == 0 {
            t[i] = '#'
        } else {
            t[i] = rune(s[j])
            j++
        }
    }
    for i, c, r := 0, 0, 0; i < m; i++ {
        l := 1
        if r > i {
            l = min(ans[2*c-i], r-i)
        }
        for i+l < m && i >= l && t[i-l] == t[i+l] {
            l++
        }
        if i+l > r {
            r, c = i+l, i
        }
        ans = append(ans, l-1)
    }
    return
}
