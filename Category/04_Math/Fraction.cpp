// 将连分数转化为最简分数
//
// 如 [3, 2, 0, 2]，表示 3 + (1 / (2 + (1 / (0 + 1 / 2))))
//
auto Change(const vector<int>& a) {
    int n = a.size();
    vector<int> h(n + 2), k(n + 2);
    h[0] = 0, h[1] = 1;
    k[0] = 1, k[1] = 0;
    for (int i = 0; i < n; i++) {
        h[i + 2] = a[i] * h[i + 1] + h[i];
        k[i + 2] = a[i] * k[i + 1] + k[i];
    }
    return array {h[n + 1], k[n + 1]};
}


// A/B < P/Q < C/D
// return {P, Q} -- 首先Q最小，其次P最小
auto dfs = [&](auto&& dfs, i128 A, i128 B, i128 C, i128 D) -> pair<i128, i128> {
    auto k = A / B;
    if (k + 1 < (C + D - 1) / D) {
        return {k + 1, 1ULL};
    }

    A -= k * B;
    C -= k * D;

    if (A == 0) {
        auto p2 = D / C + 1;
        return {k * p2 + 1, p2};
    }

    auto [p2, q2] = dfs(dfs, D, C, B, A);
    return {k * p2 + q2, p2};
};


// 运算类
//
struct Frac {
    i64 x, y;

public:
    constexpr Frac(i64 x_ = 0, i64 y_ = 1) : x(x_), y(y_) {
        if (y_ == 0) {
            throw "Denominator cannot be zero";
        }
        if (y < 0) {
            y = -y;
            x = -x;
        }
    }

public:
    // consteval
    constexpr long double val() const { return 1.l * x / y; }

    constexpr Frac norm() const {
        i64 p = gcd(x, y);
        return {x / p, y / p};
    }

public:
    constexpr Frac& operator+=(const Frac& i) { return x = x * i.y + y * i.x, y *= i.y, *this; }
    constexpr Frac& operator-=(const Frac& i) { return x = x * i.y - y * i.x, y *= i.y, *this; }
    constexpr Frac& operator*=(const Frac& i) { return x *= i.x, y *= i.y, *this; }
    constexpr Frac& operator/=(const Frac& i) {
        x *= i.y;
        y *= i.x;
        if (y < 0) {
            x = -x;
            y = -y;
        }
        return *this;
    }

    friend constexpr Frac operator+(Frac i, const Frac& j) { return i += j; }
    friend constexpr Frac operator-(Frac i, const Frac& j) { return i -= j; }
    friend constexpr Frac operator-(Frac i) { return Frac(-i.x, i.y); }
    friend constexpr Frac operator*(Frac i, const Frac& j) { return i *= j; }
    friend constexpr Frac operator/(Frac i, const Frac& j) { return i /= j; }

    friend constexpr strong_ordering operator<=>(const Frac& i, const Frac& j) { return i.x * j.y <=> i.y * j.x; }
    friend constexpr bool operator==(const Frac& i, const Frac& j) { return i.x * j.y == i.y * j.x; }
};
