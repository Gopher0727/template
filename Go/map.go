// https://github.com/EndlessCheng/codeforces-go/blob/master/copypasta/treap/map/map.go
type Treap[K comparable, V any] struct {
    rd         uint
    root       *Node[K, V]
    comparator func(a, b K) int
}

func NewTreap[K cmp.Ordered, V any]() *Treap[K, V] {
    return &Treap[K, V]{
        rd:         uint(time.Now().UnixNano()),
        comparator: cmp.Compare[K],
    }
}

func NewTreapWith[K comparable, V any](comp func(a, b K) int) *Treap[K, V] {
    return &Treap[K, V]{
        rd:         uint(time.Now().UnixNano()),
        comparator: comp,
    }
}

func (t *Treap[K, V]) Size() int { return t.root.Size() }

func (t *Treap[K, V]) Empty() bool { return t.Size() == 0 }

func (t *Treap[K, V]) Find(key K) *Node[K, V] {
    o := t.Kth(t.Lowerbound(key))
    if o == nil || o.key != key {
        return nil
    }
    return o
}

func (t *Treap[K, V]) Put(key K, value V) { t.root = t._put(t.root, key, value) }

func (t *Treap[K, V]) Delete(key K) { t.root = t._delete(t.root, key) }

func (t *Treap[K, V]) Min() *Node[K, V] { return t.Kth(0) }

func (t *Treap[K, V]) Max() *Node[K, V] { return t.Kth(t.Size() - 1) }

// Prev 返回小于 key 的最大节点
func (t *Treap[K, V]) Prev(key K) *Node[K, V] { return t.Kth(t.Lowerbound(key) - 1) }

// Next 返回大于 key 的最小节点
func (t *Treap[K, V]) Next(key K) *Node[K, V] { return t.Kth(t.Upperbound(key)) }

func (t *Treap[K, V]) Lowerbound(key K) (kth int) {
    for o := t.root; o != nil; {
        c := t.comparator(key, o.key)
        if c < 0 {
            o = o.son[0]
        } else if c > 0 {
            kth += o.son[0].Size() + 1
            o = o.son[1]
        } else {
            kth += o.son[0].Size()
            break
        }
    }
    return
}

func (t *Treap[K, V]) Upperbound(key K) (kth int) {
    for o := t.root; o != nil; {
        c := t.comparator(key, o.key)
        if c < 0 {
            o = o.son[0]
        } else if c > 0 {
            kth += o.son[0].Size() + 1
            o = o.son[1]
        } else {
            kth += o.son[0].Size() + 1
            break
        }
    }
    return
}

// Kth 返回第 k 小的节点，k 从 0 开始
func (t *Treap[K, V]) Kth(k int) *Node[K, V] {
    if k < 0 || k >= t.Size() {
        return nil
    }
    o := t.root
    for o != nil {
        leftSize := o.son[0].Size()
        if k < leftSize {
            o = o.son[0]
        } else {
            k -= leftSize + 1
            if k < 0 {
                break
            }
            o = o.son[1]
        }
    }
    return o
}

func (t *Treap[K, V]) _put(o *Node[K, V], key K, value V) *Node[K, V] {
    if o == nil {
        o = &Node[K, V]{priority: t.fastRand(), key: key, value: value}
    } else {
        c := t.comparator(key, o.key)
        if c == 0 { // 相等
            o.value = value
        } else {
            // c < 0 去左边，c > 0 去右边
            d := 0
            if c > 0 {
                d = 1
            }
            o.son[d] = t._put(o.son[d], key, value)
            if o.son[d].priority > o.priority {
                o = o.rotate(d ^ 1)
            }
        }
    }
    o.pull()
    return o
}

func (t *Treap[K, V]) _delete(o *Node[K, V], key K) *Node[K, V] {
    if o == nil {
        return nil
    }
    if c := t.comparator(key, o.key); c != 0 {
        // < 0 去左边，> 0 去右边
        d := 0
        if c > 0 {
            d = 1
        }
        o.son[d] = t._delete(o.son[d], key)
    } else { // 相等，删除
        if o.son[1] == nil {
            return o.son[0]
        }
        if o.son[0] == nil {
            return o.son[1]
        }
        d := 0
        if o.son[0].priority < o.son[1].priority {
            d = 1
        }
        o = o.rotate(d)
        o.son[d] = t._delete(o.son[d], key)
    }
    o.pull()
    return o
}

func (t *Treap[K, V]) fastRand() uint {
    t.rd ^= t.rd << 13
    t.rd ^= t.rd >> 17
    t.rd ^= t.rd << 5
    return t.rd
}

type Node[K comparable, V any] struct {
    key      K
    value    V
    son      [2]*Node[K, V]
    priority uint
    subSize  int
}

func (o *Node[K, V]) Size() int {
    if o != nil {
        return o.subSize
    }
    return 0
}

func (o *Node[K, V]) rotate(d int) *Node[K, V] {
    x := o.son[d^1]
    o.son[d^1] = x.son[d]
    x.son[d] = o
    o.pull()
    x.pull()
    return x
}

func (o *Node[K, V]) pull() {
    o.subSize = 1 + o.son[0].Size() + o.son[1].Size()
}
