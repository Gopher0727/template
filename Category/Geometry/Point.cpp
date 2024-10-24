template <typename T>
struct Point {
    T x, y;

    Point() = default;
    Point(const T& x_, const T& y_) : x(x_), y(y_) {}

    template <typename U = ll>
    operator Point<U>() const {
        return Point<U>(U(x), U(y));
    }

    Point operator-() const { return Point(-x, -y); }
    T operator^(const Point& a) const { return x * a.y - y * a.x; }

    Point& operator+=(const Point& p) { return x += p.x, y += p.y, *this; }
    Point& operator-=(const Point& p) { return x -= p.x, y -= p.y, *this; }
    Point& operator*=(const T& v) { return x *= v, y *= v, *this; }
    Point& operator/=(const T& v) { return x /= v, y /= v, *this; }

    friend Point operator+(Point a, const Point& b) { return a += b; }
    friend Point operator-(Point a, const Point& b) { return a -= b; }
    friend Point operator*(Point a, const T& b) { return a *= b; }
    friend Point operator*(const T& a, Point b) { return b *= a; }
    friend Point operator/(Point a, const T& b) { return a /= b; }
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }

    friend std::istream& operator>>(std::istream& is, Point& p) { return is >> p.x >> p.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) { return os << "(" << p.x << ", " << p.y << ")"; }
};
using point = Point<ll>;

const double PI = numbers::pi; // acos(-1);
double toArc(double x) { // 角度转弧度
    return PI / 180 * x;
}
