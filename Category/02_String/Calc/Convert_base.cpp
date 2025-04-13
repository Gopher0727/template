// 辅助函数，用于去除字符串前后的空白字符
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == string::npos || end == string::npos) {
        return "";
    }
    return s.substr(start, end - start + 1);
}

/**
 * 将数值字符串从进制 from_base 转换为进制 to_base 的数值字符串。
 * 支持的进制范围为 2 ~ 16。
 *
 * 参数:
 *    num_str: 输入的数字字符串，可以包含前导空白和 + 或 - 符号。
 *    from_base: 输入的进制（必须在 2 到 16 范围内）。
 *    to_base: 输出的进制（必须在 2 到 16 范围内）。
 *
 * 返回:
 *    转换后的数字字符串。
 *
 * 示例:
 *    convert_base("1010", 2, 10)  -> 返回 "10"
 *    convert_base("FF", 16, 10)   -> 返回 "255"
 *    convert_base("255", 10, 16)  -> 返回 "FF"
 */
string convert_base(const string& num_str, int from_base, int to_base) {
    if (!(2 <= from_base && from_base <= 16 && 2 <= to_base && to_base <= 16)) {
        throw invalid_argument("进制必须在 2 到 16 之间。");
    }

    string s = trim(num_str);
    if (s.empty()) {
        throw invalid_argument("输入为空字符串。");
    }

    bool neg = false;
    if (s[0] == '+' || s[0] == '-') {
        if (s[0] == '-') {
            neg = true;
        }
        s = s.substr(1);
    }

    i64 value = 0;
    for (char ch : s) {
        int digit;
        if (isdigit(ch)) {
            digit = ch - '0';
        } else {
            // 处理大写或小写字母（A~F 表示 10~15）
            digit = toupper(ch) - 'A' + 10;
        }
        if (digit >= from_base) {
            throw invalid_argument(string("无效的字符 ") + ch + "，不属于进制 " + to_string(from_base) + "。");
        }
        value = value * from_base + digit;
    }
    if (value == 0) {
        return "0";
    }

    const string digits = "0123456789ABCDEF";
    string result;
    while (value) {
        int rem = value % to_base;
        result.push_back(digits[rem]);
        value /= to_base;
    }
    if (neg) {
        result.push_back('-');
    }
    reverse(result.begin(), result.end());
    return result;
}
