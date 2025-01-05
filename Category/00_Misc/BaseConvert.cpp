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
