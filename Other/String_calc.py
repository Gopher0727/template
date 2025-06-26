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


def convert_base(num_str: str, from_base: int, to_base: int) -> str:
    """
    将数值字符串从进制 from_base 转换为进制 to_base 的数值字符串。
    支持的进制范围为 2 ~ 16。

    参数:
      num_str: 输入的数字字符串，可以包含前导空格和 + 或 - 符号。
      from_base: 输入的进制（必须在 2 到 16 范围内）。
      to_base: 输出的进制（必须在 2 到 16 范围内）。

    返回:
      转换后的数字字符串。

    示例:
      convert_base("1010", 2, 10)  # 返回 "10"
      convert_base("FF", 16, 10)   # 返回 "255"
      convert_base("255", 10, 16)  # 返回 "FF"
    """

    if not (2 <= from_base <= 16 and 2 <= to_base <= 16):
        raise ValueError("进制必须在 2 到 16 之间。")

    num_str = num_str.strip()
    if not num_str:
        raise ValueError("输入为空字符串。")

    neg = False
    if num_str[0] in "+-":
        if num_str[0] == "-":
            neg = True
        num_str = num_str[1:]

    value = 0
    for ch in num_str:
        if ch.isdigit():
            digit = int(ch)
        else:
            # 处理大写或小写字母（A~F 表示 10~15）
            digit = ord(ch.upper()) - ord("A") + 10
        if digit >= from_base:
            raise ValueError(f"无效的字符 {ch}，不属于进制 {from_base}。")
        value = value * from_base + digit
    if value == 0:
        return "0"

    digits = "0123456789ABCDEF"
    result = []
    while value:
        result.append(digits[value % to_base])
        value //= to_base
    if neg:
        result.append("-")
    return "".join(result[::-1])
