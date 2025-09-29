type Trie struct {
    son   [26]*Trie
    isEnd bool
    cnt   int
}

func (t *Trie) Insert(s string) {
    p := t
    for _, ch := range s {
        i := ch - 'a'
        if p.son[i] == nil {
            p.son[i] = &Trie{}
        }
        p = p.son[i]
    }
    p.isEnd = true
    p.cnt++
}

func (t *Trie) Find(s string) bool {
    p := t
    for _, ch := range s {
        i := ch - 'a'
        if p.son[i] == nil {
            return false
        }
        p = p.son[i]
    }
    return p.isEnd
}

func (t *Trie) StartsWith(s string) bool {
    p := t
    for _, ch := range s {
        i := ch - 'a'
        if p.son[i] == nil {
            return false
        }
        p = p.son[i]
    }
    return true
}

func (t *Trie) GetCnt(s string) int {
    p := t
    for _, ch := range s {
        i := ch - 'a'
        if p.son[i] == nil {
            return 0
        }
        p = p.son[i]
    }
    return p.cnt
}
