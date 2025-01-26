typedef unsigned int ui;
typedef unsigned long long ll;
#define all(x) (x).begin(), (x).end()
namespace NTT // 禁止混用三模与普通 NTT
{
    mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
#define all(x) (x).begin(), (x).end()
    const int N = 1 << 21; // 务必修改
    // #define MTT
    // #define CRT
    typedef unsigned int ui;
    typedef unsigned long long ll;
    const ui g = 3, f = 1u << 31, I = 86'583'718; // g^((p-1)/4)
#ifndef MTT
    const ui p = 998244353;
    ui w[N];
#else
    const ui p = 1e9 + 7;
    const ui p1 = 469'762'049, p2 = 998'244'353, p3 = 1'004'535'809; // 三模，原根都是 3，非常好
    const ui inv_p1 = 554'580'198, inv_p12 = 395'249'030; // 三模，1 关于 2 逆，1*2 关于 3 逆，1*2 mod 3
    ui w1[N], w2[N], w3[N]; // 三模
#endif
    ui r[N];
    ui inv[N], fac[N], ifac[N], W; // W for mosqrt
    ui ksm(ui x, ui y) {
        ui r = 1;
        while (y) {
            if (y & 1) r = (ll) r * x % p;
            x = (ll) x * x % p;
            y >>= 1;
        }
        return r;
    }
#ifdef MTT
#ifdef CRT
    ui ksm1(ui x, ui y) {
        ui r = 1;
        while (y) {
            if (y & 1) r = (ll) r * x % p1;
            x = (ll) x * x % p1;
            y >>= 1;
        }
        return r;
    }
    ui ksm2(ui x, ui y) {
        ui r = 1;
        while (y) {
            if (y & 1) r = (ll) r * x % p2;
            x = (ll) x * x % p2;
            y >>= 1;
        }
        return r;
    }
    ui ksm3(ui x, ui y) {
        ui r = 1;
        while (y) {
            if (y & 1) r = (ll) r * x % p3;
            x = (ll) x * x % p3;
            y >>= 1;
        }
        return r;
    }
#endif
#endif
    vector<ui> getinvs(vector<ui> a) {
        static ui l[N], r[N];
        int n = a.size(), i;
        if (n <= 2) {
            for (i = 0; i < n; i++) a[i] = ksm(a[i], p - 2);
            return a;
        }
        l[0] = a[0];
        r[n - 1] = a[n - 1];
        for (i = 1; i < n; i++) l[i] = (ll) l[i - 1] * a[i] % p;
        for (i = n - 2; i; i--) r[i] = (ll) r[i + 1] * a[i] % p;
        ui x = ksm(l[n - 1], p - 2);
        a[0] = (ll) x * r[1] % p;
        a[n - 1] = (ll) x * l[n - 2] % p;
        for (i = 1; i < n - 1; i++) a[i] = (ll) x * l[i - 1] % p * r[i + 1] % p;
        return a;
    }
    struct P {
        ui x, y;
        P(ui a = 0, ui b = 0) : x(a), y(b) {}
        P operator*(P& a) { return P(((ll) x * a.x + (ll) y * a.y % p * W) % p, ((ll) x * a.y + (ll) y * a.x) % p); }
    };
    ui ksm(P x, ui y) {
        P r(1, 0);
        while (y) {
            if (y & 1) r = r * x;
            x = x * x;
            y >>= 1;
        }
        return r.x;
    }
    int mosqrt(ui x) {
        if (x == 0) return 0;
        if (ksm(x, p - 1 >> 1) != 1) {
            cerr << "No mosqrt" << endl;
            exit(0);
        }
        ui y;
        do y = rnd() % p;
        while (ksm(W = ((ll) y * y % p + p - x) % p, p - 1 >> 1) <= 1); // not for p=2
        y = ksm(P(y, 1), p + 1 >> 1);
        return y * 2 < p ? y : p - y;
    }
#ifdef MTT
#ifdef CRT
    void init(ui n) {
        static int pre = 0;
        if (pre == n) return;
        ui b = __builtin_ctz(n) - 1, i, j, k, l, wn;
        for (i = 1; i < n; i++) r[i] = r[i >> 1] >> 1 | (i & 1) << b;
        ++b;
        if (pre < n) {
            for (j = k = 1; j < n; j = l, k++) {
                l = j << 1;
                wn = ksm1(g, p1 - 1 >> k);
                w1[j] = 1;
                for (i = j + 1; i < l; i++) w1[i] = (ll) w1[i - 1] * wn % p1;
            }
            for (j = k = 1; j < n; j = l, k++) {
                l = j << 1;
                wn = ksm2(g, p2 - 1 >> k);
                w2[j] = 1;
                for (i = j + 1; i < l; i++) w2[i] = (ll) w2[i - 1] * wn % p2;
            }
            for (j = k = 1; j < n; j = l, k++) {
                l = j << 1;
                wn = ksm3(g, p3 - 1 >> k);
                w3[j] = 1;
                for (i = j + 1; i < l; i++) w3[i] = (ll) w3[i - 1] * wn % p3;
            }
        }
        pre = n;
    }
#endif
#else
    void init(ui n) {
        static int pre = 0;
        if (pre == n) return;
        ui b = __builtin_ctz(n) - 1, i, j, k, l, wn;
        for (i = 1; i < n; i++) r[i] = r[i >> 1] >> 1 | (i & 1) << b;
        ++b;
        if (pre < n) {
            for (j = k = 1; j < n; j = l, k++) {
                l = j << 1;
                wn = ksm(g, p - 1 >> k);
                w[j] = 1;
                for (i = j + 1; i < l; i++) w[i] = (ll) w[i - 1] * wn % p;
            }
        }
        pre = n;
    }
#endif
    ui cal(ui x) {
        return 1u << 32 - __builtin_clz(max(x, 2u) - 1);
    }
    void getinv(int n) {
        static int pre = 0;
        if (!pre) pre = inv[1] = 1;
        if (n <= pre) return;
        for (ui i = pre + 1, j; i <= n; i++) {
            j = p / i;
            inv[i] = (ll) (p - j) * inv[p - i * j] % p;
        }
        pre = n;
    }
    void getfac(int n) {
        static int pre = -1;
        if (pre == -1) pre = 0, ifac[0] = fac[0] = 1;
        if (n <= pre) return;
        getinv(n);
        for (ui i = pre + 1, j; i <= n; i++) fac[i] = (ll) fac[i - 1] * i % p, ifac[i] = (ll) ifac[i - 1] * inv[i] % p;
        pre = n;
    }
    struct Q {
        vector<ui> a;
        ui* pt() { return a.data(); }
        Q(ui x = 2) {
            x = cal(x);
            a.resize(x);
        }
        ui fx(ui x) {
            ui r = 0;
            int i;
            for (i = a.size() - 1; i >= 0 && !a[i]; i--);
            for (; i >= 0; i--) r = ((ll) r * x + a[i]) % p;
            return r;
        }
#ifdef MTT
#ifdef CRT
        void dft1(int o = 0) {
            ui n = a.size(), i, j, k, x, y, *f, *g, *wn, *A = a.data();
            init(n);
            for (i = 1; i < n; i++)
                if (i < r[i]) swap(A[i], A[r[i]]);
            for (k = 1; k < n; k <<= 1) {
                wn = w1 + k;
                for (i = 0; i < n; i += k << 1) {
                    f = A + i;
                    g = A + i + k;
                    for (j = 0; j < k; j++) {
                        x = f[j];
                        y = (ll) g[j] * wn[j] % p1;
                        if (x + y >= p1)
                            f[j] = x + y - p1;
                        else
                            f[j] = x + y;
                        if (x < y)
                            g[j] = x - y + p1;
                        else
                            g[j] = x - y;
                    }
                }
            }
            if (o) {
                x = ksm1(n, p1 - 2);
                for (i = 0; i < n; i++) A[i] = (ll) A[i] * x % p1;
                reverse(A + 1, A + n);
            }
        }
        void dft2(int o = 0) {
            ui n = a.size(), i, j, k, x, y, *f, *g, *wn, *A = a.data();
            init(n);
            for (i = 1; i < n; i++)
                if (i < r[i]) swap(A[i], A[r[i]]);
            for (k = 1; k < n; k <<= 1) {
                wn = w2 + k;
                for (i = 0; i < n; i += k << 1) {
                    f = A + i;
                    g = A + i + k;
                    for (j = 0; j < k; j++) {
                        x = f[j];
                        y = (ll) g[j] * wn[j] % p2;
                        if (x + y >= p2)
                            f[j] = x + y - p2;
                        else
                            f[j] = x + y;
                        if (x < y)
                            g[j] = x - y + p2;
                        else
                            g[j] = x - y;
                    }
                }
            }
            if (o) {
                x = ksm2(n, p2 - 2);
                for (i = 0; i < n; i++) A[i] = (ll) A[i] * x % p2;
                reverse(A + 1, A + n);
            }
        }
        void dft3(int o = 0) {
            ui n = a.size(), i, j, k, x, y, *f, *g, *wn, *A = a.data();
            init(n);
            for (i = 1; i < n; i++)
                if (i < r[i]) swap(A[i], A[r[i]]);
            for (k = 1; k < n; k <<= 1) {
                wn = w3 + k;
                for (i = 0; i < n; i += k << 1) {
                    f = A + i;
                    g = A + i + k;
                    for (j = 0; j < k; j++) {
                        x = f[j];
                        y = (ll) g[j] * wn[j] % p3;
                        if (x + y >= p3)
                            f[j] = x + y - p3;
                        else
                            f[j] = x + y;
                        if (x < y)
                            g[j] = x - y + p3;
                        else
                            g[j] = x - y;
                    }
                }
            }
            if (o) {
                x = ksm3(n, p3 - 2);
                for (i = 0; i < n; i++) A[i] = (ll) A[i] * x % p3;
                reverse(A + 1, A + n);
            }
        }
#endif
#else
        void dft(int o = 0) {
            ui n = a.size(), i, j, k, x, y, *f, *g, *wn, *A = pt();
            init(n);
            for (i = 1; i < n; i++)
                if (i < r[i]) swap(A[i], A[r[i]]);
            for (k = 1; k < n; k <<= 1) {
                wn = w + k;
                for (i = 0; i < n; i += k << 1) {
                    f = A + i;
                    g = A + i + k;
                    for (j = 0; j < k; j++) {
                        x = f[j];
                        y = (ll) g[j] * wn[j] % p;
                        if (x + y >= p)
                            f[j] = x + y - p;
                        else
                            f[j] = x + y;
                        if (x < y)
                            g[j] = x - y + p;
                        else
                            g[j] = x - y;
                    }
                }
            }
            if (o) {
                getinv(n);
                x = inv[n];
                for (i = 0; i < n; i++) A[i] = (ll) A[i] * x % p;
                reverse(A + 1, A + n);
            }
        }
        void hf_dft(int o = 0) {
            ui n = a.size() >> 1, i, j, k, x, y, *f, *g, *wn, *A = pt();
            init(n);
            for (i = 1; i < n; i++)
                if (i < r[i]) swap(A[i], A[r[i]]);
            for (k = 1; k < n; k <<= 1) {
                wn = w + k;
                for (i = 0; i < n; i += k << 1) {
                    f = A + i;
                    g = A + i + k;
                    for (j = 0; j < k; j++) {
                        x = f[j];
                        y = (ll) g[j] * wn[j] % p;
                        if (x + y >= p)
                            f[j] = x + y - p;
                        else
                            f[j] = x + y;
                        if (x < y)
                            g[j] = x - y + p;
                        else
                            g[j] = x - y;
                    }
                }
            }
            if (o) {
                getinv(n);
                x = inv[n];
                for (i = 0; i < n; i++) A[i] = (ll) A[i] * x % p;
                reverse(A + 1, A + n);
            }
        }
#endif
        Q dao() {
            ui n = a.size();
            Q r(n);
            for (ui i = 1; i < n; i++) r.a[i - 1] = (ll) a[i] * i % p;
            return r;
        }
        Q ji() {
            ui n = a.size();
            getinv(n - 1);
            Q r(n);
            for (ui i = 1; i < n; i++) r.a[i] = (ll) a[i - 1] * inv[i] % p;
            return r;
        }
        Q operator-() const {
            Q r = *this;
            for (ui& x : r.a)
                if (x) x = p - x;
            return r;
        }
        Q operator+(ui x) const {
            Q r = *this;
            r += x;
            return r;
        }
        Q& operator+=(ui x) {
            if ((a[0] += x) >= p) a[0] -= p;
            return *this;
        }
        Q operator-(ui x) const {
            Q r = *this;
            r -= x;
            return r;
        }
        Q& operator-=(ui x) {
            if (a[0] < x)
                a[0] = a[0] + p - x;
            else
                a[0] -= x;
            return *this;
        }
        Q operator*(ui k) const {
            Q r = *this;
            r *= k;
            return r;
        }
        Q& operator*=(ui k) {
            for (ui& x : a) x = (ll) x * k % p;
            return *this;
        }
        Q operator+(Q f) const {
            f += *this;
            return f;
        }
        Q& operator+=(const Q& f) {
            ui n = f.a.size();
            if (a.size() < n) a.resize(n);
            for (ui i = 0; i < n; i++)
                if ((a[i] += f.a[i]) >= p) a[i] -= p;
            return *this;
        }
        Q operator-(Q f) const {
            Q r = *this;
            r -= f;
            return r;
        }
        Q& operator-=(const Q& f) {
            ui n = f.a.size();
            if (a.size() < n) a.resize(n);
            for (ui i = 0; i < n; i++)
                if (a[i] < f.a[i])
                    a[i] += p - f.a[i];
                else
                    a[i] -= f.a[i];
            return *this;
        }
        Q operator*(Q f) const {
            f *= *this;
            return f;
        }
#ifdef MTT
#ifdef CRT
        void operator*=(Q g3) {
            Q f, g1, g2;
            g1 = g2 = g3;
            assert(a.size() == g3.a.size());
            ui n = a.size() << 1, i;
            ll x;
            a.resize(n);
            g1.a.resize(n);
            g2.a.resize(n);
            g3.a.resize(n);
            g1.dft1();
            g2.dft2();
            g3.dft3();
            f = *this;
            f.dft1();
            for (i = 0; i < n; i++) g1.a[i] = (ll) g1.a[i] * f.a[i] % p1;
            g1.dft1(1);
            f = *this;
            f.dft2();
            for (i = 0; i < n; i++) g2.a[i] = (ll) g2.a[i] * f.a[i] % p2;
            g2.dft2(1);
            f = *this;
            f.dft3();
            for (i = 0; i < n; i++) g3.a[i] = (ll) g3.a[i] * f.a[i] % p3;
            g3.dft3(1);
            a.resize(n >>= 1);
            ui _p12 = (ll) p1 * p2 % p;
            for (i = 0; i < n; i++) {
                x = (ll) (g2.a[i] + p2 - g1.a[i]) * inv_p1 % p2 * p1 + g1.a[i];
                a[i] = ((x + p3 - g3.a[i]) % p3 * (p3 - inv_p12) % p3 * _p12 + x) % p;
            }
        } // 三模，板子 OJ 5e5 0.9s
#else
        void operator*=(const Q& g) {
            ui n = a.size(), m = (1 << 15) - 1, i;
            assert(n == g.a.size());
            n <<= 1;
            foly a0(n), a1(n), b0(n), b1(n), u(n), v(n);
            n >>= 1;
            for (i = 0; i < n; i++) a0.a[i].x = a[i] >> 15, a1.a[i].x = a[i] & m;
            for (i = 0; i < n; i++) b0.a[i].x = g.a[i] >> 15, b1.a[i].x = g.a[i] & m;
            ddt(a0, a1);
            ddt(b0, b1);
            n <<= 1;
            for (i = 0; i < n; i++) {
                u.a[i] = a0.a[i] * b0.a[i] + FFT::I * a1.a[i] * b0.a[i];
                v.a[i] = a0.a[i] * b1.a[i] + FFT::I * a1.a[i] * b1.a[i];
            }
            u.dft(1);
            v.dft(1);
            n >>= 1;
            a.resize(n);
            for (i = 0; i < n; i++) a[i] = ((((ll) dtol(u.a[i].x) << 15) % p + dtol(u.a[i].y) + dtol(v.a[i].x) << 15) + dtol(v.a[i].y)) % p;
        } // 4 次拆系数
        void operator|=(const Q& g) // 直接卷积
        {
            ui n = cal(a.size() + g.a.size() - 1), m = (1 << 15) - 1, i;
            foly a0(n), a1(n), b0(n), b1(n), u(n), v(n);
            for (i = 0; i < a.size(); i++) a0.a[i].x = a[i] >> 15, a1.a[i].x = a[i] & m;
            for (i = 0; i < g.a.size(); i++) b0.a[i].x = g.a[i] >> 15, b1.a[i].x = g.a[i] & m;
            ddt(a0, a1);
            ddt(b0, b1);
            for (i = 0; i < n; i++) {
                u.a[i] = a0.a[i] * b0.a[i] + FFT::I * a1.a[i] * b0.a[i];
                v.a[i] = a0.a[i] * b1.a[i] + FFT::I * a1.a[i] * b1.a[i];
            }
            u.dft(1);
            v.dft(1);
            a.resize(n);
            for (i = 0; i < n; i++) a[i] = ((((ll) dtol(u.a[i].x) << 15) % p + dtol(u.a[i].y) + dtol(v.a[i].x) << 15) + dtol(v.a[i].y)) % p;
        } // 4 次拆系数，板子 OJ 5e5 1.4s 精度也爆，待修复
#endif
#else
        Q& operator|=(Q f) // 直接卷积，不 shift
        {
            ui n = cal(a.size() + f.a.size() - 1);
            a.resize(n);
            f.a.resize(n);
            dft();
            f.dft();
            for (ui i = 0; i < n; i++) a[i] = (ll) a[i] * f.a[i] % p;
            dft(1);
            return *this;
        }
        Q operator|(Q f) const {
            f |= *this;
            return f;
        }
        Q& operator*=(Q f) // 群内卷积
        {
            assert(a.size() == f.a.size());
            ui n = a.size() << 1;
            a.resize(n);
            f.a.resize(n);
            dft();
            f.dft();
            for (ui i = 0; i < n; i++) a[i] = (ll) a[i] * f.a[i] % p;
            dft(1);
            a.resize(n >> 1);
            return *this;
        }
        Q& operator&=(const Q& f) // 卷积并 shift
        {
            *this |= f;
            int n = a.size(), i;
            for (i = n - 1; i >= 2; i--)
                if (a[i]) break;
            a.resize(cal(i + 1));
            return *this;
        }
        Q operator&(Q f) const {
            f &= *this;
            return f;
        }
        Q& operator^=(Q f) // 差卷积
        {
            assert(a.size() == f.a.size());
            ui n = a.size();
            reverse(all(f.a));
            *this |= f;
            copy(a.data() + n - 1, a.data() + n * 2 - 1, a.data());
            a.resize(n);
            return *this;
        }
        Q operator^(const Q& f) const {
            Q g = *this;
            g ^= f;
            return g;
        }
#endif
#ifdef MTT
        Q operator~() {
            Q q = (*this), r(2);
            ui n = a.size() << 1, i, j, k;
            a.resize(n);
            r.a[0] = ksm(a[0], p - 2);
            for (j = 2; j <= n; j <<= 1) {
                k = j >> 1;
                r.a.resize(j);
                q.a.resize(j);
                copy_n(pt(), k, q.pt());
                r = -(q * r - 2) * r;
                r.a.resize(k);
            }
            n >>= 1;
            a.resize(n);
            return r;
        } // trivial
#else
        Q operator~() {
            Q q = (*this), r(2), g(2);
            ui n = a.size(), i, j, k;
            r.a[0] = ksm(a[0], p - 2);
            for (j = 2; j <= n; j <<= 1) {
                k = j >> 1;
                r.a.resize(j);
                g = r;
                q.a.resize(j);
                copy_n(pt(), j, q.pt());
                r.dft();
                q.dft();
                for (i = 0; i < j; i++) q.a[i] = (ll) q.a[i] * r.a[i] % p;
                q.dft(1);
                fill_n(q.pt(), k, 0);
                q.dft();
                for (i = 0; i < j; i++) r.a[i] = (ll) q.a[i] * r.a[i] % p;
                r.dft(1);
                copy_n(g.pt(), k, r.pt());
                for (i = k; i < j; i++)
                    if (r.a[i]) r.a[i] = p - r.a[i];
            }
            return r;
        } // inv(1 6 3 4 9)=(1 998244347 33 998244169 1020)
#endif
        Q operator/(Q f) const { return (*this) * ~f; }
        void operator/=(Q f) {
            f = ~f;
            (*this) *= f;
        }
    };
    Q read(int n) {
        Q r(n);
        int P = p;
        int x;
        for (int i = 0; i < n; i++) cin >> x, r.a[i] = (x % P + P) % P;
        // for (int i=0;i<n;i++) cin>>r.a[i];
        return r;
    }
    void write(Q& r, int n = -1) {
        if (n == 0) return;
        if (n == -1) n = r.a.size();
        for (int i = 0; i < n - 1; i++) cout << r.a[i] << " ";
        cout << r.a[n - 1] << endl;
    }
#ifndef MTT
    Q sqr(Q b) {
        ui n = b.a.size() << 1, i;
        b.a.resize(n);
        b.dft();
        for (i = 0; i < n; i++) b.a[i] = (ll) b.a[i] * b.a[i] % p;
        b.dft(1);
        b.a.resize(n >> 1);
        return b;
    }
    vector<Q> cd;
    void cdq(Q& f, Q& g, ui l, ui r) // g_0=1,i*g_i=g_{i-j}*f_j,use for exp_cdq
    {
        ui i, m = l + r >> 1, n = r - l, nn = n >> 1;
        if (l == 0 && r == f.a.size()) {
            getinv(n - 1);
            g.a.resize(n);
            for (i = 0; i < n; i++) g.a[i] = 0;
            cd.clear();
            cd.reserve(__builtin_ctz(n));
            Q a(2);
            for (i = 2; i <= n; i <<= 1) {
                a.a.resize(i);
                for (ui j = 0; j < i; j++) a.a[j] = f.a[j];
                a.dft();
                cd.push_back(a);
            }
        }
        if (l + 1 == r) {
            if (l == 0)
                g.a[l] = 1;
            else
                g.a[l] = (ll) g.a[l] * inv[l] % p;
            return;
        }
        cdq(f, g, l, m);
        Q a = cd[__builtin_ctz(n) - 1], b(n);
        for (i = 0; i < nn; i++) b.a[i] = g.a[l + i];
        b.dft();
        for (i = 0; i < n; i++) a.a[i] = (ll) a.a[i] * b.a[i] % p;
        a.dft(1);
        for (i = m; i < r; i++)
            if ((g.a[i] += a.a[i - l]) >= p) g.a[i] -= p;
        cdq(f, g, m, r);
    }
    Q exp_cdq(Q f) {
        Q g(2);
        ui n = f.a.size();
        for (ui i = 1; i < n; i++) f.a[i] = (ll) f.a[i] * i % p;
        cdq(f, g, 0, n);
        return g;
    }
    Q sqrt(Q b) {
        Q q(2), f(2), r(2);
        ui n = b.a.size();
        int i, j = n, l;
        for (i = 0; i < n; i++)
            if (b.a[i]) {
                j = i;
                break;
            }
        if (j == n) return b;
        if (j & 1) {
            puts("-1");
            exit(0);
        }
        l = j >> 1;
        for (i = 0; i < n - j; i++) b.a[i] = b.a[i + j];
        for (i = n - j; i < n; i++) b.a[i] = 0;
        r.a[0] = i = mosqrt(b.a[0]);
        assert(i != -1);
        for (j = 2; j <= n; j <<= 1) {
            r.a.resize(j);
            q = ~r;
            f.a.resize(j << 1);
            for (i = 0; i < j; i++) f.a[i] = b.a[i];
            q.a.resize(j << 1);
            r.a.resize(j << 1);
            q.dft();
            r.dft();
            f.dft();
            for (i = 0; i < j << 1; i++)
                if ((r.a[i] = (ll) q.a[i] * ((ll) r.a[i] * r.a[i] % p + f.a[i]) % p) & 1)
                    r.a[i] = r.a[i] + p >> 1;
                else
                    r.a[i] >>= 1;
            r.dft(1);
            for (i = j; i < j << 1; i++) r.a[i] = 0;
        }
        r.a.resize(n);
        for (i = n - 1; i >= l; i--) r.a[i] = r.a[i - l];
        for (i = 0; i < l; i++) r.a[i] = 0;
        return r;
    } // sqrt(1 8596489 489489 4894 1564 489 35789489)=(1 503420421 924499237 13354513 217017417 707895465 411020414)
#endif
    Q ln(Q b) {
        return (b.dao() / b).ji();
    } // ln(1 927384623 878326372 3882 273455637 998233543)=(0 927384623 817976920 427326948 149643566 610586717)
#ifdef MTT
    Q exp(Q f) {
        Q q(2), r(2);
        ui n = f.a.size() << 1, i, j, k;
        r.a[0] = 1;
        for (j = 2; j <= n; j <<= 1) {
            k = j >> 1;
            r.a.resize(j);
            q.a.resize(j);
            for (i = 0; i < k; i++) q.a[i] = f.a[i];
            r = r * (q - ln(r) + 1);
            r.a.resize(k);
        }
        return r;
    }
#else
    Q exp(Q b) {
        Q q(2), r(2);
        ui n = b.a.size(), i, j;
        r.a[0] = 1;
        for (j = 2; j <= n; j <<= 1) {
            r.a.resize(j);
            q = ln(r);
            for (i = 0; i < j; i++)
                if ((q.a[i] = b.a[i] + p - q.a[i]) >= p) q.a[i] -= p;
            (++q.a[0]) %= p;
            r.a.resize(j << 1);
            q.a.resize(j << 1);
            r.dft();
            q.dft();
            for (i = 0; i < j << 1; i++) r.a[i] = (ll) r.a[i] * q.a[i] % p;
            r.dft(1);
            r.a.resize(j);
        }
        return r;
    } // exp(0 927384623 817976920 427326948 149643566 610586717)=(1 927384623 878326372 3882 273455637 998233543)
    void mul(Q& a, Q& b) {
        ui n = a.a.size();
        a.dft();
        b.dft();
        for (ui i = 0; i < n; i++) a.a[i] = (ll) a.a[i] * b.a[i] % p;
        a.dft(1);
    }
    Q exp_new(Q b) {
        Q h(2), f(2), r(2), u(2), v(2);
        ui n = b.a.size(), i, j, k;
        r.a[0] = 1;
        h.a[0] = 1;
        for (j = 2; j <= n; j <<= 1) {
            f.a.resize(j);
            for (i = 0; i < j; i++) f.a[i] = b.a[i];
            f = f.dao();
            k = j >> 1;
            for (i = 0; i < k - 1; i++) {
                if ((f.a[i + k] += f.a[i]) >= p) f.a[i + k] -= p;
                f.a[i] = 0;
            }
            for (i = k - 1; i < j; i++)
                if (f.a[i]) f.a[i] = p - f.a[i];
            u.a.resize(k);
            v.a.resize(k);
            copy_n(r.pt(), k, u.pt());
            copy_n(h.pt(), k, v.pt());
            u = u.dao();
            mul(u, v);
            for (i = 0; i < k - 1; i++)
                if ((f.a[i + k] += u.a[i]) >= p) f.a[i + k] -= p;
            if ((f.a[k - 1] += u.a[k - 1]) >= p) f.a[k - 1] -= p;
            copy_n(r.pt(), k, u.pt());
            u.dft();
            for (i = 0; i < k; i++) u.a[i] = (ll) u.a[i] * v.a[i] % p;
            u.dft(1);
            (u.a[0] += p - 1) %= p;
            u.a.resize(j);
            v.a.resize(j);
            copy_n(b.pt(), k, v.pt());
            v = v.dao();
            mul(u, v);
            for (i = 0; i < k; i++)
                if (f.a[i + k] < u.a[i])
                    f.a[i + k] += p - u.a[i];
                else
                    f.a[i + k] -= u.a[i];
            f = f.ji();
            copy_n(r.pt(), k, u.pt());
            fill_n(u.pt() + k, k, 0);
            mul(u, f);
            r.a.resize(j);
            for (i = k; i < j; i++)
                if (u.a[i])
                    r.a[i] = p - u.a[i];
                else
                    r.a[i] = 0;
            if (j != n) h = ~r;
        }
        return r;
    }
    Q sqrt_new(Q b) {
        Q q(2), r(2), h(2);
        ui n = b.a.size();
        int i, j = n, k, l;
        for (i = 0; i < n; i++)
            if (b.a[i]) {
                j = i;
                break;
            }
        if (j == n) return b;
        if (j & 1) {
            puts("-1");
            exit(0);
        }
        l = j >> 1;
        for (i = 0; i < n - j; i++) b.a[i] = b.a[i + j];
        for (i = n - j; i < n; i++) b.a[i] = 0;
        r.a[0] = mosqrt(b.a[0]);
        h.a[0] = ksm(r.a[0], p - 2);
        r.a.resize(1);
        ui i2 = ksm(2, p - 2);
        for (j = 2; j <= n; j <<= 1) {
            k = j >> 1;
            q = r;
            q.dft();
            for (i = 0; i < k; i++) q.a[i] = (ll) q.a[i] * q.a[i] % p;
            q.dft(1);
            q.a.resize(j);
            for (i = k; i < j; i++) q.a[i] = (ll) (q.a[i - k] + p * 2u - b.a[i] - b.a[i - k]) * i2 % p;
            for (i = 0; i < k; i++) q.a[i] = 0;
            h.a.resize(j);
            mul(q, h);
            r.a.resize(j);
            for (i = k; i < j; i++)
                if (q.a[i]) r.a[i] = p - q.a[i];
            if (j != n) h = ~r;
        }
        r.a.resize(n);
        for (i = n - 1; i >= l; i--) r.a[i] = r.a[i - l];
        for (i = 0; i < l; i++) r.a[i] = 0;
        return r;
    }
    Q pow(Q b, ui m) {
        ui n = b.a.size();
        int i, j = n, k;
        for (i = 0; i < n; i++)
            if (b.a[i]) {
                j = i;
                break;
            }
        if (j == n) return b;
        if ((ll) j * m >= n) {
            fill_n(b.pt(), n, 0);
            return b;
        }
        for (i = 0; i < n - j; i++) b.a[i] = b.a[i + j];
        for (i = n - j; i < n; i++) b.a[i] = 0;
        k = b.a[0];
        assert(k);
        b = exp_new(ln(b * ksm(k, p - 2)) * m) * ksm(k, m);
        j *= m;
        for (i = n - 1; i >= j; i--) b.a[i] = b.a[i - j];
        for (i = 0; i < j; i++) b.a[i] = 0;
        return b;
    }
    Q pow(Q b, string s) {
        ui n = b.a.size();
        int i, j = n, k;
        for (i = 0; i < n; i++)
            if (b.a[i]) {
                j = i;
                break;
            }
        if (j == n) return b;
        if (j) {
            if (s.size() > 8 || j * stoll(s) >= n) {
                fill_n(b.pt(), n, 0);
                return b;
            }
        }
        ui m0 = 0, m1 = 0;
        for (auto c : s) m0 = (m0 * 10ll + c - '0') % p, m1 = (m1 * 10ll + c - '0') % (p - 1);
        for (i = 0; i < n - j; i++) b.a[i] = b.a[i + j];
        for (i = n - j; i < n; i++) b.a[i] = 0;
        k = b.a[0];
        assert(k);
        b = exp_new(ln(b * ksm(k, p - 2)) * m0) * ksm(k, m1);
        j *= m0;
        for (i = n - 1; i >= j; i--) b.a[i] = b.a[i - j];
        for (i = 0; i < j; i++) b.a[i] = 0;
        return b;
    }
    Q pow2(Q b, ui m) {
        Q r(b.a.size());
        r.a[0] = 1;
        while (m) {
            if (m & 1) r = r * b;
            if (m >>= 1) b = b * b;
        }
        return r;
    }
    pair<Q, Q> div(Q a, Q b) {
        int n = 0, m = 0, l, i, nn = a.a.size();
        for (i = a.a.size() - 1; i >= 0; i--)
            if (a.a[i]) {
                n = i + 1;
                break;
            }
        for (i = b.a.size() - 1; i >= 0; i--)
            if (b.a[i]) {
                m = i + 1;
                break;
            }
        assert(m);
        if (n < m) return make_pair(Q(2), a);
        l = cal(n + m - 1);
        Q c(n), d(m);
        reverse_copy(a.a.data(), a.a.data() + n, c.a.data());
        reverse_copy(b.a.data(), b.a.data() + m, d.a.data());
        c.a.resize(cal(n - m + 1));
        d.a.resize(c.a.size());
        d = ~d;
        for (i = n - m + 1; i < c.a.size(); i++) c.a[i] = d.a[i] = 0;
        c *= d;
        for (i = n - m + 1; i < c.a.size(); i++) c.a[i] = 0;
        reverse(c.a.data(), c.a.data() + n - m + 1);
        n = a.a.size();
        b.a.resize(n);
        c.a.resize(n);
        d = a - c * b;
        // for (i=0;i<d.a.size();i++) cerr<<d.a[i]<<" \n"[i==d.a.size()-1];
        for (i = m; i < d.a.size(); i++) assert(d.a[i] == 0);
        c.a.resize(cal(n - m + 1));
        d.a.resize(cal(m));
        return make_pair(c, d);
    }
    Q sin(Q& f) {
        return (exp(f * I) - exp(f * (p - I))) * ksm(2 * I % p, p - 2);
    }
    Q cos(Q& f) {
        return (exp(f * I) + exp(f * (p - I))) * ksm(2, p - 2);
    }
    Q tan(Q& f) {
        return sin(f) / cos(f);
    }
    Q asin(Q& f) {
        return (f.dao() / sqrt((f * f - 1) * (p - 1))).ji();
    }
    Q acos(Q& f) {
        return ((f.dao() / sqrt((f * f - 1) * (p - 1))) * (p - 1)).ji();
    }
    Q atan(Q& f) {
        return (f.dao() / (f * f + 1)).ji();
    }
    Q cdq_inv(Q& f) {
        return (~(f - 1)) * (p - 1);
    } // g_0=1,g_i=g_{i-j}*f_j
    Q operator%(Q f, Q g) {
        return div(f, g).second;
    }
    void operator%=(Q& f, const Q& g) {
        f = f % g;
    }
    ui dt(const vector<ui>& f, const vector<ui>& a, ll m) // 常系数齐次线性递推，find a_m,a_n=a_{n-i}*f_i,f_1...k,a_0...k-1
    {
        if (m < a.size()) return a[m];
        assert(f.size() == a.size() + 1);
        int k = a.size();
        ui n = cal(k + 1 << 1), i, ans = 0, l;
        Q h(n);
        for (i = 1; i <= k; i++)
            if (f[i]) h.a[k - i] = p - f[i];
        h.a[k] = 1;
        Q g, r;
        g.a[1] = 1;
        r.a[0] = 1;
        while (m) {
            if (m & 1) r = (r & g) % h;
            l = g.a.size() << 1;
            g.a.resize(l);
            g.dft();
            for (i = 0; i < l; i++) g.a[i] = (ll) g.a[i] * g.a[i] % p;
            g.dft(1);
            g %= h;
            m >>= 1;
        }
        k = min(k, (int) r.a.size());
        for (i = 0; i < k; i++) ans = (ans + (ll) a[i] * r.a[i]) % p;
        return ans;
    } // 板子 OJ 1e5/1e18 8246ms，Luogu 32000/1e9 710ms
    ui new_dt(const vector<ui>& f, const vector<ui>& a, ll m) // 常系数齐次线性递推，find a_m,a_n=a_{n-i}*f_i,f_1...k,a_0...k-1
    {
        if (m < a.size()) return a[m];
        assert(f.size() == a.size() + 1);
        ui k = a.size(), n = cal(k + 1), lim = n * 2, x;
        int i;
        Q g(n), h(n);
        vector<ui> res(k);
        for (i = 1; i <= k; i++)
            if (f[i]) h.a[i] = p - f[i];
        h.a[0] = 1;
        for (i = 0; i < k; i++) g.a[i] = a[i];
        g *= h;
        fill(g.a.data() + k, g.a.data() + n, 0);
        ++k;
        g.a.resize(lim);
        h.a.resize(lim);
        while (m) {
            if (m & 1) {
                x = p - g.a[0];
                for (i = 1; i < k; i += 2) res[i >> 1] = (ll) x * h.a[i] % p;
                copy(g.a.data() + 1, g.a.data() + k, g.a.data());
                g.a[k - 1] = 0;
            }
            g.dft();
            h.dft();
            ui *a = g.a.data(), *b = h.a.data(), *c = a + n, *d = b + n;
            for (i = 0; i < n; i++) g.a[i] = ((ll) a[i] * d[i] + (ll) b[i] * c[i] + (ll) ((a[i] & d[i]) ^ (b[i] & c[i])) * p >> 1) % p;
            for (i = 0; i < n; i++) h.a[i] = (ll) h.a[i] * h.a[i ^ n] % p;
            g.hf_dft(1);
            h.hf_dft(1);
            fill(g.a.data() + k, g.a.data() + lim, 0);
            if (m & 1)
                for (i = 0; i < k; i++)
                    if ((g.a[i] += res[i]) >= p) g.a[i] -= p;
            fill(h.a.data() + k, h.a.data() + lim, 0);
            m >>= 1;
        }
        return g.a[0];
    } // 板子 OJ 1e5/1e18 1310ms，Luogu 32000/1e9 160ms
    Q mult(Q* a, int n) {
        if (n == 1) return a[0];
        int m = n >> 1;
        return mult(a, m) & mult(a + m, n - m);
    }
    vector<Q> pro;
    ui* X;
    vector<ui> Y;
    void build(int x, int l, int r) {
        if (l == r) {
            pro[x].a.resize(2);
            pro[x].a[0] = (p - X[l]) % p;
            pro[x].a[1] = 1;
            return;
        }
        int mid = l + r >> 1, c = x << 1;
        build(c, l, mid);
        build(c | 1, mid + 1, r);
        pro[x] = pro[c] & pro[c | 1];
    }
    void sgt_dfs(int x, int l, int r, Q f, int d) {
        if (d >= r - l + 1) {
            f %= pro[x];
            d = r - l;
            while (d > 0 && !f.a[d]) --d;
            f.a.resize(cal(d + 1));
        }
        if (r - l + 1 <= 255) {
            for (ui i = l; i <= r; i++) Y[i] = f.fx(X[i]);
            return;
        }
        int mid = l + r >> 1, c = x << 1;
        sgt_dfs(c, l, mid, f, d);
        sgt_dfs(c | 1, mid + 1, r, f, d);
    }
    vector<ui> get_fx(Q& f, vector<ui>& x) {
        int m = x.size(), i, j;
        int n = f.a.size() - 1;
        pro.resize(m * 4 + 8);
        while (n > 1 && !f.a[n]) --n;
        X = x.data();
        Y.resize(m);
        build(1, 0, m - 1);
        sgt_dfs(1, 0, m - 1, f, n);
        return Y;
    }
    void new_build(int x, int n) {
        if (n == 1) {
            pro[x].a.resize(2);
            pro[x].a[0] = 1;
            pro[x].a[1] = (p - *(X++)) % p;
            return;
        }
        int mid = n + 1 >> 1, c = x << 1;
        new_build(c, mid);
        new_build(c | 1, n - mid);
        pro[x] = pro[c] & pro[c | 1];
    }
    const int get_fx_lim2 = 30;
    void new_sgt_dfs(int x, int l, int r, Q f) {
        if (r - l + 1 <= get_fx_lim2) {
            int m = r - l + 1, m1, m2, mid = l + r >> 1, i, j, k;
            static ui g[get_fx_lim2 + 2], g1[get_fx_lim2 + 2], g2[get_fx_lim2 + 2];
            m1 = m2 = r - l;
            memcpy(g1, f.a.data(), m * sizeof g1[0]);
            memcpy(g2, f.a.data(), m * sizeof g2[0]);
            for (i = mid + 1; i <= r; i++) {
                for (k = 0; k < m1; k++) g1[k] = (g1[k] + (ll) g1[k + 1] * (p - X[i])) % p;
                --m1;
            }
            for (i = l; i <= mid; i++) {
                for (k = 0; k < m2; k++) g2[k] = (g2[k] + (ll) g2[k + 1] * (p - X[i])) % p;
                --m2;
            }
            for (i = l; i <= mid; i++) {
                m = m1;
                memcpy(g, g1, (m1 + 1) * sizeof g[0]);
                for (j = l; j <= mid; j++)
                    if (i != j) {
                        for (k = 0; k < m; k++) g[k] = (g[k] + (ll) g[k + 1] * (p - X[j])) % p;
                        --m;
                    }
                Y[i] = g[0];
            }
            for (i = mid + 1; i <= r; i++) {
                m = m2;
                memcpy(g, g2, (m2 + 1) * sizeof g[0]);
                for (j = mid + 1; j <= r; j++)
                    if (i != j) {
                        for (k = 0; k < m; k++) g[k] = (g[k] + (ll) g[k + 1] * (p - X[j])) % p;
                        --m;
                    }
                Y[i] = g[0];
            }
            return;
        }
        int mid = l + r >> 1, c = x << 1, n = f.a.size();
        pro[c].a.resize(n);
        pro[c | 1].a.resize(n);
        f.dft();
        reverse(all(pro[c].a));
        pro[c].dft();
        for (int i = 0; i < n; i++) pro[c].a[i] = (ll) pro[c].a[i] * f.a[i] % p;
        pro[c].dft(1);
        rotate(all(pro[c].a) - 1, pro[c].a.end());
        pro[c].a.resize(cal(r - mid));
        fill(pro[c].a.begin() + r - mid, pro[c].a.end(), 0);
        c ^= 1;
        reverse(all(pro[c].a));
        pro[c].dft();
        for (int i = 0; i < n; i++) pro[c].a[i] = (ll) pro[c].a[i] * f.a[i] % p;
        pro[c].dft(1);
        rotate(all(pro[c].a) - 1, pro[c].a.end());
        pro[c].a.resize(cal(mid - l + 1));
        fill(pro[c].a.begin() + mid - l + 1, pro[c].a.end(), 0);
        c ^= 1;
        new_sgt_dfs(c, l, mid, pro[c | 1]);
        new_sgt_dfs(c | 1, mid + 1, r, pro[c]);
    }
    vector<ui> new_get_fx(Q f, vector<ui>& x) // 多项式多点求值
    {
        if (x.size() == 0) return x;
        int m = x.size(), i, j;
        if (x.size() <= 10) {
            Y.resize(m);
            for (i = 0; i < m; i++) Y[i] = f.fx(x[i]);
            return Y;
        }
        int n = f.a.size();
        while (n > 1 && !f.a[n - 1]) --n;
        if (cal(n) != f.a.size()) f.a.resize(cal(n));
        X = x.data();
        Y.resize(m);
        pro.resize(m * 4 + 8);
        new_build(1, m);
        X = x.data();
        pro[1].a.resize(f.a.size());
        f ^= ~pro[1];
        f.a.resize(cal(m));
        fill(f.a.begin() + min(m, n), f.a.end(), 0);
        new_sgt_dfs(1, 0, m - 1, f);
        return Y;
    } // 板子 OJ 2^17 550ms
    vector<Q> sum;
    void get_poly_build(int x, int n) {
        if (n == 1) {
            sum[x].a.resize(2);
            sum[x].a[1] = 1;
            sum[x].a[0] = (p - *(X++)) % p;
            return;
        }
        int mid = n + 1 >> 1, c = x << 1;
        get_poly_build(c, mid);
        get_poly_build(c | 1, n - mid);
        sum[x] = sum[c] & sum[c | 1];
    }
    void get_poly_dfs(int x, int l, int r) {
        if (l == r) {
            pro[x].a.resize(2);
            pro[x].a[0] = Y[l];
            pro[x].a[1] = 0;
            return;
        }
        int c = x << 1, mid = l + r >> 1;
        get_poly_dfs(c, l, mid);
        get_poly_dfs(c | 1, mid + 1, r);
        pro[x] = (pro[c] & sum[c | 1]) + (pro[c | 1] & sum[c]);
    }
    Q get_poly(vector<ui>& x, vector<ui>& y) // 多项式快速插值
    {
        assert(x.size() == y.size());
        int n = x.size(), i, j;
        if (n == 0) return Q(2);
        if (n == 1) {
            Q f(2);
            f.a[0] = y[0];
            return f;
        }
        if (1) {
            auto vv = x;
            sort(all(vv));
            assert(unique(all(vv)) - vv.begin() == n);
        }
        sum.resize(4 * n + 8);
        X = x.data();
        get_poly_build(1, n);
        sum[1] = sum[1].dao();
        auto v = new_get_fx(sum[1], x);
        pro.resize(4 * n + 8);
        assert(v.size() == n);
        Y = getinvs(v);
        for (i = 0; i < n; i++) Y[i] = (ll) Y[i] * y[i] % p;
        get_poly_dfs(1, 0, n - 1);
        pro[1].a.resize(cal(n));
        return pro[1];
    } // 板子 OJ 2^17 1.3s
    Q comp(const Q& f, Q g) // 多项式复合 f(g(x))=[x^i]f(x)g(x)^i
    {
        int n = f.a.size(), l = ceil(::sqrt(n)), i, j;
        assert(n >= g.a.size()); // 返回 n-1 次多项式
        vector<Q> a(l + 1), b(l);
        a[0].a.resize(n);
        a[0].a[0] = 1;
        a[1] = g;
        g.a.resize(n * 2);
        Q w = g, u, v(n);
        w.dft();
        u = w;
        for (i = 2; i <= l; i++) {
            if (i > 2) u.dft();
            for (j = 0; j < n * 2; j++) u.a[j] = (ll) u.a[j] * w.a[j] % p;
            u.dft(1);
            fill(u.a.data() + n, u.a.data() + n * 2, 0);
            a[i] = u;
        }
        w = a[l];
        w.dft();
        u = w;
        for (i = 2; i <= l; i++) a[i].a.resize(n);
        for (i = 2; i < l; i++) {
            if (i > 2) u.dft();
            b[i - 1] = u;
            for (j = 0; j < n * 2; j++) u.a[j] = (ll) u.a[j] * w.a[j] % p;
            u.dft(1);
            fill(u.a.data() + n, u.a.data() + n * 2, 0);
        }
        if (l > 2) u.dft();
        b[l - 1] = u;
        for (i = 0; i < l; i++) {
            fill(all(v.a), 0);
            for (j = 0; j < l; j++)
                if (i * l + j < n) v += a[j] * f.a[i * l + j];
            if (i == 0)
                u = v;
            else {
                v.a.resize(n * 2);
                v.dft();
                for (j = 0; j < n * 2; j++) v.a[j] = (ll) v.a[j] * b[i].a[j] % p;
                v.dft(1);
                v.a.resize(n);
                u += v;
            }
        }
        return u;
    } // n^2+n\sqrt n\log n，8000 板子 OJ 300ms，20000 luogu 3.5s
    Q comp_inv(Q f) // 多项式复合逆 g(f(x))=x，求 g，[x^n]g=([x^{n-1}](x/f)^n)/n，要求常数 0 一次非 0
    {
        assert(!f.a[0] && f.a[1]);
        int n = f.a.size(), l = ceil(::sqrt(n)), i, j, k, m; // l>=2
        for (i = 1; i < n; i++) f.a[i - 1] = f.a[i];
        f.a[n - 1] = 0;
        f = ~f;
        getinv(n * 2);
        vector<Q> a(l + 1), b(l);
        Q u, v;
        u = a[1] = f;
        u.a.resize(n * 2);
        u.dft();
        v = u;
        for (i = 2; i <= l; i++) {
            if (i > 2) u.dft();
            for (j = 0; j < n * 2; j++) u.a[j] = (ll) u.a[j] * v.a[j] % p;
            u.dft(1);
            fill(u.a.data() + n, u.a.data() + n * 2, 0);
            a[i] = u;
        }
        b[0].a.resize(n);
        b[0].a[0] = 1;
        b[1] = a[l];
        u.dft();
        v = u;
        for (i = 2; i < l; i++) {
            if (i > 2) u.dft();
            for (j = 0; j < n * 2; j++) u.a[j] = (ll) u.a[j] * v.a[j] % p;
            u.dft(1);
            fill(u.a.data() + n, u.a.data() + n * 2, 0);
            b[i] = u;
        }
        u.a.resize(n);
        u.a[0] = 0;
        for (i = 0; i < l; i++)
            for (j = 1; j <= l; j++)
                if (i * l + j < n) {
                    m = i * l + j - 1;
                    ui r = 0, *f = b[i].a.data(), *g = a[j].a.data();
                    for (k = 0; k <= m; k++) r = (r + (ll) f[k] * g[m - k]) % p;
                    u.a[m + 1] = (ll) r * inv[m + 1] % p;
                }
        return u;
    }
    Q shift(Q f, ui c) // get f(x+c)
    {
        int n = f.a.size(), i, j;
        Q g(n);
        getfac(n);
        for (i = 0; i < n; i++) f.a[i] = (ll) f.a[i] * fac[i] % p;
        g.a[0] = 1;
        for (i = 1; i < n; i++) g.a[i] = (ll) g.a[i - 1] * c % p;
        for (i = 0; i < n; i++) g.a[i] = (ll) g.a[i] * ifac[i] % p;
        f ^= g;
        for (i = 0; i < n; i++) f.a[i] = (ll) f.a[i] * ifac[i] % p;
        return f;
    }
    vector<ui> point_shift(vector<ui> y, ui c, ui m) //[0,n) 点值 -> [c,c+m) 点值
    {
        assert(y.size());
        if (y.size() == 1) return vector<ui>(m, y[0]);
        vector<ui> r, res;
        r.reserve(m);
        int n = y.size(), i, j, mm = m;
        while (c < n && m) r.push_back(y[c++]), --m;
        if (c + m > p) {
            res = point_shift(y, 0, c + m - p);
            m = p - c;
        }
        if (!m) {
            r.insert(r.end(), all(res));
            return r;
        }
        int len = cal(m + n - 1), l = m + n - 1;
        for (i = n & 1; i < n; i += 2)
            if (y[i]) y[i] = p - y[i];
        getfac(n);
        for (i = 0; i < n; i++) y[i] = (ll) y[i] * ifac[i] % p * ifac[n - 1 - i] % p;
        y.resize(len);
        Q f, g;
        vector<ui> v(m + n - 1);
        c -= n - 1;
        for (i = 0; i < l; i++) v[i] = (c + i) % p;
        f.a = y;
        g.a = getinvs(v);
        g.a.resize(len);
        f |= g;
        vector<ui> u(m);
        for (i = n - 1; i < l; i++) u[i - (n - 1)] = f.a[i];
        v.resize(m);
        for (i = 0; i < m; i++) v[i] = c + i;
        v = getinvs(v);
        c += n;
        ui tmp = 1;
        for (i = c - n; i < c; i++) tmp = (ll) tmp * i % p;
        for (i = 0; i < m; i++) u[i] = (ll) u[i] * tmp % p, tmp = (ll) tmp * (c + i) % p * v[i] % p;
        r.insert(r.end(), all(u));
        r.insert(r.end(), all(res));
        assert(r.size() == mm);
        return r;
    } // 板子 OJ 20w 370ms，luogu 16w 150ms
    const ui B = 1e5;
    ui a[B + 2], b[B + 2];
    ui mic(ui x) {
        return (ll) a[x % B] * b[x / B] % p;
    }
    vector<ui> Z_trans(Q f, ui c, ui m) // 求 f(c^[0,m))。核心 ij=C(i+j,2)-C(i,2)-C(j,2)
    {
        ui i, j, n = f.a.size();
        if ((ll) n * m < B * 5) {
            vector<ui> r(m);
            for (i = 0, j = 1; i < m; i++) r[i] = f.fx(j), j = (ll) j * c % p;
            return r;
        }
        ui l = cal(m += n - 1);
        Q g(l);
        assert((ll) B * B > p);
        a[0] = b[0] = g.a[0] = g.a[1] = 1;
        for (i = 1; i <= B; i++) a[i] = (ll) a[i - 1] * c % p;
        c = a[B];
        for (i = 1; i <= B; i++) b[i] = (ll) b[i - 1] * c % p;
        for (i = 2; i < n; i++) f.a[i] = (ll) f.a[i] * mic((ll) (p * 2 - 2 - i) * (i - 1) / 2 % (p - 1)) % p;
        reverse(all(f.a));
        f.a.resize(l);
        for (i = 2; i < m; i++) g.a[i] = mic((ll) i * (i - 1) / 2 % (p - 1));
        f.dft();
        g.dft();
        for (i = 0; i < l; i++) f.a[i] = (ll) f.a[i] * g.a[i] % p;
        f.dft(1);
        vector<ui> r(f.a.data() + n - 1, f.a.data() + m);
        m -= n - 1;
        for (i = 2; i < m; i++) r[i] = (ll) r[i] * mic((ll) (p * 2 - 2 - i) * (i - 1) / 2 % (p - 1)) % p;
        return r;
    } // luogu 1e6 400ms
    vector<ui> get_Bell(int n) // B(0...n)
    {
        ++n;
        getfac(n - 1);
        Q f(n);
        int i;
        for (i = 1; i < n; i++) f.a[i] = ifac[i];
        f = exp(f);
        for (i = 2; i < n; i++) f.a[i] = (ll) f.a[i] * fac[i] % p;
        return vector<ui>(f.a.data(), f.a.data() + n);
    }
    vector<ui> get_S1_row(int n, int m) // S1(n,0...m),O(nlogn),unsigned
    {
        int cm = cal(++m);
        if (n == 0) {
            vector<ui> r(m);
            r[0] = 1;
            return r;
        }
        auto dfs = [&](auto self, int n) {
            if (n == 1) {
                Q f(2);
                f.a[1] = 1;
                return f;
            }
            Q f = self(self, n >> 1);
            f |= shift(f, n >> 1);
            if (n & 1) {
                f.a.resize(cal(n + 1));
                copy_n(f.a.data(), n, f.a.data() + 1);
                --n;
                for (ui i = 0; i <= n; i++) f.a[i] = (f.a[i] + (ll) f.a[i + 1] * n) % p;
            }
            if (f.a.size() > cm) f.a.resize(cm);
            return f;
        };
        Q f = dfs(dfs, n);
        return vector<ui>(f.a.data(), f.a.data() + m);
    }
    vector<ui> get_S1_column(int n, int m) // S1(0...n,m),O(nlogn)
    {
        if (m == 0) {
            vector<ui> r(n + 1);
            r[0] = 1;
            return r;
        }
        Q f(n + 1);
        getfac(max(n, m));
        int i;
        for (i = 1; i <= n; i++) f.a[i] = inv[i];
        f = pow(f, m);
        for (i = m; i <= n; i++) f.a[i] = (ll) f.a[i] * fac[i] % p * ifac[m] % p;
        return vector<ui>(f.a.data(), f.a.data() + n + 1);
    }
    vector<ui> get_S2_row(int n, int m) // S2(n,0...m),O(mlogm)
    {
        int tm = ++m;
        if (n == 0) {
            vector<ui> r(m);
            r[0] = 1;
            return r;
        }
        m = min(m, n + 1);
        ui pr[m], pw[m], cnt = 0;
        int i, j;
        fill_n(pw, m, 0);
        pw[1] = 1;
        for (i = 2; i < m; i++) {
            if (!pw[i]) pr[cnt++] = i, pw[i] = ksm(i, n);
            for (j = 0; i * pr[j] < m; j++) {
                pw[i * pr[j]] = (ll) pw[i] * pw[pr[j]] % p;
                if (i % pr[j] == 0) break;
            }
        }
        getfac(m - 1);
        Q f(m), g(m);
        for (i = 0; i < m; i += 2) f.a[i] = ifac[i];
        for (i = 1; i < m; i += 2) f.a[i] = p - ifac[i];
        for (i = 1; i < m; i++) g.a[i] = (ll) pw[i] * ifac[i] % p;
        f *= g;
        vector<ui> r(f.a.data(), f.a.data() + m);
        r.resize(tm);
        return r;
    }
    vector<ui> get_S2_column(int n, int m) // S2(0...n,m),O(nlogn)
    {
        if (m == 0) {
            vector<ui> r(n + 1);
            r[0] = 1;
            return r;
        }
        Q f(n + 1);
        getfac(max(n, m));
        int i;
        for (i = 1; i <= n; i++) f.a[i] = ifac[i];
        f = pow(f, m);
        for (i = m; i <= n; i++) f.a[i] = (ll) f.a[i] * fac[i] % p * ifac[m] % p;
        return vector<ui>(f.a.data(), f.a.data() + n + 1);
    }
    vector<ui> get_signed_S1_row(int n, int m) {
        auto v = get_S1_row(n, m);
        for (ui i = 1 ^ n & 1; i <= m; i += 2)
            if (v[i]) v[i] = p - v[i];
        return v;
    }
    vector<ui> get_Bernoulli(int n) // B(0...n)
    {
        getfac(++n);
        int i;
        Q f(n);
        for (i = 0; i < n; i++) f.a[i] = ifac[i + 1];
        f = ~f;
        for (i = 0; i < n; i++) f.a[i] = (ll) f.a[i] * fac[i] % p;
        return vector<ui>(f.a.data(), f.a.data() + n);
    }
    vector<ui> get_partition(int n) // P(0...n)，分拆数
    {
        Q f(++n);
        int i, l = 0, r = 0;
        while (--l)
            if (3 * l * l - l >= n * 2) break;
        while (++r)
            if (3 * r * r - r >= n * 2) break;
        ++l;
        for (i = l + abs(l) % 2; i < r; i += 2) f.a[3 * i * i - i >> 1] = 1;
        for (i = l + abs(l + 1) % 2; i < r; i += 2) f.a[3 * i * i - i >> 1] = p - 1;
        f = ~f;
        return vector<ui>(f.a.data(), f.a.data() + n);
    }
#endif
} // namespace NTT
using NTT::p;
using NTT::read;
using NTT::ui;
#define poly NTT::Q
const int N = 1e6 + 10;
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    string s;
    cin >> n >> s;
    poly f(n);
    for (int i = 0; i < n; i++) cin >> f.a[i];
    // for (int i=0;i<n;i++) cout<<f.a[i]<<" \n"[i==n-1];
    auto g = pow(f, s);
    // cout<<f.a.size()<<endl;
    for (int i = 0; i < n; i++) cout << g.a[i] << " \n"[i == n - 1];
}
