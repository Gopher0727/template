# 作者：灵茶山艾府
# 链接：https://leetcode.cn/discuss/post/3581838/fen-xiang-gun-ti-dan-dong-tai-gui-hua-ru-007o/
def DigitalDP(low: int, high: int) -> int:
    high_s = list(map(int, str(high)))
    n = len(high_s)
    low_s = list(map(int, str(low).zfill(n)))

    @cache
    def dfs(i: int, diff: int, limit_low: bool, limit_high: bool) -> int: # todo
        if i == n:
            return 1 if diff == 0 else 0

        lo = low_s[i] if limit_low else 0
        hi = high_s[i] if limit_high else 9

        res = 0
        for d in range(lo, hi + 1):
            res += dfs(
                i + 1,
                diff + (d if i % 2 == 1 else -d), # todo
                limit_low and d == lo,
                limit_high and d == hi,
            )
        return res

    return dfs(0, 0, True, True)
