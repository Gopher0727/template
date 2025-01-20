class DSU:
    __slots__ = "parent", "_size", "block"

    def __init__(self, n):
        self.block = n
        self.parent = list(range(n))
        self._size = [1] * n

    def __len__(self):
        return self.block

    def find(self, x):
        t = x
        while self.parent[x] != x:
            x = self.parent[x]
        while t != x:
            t, self.parent[t] = self.parent[t], x
        return x

    def merge(self, x, y):
        rx, ry = self.find(x), self.find(y)
        if rx == ry:
            return False
        if self._size[rx] > self._size[ry]:
            rx, ry = ry, rx
        self.parent[rx] = ry
        self._size[ry] += self._size[rx]
        self.block -= 1
        return True

    def same(self, x, y):
        return self.find(x) == self.find(y)

    def size(self, x):
        return self._size[self.find(x)]
