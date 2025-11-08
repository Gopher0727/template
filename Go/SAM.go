const ALPHABET_SIZE = 26

type Node struct {
    next   [ALPHABET_SIZE]int
    fail   int
    length int // 从初始态(空串)到该状态代表的最长子串长度
    cnt    int // 该状态对应的 endpos 大小 (出现次数)
}

type SAM struct {
    t []Node
}

func NewSAM() *SAM {
    s := &SAM{}
    s.t = make([]Node, 2)
    // 状态 0 是 "虚拟根"
    for i := range ALPHABET_SIZE {
        s.t[0].next[i] = 1
    }
    s.t[0].length = -1
    s.t[1].length = 0
    return s
}

func (sam *SAM) NewNode() int {
    sam.t = append(sam.t, Node{})
    return len(sam.t) - 1
}

func (sam *SAM) InsertString(s string) int {
    p := 1
    for _, ch := range s {
        p = sam.insert(p, int(ch-'a'))
    }
    return p
}

// 以状态 p 为当前尾部状态，插入字符 c，并返回插入后新的尾部状态
func (sam *SAM) insert(p, c int) int {
    // 已存在 p-(c)->q
    if sam.t[p].next[c] != 0 {
        q := sam.t[p].next[c]
        // 如果恰好满足 len[q] == len[p] + 1，则直接复用 q
        if sam.t[q].length == sam.t[p].length+1 {
            return q
        }
        // 否则需要拆分（clone）出 r
        r := sam.NewNode()
        // 复制 q 的转移、fail，设置 length 为 length[p] + 1
        sam.t[r].next = sam.t[q].next
        sam.t[r].fail = sam.t[q].fail
        sam.t[r].length = sam.t[p].length + 1
        sam.t[r].cnt = 0
        // 把 q 的 fail 指向 r，并把沿 fail 链上所有从 p 沿 c 到 q 的转移改为到 r
        sam.t[q].fail = r
        for p >= 0 && sam.t[p].next[c] == q {
            sam.t[p].next[c] = r
            p = sam.t[p].fail
        }
        return r
    }
    // 不存在从 p 出发的 c 边，需要创建新状态 cur
    cur := sam.NewNode()
    sam.t[cur].length = sam.t[p].length + 1
    sam.t[cur].cnt = 1
    // 沿 fail 链向上，为所有缺少 c 边的状态添加指向 cur 的转移
    for p >= 0 && sam.t[p].next[c] == 0 {
        sam.t[p].next[c] = cur
        p = sam.t[p].fail
    }
    sam.t[cur].fail = sam.insert(p, c)
    return cur
}

// 返回包含本质不同的字符串的个数
func (sam *SAM) GetDistinctCount() (diff int) {
    for i := 1; i < len(sam.t); i++ {
        diff += sam.t[i].length - sam.t[sam.t[i].fail].length
    }
    return
}

// 统计每个状态对应子串出现次数
// ! 只能运行一次
func (sam *SAM) GetOccurrences() {
    siz := len(sam.t)
    maxLen := 0
    for i := range siz {
        maxLen = max(maxLen, sam.t[i].length)
    }

    bucket := make([]int, maxLen+1)
    for i := 1; i < siz; i++ {
        bucket[sam.t[i].length]++
    }
    for i := 1; i <= maxLen; i++ {
        bucket[i] += bucket[i-1]
    }

    order := make([]int, siz)
    for i := siz - 1; i >= 1; i-- {
        l := sam.t[i].length
        bucket[l]--
        order[bucket[l]] = i
    }

    // 反向遍历（len 从大到小），把 cnt 累加到 fail
    for i := len(order) - 1; i > 0; i-- {
        v := order[i]
        f := sam.t[v].fail
        sam.t[f].cnt += sam.t[v].cnt
    }
}

// 查询模式串出现次数
// ! 在 getOccurrences() 调用之后
func (sam *SAM) CountOccurrences(p string) int {
    u := 1
    for _, ch := range p {
        c := int(ch - 'a')
        if sam.t[u].next[c] == 0 {
            return 0
        }
        u = sam.t[u].next[c]
    }
    return sam.t[u].cnt
}
