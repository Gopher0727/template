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


// 运算类
//
template <class T>
struct Frac {
    T x, y;

public:
    constexpr Frac(T x_ = 0, T y_ = 1) : x(x_), y(y_) {
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
    constexpr long double val() const { return static_cast<long double>(x) / y; }

    constexpr Frac norm() const {
        T p = gcd(x, y);
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

    friend constexpr Frac operator+(const Frac& i, const Frac& j) { return i += j; }
    friend constexpr Frac operator-(const Frac& i, const Frac& j) { return i -= j; }
    friend constexpr Frac operator-(const Frac& i) { return Frac(-i.x, i.y); }
    friend constexpr Frac operator*(const Frac& i, const Frac& j) { return i *= j; }
    friend constexpr Frac operator/(const Frac& i, const Frac& j) { return i /= j; }

    friend constexpr strong_ordering operator<=>(const Frac& i, const Frac& j) { return 1ll * i.x * j.y <=> 1ll * i.y * j.x; }
    friend constexpr bool operator==(const Frac& i, const Frac& j) { return 1ll * i.x * j.y == 1ll * i.y * j.x; }

    friend constexpr auto& operator<<(ostream& os, const Frac& i) {
        auto j = i.norm();
        if (j.y == 1) {
            return os << j.x;
        } else {
            return os << j.x << "/" << j.y;
        }
    }
};
