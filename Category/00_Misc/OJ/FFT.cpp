#include <bits/stdc++.h>
using i64 = long long;
using namespace std;

const int N = 2e5 + 5, P = 998244353;
using i64 = long long;
using Poly = vector<int>;
/*---------------------------------------------------------------------------*/
struct Complex {
    double x = 0.0;
    double y = 0.0;

    Complex() {}
    Complex(double x, double y) : x(x), y(y) {}
    Complex operator+(const Complex& rhs) const { return Complex(x + rhs.x, y + rhs.y); }
    Complex operator-(const Complex& rhs) const { return Complex(x - rhs.x, y - rhs.y); }
    Complex operator*(const Complex& rhs) const { return Complex(x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x); }
    Complex conj() const { return Complex(x, -y); }
};

namespace FFT {
    constexpr double PI = 3.14159265358979323846;
    int binary_upper_bound_in_bits(int w) {
        if (w <= 0) return 1;
        const int highbit = 31 - __builtin_clz(w);
        return highbit + 1;
    }
    void fft(vector<Complex>& a, int L, int flag) {
        // flag == 1 : forward; flag == -1 : inverse.
        // L should be a power of 2.
        int k = binary_upper_bound_in_bits(L - 1);
        vector<Complex> w(L);
        vector<int> rev(L);
        for (int i = 0; i < L; ++i) {
            w[i] = Complex(cos(2 * PI / L * i), flag * sin(2 * PI / L * i));
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
            if (i < rev[i]) swap(a[i], a[rev[i]]);
        }
        for (int l = 2; l <= L; l <<= 1) {
            int m = l >> 1;
            for (int i = 0; i < L; i += l) {
                for (int k = 0; k < m; ++k) {
                    Complex t = w[L / l * k] * a[i + k + m];
                    a[i + k + m] = a[i + k] - t;
                    a[i + k] = a[i + k] + t;
                }
            }
        }
        if (flag == -1)
            for (int i = 0; i < L; ++i) a[i].x /= L;
    }
} // namespace FFT

/*---------------------------------------------------------------------------*/
namespace Polynomial {
    // basic operator
    int norm(int n) {
        return 1 << (__lg(n - 1) + 1);
    }
    void norm(Poly& a) {
        if (!a.empty())
            a.resize(norm(a.size()), 0);
        else
            a = {0};
    }

    // 任意模数多项式乘法
    Poly multiply_mod_any(Poly f, Poly g, int mod) {
        int L = norm((int) f.size() + (int) g.size() - 1);
        vector<Complex> a(L), b(L), c(L), d(L);
        for (int i = 0; i < f.size(); ++i) {
            a[i].x = int(f[i]) >> 15;
            b[i].x = int(f[i]) & 32767;
        }
        for (int i = 0; i < g.size(); ++i) {
            c[i].x = int(g[i]) >> 15;
            d[i].x = int(g[i]) & 32767;
        }
        FFT::fft(a, L, 1);
        FFT::fft(b, L, 1);
        FFT::fft(c, L, 1);
        FFT::fft(d, L, 1);
        for (int i = 0; i < L; ++i) {
            Complex ta = a[i], tb = b[i], tc = c[i], td = d[i];
            a[i] = ta * tc;
            b[i] = ta * td + tb * tc;
            c[i] = tb * td;
        }
        FFT::fft(a, L, -1);
        FFT::fft(b, L, -1);
        FFT::fft(c, L, -1);
        Poly z((int) f.size() + (int) g.size() - 1);
        for (int i = 0; i < z.size(); ++i) {
            z[i] = (((i64) (a[i].x + .5) % mod << 30) + ((i64) (b[i].x + .5) % mod << 15) + (i64) (c[i].x + .5) % mod) % mod;
        }
        return z;
    }
} // namespace Polynomial
using namespace Polynomial;

int main() {}
