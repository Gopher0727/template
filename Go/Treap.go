type Node struct {
    key      int
    priority int
    left     *Node
    right    *Node
}

type Treap struct {
    o   *Node
    rnd *rand.Rand
}

func NewTreap() *Treap {
    return &Treap{rnd: rand.New(rand.NewSource(time.Now().UnixNano()))}
}

func split(o *Node, key int) (left *Node, right *Node) {
    if o == nil {
        return nil, nil
    }
    if o.key < key {
        l, r := split(o.right, key)
        o.right = l
        return o, r
    } else {
        l, r := split(o.left, key)
        o.left = r
        return l, o
    }
}

func merge(a, b *Node) *Node {
    if a == nil {
        return b
    }
    if b == nil {
        return a
    }
    if a.priority < b.priority {
        a.right = merge(a.right, b)
        return a
    } else {
        b.left = merge(a, b.left)
        return b
    }
}

func (t *Treap) Insert(key int) {
    a, b := split(t.o, key)
    n := &Node{key: key, priority: t.rnd.Int()}
    t.o = merge(merge(a, n), b)
}

func (t *Treap) Erase(key int) {
    a, b := split(t.o, key)
    _, b2 := split(b, key+1)
    t.o = merge(a, b2)
}

func (t *Treap) LowerBound(x int) *Node {
    cur := t.o
    var res *Node = nil
    for cur != nil {
        if cur.key >= x {
            res = cur
            cur = cur.left
        } else {
            cur = cur.right
        }
    }
    return res
}
