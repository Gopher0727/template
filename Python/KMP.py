def calcPi(s: str) -> list[int]:
    n = len(s)
    pi = [0] * n
    mat = 0
    for i in range(1, n):
        while mat > 0 and s[mat] != s[i]:
            mat = pi[mat - 1]
        if s[mat] == s[i]:
            mat += 1
        pi[i] = mat
    return pi


def nxtArray(s: str) -> list[int]:
    n = len(s)
    nxt = [-1] * (n + 1)
    j = 0
    for i in range(2, n + 1):
        while j != -1 and s[i - 1] != s[j]:
            j = nxt[j]
        j += 1
        nxt[i] = j
    return nxt


def KMP(s: str, p: str) -> int:
    nxt = nxtArray(p)
    n, m = len(s), len(p)
    x, y = 0, 0
    while x < n and y < m:
        if s[x] == p[y]:
            x += 1
            y += 1
        elif y == 0:
            x += 1
        else:
            y = nxt[y]
    return x - y if y == m else -1


def KMPSearch(s: str, p: str) -> list[int]:
    pi = calcPi(p)
    n, m = len(s), len(p)
    pos = []
    mat = 0
    for i in range(n):
        while mat > 0 and p[mat] != s[i]:
            mat = pi[mat - 1]
        if p[mat] == s[i]:
            mat += 1
        if mat == m:
            pos.append(i - m + 1)
            mat = pi[mat - 1]
    return pos
