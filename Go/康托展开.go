// 核心思想：对于第 i 个位置，若该位置的数是未出现在其左侧的数中第 k 大的，那么有 (k−1)×(N−i)! 种方案在该位置上比这个排列小

// 康托展开（Cantor Expansion）
//
// 返回所给排列 perm（元素在 [1,n]）的字典序名次（可以从 0 或从 1 开始，具体看代码末尾）
func RankPermutation(perm []int) (ans int) {
    n := len(perm)

    fen := make(Fenwick, n)
    for i := range n {
        fen.Add(i, 1)
    }

    for i := range n {
        ans += fen.Query(perm[i]-1) * fac[n-i-1]
        ans %= MOD
        fen.Add(perm[i]-1, -1)
    }
    ans++ // 从 1 开始的排名
    ans %= MOD

    return
}

// 逆康托展开（Inverse Cantor Expansion）
//
// 返回字典序第 k 小的排列，元素范围为 [1,n]。Attention: n <= 20
func kthPermutation(n, k int) (ans []int) {
    k-- // 从 1 开始的排名

    fen := make(Fenwick, n)
    for i := range n {
        fen.Add(i, 1)
    }

    ans = make([]int, n)
    for i := range n {
        x := fen.Select(k / fac[n-i-1])
        ans[i] = x + 1
        fen.Add(x, -1)
        k %= fac[n-i-1]
    }
    return
}
