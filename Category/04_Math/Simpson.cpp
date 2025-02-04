class Integral {
    static constexpr double EPS = 1e-9;
    const double Pi = acos(-1.0);
    function<double(double)> f;

    double integral(double l, double r, double eps, double st) const {
        double mid = (l + r) / 2;
        double sl = simpson(l, mid);
        double sr = simpson(mid, r);
        if (abs(sl + sr - st) <= 15 * eps) {
            return sl + sr + (sl + sr - st) / 15;
        }
        return integral(l, mid, eps / 2, sl) + integral(mid, r, eps / 2, sr);
    }

public:
    explicit Integral(function<double(double)> func) : f(func) {}

    double simpson(double l, double r) const { return (f(l) + 4 * f((l + r) / 2) + f(r)) * (r - l) / 6; }

    double integral(double l, double r) const { return integral(l, r, EPS, simpson(l, r)); }
};
