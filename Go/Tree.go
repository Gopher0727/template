// Tree 带权树
type Tree struct {
	n    int
	root int
	adj  [][][2]int
	pa   []int
	up   [][]int
	in   []int
	out  []int
	dep  []int
	dis  []int // 带权距离
	ord  []int
}

func NewTree(n int) *Tree {
	LOG := bits.Len(uint(n))
	up := make([][]int, LOG)
	for i := range LOG {
		up[i] = make([]int, n)
		for j := range n {
			up[i][j] = -1
		}
	}
	return &Tree{
		n:    n,
		root: 0,
		adj:  make([][][2]int, n),
		pa:   make([]int, n),
		up:   up,
		in:   make([]int, n),
		out:  make([]int, n),
		dep:  make([]int, n),
		dis:  make([]int, n),
		ord:  make([]int, n),
	}
}

// AddEdge 添加一条从 u 到 v，权重为 w 的单向边
func (t *Tree) AddEdge(u, v, w int) { t.adj[u] = append(t.adj[u], [2]int{v, w}) }

func (t *Tree) SetRoot(r int) { t.root = r }

func (t *Tree) Work(r int) {
	t.root = r
	clk := 0
	var dfs func(u, p int)
	dfs = func(u, p int) {
		t.pa[u] = p
		t.up[0][u] = p
		t.in[u] = clk
		t.ord[clk] = u
		clk++
		for _, e := range t.adj[u] {
			v, w := e[0], e[1]
			if v == p {
				continue
			}
			t.dep[v] = t.dep[u] + 1
			t.dis[v] = t.dis[u] + w
			dfs(v, u)
		}
		t.out[u] = clk
	}
	dfs(r, -1)

	for k := 1; k < len(t.up); k++ {
		for v := range t.n {
			m := t.up[k-1][v]
			if m == -1 {
				t.up[k][v] = -1
			} else {
				t.up[k][v] = t.up[k-1][m]
			}
		}
	}
}

func (t *Tree) GetKthAncestor(v, k int) int {
	for i := range len(t.up) {
		if k>>i&1 == 1 {
			v = t.up[i][v]
			if v == -1 {
				break
			}
		}
	}
	return v
}

func (t *Tree) LCA(u, v int) int {
	if t.dep[u] < t.dep[v] {
		u, v = v, u
	}
	u = t.GetKthAncestor(u, t.dep[u]-t.dep[v])
	if u == v {
		return u
	}
	for k := len(t.up) - 1; k >= 0; k-- {
		if t.up[k][u] != t.up[k][v] {
			u = t.up[k][u]
			v = t.up[k][v]
		}
	}
	return t.up[0][u]
}

// GetDistance 返回节点 u 和 v 之间的边数距离和带权距离
func (t *Tree) GetDistance(u, v int) (int, int) {
	o := t.LCA(u, v)
	return t.dep[u] + t.dep[v] - 2*t.dep[o], t.dis[u] + t.dis[v] - 2*t.dis[o]
}
