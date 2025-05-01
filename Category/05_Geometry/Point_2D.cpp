template <typename T>
struct Point {
    T x = 0, y = 0;

    Point(const T& x_ = 0, const T& y_ = 0) : x(x_), y(y_) {}

    template <typename U>
    operator Point<U>() const {
        return Point<U>(U(x), U(y));
    }

    Point& operator+=(const Point& p) { return x += p.x, y += p.y, *this; }
    Point& operator-=(const Point& p) { return x -= p.x, y -= p.y, *this; }
    Point& operator*=(const T& v) { return x *= v, y *= v, *this; }
    Point& operator/=(const T& v) { return x /= v, y /= v, *this; }
    Point operator-() const { return Point(-x, -y); }
    T operator^(const Point& a) const { return x * a.y - y * a.x; }

    friend Point operator+(Point a, const Point& b) { return a += b; }
    friend Point operator-(Point a, const Point& b) { return a -= b; }
    friend Point operator*(Point a, const T& b) { return a *= b; }
    friend Point operator/(Point a, const T& b) { return a /= b; }
    friend Point operator*(const T& a, Point b) { return b *= a; }
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
    friend istream& operator>>(istream& is, Point& p) { return is >> p.x >> p.y; }
    friend ostream& operator<<(ostream& os, const Point& p) { return os << "(" << p.x << ", " << p.y << ")"; }

    T length() const { return hypot(x, y); }

    friend T dot(const Point& a, const Point& b) { // 点积（内积）
        return a.x * b.x + a.y * b.y;
    }
    friend T square(const Point& p) { return dot(p, p); }
    friend T distance(const Point& a, const Point& b) { return hypot(a.x - b.x, a.y - b.y); }
    friend Point norm(const Point& p) {
        T len = hypot(p.x, p.y);
        if (len == 0) {
            return Point(0, 0);
        }
        return p / len;
    }
    friend Point rotate(const Point& a) { return Point(-a.y, a.x); }
    friend int sgn(const Point& a) {
        if (a.y > 0 || (a.y == 0 && a.x > 0)) {
            return 1;
        }
        if (a.y == 0 && a.x == 0) { // 原点
            return 0;
        }
        return -1;
    }
};
using Real = long double;
using P = Point<Real>;

const Real PI = numbers::pi; // acos(-1);
Real toArc(Real x) { // 角度转弧度
    return PI / 180 * x;
}
