template <typename T>
constexpr T qpow(T x, uint64_t k) {
    T res = 1;
    for (; k; k >>= 1) {
        if (k & 1) {
            res *= x;
        }
        x *= x;
    }
    return res;
}

template <uint64_t P>
constexpr uint64_t mulMod(uint64_t a, uint64_t b) {
    return a * 1ull * b % P;
}

template <typename T, T P>
class ModIntBase {
private:
    T x;

public:
    constexpr ModIntBase() : x(0) {}
    template <integral U>
    constexpr ModIntBase(U x_ = 0) : x(norm(x_ % U(P))) {}

    constexpr T norm(T x) {
        if ((x >> (8 * sizeof(T) - 1) & 1) == 1) {
            x += P;
        }
        if (x >= P) {
            x -= P;
        }
        return x;
    }
    constexpr T val() const { return x; }

    constexpr ModIntBase operator-() const {
        ModIntBase res;
        res.x = norm(P - x);
        return res;
    }
    constexpr ModIntBase inv() const { return qpow(*this, P - 2); }

public:
    constexpr ModIntBase& operator*=(const ModIntBase& rv) & { return *this = mulMod<P>(x, rv.val()); }
    constexpr ModIntBase& operator+=(const ModIntBase& rv) & { return *this = norm(x + rv.x); }
    constexpr ModIntBase& operator-=(const ModIntBase& rv) & { return *this = norm(x - rv.x); }
    constexpr ModIntBase& operator/=(const ModIntBase& rv) & { return *this *= rv.inv(); }
    constexpr ModIntBase& operator++() & { return x = norm(x + 1), *this; }
    constexpr ModIntBase& operator--() & { return x = norm(x - 1), *this; }
    constexpr ModIntBase operator++(int) { // 后置
        ModIntBase tmp(*this);
        ++*this;
        return tmp;
    }
    constexpr ModIntBase operator--(int) {
        ModIntBase tmp(*this);
        --*this;
        return tmp;
    }

    friend constexpr ModIntBase operator+(ModIntBase lv, const ModIntBase& rv) { return lv += rv; }
    friend constexpr ModIntBase operator-(ModIntBase lv, const ModIntBase& rv) { return lv -= rv; }
    friend constexpr ModIntBase operator*(ModIntBase lv, const ModIntBase& rv) { return lv *= rv; }
    friend constexpr ModIntBase operator/(ModIntBase lv, const ModIntBase& rv) { return lv /= rv; }
    friend constexpr bool operator<(ModIntBase lv, const ModIntBase rv) { return lv.val() < rv.val(); }
    friend constexpr bool operator==(ModIntBase lv, const ModIntBase rv) { return lv.val() == rv.val(); }
    friend constexpr bool operator!=(ModIntBase lv, const ModIntBase rv) { return lv.val() != rv.val(); }

    friend constexpr istream& operator>>(istream& in, ModIntBase<T, P>& v) {
        T x;
        in >> x;
        v.x = v.norm(x % T(P));
        return in;
    }
    friend constexpr ostream& operator<<(ostream& os, const ModIntBase& v) { return os << v.val(); }
};

template <uint64_t P>
using ModInt64 = ModIntBase<uint64_t, P>;
using MI = ModInt64<MOD>;

// Attention:
// qpow 取模时，第一个参数应为 MI 类型
// 除数为 MI 类型时，不要连乘...
// 默认模数是 1e9+7，注意修改...
