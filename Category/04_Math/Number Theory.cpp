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

    for (int i = 2; i <= n; i++) {
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


// 求解形如 ax+by=c 的不定方程的任意一组解
template <typename T>
auto exgcd(T a, T b, T& x, T& y) {
    if (b == 0) {
        return x = 1, y = 0, a;
    }
    T g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}
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
auto sol(T a, T b, T m) {
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
    return pair {x, m / g};
}


// 求逆元
template <typename T>
array<T, 3> exgcd(T a, T b) {
    if (b == 0) {
        return {a, 1, 0};
    }
    auto [g, x, y] = exgcd(b, a % b);
    return {g, y, x - a / b * y};
}
template <typename T>
auto getInv(T a, T m) {
    auto [g, x, y] = exgcd(a, m);
    return g == 1 ? (x % m + m) % m : -1;
}


// 求逆元
template <typename T>
auto getInv(T a, T m) {
    T m0 = m, t, q;
    T x0 = 0, x1 = 1;
    if (m == 1) {
        return 0;
    }
    while (a > 1) {
        q = a / m;
        t = m;

        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) {
        x1 += m0;
    }
    return x1;
}


// CRT（中国剩余定理）
// ! 要求模数互质
template <typename T = i64, typename U = i64>
auto findMinX(vector<U>& num, vector<U>& rem) {
    T prod = 1; // 需要注意模数的乘积大小
    for (int i = 0; i < num.size(); i++) {
        prod *= num[i];
    }

    T result = 0;
    for (int i = 0; i < num.size(); i++) {
        T pp = prod / num[i];
        result += rem[i] * getInv(pp, num[i]) % prod * pp % prod;
        result %= prod;
    }
    return result % prod;
}


// ExCRT 扩展中国剩余定理
//
template <typename T>
auto exgcd(T a, T b, T& x, T& y) {
    if (b == 0) {
        return x = 1, y = 0, a;
    }
    T g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}
template <typename T>
bool mergeCongruences(T n1, T a1, T n2, T a2, T& n3, T& a3) {
    T x, y;
    T g = exgcd<T>(n1, n2, x, y);
    if ((a2 - a1) % g != 0) { // No solution exists
        return false;
    }
    T lcm = n1 * n2 / g;
    x *= (a2 - a1) / g;
    x = (x % (n2 / g) + (n2 / g)) % (n2 / g); // Ensure x is positive
    a3 = (a1 + x * n1) % lcm;
    n3 = lcm;
    return true;
}
template <typename T = i64, typename U = i64>
T excrt(vector<U>& num, vector<U>& rem) {
    T n1 = num[0], a1 = rem[0];
    for (int i = 1; i < rem.size(); i++) {
        T n2 = num[i], a2 = rem[i];
        T n3, a3;
        if (!mergeCongruences(n1, a1, n2, a2, n3, a3)) { // No solution
            return -1;
        }
        n1 = n3;
        a1 = a3;
    }
    return a1;
}


// 离散对数（BSGS，BabyStepGiantStep）
//
// 以 O(sqrt(p)) 的复杂度求解 a^x = b (mod p)。
// 其中标准 BSGS 算法不能计算 a 与 p 互质的情况，而 exbsgs 则可以。
//
namespace BSGS {
    i64 a, b, p;
    map<i64, i64> f;

    i64 gcd(i64 a, i64 b) {
        return b > 0 ? gcd(b, a % b) : a;
    }

    i64 ps(i64 n, i64 k, int p) {
        i64 r = 1;
        for (; k; k >>= 1) {
            if (k & 1) r = r * n % p;
            n = n * n % p;
        }
        return r;
    }

    void exgcd(i64 a, i64 b, i64& x, i64& y) {
        if (!b) {
            x = 1, y = 0;
        } else {
            exgcd(b, a % b, x, y);
            i64 t = x;
            x = y;
            y = t - a / b * y;
        }
    }

    i64 inv(i64 a, i64 b) {
        i64 x, y;
        exgcd(a, b, x, y);
        return (x % b + b) % b;
    }

    i64 bsgs(i64 a, i64 b, i64 p) {
        f.clear();
        int m = ceil(sqrt(p));
        b %= p;
        for (int i = 1; i <= m; i++) {
            b = b * a % p;
            f[b] = i;
        }
        i64 tmp = ps(a, m, p);
        b = 1;
        for (int i = 1; i <= m; i++) {
            b = b * tmp % p;
            if (f.count(b) > 0) {
                return (i * m - f[b] + p) % p;
            }
        }
        return -1;
    }

    i64 exbsgs(i64 a, i64 b, i64 p) {
        a %= p, b %= p;
        if (b == 1 || p == 1) {
            return 0;
        }
        i64 g = gcd(a, p), k = 0, na = 1;
        while (g > 1) {
            if (b % g != 0) {
                return -1;
            }
            k++;
            b /= g;
            p /= g;
            na = na * (a / g) % p;
            if (na == b) {
                return k;
            }
            g = gcd(a, p);
        }
        i64 f = bsgs(a, b * inv(na, p) % p, p);
        if (f == -1) {
            return -1;
        }
        return f + k;
    }
} // namespace BSGS
using namespace BSGS;
