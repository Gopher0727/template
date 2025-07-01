template <class T>
constexpr T qpow(T x, uint64_t a, T res = 1) {
    for (; a; a >>= 1, x *= x) {
        if (a & 1) {
            res *= x;
        }
    }
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

class Barrett {
private:
    uint32_t m;
    uint64_t im;

public:
    Barrett(uint32_t m_) : m(m_), im((uint64_t) (-1) / m_ + 1) {}

    constexpr uint32_t mod() const { return m; }

    constexpr uint32_t mul(uint32_t a, uint32_t b) const {
        uint64_t z = a;
        z *= b;
        uint64_t x = uint64_t(((unsigned __int128) (z) *im) >> 64);
        uint32_t v = uint32_t(z - x * m);
        if (m <= v) {
            v += m;
        }
        return v;
    }
};

template <uint32_t Id>
class DynModInt {
private:
    uint32_t x;
    static Barrett bt;

public:
    constexpr DynModInt() : x(0) {}

    template <unsigned_integral T>
    constexpr DynModInt(T x_) : x(x_ % mod()) {}

    template <signed_integral T>
    constexpr DynModInt(T x_) {
        int v = x_ % int(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }

    constexpr static void setMod(uint32_t m) { bt = m; }

    static uint32_t mod() { return bt.mod(); }

    constexpr uint32_t val() const { return x; }

    constexpr DynModInt operator-() const {
        DynModInt res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }

    constexpr DynModInt inv() const {
        auto v = invGcd(x, mod());
        assert(v.first == 1);
        return v.second;
    }

    constexpr DynModInt& operator*=(const DynModInt& rv) & {
        x = bt.mul(x, rv.val());
        return *this;
    }
    constexpr DynModInt& operator+=(const DynModInt& rv) & {
        x += rv.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr DynModInt& operator-=(const DynModInt& rv) & {
        x -= rv.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr DynModInt& operator/=(const DynModInt& rv) & { return *this *= rv.inv(); }

    constexpr DynModInt& operator++() & {
        x += 1;
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr DynModInt operator++(int) & {
        DynModInt tmp = *this;
        ++*this;
        return tmp;
    }
    constexpr DynModInt& operator--() & {
        if (x == 0) {
            x = mod();
        }
        x -= 1;
        return *this;
    }
    constexpr DynModInt operator--(int) & {
        DynModInt tmp = *this;
        --*this;
        return tmp;
    }

    friend constexpr DynModInt operator*(DynModInt lv, const DynModInt& rv) { return lv *= rv; }
    friend constexpr DynModInt operator+(DynModInt lv, const DynModInt& rv) { return lv += rv; }
    friend constexpr DynModInt operator-(DynModInt lv, const DynModInt& rv) { return lv -= rv; }
    friend constexpr DynModInt operator/(DynModInt lv, const DynModInt& rv) { return lv /= rv; }

    friend constexpr bool operator==(const DynModInt& lv, const DynModInt& rv) { return lv.val() == rv.val(); }
    friend constexpr strong_ordering operator<=>(const DynModInt& lv, const DynModInt& rv) { return lv.val() <=> rv.val(); }

    friend constexpr istream& operator>>(istream& is, DynModInt& v) {
        i64 x;
        is >> x;
        v = x;
        return is;
    }
    friend constexpr ostream& operator<<(ostream& os, const DynModInt& v) { return os << v.val(); }
};
template <uint32_t Id>
Barrett DynModInt<Id>::bt = 998244353;
using Mint = DynModInt<0>;

// qpow 取模时，第一个参数应为 Mint 类型
// Mint::setMod()
