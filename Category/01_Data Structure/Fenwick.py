class FenwickTree:
    def __init__(self, n):
        self.n = n
        self.bit = [0] * n
        length = (self.n + 1).bit_length() - 1
        self.powers = [1 << i for i in range(length, -1, -1)]
        self.tot = 0

    def sum(self, r):
        res = 0
        while r >= 0:
            res += self.bit[r]
            r = (r & (r + 1)) - 1
        return res

    def rsum(self, l, r):
        return self.sum(r) - self.sum(l - 1)

    def add(self, idx, delta):
        while idx < self.n:
            self.bit[idx] += delta
            idx = idx | (idx + 1)
        self.tot += delta

    def bisect_min_larger(self, num):
        if num <= 0:
            return -1
        note = -1
        tmp = 0
        for power in self.powers:
            if note + power >= self.n or tmp + self.bit[note + power] >= num:
                continue
            note += power
            tmp += self.bit[note]
        return note + 1

    def bisect_max_smaller(self, num):
        if num > self.tot:
            return self.n
        note = -1
        tmp = 0
        for power in self.powers:
            if note + power >= self.n or tmp + self.bit[note + power] >= num:
                continue
            note += power
            tmp += self.bit[note]
        return note
