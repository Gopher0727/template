class LCA:
    __slots__ = "depth", "parents"

    def __init__(self, g):
        n = len(g)
        m = n.bit_length()
        depth = [0] * n
        parents = [[-1] * n for _ in range(m)]

        rt = 0
        stk = [rt]
        while stk:
            u = stk.pop()
            for v in g[u]:
                if parents[0][u] != v:
                    parents[0][v] = u
                    depth[v] = depth[u] + 1
                    stk.append(v)

        for d in range(m - 1):
            for u in range(n):
                pa = parents[d][u]
                if pa != -1:
                    parents[d + 1][u] = parents[d][pa]

        self.depth = depth
        self.parents = parents

    def get_kth_ancestor(self, u: int, k: int) -> int:
        while k and u != -1:
            lb = k & -k
            u = self.parents[lb.bit_length() - 1][u]
            k ^= lb
        return u

    def get_lca(self, u: int, v: int) -> int:
        if self.depth[u] < self.depth[v]:
            u, v = v, u
        u = self.get_kth_ancestor(u, self.depth[u] - self.depth[v])
        if u == v:
            return u
        for d in range(self.depth[u].bit_length() - 1, -1, -1):
            pau, pav = self.parents[d][u], self.parents[d][v]
            if pau != pav:
                u, v = pau, pav
        return self.parents[0][u]

    def get_lca_all(self, nodes) -> int:
        lca = nodes[0]
        for i in range(1, len(nodes)):
            lca = self.get_lca(lca, nodes[i])
        return lca


# another template
class LCA:
    def __init__(self, parent, depth):
        # g[v]: (cost, u)
        self.n = len(parent)
        self.num = (self.n).bit_length()
        self.depth = depth[:]
        self.parent = [[-1] * self.n for i in range(self.num)]
        self.parent[0] = parent[:]
        # doubling
        for k in range(self.num - 1):
            for v in range(self.n):
                if self.parent[k][v] == -1:
                    self.parent[k + 1][v] = -1
                else:
                    self.parent[k + 1][v] = self.parent[k][self.parent[k][v]]

    def getLCA(self, u, v):
        if self.depth[u] > self.depth[v]:
            u, v = v, u
        for k in range(self.num):
            if ((self.depth[v] - self.depth[u]) >> k) & 1:
                v = self.parent[k][v]
        if u == v:
            return u

        for k in reversed(range(self.num)):
            if self.parent[k][u] != self.parent[k][v]:
                u = self.parent[k][u]
                v = self.parent[k][v]
        return self.parent[0][u]

    def search(self, v, x):
        for k in reversed(range(self.num)):
            if x >> k & 1:
                v = self.parent[k][v]
        return v
