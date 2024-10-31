namespace Date {
    const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool isLeap(int y) {
        return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
    }
    int daysInMonth(int y, int m) {
        return d[m - 1] + (isLeap(y) && m == 2);
    }
    int getDay(int y, int m, int d) {
        int ans = 0;
        for (int i = 1970; i < y; i++) {
            ans += 365 + isLeap(i);
        }
        for (int i = 1; i < m; i++) {
            ans += daysInMonth(y, i);
        }
        ans += d;
        return (ans + 2) % 7 + 1;
    }
} // namespace Date
using namespace Date;


namespace Sqrt {
    // x * x <= n
    ll mSqrt(ll n) {
        ll x = sqrt(n);
        while (x * x > n) {
            x--;
        }
        while ((x + 1) * (x + 1) <= n) {
            x++;
        }
        return x;
    }
    // x * (x+1) / 2 <= x
    ll mGet(ll n) {
        ll x = sqrt(2.0L * n);
        while (x * (x + 1) / 2 < n) {
            x++;
        }
        while (x * (x - 1) / 2 + 1 > n) {
            x--;
        }
        return x;
    }
} // namespace Sqrt
using namespace Sqrt;


namespace Log {
    // a ^ t >= b
    int log_a(int a, int b) {
        int t = 0;
        ll v = 1;
        while (v < b) {
            v *= a;
            t++;
        }
        return t;
    }
    // >= __lg(t)
    int llog(int a, int b) {
        if (a <= b) {
            int l = log_a(a, b);
            return (l == 0 ? 0 : __lg(2 * l - 1));
        }
        int l = log_a(b, a + 1) - 1;
        assert(l > 0);
        return -__lg(l);
    }
} // namespace Log
using namespace Log;


namespace setBit {
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
}


namespace baseConvert {
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
}


namespace power {
    // __lg(x)  不超过 x 的最大的 2 的幂次
    int power = 0;
    while((1 << power) <= (x >> 1)) {
        power++;
    }
}


namespace gcd_lcm {
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
}
