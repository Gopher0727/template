type DSU struct {
    pa  []int
    siz []int
    stk [][2]int
}

func NewDSU(n int) *DSU {
    pa := make([]int, n)
    for i := range pa {
        pa[i] = i
    }
    siz := make([]int, n)
    for i := range siz {
        siz[i] = 1
    }
    return &DSU{pa, siz, [][2]int{}}
}

func (dsu *DSU) Find(x int) int {
    for x != dsu.pa[x] {
        x = dsu.pa[x]
    }
    return x
}

func (dsu *DSU) Merge(x, y int) bool {
    px, py := dsu.Find(x), dsu.Find(y)
    if px == py {
        return false
    }
    if dsu.siz[px] > dsu.siz[py] {
        px, py = py, px
    }
    dsu.pa[px] = py
    dsu.siz[py] += dsu.siz[px]
    dsu.stk = append(dsu.stk, [2]int{px, py})
    return true
}

func (dsu *DSU) Same(x, y int) bool {
    return dsu.Find(x) == dsu.Find(y)
}

func (dsu *DSU) Size(x int) int {
    return dsu.siz[dsu.Find(x)]
}

func (dsu *DSU) Version() int {
    return len(dsu.stk)
}

func (dsu *DSU) Rollback(v int) {
    for len(dsu.stk) > v {
        l := len(dsu.stk)
        x, y := dsu.stk[l-1][0], dsu.stk[l-1][1]
        dsu.stk = dsu.stk[:l-1]
        dsu.siz[y] -= dsu.siz[x]
        dsu.pa[x] = x
    }
}
