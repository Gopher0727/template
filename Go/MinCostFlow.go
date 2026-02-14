// 定义超级源点与超级汇点
// todo
var S int
var T int

type Neighbor struct {
    to, rid, cap, cost int
}
g := make([][]Neighbor, T+1)

AddEdge := func(from, to, cap, cost int) {
    g[from] = append(g[from], Neighbor{to, len(g[to]), cap, cost})
    g[to] = append(g[to], Neighbor{from, len(g[from]) - 1, 0, -cost})
}

// 左部向右部连边，超级源点向左部连边，右部向超级汇点连边
// todo

pa := make([][2]int, T+1)
dis := make([]int, T+1)
inQ := make([]bool, T+1)

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
        inQ[v] = true
        for i := range g[v] {
            if g[v][i].cap == 0 {
                continue
            }
            w := g[v][i].to
            if nd := dis[v] + g[v][i].cost; nd < dis[w] {
                dis[w] = nd
                pa[w] = [2]int{v, i}
                if !inQ[w] {
                    inQ[w] = true
                    q = append(q, w)
                }
            }
        }
    }
    return dis[T] < math.MaxInt // todo
}

minCost := 0
for spfa() {
    minF := math.MaxInt
    for v := T; v != S; {
        pv, pi := pa[v][0], pa[v][1]
        minF = min(minF, g[pv][pi].cap)
        v = pv
    }
    for v := T; v != S; {
        pv, pi := pa[v][0], pa[v][1]
        g[pv][pi].cap -= minF
        g[v][g[pv][pi].rid].cap += minF
        v = pv
    }
    minCost += dis[T] * minF
}
