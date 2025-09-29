type SegTree struct {
    n    int
    info []Info
    tag  []Tag
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
        tag:  make([]Tag, siz),
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

func (seg *SegTree) apply(o int, t Tag) {
    seg.info[o].applyInfo(t)
    seg.tag[o].applyTag(t)
}

func (seg *SegTree) push(o int) {
    seg.apply(o<<1, seg.tag[o])
    seg.apply(o<<1|1, seg.tag[o])
    seg.tag[o] = Tag{}
}

func (seg *SegTree) pull(o int) {
    seg.info[o] = mergeInfo(seg.info[o<<1], seg.info[o<<1|1])
}

func (seg *SegTree) ModifyPoint(p int, v Info) {
    if p < 0 || p >= seg.n {
        panic("SegTree.ModifyPoint: index out of range")
    }
    var dfs func(o, l, r int)
    dfs = func(o, l, r int) {
        if l == r {
            seg.info[o] = v
            return
        }
        seg.push(o)
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

func (seg *SegTree) ModifyRange(L, R int, t Tag) {
    if L < 0 || R >= seg.n || L > R {
        panic("SegTree.ModifyRange: invalid range")
    }
    var dfs func(o, l, r int)
    dfs = func(o, l, r int) {
        if L <= l && r <= R {
            seg.apply(o, t)
            return
        }
        seg.push(o)
        mid := (l + r) >> 1
        if L <= mid {
            dfs(o<<1, l, mid)
        }
        if mid < R {
            dfs(o<<1|1, mid+1, r)
        }
        seg.pull(o)
    }
    dfs(1, 0, seg.n-1)
}

func (seg *SegTree) QueryPoint(p int) Info {
    if p < 0 || p >= seg.n {
        panic("SegTree.QueryPoint: index out of range")
    }
    var dfs func(o, l, r int) Info
    dfs = func(o, l, r int) Info {
        if l == r {
            return seg.info[o]
        }
        seg.push(o)
        mid := (l + r) >> 1
        if p <= mid {
            return dfs(o<<1, l, mid)
        }
        return dfs(o<<1|1, mid+1, r)
    }
    return dfs(1, 0, seg.n-1)
}

func (seg *SegTree) QueryRange(L, R int) Info {
    if L < 0 || R >= seg.n || L > R {
        panic("SegTree.QueryRange: invalid range")
    }
    var dfs func(o, l, r int) Info
    dfs = func(o, l, r int) Info {
        if L <= l && r <= R {
            return seg.info[o]
        }
        seg.push(o)
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
        seg.push(o)
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
        seg.push(o)
        mid := (l + r) >> 1
        if j := dfs(o<<1|1, mid+1, r); j != -1 {
            return j
        }
        return dfs(o<<1, l, mid)
    }
    return dfs(1, 0, seg.n-1)
}

type Tag struct {
    add int
}

func (t *Tag) applyTag(other Tag) {
    if other.add != 0 {
        t.add += other.add
    }
}

type Info struct {
    sum, len int
}

func (info *Info) applyInfo(t Tag) {
    info.sum += info.len * t.add
}

func mergeInfo(a, b Info) Info {
    return Info{
        sum: a.sum + b.sum,
        len: a.len + b.len,
    }
}
