class Trie:
    __slots__ = "son", "isEnd", "cnt"

    def __init__(self):
        self.son = {}
        self.isEnd = False
        self.cnt = 0

    def insert(self, s: str) -> None:
        cur = self
        for ch in s:
            if ch not in cur.son:
                cur.son[ch] = Trie()
            cur = cur.son[ch]
        cur.isEnd = True

    def search(self, s: str) -> bool:
        cur = self
        for ch in s:
            if ch not in cur.son:
                return False
            cur = cur.son[ch]
        return cur.isEnd

    def startsWith(self, s: str) -> bool:
        cur = self
        for ch in s:
            if ch not in cur.son:
                return False
            cur = cur.son[ch]
        return True
