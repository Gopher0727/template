func NxtArray(s string) []int {
    n := len(s)
    nxt := make([]int, n+1)
    nxt[0] = -1
    for i, j := 2, 0; i <= n; i++ {
        for j != -1 && s[i-1] != s[j] {
            j = nxt[j]
        }
        j++
        nxt[i] = j
    }
    return nxt
}

func KMP(s, p string) int {
    nxt := NxtArray(p)

    n, m := len(s), len(p)
    i, j := 0, 0
    for i < n && j < m {
        if s[i] == p[j] {
            i++
            j++
        } else if j == 0 {
            i++
        } else {
            j = nxt[j]
        }
    }
    if j == m {
        return i - j
    }
    return -1
}
