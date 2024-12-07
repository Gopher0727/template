// 蛇形路径
//
vector<string> snakeWay(const vector<string>& g) {
    int n = g.size(), m = g[0].size();
    for (int r = 0; r * 2 <= min(n, m) - 1; ++r) {
        string t;
        for (int i = r + 1; i < m - r; ++i) {
            t.push_back(g[r][i]);
        }
        for (int i = r + 1; i < n - r; ++i) {
            t.push_back(g[i][m - 1 - r]);
        }
        for (int i = m - 2 - r; i >= r; --i) {
            t.push_back(g[n - 1 - r][i]);
        }
        for (int i = n - 2 - r; i >= r; --i) {
            t.push_back(g[i][r]);
        }
        cout << t << "\n";
    }
}

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
