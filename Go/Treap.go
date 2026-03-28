var rnd = rand.New(rand.NewPCG(rand.Uint64(), rand.Uint64()))

type Node struct {
    key   int
    cnt   int // 权重
    siz   int // 子树大小
    prio  int // 优先级
    left  *Node
    right *Node
}

func (o *Node) Insert(k, c int) *Node {
    l, r := o.Split(k)
    m, r := r.Split(k + 1)
    if m == nil {
        m = &Node{
            key:   k,
            cnt:   c,
            siz:   c,
            prio:  rnd.Int(),
            left:  nil,
            right: nil,
        }
    } else {
        m.cnt += c
        m.siz += c
    }
    return l.Merge(m.Merge(r))
}

func (o *Node) Erase(k, c int) *Node {
    l, r := o.Split(k)
    m, r := r.Split(k + 1)
    if m == nil || m.cnt < c {
        goto O
    }
    if m.cnt > c {
        m.cnt -= c
        m.siz -= c
    } else if m.cnt == c {
        m = nil
    }
    return l.Merge(m.Merge(r))
O:
    _, _, line, _ := runtime.Caller(1)
    panic("[line " + strconv.FormatInt(int64(line), 10) + "] key is insufficient")
}

// Split returns l: < k / r: >= k
func (o *Node) Split(k int) (*Node, *Node) {
    if o == nil {
        return nil, nil
    }
    if o.key < k {
        l, r := o.right.Split(k)
        o.right = l
        o.pull()
        return o, r
    } else {
        l, r := o.left.Split(k)
        o.left = r
        o.pull()
        return l, o
    }
}

func (o *Node) Merge(other *Node) *Node {
    if o == nil {
        return other
    }
    if other == nil {
        return o
    }
    if o.prio < other.prio {
        o.right = o.right.Merge(other)
        o.pull()
        return o
    } else {
        other.left = o.Merge(other.left)
        other.pull()
        return other
    }
}

// BinarySearch return res: >= x
func (o *Node) BinarySearch(x int) (res *Node) {
    if o == nil {
        return
    }
    p := o
    for p != nil {
        if p.key >= x {
            res = p
            p = p.left
        } else {
            p = p.right
        }
    }
    return
}

func (o *Node) Size() int {
    if o == nil {
        return 0
    }
    return o.siz
}

func (o *Node) pull() {
    if o != nil {
        o.siz = o.cnt + o.left.Size() + o.right.Size()
    }
}
