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


# a @ b，其中 @ 是矩阵乘法
def multiply(a: List[List[int]], b: List[List[int]]) -> List[List[int]]:
    return [
        [sum(x * y for x, y in zip(row, col)) % MOD for col in zip(*b)] for row in a
    ]


# a^n
def qpow(a: List[List[int]], b: int) -> List[List[int]]:
    n = len(a)
    res = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while b:
        if b & 1:
            res = multiply(a, res)
        a = multiply(a, a)
        b >>= 1
    return res


# a^n @ f0
def qpow_mul(a: List[List[int]], b: int, f: List[List[int]]) -> List[List[int]]:
    res = f
    while b:
        if b & 1:
            res = multiply(a, res)
        a = multiply(a, a)
        b >>= 1
    return res
