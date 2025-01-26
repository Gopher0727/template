// 离散对数（BSGS，BabyStepGiantStep）
//
// 以 O(sqrt(p)) 的复杂度求解 a^x = b (mod p)。
// 其中标准 BSGS 算法不能计算 a 与 p 互质的情况，而 exbsgs 则可以。
//
namespace BSGS {
    ll a, b, p;
    map<ll, ll> f;

    ll gcd(ll a, ll b) {
        return b > 0 ? gcd(b, a % b) : a;
    }

    ll ps(ll n, ll k, int p) {
        ll r = 1;
        for (; k; k >>= 1) {
            if (k & 1) r = r * n % p;
            n = n * n % p;
        }
        return r;
    }

    void exgcd(ll a, ll b, ll& x, ll& y) {
        if (!b) {
            x = 1, y = 0;
        } else {
            exgcd(b, a % b, x, y);
            ll t = x;
            x = y;
            y = t - a / b * y;
        }
    }

    ll inv(ll a, ll b) {
        ll x, y;
        exgcd(a, b, x, y);
        return (x % b + b) % b;
    }

    ll bsgs(ll a, ll b, ll p) {
        f.clear();
        int m = ceil(sqrt(p));
        b %= p;
        for (int i = 1; i <= m; i++) {
            b = b * a % p;
            f[b] = i;
        }
        ll tmp = ps(a, m, p);
        b = 1;
        for (int i = 1; i <= m; i++) {
            b = b * tmp % p;
            if (f.count(b) > 0) {
                return (i * m - f[b] + p) % p;
            }
        }
        return -1;
    }

    ll exbsgs(ll a, ll b, ll p) {
        a %= p, b %= p;
        if (b == 1 || p == 1) {
            return 0;
        }
        ll g = gcd(a, p), k = 0, na = 1;
        while (g > 1) {
            if (b % g != 0) {
                return -1;
            }
            k++;
            b /= g;
            p /= g;
            na = na * (a / g) % p;
            if (na == b) {
                return k;
            }
            g = gcd(a, p);
        }
        ll f = bsgs(a, b * inv(na, p) % p, p);
        if (f == -1) {
            return -1;
        }
        return f + k;
    }
} // namespace BSGS
using namespace BSGS;
