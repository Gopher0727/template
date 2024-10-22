template <typename T>
T exgcd(T& a, T& b, T& x, T& y) {
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
