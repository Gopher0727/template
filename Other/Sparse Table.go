type Info struct {
    max, min int
}

type SparseTable struct {
    g  [][]Info
    Op func(Info, Info) Info
}

func NewSparseTable(a []Info, Op func(Info, Info) Info) SparseTable {
    n := len(a)
    s := bits.Len(uint(n)) - 1
    g := make([][]Info, s+1)
    for i := range g {
        g[i] = make([]Info, n)
    }
    for i := range n {
        g[0][i] = a[i]
    }
    for i := 1; i <= s; i++ {
        for j := 0; j+1<<i-1 < n; j++ {
            g[i][j] = Op(g[i-1][j], g[i-1][j+1<<(i-1)])
        }
    }
    return SparseTable{
        g:  g,
        Op: Op,
    }
}

func (st SparseTable) Query(l, r int) Info {
    s := bits.Len(uint(r-l+1)) - 1
    return st.Op(st.g[s][l], st.g[s][r-1<<s+1])
}
