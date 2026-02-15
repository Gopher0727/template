type HLD struct {
	n    int
	root int
	clk  int
	g    [][]int
	pa   []int
	siz  []int
	dep  []int
	top  []int
	in   []int
	out  []int
	seq  []int
}

func NewHLD(n int) *HLD {
	return &HLD{
		n:    n,
		root: 0,
		clk:  0,
		g:    make([][]int, n),
		pa:   make([]int, n),
		siz:  make([]int, n),
		dep:  make([]int, n),
		top:  make([]int, n),
		in:   make([]int, n),
		out:  make([]int, n),
		seq:  make([]int, 0),
	}
}

func (h *HLD) AddEdge(u, v int) { h.g[u] = append(h.g[u], v) }

func (h *HLD) SetRoot(r int) { h.root = r }

func (h *HLD) Work() {
	h.pa[h.root] = -1
	h.dep[h.root] = 0
	h.top[h.root] = h.root
	h.dfs1(h.root)
	h.dfs2(h.root)
}

func (h *HLD) dfs1(u int) {
	if h.pa[u] != -1 {
		for i, v := range h.g[u] {
			if v == h.pa[u] {
				h.g[u] = append(h.g[u][:i], h.g[u][i+1:]...)
				break
			}
		}
	}
	h.siz[u] = 1
	for _, v := range h.g[u] {
		h.pa[v] = u
		h.dep[v] = h.dep[u] + 1
		h.dfs1(v)
		h.siz[u] += h.siz[v]
		if h.siz[h.g[u][0]] < h.siz[v] {
			h.g[u][0], v = v, h.g[u][0]
		}
	}
}

func (h *HLD) dfs2(u int) {
	h.in[u] = h.clk
	h.clk++
	for _, v := range h.g[u] {
		if v == h.g[u][0] {
			h.top[v] = h.g[u][0]
		} else {
			h.top[v] = v
		}
		h.dfs2(v)
	}
	h.out[u] = h.clk
}

func (h *HLD) LCA(u, v int) int {
	for h.top[u] != h.top[v] {
		if h.dep[h.top[u]] < h.dep[h.top[v]] {
			u, v = v, u
		}
		u = h.pa[h.top[u]]
	}
	if h.dep[u] > h.dep[v] {
		u, v = v, u
	}
	return u
}

func (h *HLD) Dist(u, v int) int {
	return h.dep[u] + h.dep[v] - 2*h.dep[h.LCA(u, v)]
}

func (h *HLD) Jump(u, k int) int {
	if h.dep[u] < k {
		return -1
	}
	d := h.dep[u] - k
	for h.dep[h.top[u]] > d {
		u = h.pa[h.top[u]]
	}
	return h.seq[h.in[u]-h.dep[u]+d]
}

func (h *HLD) IsAncestor(u, v int) bool {
	return h.in[u] <= h.in[v] && h.in[v] < h.out[u]
}

func (h *HLD) RootedParent(u, v int) int {
	if u == v {
		return v
	}
	if !h.IsAncestor(v, u) {
		return h.pa[v]
	}
	idx := sort.Search(len(h.g[v]), func(i int) bool {
		return h.in[h.g[v][i]] > h.in[u]
	})
	return h.g[v][idx-1]
}

func (h *HLD) RootedSize(u, v int) int {
	if u == v {
		return h.n
	}
	if !h.IsAncestor(v, u) {
		return h.siz[v]
	}
	return h.n - h.siz[h.RootedParent(u, v)]
}

func (h *HLD) RootedLCA(a, b, c int) int {
	return h.LCA(a, b) ^ h.LCA(b, c) ^ h.LCA(c, a)
}
