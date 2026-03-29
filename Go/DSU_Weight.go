type DSU struct {
    pa  []int
    xor []int
}

func NewDSU(n int) *DSU {
    pa := make([]int, n)
    for i := range pa {
        pa[i] = i
    }
    xor := make([]int, n)
    return &DSU{pa, xor}
}

func (dsu *DSU) Find(x int) int {
    if dsu.pa[x] != x {
        root := dsu.Find(dsu.pa[x])
        dsu.xor[x] ^= dsu.xor[dsu.pa[x]]
        dsu.pa[x] = root
    }
    return dsu.pa[x]
}

func (dsu *DSU) Merge(x, y int, val int) bool {
    px, py := dsu.Find(x), dsu.Find(y)
    if px == py {
        return dsu.xor[x]^dsu.xor[y] == val
    }
    dsu.xor[px] = dsu.xor[x] ^ dsu.xor[y] ^ val
    dsu.pa[px] = py
    return true
}
