// 进制转换

map<char, int> mp1;
map<int, char> mp2;
auto init = []() {
    for (char i = '0'; i <= '9'; ++i) {
        mp1[i] = i - '0', mp2[i - '0'] = i;
    }
    for (char i = 'A'; i <= 'F'; ++i) {
        mp1[i] = i - 'A' + 10, mp2[i - 'A' + 10] = i;
    }
    return 0;
}();
