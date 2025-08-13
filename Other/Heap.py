# 对顶堆查询中位数
class DualHeap:
    def __init__(self):
        # q: 大根堆（存负数）
        self.q = []
        # p: 小根堆
        self.p = []

    def adjust(self):
        # 保证 p 的元素个数 >= q 的元素个数 - 1
        while len(self.p) + 1 < len(self.q):
            heapq.heappush(self.p, -heapq.heappop(self.q))
        while len(self.p) > len(self.q):
            heapq.heappush(self.q, -heapq.heappop(self.p))

    def add(self, num):
        if not self.q or num <= -self.q[0]:
            heapq.heappush(self.q, -num)  # 大根堆用负数存
        else:
            heapq.heappush(self.p, num)
        self.adjust()

    def remove(self, num):
        tmp = []
        if self.q and num <= -self.q[0]:
            # 从大根堆删除
            while self.q:
                if -self.q[0] == num:
                    heapq.heappop(self.q)
                    break
                tmp.append(-heapq.heappop(self.q))
            for x in tmp:
                heapq.heappush(self.q, -x)
        else:
            # 从小根堆删除
            while self.p:
                if self.p[0] == num:
                    heapq.heappop(self.p)
                    break
                tmp.append(heapq.heappop(self.p))
            for x in tmp:
                heapq.heappush(self.p, x)
        self.adjust()

    def queryMedian(self):
        if len(self.p) < len(self.q):
            return -self.q[0]
        else:
            return (-self.q[0] + self.p[0]) / 2.0
