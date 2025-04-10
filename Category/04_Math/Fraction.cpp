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
    Frac(T x_ = 0, T y_ = 1) : x(x_), y(y_) {
        if (y < 0) {
            y = -y;
            x = -x;
        }
    }

public:
    constexpr double val() const { return 1. * x / y; }
    constexpr Frac norm() const { // 约分
        T p = gcd(x, y);
        return {x / p, y / p};
    }

public:
    constexpr Frac& operator+=(const Frac& i) const { return x = x * i.y + y * i.x, y *= i.y, *this; }
    constexpr Frac& operator-=(const Frac& i) const { return x = x * i.y - y * i.x, y *= i.y, *this; }
    constexpr Frac& operator*=(const Frac& i) const { return x *= i.x, y *= i.y, *this; }
    constexpr Frac& operator/=(const Frac& i) const {
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

    friend constexpr strong_ordering operator<=>(const Frac& i, const Frac& j) { return i.x * j.y <=> i.y * j.x; }
    friend constexpr bool operator==(const Frac& i, const Frac& j) { return i.x * j.y == i.y * j.x; }

    friend constexpr auto& operator<<(ostream& os, const Frac& j) {
        T p = gcd(j.x, j.y);
        if (j.y == p) {
            return os << j.x / p;
        } else {
            return os << j.x / p << "/" << j.y / p;
        }
    }
};
