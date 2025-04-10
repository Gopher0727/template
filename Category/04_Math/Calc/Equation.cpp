// ax^2 + bx + c = 0
auto sol(i64 a, i64 b, i64 c) {
    int lo = 0, hi = 1E9;
    while (lo + 1 < hi) {
        auto mid = lo + (hi - lo) / 2;
        if (a * mid * mid + b * mid + c <= 0) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    if (a * lo * lo + b * lo + c == 0) {
        return lo;
    }
    return -1;
}

auto sol(double a, double b, double c) {
    double D = b * b - 4 * a * c;
    if (D >= 0) {
        double x1 = (-b + sqrt(D)) / (2 * a);
        double x2 = (-b - sqrt(D)) / (2 * a);
        return pair {x1, x2};
    } else {
        double Re = -b / (2 * a);
        double Im = sqrt(-D) / (2 * a);
        return pair {Re, Im};
    }
}
