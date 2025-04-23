using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;

template <class T>
constexpr T qpow(T x, u64 a, T res = 1) {
    for (; a; a >>= 1, x *= x) {
        if (a & 1) {
            res *= x;
        }
    }
    return res;
}

template <u32 P>
constexpr u32 mulMod(u32 a, u32 b) {
    return a * 1ull * b % P;
}
template <u64 P>
constexpr u64 mulMod(u64 a, u64 b) {
    u64 res = a * b - u64(1.L * a * b / P - 0.5L) * P;
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

    constexpr ModIntBase inv() const { return qpow(*this, mod() - 2); }

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

class Barrett {
private:
    u32 m;
    u64 im;

public:
    Barrett(u32 m_) : m(m_), im((u64) (-1) / m_ + 1) {}

    constexpr u32 mod() const { return m; }

    constexpr u32 mul(u32 a, u32 b) const {
        u64 z = a;
        z *= b;
        u64 x = u64((u128(z) * im) >> 64);
        u32 v = u32(z - x * m);
        if (m <= v) {
            v += m;
        }
        return v;
    }
};

template <u32 Id>
class DynModInt {
private:
    u32 x;
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

    constexpr static void setMod(u32 m) { bt = m; }

    static u32 mod() { return bt.mod(); }

    constexpr u32 val() const { return x; }

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
// MOD

//
template <u32 P>
using ModInt32 = ModIntBase<u32, P>;
template <u64 P>
using ModInt64 = ModIntBase<u64, P>;
template <u32 Id>
Barrett DynModInt<Id>::bt = MOD;

using Mint = ModInt32<MOD>;
using DMint = DynModInt<0>;

// qpow 取模时，第一个参数应为 Mint 类型
