template <class T>
struct Frac {
    T x, y;

public:
    Frac() : Frac(0, 1) {}
    Frac(T x_) : Frac(x_, 1) {}
    Frac(T x_, T y_) : x(x_), y(y_) {
        if (y < 0) {
            y = -y;
            x = -x;
        }
    }

public:
    constexpr double val() const { return 1. * x / y; }
    constexpr Frac norm() const { // 调整符号、转化为最简形式
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

    friend constexpr Frac operator+(const Frac i, const Frac j) { return i += j; }
    friend constexpr Frac operator-(const Frac i, const Frac j) { return i -= j; }
    friend constexpr Frac operator-(const Frac i) { return Frac(-i.x, i.y); }
    friend constexpr Frac operator*(const Frac i, const Frac j) { return i *= j; }
    friend constexpr Frac operator/(const Frac i, const Frac j) { return i /= j; }

    friend constexpr bool operator<(const Frac i, const Frac j) { return i.x * j.y < i.y * j.x; }
    friend constexpr bool operator>(const Frac i, const Frac j) { return i.x * j.y > i.y * j.x; }
    friend constexpr bool operator==(const Frac i, const Frac j) { return i.x * j.y == i.y * j.x; }
    friend constexpr bool operator!=(const Frac i, const Frac j) { return i.x * j.y != i.y * j.x; }

    friend constexpr auto& operator<<(ostream& o, const Frac& j) {
        T p = gcd(j.x, j.y);
        if (j.y == p) {
            return o << j.x / p;
        } else {
            return o << j.x / p << "/" << j.y / p;
        }
    }
};
