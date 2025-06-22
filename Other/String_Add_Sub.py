def add_strings(num1: str, num2: str) -> str:
    """
    对两个数字字符串进行加法，返回它们的和（字符串形式）。
    """
    i, j = len(num1) - 1, len(num2) - 1
    carry = 0
    ans = []
    while i >= 0 or j >= 0 or carry:
        d1 = int(num1[i]) if i >= 0 else 0
        d2 = int(num2[j]) if j >= 0 else 0
        total = d1 + d2 + carry
        ans.append(str(total % 10))
        carry = total // 10
        i -= 1
        j -= 1
    return "".join(ans[::-1])


def compare(num1: str, num2: str) -> int:
    """
    比较两个表示正整数的字符串
    """
    if len(num1) != len(num2):
        return 1 if len(num1) > len(num2) else -1
    for a, b in zip(num1, num2):
        if a != b:
            return 1 if a > b else -1
    return 0


def subtract_strings(num1: str, num2: str) -> str:
    """
    对两个数字字符串进行减法，返回它们的差（字符串形式）。
    注意：
    - 默认输入为非负整数字符串。
    """
    cmp = compare(num1, num2)
    if cmp == 0:
        return "0"

    neg = False
    if cmp < 0:
        num1, num2 = num2, num1
        neg = True

    i, j = len(num1) - 1, len(num2) - 1
    borrow = 0
    ans = []
    while i >= 0:
        d1 = int(num1[i])
        d2 = int(num2[j]) if j >= 0 else 0
        diff = d1 - d2 - borrow
        if diff < 0:
            diff += 10
            borrow = 1
        else:
            borrow = 0
        ans.append(str(diff))
        i -= 1
        j -= 1

    while len(ans) > 1 and ans[-1] == "0":
        ans.pop()

    s = "".join(ans[::-1])
    if neg:
        s = "-" + s
    return s
