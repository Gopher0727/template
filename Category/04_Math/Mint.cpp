template <class T>
constexpr T qpow(T x, uint64_t a, T res = 1) {
    for (; a; a >>= 1, x *= x) {
        if (a & 1) {
            res *= x;
        }
    }
    return res;
}

template <uint32_t P>
constexpr uint32_t mulMod(uint32_t a, uint32_t b) {
    return a * 1ull * b % P;
}
template <uint64_t P>
constexpr uint64_t mulMod(uint64_t a, uint64_t b) {
    uint64_t res = a * b - uint64_t(1.L * a * b / P - 0.5L) * P;
    res %= P;
    return res;
}

constexpr i64 safeMod(i64 x, i64 m) {
    x %= m;
    if (x < 0) {
        x += m;
    }
    return x;
}

constexpr pair<i64, i64> invGcd(i64 a, i64 b) {
    a = safeMod(a, b);
    if (a == 0) {
        return {b, 0};
    }
    i64 s = b, t = a;
    i64 m0 = 0, m1 = 1;
    while (t) {
        i64 u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        std::swap(s, t);
        std::swap(m0, m1);
    }
    if (m0 < 0) {
        m0 += b / s;
    }
    return {s, m0};
}

template <unsigned_integral T, T P>
class ModIntBase {
private:
    T x;

public:
    constexpr ModIntBase() : x(0) {}

    template <unsigned_integral U>
    constexpr ModIntBase(U x_) : x(x_ % mod()) {}

    template <signed_integral U>
    constexpr ModIntBase(U x_ = 0) {
        using S = make_signed_t<T>;
        S v = x_ % S(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }

    constexpr static T mod() { return P; }

    constexpr T val() const { return x; }

    constexpr ModIntBase operator-() const {
        ModIntBase res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }

    constexpr ModIntBase inv() const {
        // return qpow(*this, mod() - 2);
        auto v = invGcd(x, mod());
        assert(v.first == 1);
        return v.second;
    }

public:
    constexpr ModIntBase& operator*=(const ModIntBase& rv) & {
        x = mulMod<mod()>(x, rv.val());
        return *this;
    }
    constexpr ModIntBase& operator+=(const ModIntBase& rv) & {
        x += rv.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr ModIntBase& operator-=(const ModIntBase& rv) & {
        x -= rv.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr ModIntBase& operator/=(const ModIntBase& rv) & { return *this *= rv.inv(); }

    constexpr ModIntBase& operator++() & {
        x += 1;
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr ModIntBase operator++(int) & {
        ModIntBase tmp = *this;
        ++*this;
        return tmp;
    }
    constexpr ModIntBase& operator--() & {
        if (x == 0) {
            x = mod();
        }
        x -= 1;
        return *this;
    }
    constexpr ModIntBase operator--(int) & {
        ModIntBase tmp = *this;
        --*this;
        return tmp;
    }

    friend constexpr ModIntBase operator+(ModIntBase lv, const ModIntBase& rv) { return lv += rv; }
    friend constexpr ModIntBase operator-(ModIntBase lv, const ModIntBase& rv) { return lv -= rv; }
    friend constexpr ModIntBase operator*(ModIntBase lv, const ModIntBase& rv) { return lv *= rv; }
    friend constexpr ModIntBase operator/(ModIntBase lv, const ModIntBase& rv) { return lv /= rv; }

    friend constexpr strong_ordering operator<=>(const ModIntBase& lv, const ModIntBase& rv) { return lv.val() <=> rv.val(); }
    friend constexpr bool operator==(const ModIntBase& lv, const ModIntBase& rv) { return lv.val() == rv.val(); }

    friend constexpr istream& operator>>(istream& is, ModIntBase& v) {
        i64 x;
        is >> x;
        v = x;
        return is;
    }
    friend constexpr ostream& operator<<(ostream& os, const ModIntBase& v) { return os << v.val(); }
};

template <uint32_t P>
using ModInt32 = ModIntBase<uint32_t, P>;
template <uint64_t P>
using ModInt64 = ModIntBase<uint64_t, P>;

using Mint = ModInt32<MOD>;

// qpow 取模时，第一个参数应为 Mint 类型
