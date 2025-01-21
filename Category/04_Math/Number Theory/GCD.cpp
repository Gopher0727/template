// Stein 法 (Binary GCD)：不断去除因子 2 以达到降低常数的目的。
//
int SteinGCD(int a, int b) {
    a = abs(a), b = abs(b);
    int az = __builtin_ctz(a), bz = __builtin_ctz(b);
    int z = min(az, bz);
    b >>= bz;
    while (a) {
        a >>= az;
        int diff = b - a;
        az = __builtin_ctz(diff);
        if (a < b) {
            b = a;
        }
        a = abs(diff);
    }
    return b << z;
}


// 基于值域预处理的快速 GCD
// Link: https://www.luogu.com.cn/article/6jezrwtl
//
// 1.记录 n 的最小素因子 lpf(n) = p1 和下一个数 pa[n] = n / p1^b1，类似于链表一样存储所有数的素因子分解
// 2.并入共有的素因子： 从小到大枚举 a，b 的素因子，如果存在共有的素因子，则将对应素因子的幂次的最小值并入
vector<int> primes, vis, lpf, pa;
void sieve(int n) {
    vis.resize(n + 1);
    lpf.resize(n + 1);
    pa.resize(n + 1);

    for (int i = 2; i <= n; ++i) {
        if (!vis[i]) {
            primes.emplace_back(i);
            lpf[i] = i;
            pa[i] = 1;
        }
        for (int p : primes) {
            if (i * p > n) {
                break;
            }
            vis[i * p] = 1;
            if (i % p == 0) {
                lpf[i * p] = lpf[i];
                pa[i * p] = pa[i];
                break;
            } else {
                lpf[i * p] = p;
                pa[i * p] = i;
            }
        }
    }
}
auto init = []() {
    sieve(1e6 + 10);
    return 0;
}();
int gcd(int a, int b) {
    a = abs(a), b = abs(b);
    if (a == 0 || b == 0) {
        return a + b;
    }
    int res = 1;
    while (a != 1 && b != 1) {
        if (lpf[a] == lpf[b]) {
            res *= min(a / pa[a], b / pa[b]);
            a = pa[a];
            b = pa[b];
        } else if (lpf[a] < lpf[b]) {
            a = pa[a];
        } else {
            b = pa[b];
        }
    }
    return res;
}


// 欧几里得算法：求两个数的最大公约数（辗转相除法）
// gcd(a, b) = gcd(b, a % b)


// 裴蜀定理（贝祖定理）
// 设 x,y 是不全为零的整数，对任意整数 a,b，满足 gcd(x,y) | ax+by，且存在整数 a,b, 使得 ax+by=gcd(x,y)，
// 同时 ax+by=c 有解，意味着 c 一定是 gcd(a,b) 的整数倍。


// 扩展欧几里得算法：
// · 判断方程 ax+by=c 是否有解
// · 求 ax+by=c 的任意一组解、通解、最小整数解
// · 求逆元


// b * x1 + (a % b) * y1 = gcd(a, b)
// 即 b * x1 + (a - a / b * b) * y1 = gcd(a, b)
// 即 a * y1 + b * (x1 - a / b * y1) = gcd(a, b)
// 因而有，x = y1, y = x1 - a / b * y1

template <typename T>
T exgcd(T a, T b, T& x, T& y) {
    if (b == 0) {
        return x = 1, y = 0, a;
    }
    T g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}
// 求解形如 ax+by=c 的不定方程的任意一组解
auto calc = [&](int a, int b, int c) {
    int u = 1, v = 1;
    if (a < 0) { // 负数特判（不一定有用）
        a = -a;
        u = -1;
    }
    if (b < 0) {
        b = -b;
        v = -1;
    }
    int x, y, ans;
    int d = exgcd(a, b, x, y);
    if (c % d != 0) { // 无整数解
        cout << -1 << "\n";
        return;
    }
    a /= d, b /= d, c /= d;
    x *= c, y *= c; // 得到可行解
    ans = (x % b + b - 1) % b + 1;
    auto [A, B] = pair {u * ans, v * (c - ans * a) / b}; // x 为最小正整数的特解
    ans = (y % a + a - 1) % a + 1;
    auto [C, D] = pair {u * (c - ans * b) / a, v * ans}; // y 为最小正整数的特解

    int num = (C - A) / b + 1; // x、y 均为正整数的解的组数
};


// ax + b == 0 (mod m)
template <typename T>
pair<T, T> sol(T a, T b, T m) {
    assert(m > 0);
    b *= -1;
    T x, y;
    T g = exgcd(a, m, x, y);
    if (g < 0) {
        g *= -1, x *= -1, y *= -1;
    }
    if (b % g) {
        return {-1, -1};
    }
    x = x * (b / g) % (m / g);
    if (x < 0) {
        x += m / g;
    }
    return {x, m / g};
}


template <typename T>
array<T, 3> exgcd(T a, T b) {
    if (b == 0) {
        return {a, 1, 0};
    }
    auto [g, x, y] = exgcd(b, a % b);
    return {g, y, x - a / b * y};
}
// 求逆元
template <typename T>
T getInv(T a, T m) {
    auto [g, x, y] = exgcd(a, m);
    return g == 1 ? (x % m + m) % m : -1;
}
