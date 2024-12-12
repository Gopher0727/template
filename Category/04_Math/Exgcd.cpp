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
