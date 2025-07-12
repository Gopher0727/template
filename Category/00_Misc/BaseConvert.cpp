map<char, int> ch2i;
map<int, char> i2ch;
auto init = []() {
    for (char i = '0'; i <= '9'; ++i) {
        ch2i[i] = i - '0', i2ch[i - '0'] = i;
    }
    for (char i = 'A'; i <= 'F'; ++i) {
        ch2i[i] = i - 'A' + 10, i2ch[i - 'A' + 10] = i;
    }
    return 0;
}();


auto base_repr(i64 v, int base) {
    assert(v >= 0 && base >= 2 && base <= 36);
    string s;
    while (v) {
        int d = v % base;
        s += (d < 10 ? '0' + d : 'A' + d - 10);
        v /= base;
    }
    reverse(s.begin(), s.end());
    return s;
}
