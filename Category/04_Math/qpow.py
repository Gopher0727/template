"""
def qpow(a: int, b: int, p: int):
    ans = 1
    while b:
        if b & 1:
            ans = ans * a % p
        a = a * a % p
        b >>= 1
    return ans


def qpow(a: int, b: int):
    ans = 1
    while b:
        if b & 1:
            ans *= a
        a *= a
        b >>= 1
    return ans
"""

def multiply(a: List[List[int]], b: List[List[int]]) -> List[List[int]]:
    return [[sum(x * y for x, y in zip(row, col)) % MOD for col in zip(*b)] for row in a]


# a^n @ f，其中 @ 是矩阵乘法，f 为列向量
def qpow_mul(a: List[List[int]], b: int, f: List[List[int]] = -1) -> List[List[int]]:
    n = len(a)
    if f == -1:
        f = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while b:
        if b & 1:
            f = multiply(a, f)
        a = multiply(a, a)
        b >>= 1
    return f
