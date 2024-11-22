class LazySegTree:
    __slots__ = "n", "height", "size", "idval", "idlazy", "Op", "apply", "compose", "tree", "lazy"

    def __init__(self, nums, idval, idlazy, Op, apply, compose):
        self.n = len(nums)
        self.height = (self.n - 1).bit_length()
        self.size = 1 << self.height
        self.idval = idval
        self.idlazy = idlazy
        self.Op = Op
        self.apply = apply
        self.compose = compose

        self.tree = [idval for _ in range(2 * self.size)]
        self.tree[self.size : self.size + self.n] = nums
        for i in range(self.size - 1, 0, -1):
            self.pull(i)
        self.lazy = [idlazy for _ in range(self.size)]

    def pull(self, rt):
        self.tree[rt] = self.Op(self.tree[rt * 2], self.tree[rt * 2 + 1])

    def push(self, rt):
        if self.lazy[rt] == self.idlazy:
            return
        self.modify(rt * 2, self.lazy[rt])
        self.modify(rt * 2 + 1, self.lazy[rt])
        self.lazy[rt] = self.idlazy

    def modify(self, left, right, val):
        if left > right:
            return
        left += self.size
        right += self.size

        for i in range(self.height, 0, -1):
            if left >> i << i != left:
                self.push(left >> i)
            if (right + 1) >> i << i != right + 1:
                self.push(right >> i)

        l, r = left, right
        while left <= right:
            if left & 1:
                self.modify(left, val)
                left += 1
            if not right & 1:
                self.modify(right, val)
                right -= 1
            left >>= 1
            right >>= 1

        left, right = l, r
        for i in range(1, self.height + 1):
            if left >> i << i != left:
                self.pull(left >> i)
            if (right + 1) >> i << i != right + 1:
                self.pull(right >> i)

    def modify(self, rt, val):
        self.tree[rt] = self.apply(val, self.tree[rt])
        if rt < self.size:
            self.lazy[rt] = self.compose(val, self.lazy[rt])

    def query(self, left, right):
        if left > right:
            return self.idval
        left += self.size
        right += self.size

        for i in range(self.height, 0, -1):
            if left >> i << i != left:
                self.push(left >> i)
            if (right + 1) >> i << i != right + 1:
                self.push(right >> i)

        lres, rres = self.idval, self.idval
        while left <= right:
            if left & 1:
                lres = self.Op(lres, self.tree[left])
                left += 1
            if not right & 1:
                rres = self.Op(self.tree[right], rres)
                right -= 1
            left >>= 1
            right >>= 1

        return self.Op(lres, rres)


def Op(ln, rn):
    pass


def apply(val, nd):
    pass


def compose(lz1, lz2):
    return lz1


# seg = LazySegTree(list, info, tag, Op, apply, compose)
