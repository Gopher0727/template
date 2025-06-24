class DSU:
    __slots__ = "parent", "siz", "_edges", "_loop", "block"

    def __init__(self, n: int) -> None:
        self.parent = list(range(n))
        self.siz = [1] * n
        self._edges = [0] * n
        self._loop = [False] * n
        self.block = n

    def find(self, x: int) -> int:
        rt = x
        while self.parent[x] != x:
            x = self.parent[x]
        while rt != x:
            rt, self.parent[rt] = self.parent[rt], x
        return x

    def merge(self, x: int, y: int) -> bool:
        if x == y:
            self._loop[self.find(x)] = True
        x, y = self.find(x), self.find(y)
        self._edges[x] += 1
        if x == y:
            return False
        if self.siz[x] < self.siz[y]:
            x, y = y, x
        self.parent[x] = y
        self.siz[y] += self.siz[x]
        self._edges[y] += self._edges[x]
        self._loop[y] |= self._loop[x]
        self.block -= 1
        return True

    def same(self, x: int, y: int) -> bool:
        return self.find(x) == self.find(y)

    def size(self, x: int) -> int:
        return self.siz[self.find(x)]

    # 判断连通块里有无自环
    def self_loop(self, x: int) -> bool:
        return self._loop[self.find(x)]

    # 判断连通块是否成环（包括自环）
    def loop(self, x: int) -> bool:
        return self.siz[self.find(x)] <= self._edges[self.find(x)]

    def edges(self, x: int) -> int:
        return self._edges[self.find(x)]

    def groups(self):
        n = len(self.parent)
        res = []
        idx = [-1] * n
        for i in range(n):
            rt = self.find(i)
            if idx[rt] == -1:
                idx[rt] = len(res)
                res.append([])
            res[idx[rt]].append(i)
        return res
