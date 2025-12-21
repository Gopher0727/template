type Tree struct {
	n    int
	root int
	adj  [][]int
	pa   []int
	in   []int
	out  []int
	dep  []int
	ord  []int
}

func NewTree(n int) *Tree {
	return &Tree{
		n:    n,
		root: 0,
		adj:  make([][]int, n),
		pa:   make([]int, n),
		in:   make([]int, n),
		out:  make([]int, n),
		dep:  make([]int, n),
		ord:  make([]int, n),
	}
}

func (t *Tree) AddEdge(u, v int) { t.adj[u] = append(t.adj[u], v) }

func (t *Tree) SetRoot(r int) { t.root = r }

func (t *Tree) Build() {
	clk := 0
	var dfs func(u, p int)
	dfs = func(u, p int) {
		t.pa[u] = p
		t.in[u] = clk
		t.ord[clk] = u
		clk++
		for _, v := range t.adj[u] {
			if v == p {
				continue
			}
			t.adj[u] = append(t.adj[u], v)
			t.dep[v] = t.dep[u] + 1
			dfs(v, u)
		}
		t.out[u] = clk
	}
	dfs(t.root, -1)
}
