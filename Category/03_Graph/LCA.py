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
