type DSU struct {
	pa    []int
	siz   []int
	block int
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
	return &DSU{pa, siz, n}
}

func (dsu *DSU) Find(x int) int {
	root := x
	for dsu.pa[root] != root {
		root = dsu.pa[root]
	}
	for dsu.pa[x] != root {
		dsu.pa[x], x = root, dsu.pa[x]
	}
	return root
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
	dsu.block--
	return true
}

func (dsu *DSU) Same(x, y int) bool {
	return dsu.Find(x) == dsu.Find(y)
}

func (dsu *DSU) Size(x int) int {
	return dsu.siz[dsu.Find(x)]
}
