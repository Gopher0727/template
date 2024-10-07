const double PI = numbers::pi; // acos(-1);

template <typename T = ll>
struct Point {
    T x, y;
    Point(const T& x_ = 0, const T& y_ = 0) : x(x_), y(y_) {}

    template <typename U = ll>
    operator Point<U>() {
        return Point<U>(U(x), U(y));
    }

    Point operator-() const { return Point(-x, -y); }
    int operator^(const Point& a) { return x * a.y - y * a.x; }

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

double toArc(double x) { // 角度转弧度
    return PI / 180 * x;
}

// hypot() 求直角三角形斜边长度
