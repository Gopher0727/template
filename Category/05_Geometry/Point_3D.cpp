template <typename T>
struct Point {
    T x = 0, y = 0, z = 0;

    Point(const T& x_ = 0, const T& y_ = 0, const T& z_ = 0) : x(x_), y(y_), z(z_) {}

    template <typename U>
    operator Point<U>() const {
        return Point<U>(U(x), U(y), U(z));
    }

    Point& operator+=(const Point& p) { return x += p.x, y += p.y, z += p.z, *this; }
    Point& operator-=(const Point& p) { return x -= p.x, y -= p.y, z -= p.z, *this; }
    Point& operator*=(const T& v) { return x *= v, y *= v, z *= v, *this; }
    Point& operator/=(const T& v) { return x /= v, y /= v, z /= v, *this; }
    Point operator-() const { return Point(-x, -y, -z); }
    T operator^(const Point& a) const { return a.y * z - a.z * y, a.z * x - a.x * z, a.x * y - a.y * x; }

    friend Point operator+(Point a, const Point& b) { return a += b; }
    friend Point operator-(Point a, const Point& b) { return a -= b; }
    friend Point operator*(Point a, const T& b) { return a *= b; }
    friend Point operator/(Point a, const T& b) { return a /= b; }
    friend Point operator*(const T& a, Point b) { return b *= a; }
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
    friend istream& operator>>(istream& is, Point& p) { return is >> p.x >> p.y >> p.z; }
    friend ostream& operator<<(ostream& os, const Point& p) { return os << "(" << p.x << ", " << p.y << ", " << p.z << ")"; }

    T length() { return hypot(x, y, z); }
    T Angle() { return acos((x * x + y * y - z * z) / 2 / x / y); }
    Point norm() {
        T l = length();
        return {x / l, y / l, z / l};
    }
    friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
};
Point cross(const Point &a, const Point &b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
using Real = long double;
using P = Point<Real>;
