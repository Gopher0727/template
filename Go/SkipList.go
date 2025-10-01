const (
    maxLevel = 32
    p        = 0.25
)

type SkipListNode struct {
    val int
    nxt []*SkipListNode
}

type SkipList struct {
    head  *SkipListNode
    level int
}

func NewSkipList() SkipList {
    return SkipList{
        head: &SkipListNode{
            val: -1,
            nxt: make([]*SkipListNode, maxLevel),
        },
        level: 0,
    }
}

func (s *SkipList) Search(target int) bool {
    cur := s.head
    for i := maxLevel - 1; i >= 0; i-- {
        for cur.nxt[i] != nil && cur.nxt[i].val < target {
            cur = cur.nxt[i]
        }
    }
    cur = cur.nxt[0]
    return cur != nil && cur.val == target
}

func (s *SkipList) Add(num int) {
    upd := make([]*SkipListNode, maxLevel)
    for i := range upd {
        upd[i] = s.head
    }

    cur := s.head
    for i := maxLevel - 1; i >= 0; i-- {
        for cur.nxt[i] != nil && cur.nxt[i].val < num {
            cur = cur.nxt[i]
        }
        upd[i] = cur
    }

    level := s.randomLevel()
    s.level = max(s.level, level)

    newNode := &SkipListNode{
        val: num,
        nxt: make([]*SkipListNode, level),
    }
    for i, node := range upd[:level] {
        newNode.nxt[i] = node.nxt[i]
        node.nxt[i] = newNode
    }
}

func (s *SkipList) Erase(num int) bool {
    upd := make([]*SkipListNode, maxLevel)

    cur := s.head
    for i := maxLevel - 1; i >= 0; i-- {
        for cur.nxt[i] != nil && cur.nxt[i].val < num {
            cur = cur.nxt[i]
        }
        upd[i] = cur
    }
    cur = cur.nxt[0]
    if cur == nil || cur.val != num {
        return false
    }

    for i := 0; i < s.level && upd[i].nxt[i] == cur; i++ {
        upd[i].nxt[i] = cur.nxt[i]
    }
    for s.level > 1 && s.head.nxt[s.level-1] == nil {
        s.level--
    }
    return true
}

func (s *SkipList) randomLevel() int {
    level := 1
    for level < maxLevel && rand.Float64() < p {
        level++
    }
    return level
}
