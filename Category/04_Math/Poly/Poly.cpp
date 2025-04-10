namespace Polynomial {
    const int P = 998244353;

    int nttPow(int x, int n) {
        int res = 1;
        for (; n > 0; n >>= 1, x = (1LL * x * x) % P) {
            if (n % 2 == 1) {
                res = 1LL * res * x % P;
            }
        }
        return res;
    }

    vector<int> omega(31), omegaInv(31);
    auto init = []() {
        const int g = 3;
        const int invG = 332748118;
        for (int i = 1; i < omega.size(); i++) {
            omega[i] = nttPow(g, (P - 1) / (1 << i));
            omegaInv[i] = nttPow(invG, (P - 1) / (1 << i));
        }
        return 0;
    }();

    struct NTT {
        int n, invN;

        explicit NTT(int n) : n(n), invN(nttPow(n, P - 2)) {}

    public:
        void transform(vector<int>& a, const vector<int>& omega) const {
            for (int i = 0, j = 0; i < n; i++) {
                if (i > j) {
                    swap(a[i], a[j]);
                }
                for (int l = n >> 1;; l >>= 1) {
                    j ^= l;
                    if (j >= l) {
                        break;
                    }
                }
            }
            for (int l = 2, li = 1; l <= n; l <<= 1) {
                int m = l >> 1;
                int wn = omega[li++];
                for (int st = 0; st < n; st += l) {
                    for (int i = 0, w = 1; i < m; i++) {
                        int d = 1LL * a[st + m + i] * w % P;
                        a[st + m + i] = (a[st + i] - d + P) % P;
                        a[st + i] = (a[st + i] + d) % P;
                        w = 1LL * w * wn % P;
                    }
                }
            }
        }

        void dft(vector<int>& a) const { transform(a, omega); }

        void idft(vector<int>& a) const {
            transform(a, omegaInv);
            for (int& v : a) {
                v = 1LL * v * invN % P;
            }
        }
    };

    vector<int> inv {0, 1};
    auto Inv(int m) {
        int n = inv.size();
        if (m < n) {
            return;
        }
        inv.resize(m);
        for (int i = n; i < m; ++i) {
            inv[i] = P - inv[P % i] * 1ll * (P / i) % P;
        }
        n = m;
    }

    // Tonelli–Shanks 模素数的情况下，计算平方根
    int modSqrt(int a) {
        if (a == 0) { // 如果 a 是 0，直接返回 0
            return 0;
        }
        // if (P == 2) { // p 必须是素数，且 (a / p) == 1
        //     return a % 2;
        // }
        // if (P % 4 == 3) { // 确保 p 是奇素数
        //     // 如果 p ≡ 3 (mod 4)，可以直接使用 a^( (p+1)/4 )
        //     return nttPow(a, (P + 1) / 4);
        // }

        // 计算 s 和 q，P - 1 = s * 2^q
        int s = 0, q = P - 1;
        while (q % 2 == 0) {
            s++;
            q /= 2;
        }

        // x 初始为 a^((q+1)/2)（随机选一个非平方数）
        int z = 2;
        while (nttPow(z, (P - 1) / 2) == 1) {
            z++;
        }

        int m = s;
        int c = nttPow(z, q); // c = z^q % p
        int t = nttPow(a, q); // t = a^q % p
        int r = nttPow(a, (q + 1) / 2); // r = a^((q+1)/2) % p
        while (t != 0 && t != 1) {
            int t2i = t;
            int i = 0;
            for (i = 1; i < m; ++i) {
                t2i = 1LL * t2i * t2i % P;
                if (t2i == 1) break;
            }
            int b = nttPow(c, 1 << (m - i - 1));
            m = i;
            c = nttPow(b, 2);
            t = 1LL * t * c % P;
            r = 1LL * r * b % P;
        }
        return t == 0 ? 0 : r;
    }

    struct Poly : vector<int> {
        explicit Poly(int n) : vector<int>(n) {}

    public:
        Poly operator+(const Poly& b) const {
            int n = this->size();
            Poly c(n);
            for (int i = 0; i < n; i++) {
                c[i] = (this->at(i) + b[i]) % P;
            }
            return c;
        }
        Poly operator-(const Poly& b) const {
            int n = this->size();
            Poly c(n);
            for (int i = 0; i < n; i++) {
                c[i] = (this->at(i) - b[i] + P) % P;
            }
            return c;
        }
        Poly operator-() const {
            int n = this->size();
            Poly c(n);
            for (int i = 0; i < n; i++) {
                if (this->at(i) > 0) {
                    c[i] = (P - this->at(i)) % P;
                }
            }
            return c;
        }
        Poly operator*(int k) const {
            k %= P;
            int n = this->size();
            Poly b(n);
            for (int i = 0; i < n; i++) {
                b[i] = 1LL * this->at(i) * k % P;
            }
            return b;
        }
        Poly& operator+=(Poly b) { return (*this) = (*this) + b; }
        Poly& operator-=(Poly b) { return (*this) = (*this) - b; }
        Poly& operator*=(int k) { return (*this) = (*this) * k; }

    public:
        Poly resize(int n) const { // 返回副本
            Poly b(n);
            std::copy(this->begin(), min(this->begin() + n, this->end()), b.begin());
            return b;
        }

        Poly conv(const Poly& b) const {
            int n = this->size(), m = b.size();
            int limit = 1 << bit_width(n + m - 1ull);
            Poly A = this->resize(limit);
            Poly B = b.resize(limit);
            NTT t(limit);
            t.dft(A);
            t.dft(B);
            for (int i = 0; i < limit; i++) {
                A[i] = 1LL * A[i] * B[i] % P;
            }
            t.idft(A);
            A.resize(n + m - 1);
            return A;
        }

        Poly polyConvNTTs(const vector<Poly>& coefs) const {
            int n = coefs.size();
            if (n == 1) {
                return coefs[0];
            }
            Poly left = polyConvNTTs(vector<Poly>(coefs.begin(), coefs.begin() + n / 2));
            Poly right = polyConvNTTs(vector<Poly>(coefs.begin() + n / 2, coefs.end()));
            return left.conv(right);
        }

        Poly reverse() {
            std::reverse(this->begin(), this->end());
            return *this;
        }

        Poly reverseCopy() const {
            int n = this->size();
            Poly b(n);
            for (int i = 0; i < n; i++) {
                b[n - 1 - i] = this->at(i);
            }
            return b;
        }

        Poly lsh(int k) const {
            int n = this->size();
            Poly b(n);
            if (k < n) {
                std::copy(this->begin(), this->end(), b.begin() + k);
            }
            return b;
        }

        Poly rsh(int k) const {
            int n = this->size();
            Poly b(n);
            if (k < n) {
                std::copy(this->begin() + k, this->end(), b.begin());
            }
            return b;
        }

        Poly derivative() const {
            int n = this->size();
            Poly d(n);
            for (int i = 1; i < n; i++) {
                d[i - 1] = 1LL * this->at(i) * i % P;
            }
            return d;
        }

        Poly integral() const {
            int n = this->size();
            Poly s(n);
            Inv(n);
            for (int i = 1; i < n; i++) {
                s[i] = 1LL * this->at(i - 1) * Polynomial::inv[i] % P;
            }
            return s;
        }

        Poly inv() const {
            int n = this->size();
            int m = 1 << bit_width(n * 1ull);
            Poly A = this->resize(m);
            Poly invA(m);
            invA[0] = nttPow(A[0], P - 2);
            for (int l = 2; l <= m; l <<= 1) {
                Poly b = A.resize(l).resize(l << 1);
                Poly iv = invA.resize(l).resize(l << 1);
                NTT t(l << 1);
                t.dft(b);
                t.dft(iv);
                for (int i = 0; i < iv.size(); i++) {
                    b[i] = 1LL * iv[i] * (2 - 1LL * b[i] * iv[i] % P + P) % P;
                }
                t.idft(b);
                std::copy(b.begin(), min(b.begin() + l, b.end()), invA.begin());
            }
            return invA.resize(n);
        }

        Poly div(const Poly& b) const {
            int k = this->size() - b.size() + 1;
            if (k <= 0) {
                return Poly(1);
            }
            Poly A = this->reverseCopy().resize(k);
            Poly B = b.reverseCopy().resize(k);
            return A.conv(B.inv()).resize(k).reverse();
        }

        Poly mod(const Poly& b) const {
            int m = b.size();
            return this->resize(m - 1) - this->div(b).conv(b).resize(m - 1);
        }

        auto divmod(const Poly& b) const {
            int m = b.size();
            Poly quo = this->div(b);
            Poly rem = this->resize(m - 1) - quo.conv(b).resize(m - 1);
            return pair {quo, rem};
        }

        Poly sqrt() const {
            const int inv2 = (P + 1) / 2;
            int n = this->size();
            int m = 1 << bit_width(n * 1ull);
            Poly A = this->resize(m);
            Poly rt(m);
            rt[0] = 1;
            if (this->at(0) != 1) {
                rt[0] = modSqrt(this->at(0)); // memo
            }
            for (int l = 2; l <= m; l <<= 1) {
                Poly b = A.resize(l).resize(l << 1);
                Poly r = rt.resize(l).resize(l << 1);
                Poly ir = rt.resize(l).inv().resize(l << 1);
                NTT t(l << 1);
                t.dft(b);
                t.dft(r);
                t.dft(ir);
                for (int i = 0; i < r.size(); i++) {
                    b[i] = (b[i] + 1LL * r[i] * r[i] % P) * inv2 % P * ir[i] % P;
                }
                t.idft(b);
                std::copy(b.begin(), min(b.begin() + l, b.end()), rt.begin());
            }
            return rt.resize(n);
        }

        Poly ln() const {
            if (this->at(0) != 1) {
                throw std::invalid_argument("The first element must be 1 for ln.");
            }
            return this->derivative().conv(this->inv()).resize(this->size()).integral();
        }

        Poly exp() const {
            if (this->at(0) != 0) {
                throw std::invalid_argument("The first element must be 0 for exp.");
            }
            int n = this->size();
            int m = 1 << bit_width(n * 1ull);
            Poly A = this->resize(m);
            Poly e(m);
            e[0] = 1;
            for (int l = 2; l <= m; l <<= 1) {
                Poly b = e.resize(l).ln();
                b[0]--;
                for (int i = 0; i < l; i++) {
                    b[i] = (A[i] - b[i] + P) % P;
                }
                e = b.conv(e.resize(l)).resize(l);
            }
            return e.resize(n);
        }

        Poly pow(int k) const {
            int n = this->size();
            if (k >= n && this->at(0) == 0) {
                return Poly(n);
            }
            int k1 = k % (P - 1);
            k %= P;
            if (this->at(0) == 1) {
                return (this->ln() * k).exp();
            }
            int shift = 0;
            while (shift < n && this->at(shift) == 0) {
                ++shift;
            }
            if (shift * k >= n) {
                return Poly(n);
            }
            Poly a = this->rsh(shift);
            a = a * nttPow(a[0], P - 2); // todo
            return ((a.ln() * k).exp() * nttPow(a[0], k1)).lsh(shift * k);
        }

        auto sincos() const {
            if (this->at(0) != 0) {
                throw std::invalid_argument("The first element must be 0 for sincos()");
            }
            const int i = 911660635;
            const int inv2i = 43291859;
            const int inv2 = (P + 1) / 2;
            Poly e = (*this * i).exp();
            Poly invE = e.inv();
            Poly sin = (e - invE) * inv2i;
            Poly cos = (e + invE) * inv2;
            return pair {sin, cos};
        }

        Poly tan() const {
            auto [sin, cos] = this->sincos();
            return sin.conv(cos.inv());
        }

        Poly asin() const {
            if (this->at(0) != 0) {
                throw std::invalid_argument("The first element must be 0 for asin()");
            }
            int n = this->size();
            Poly b = -(this->conv(*this).resize(n));
            b[0] = 1;
            return this->derivative().conv(b.sqrt().inv()).resize(n).integral();
        }

        Poly acos() const { return -(this->asin()); }

        Poly atan() const {
            if (this->at(0) != 0) {
                throw std::invalid_argument("The first element must be 0 for atan()");
            }
            int n = this->size();
            Poly b = this->conv(*this).resize(n);
            b[0] = 1;
            return this->derivative().conv(b.inv()).resize(n).integral();
        }
    };
}; // namespace Polynomial
using namespace Polynomial;
