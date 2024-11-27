// Link: https://leetcode.cn/problems/select-cells-in-grid-with-maximum-score/

func maxScore(grid [][]int) int {
    pos := map[int]int{}
    for i, row := range grid {
        for _, x := range row {
            // 保存所有包含 x 的行号（压缩成二进制数）
            pos[x] |= 1 << i
        }
    }

    // 建图
    k := len(pos)
    m := len(grid)
    // rid 为反向边在邻接表中的下标
    type neighbor struct{ to, rid, cap, cost int }
    g := make([][]neighbor, k+m+2)
    addEdge := func(from, to, cap, cost int) {
        g[from] = append(g[from], neighbor{to, len(g[to]), cap, cost})
        g[to] = append(g[to], neighbor{from, len(g[from]) - 1, 0, -cost})
    }
    S := k + m
    T := k + m + 1
    i := 0
    for x, posMask := range pos {
        for t := uint(posMask); t > 0; t &= t - 1 {
            j := bits.TrailingZeros(t)
            addEdge(i, k+j, 1, 0)
        }
        addEdge(S, i, 1, -x)
        i++
    }
    for j := range grid {
        addEdge(k+j, T, 1, 0)
    }

    // 下面是费用流模板
    dis := make([]int, len(g))
    type vi struct{ v, i int }
    fa := make([]vi, len(g))
    inQ := make([]bool, len(g))
    spfa := func() bool {
        for i := range dis {
            dis[i] = math.MaxInt
        }
        dis[S] = 0
        inQ[S] = true
        q := []int{S}
        for len(q) > 0 {
            v := q[0]
            q = q[1:]
            inQ[v] = false
            for i, e := range g[v] {
                if e.cap == 0 {
                    continue
                }
                w := e.to
                newD := dis[v] + e.cost
                if newD < dis[w] {
                    dis[w] = newD
                    fa[w] = vi{v, i}
                    if !inQ[w] {
                        inQ[w] = true
                        q = append(q, w)
                    }
                }
            }
        }
        // 循环结束后所有 inQ[v] 都为 false，无需重置
        return dis[T] < math.MaxInt
    }

    minCost := 0
    for spfa() {
        minF := math.MaxInt
        for v := T; v != S; {
            p := fa[v]
            minF = min(minF, g[p.v][p.i].cap)
            v = p.v
        }
        for v := T; v != S; {
            p := fa[v]
            e := &g[p.v][p.i]
            e.cap -= minF
            g[v][e.rid].cap += minF
            v = p.v
        }
        minCost += dis[T] * minF
    }
    return -minCost
}
