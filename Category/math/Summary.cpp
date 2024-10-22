//* Gosper's Hack
// 查找下一个置位数与 num 相等的数
int Gosper(int num) {
    int lb = num & -num;
    int x = num + lb;
    int y = (num ^ x) / lb >> 2;
    return x | y;
}


// __builtin_popcount()  求 x 置位数
int popcount(int x) {
    int cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1; // x -= x & -x
    }
    return cnt;
}


// 进制转换
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


// __lg(x)  不超过 x 的最大的 2 的幂次
int power = 0;
while((1 << power) <= (x >> 1)) {
    power++;
}


// gcd 与 lcm
int gcd_(int a, int b) {
    if (b > a) {
        std::swap(a, b);
    }
    return b == 0 ? a : gcd_(b, a % b);
}
int gcd_(int a, int b) {
    while (a != 0) {
        int tmp = a;
        a = b % a;
        b = tmp;
    }
    return b;
}
int lcm_(int a, int b) {
    return a / gcd_(a, b) * b;
}
