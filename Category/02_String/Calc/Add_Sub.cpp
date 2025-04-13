/**
 * 对两个数字字符串进行加法，返回它们的和（字符串形式）。
 */
string add_strings(const string& num1, const string& num2) {
    string s;
    for (int i = num1.size() - 1, j = num2.size() - 1, carry = 0; i >= 0 || j >= 0 || carry; i--, j--) {
        int d1 = (i >= 0) ? (num1[i] - '0') : 0;
        int d2 = (j >= 0) ? (num2[j] - '0') : 0;
        int total = d1 + d2 + carry;
        s.push_back('0' + (total % 10));
        carry = total / 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

/**
 * 比较两个表示正整数的字符串
 * 返回 1 表示 num1 > num2，返回 -1 表示 num1 < num2，返回 0 表示两者相等。
 */
int compare(const string& num1, const string& num2) {
    int n = num1.length(), m = num2.length();
    if (n != m) {
        return (n > m) ? 1 : -1;
    }
    for (int i = 0; i < n; i++) {
        if (num1[i] != num2[i]) {
            return (num1[i] > num2[i]) ? 1 : -1;
        }
    }
    return 0;
}

/**
 * 对两个数字字符串进行减法，返回它们的差（字符串形式）。
 * 注意：
 *  - 默认输入为非负整数字符串。
 */
string subtract_strings(const string& num1, const string& num2) {
    int cmp = compare(num1, num2);
    if (cmp == 0) {
        return "0";
    }

    bool neg = false;
    string a = num1, b = num2;
    if (cmp < 0) {
        swap(a, b);
        neg = true;
    }

    string s;
    for (int i = a.size() - 1, j = b.size() - 1, borrow = 0; i >= 0; i--, j--) {
        int d1 = a[i] - '0';
        int d2 = (j >= 0) ? (b[j] - '0') : 0;
        int diff = d1 - d2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        s.push_back('0' + diff);
    }
    while (s.size() > 1 && s.back() == '0') {
        s.pop_back();
    }
    if (neg) {
        s.push_back('-');
    }
    reverse(s.begin(), s.end());
    return s;
}
