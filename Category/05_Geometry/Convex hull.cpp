// 凸包 (Convex hull)：
//
// 平面上 N 个点，用一个周长最小且面积最小的凸多边形覆盖这 N 个点（所有点在其上或其内），所求凸多边形就叫做这个点集的凸包。
//
// Graham 扫描法：
// 选出横坐标最小的点（相同时取纵坐标最小的点）作为极点，显然这个点在凸包上；
// 其余点进行极角排序，极角相同时按照`与极点距离由近到远`的顺序排序。
// 用栈存储凸包上的点，先将选出的极点和极角序最小的 2 的点依次入栈，
// 按顺序扫描每个点，检查栈顶的两个点与当前点构成的折线段是否 “拐” 向右侧，是则弹出栈顶，
// 并继续检查，直至不满足条件或者栈中不足两个点。
//
// 两个向量 a, b 满足 a 的极角小于 b 的极角  <=>  b 在 a 的逆时针方向  <=>  a × b > 0
//
bool cmp(int u, int v) {
    int det = (p[u] - p[1]) * (p[v] - p[1]);
    if (det != 0) {
        return det > 0; // 逆时针
    }
    return (p[u] - p[1]).norm() < (p[v] - p[1]).norm(); // 比较与极点的距离
}


// 以下为 静态凸包

// Point with int64
struct Point {
    i64 x;
    i64 y;
    Point() : x {0}, y {0} {}
    Point(i64 x_, i64 y_) : x {x_}, y {y_} {}
};
Point operator+(Point a, Point b) {
    return Point(a.x + b.x, a.y + b.y);
}
Point operator-(Point a, Point b) {
    return Point(a.x - b.x, a.y - b.y);
}
auto dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
auto cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
auto getHull(vector<Point> p) {
    ranges::sort(p, [&](auto a, auto b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });

    vector<Point> hi, lo;
    for (auto p : p) {
        while (hi.size() > 1 && cross(hi.back() - hi[hi.size() - 2], p - hi.back()) >= 0) {
            hi.pop_back();
        }
        while (!hi.empty() && hi.back().x == p.x) {
            hi.pop_back();
        }
        hi.push_back(p);
        while (lo.size() > 1 && cross(lo.back() - lo[lo.size() - 2], p - lo.back()) <= 0) {
            lo.pop_back();
        }
        if (lo.empty() || lo.back().x < p.x) {
            lo.push_back(p);
        }
    }
    return make_pair(hi, lo);
}

const double inf = INFINITY;


// Point with complex
//
using Point = complex<i64>;
#define x real
#define y imag
auto dot(const Point& a, const Point& b) {
    return (conj(a) * b).x();
}
auto cross(const Point& a, const Point& b) {
    return (conj(a) * b).y();
}
auto rot(const Point& p) {
    return Point(-p.y(), p.x());
}
auto complexHull(vector<Point> a) {
    ranges::sort(a, [&](auto a, auto b) {
        if (a.x() != b.x()) {
            return a.x() < b.x();
        } else {
            return a.y() < b.y();
        }
    });

    vector<Point> l, h;
    for (auto p : a) {
        while (l.size() > 1 && cross(l.back() - l[l.size() - 2], p - l.back()) <= 0) {
            l.pop_back();
        }
        while (h.size() > 1 && cross(h.back() - h[h.size() - 2], p - h.back()) >= 0) {
            h.pop_back();
        }
        l.push_back(p);
        h.push_back(p);
    }
    ranges::reverse(h);
    h.insert(h.end(), l.begin() + 1, l.end() - 1);
    return h;
}
int sgn(Point p) {
    if (p.y() > 0 || (p.y() == 0 && p.x() < 0)) {
        return 0;
    } else {
        return 1;
    }
}
