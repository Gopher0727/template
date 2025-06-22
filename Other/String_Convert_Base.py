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
