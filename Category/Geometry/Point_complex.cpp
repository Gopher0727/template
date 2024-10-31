using Point = complex<long double>;
long double dot(const Point& a, const Point& b) {
    return (conj(a) * b).real();
}
long double cross(const Point& a, const Point& b) {
    return (conj(a) * b).imag();
}
long double length(const Point& a) {
    return sqrt(dot(a, a));
}
long double dist(const Point& a, const Point& b) {
    return length(a - b);
}
long double get(const Point& a, const Point& b, const Point& c, const Point& d) {
    return dist(d, a + (b - a) * cross(c - a, d - a) / cross(b - a, d - c));
}
