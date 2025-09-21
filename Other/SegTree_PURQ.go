type SegTree struct {
    n    int
    info []Info
}

func NewSegTree(init []Info) *SegTree {
    n := len(init)
    if n == 0 {
        panic("SegTree: init length must be > 0")
    }
    siz := 2 << bits.Len(uint(n-1))
    seg := &SegTree{
        n:    n,
        info: make([]Info, siz),
    }
    var build func(o, l, r int)
    build = func(o, l, r int) {
        if l == r {
            seg.info[o] = init[l]
            return
        }
        mid := (l + r) >> 1
        build(o<<1, l, mid)
        build(o<<1|1, mid+1, r)
        seg.info[o] = mergeInfo(seg.info[o<<1], seg.info[o<<1|1])
    }
    build(1, 0, n-1)
    return seg
}

func (seg *SegTree) pull(o int) {
    seg.info[o] = mergeInfo(seg.info[o<<1], seg.info[o<<1|1])
}

func (seg *SegTree) Modify(p int, v Info) {
    if p < 0 || p >= seg.n {
        panic("SegTree.Modify: index out of range")
    }
    var dfs func(o, l, r int)
    dfs = func(o, l, r int) {
        if l == r {
            seg.info[o] = v
            return
        }
        mid := (l + r) >> 1
        if p <= mid {
            dfs(o<<1, l, mid)
        } else {
            dfs(o<<1|1, mid+1, r)
        }
        seg.pull(o)
    }
    dfs(1, 0, seg.n-1)
}

func (seg *SegTree) Query(p int) Info {
    if p < 0 || p >= seg.n {
        panic("SegTree.Query: index out of range")
    }
    var dfs func(o, l, r int) Info
    dfs = func(o, l, r int) Info {
        if l == r {
            return seg.info[o]
        }
        mid := (l + r) >> 1
        if p <= mid {
            return dfs(o<<1, l, mid)
        }
        return dfs(o<<1|1, mid+1, r)
    }
    return dfs(1, 0, seg.n-1)
}

func (seg *SegTree) RangeQuery(L, R int) Info {
    if L < 0 || R >= seg.n || L > R {
        panic("SegTree.RangeQuery: invalid range")
    }
    var dfs func(o, l, r int) Info
    dfs = func(o, l, r int) Info {
        if L <= l && r <= R {
            return seg.info[o]
        }
        mid := (l + r) >> 1
        if R <= mid {
            return dfs(o<<1, l, mid)
        }
        if mid < L {
            return dfs(o<<1|1, mid+1, r)
        }
        return mergeInfo(dfs(o<<1, l, mid), dfs(o<<1|1, mid+1, r))
    }
    return dfs(1, 0, seg.n-1)
}

func (seg *SegTree) FindFirst(L, R int, pred func(Info) bool) int {
    if L < 0 || R >= seg.n || L > R {
        return -1
    }
    var dfs func(o, l, r int) int
    dfs = func(o, l, r int) int {
        if r < L || l > R || !pred(seg.info[o]) {
            return -1
        }
        if l == r {
            return l
        }
        mid := (l + r) >> 1
        if j := dfs(o<<1, l, mid); j != -1 {
            return j
        }
        return dfs(o<<1|1, mid+1, r)
    }
    return dfs(1, 0, seg.n-1)
}

func (seg *SegTree) FindLast(L, R int, pred func(Info) bool) int {
    if L < 0 || R >= seg.n || L > R {
        return -1
    }
    var dfs func(o, l, r int) int
    dfs = func(o, l, r int) int {
        if r < L || l > R || !pred(seg.info[o]) {
            return -1
        }
        if l == r {
            return l
        }
        mid := (l + r) >> 1
        if j := dfs(o<<1|1, mid+1, r); j != -1 {
            return j
        }
        return dfs(o<<1, l, mid)
    }
    return dfs(1, 0, seg.n-1)
}

type Info struct {
    max, min int
}

func mergeInfo(a, b Info) Info {
    return Info{
        max: max(a.max, b.max),
        min: min(a.min, b.min),
    }
}
