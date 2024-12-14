// 素数测试与因式分解（Miller-Rabin & Pollard-Rho）
ll mul(ll a, ll b, ll m) {
    return static_cast<__int128>(a) * b % m;
}
ll qpow(ll a, ll b, ll m) {
    ll res = 1 % m;
    for (; b; b >>= 1, a = mul(a, a, m)) {
        if (b & 1) {
            res = mul(res, a, m);
        }
    }
    return res;
}
bool isprime(ll n) {
    if (n < 2) {
        return false;
    }
    int s = __builtin_ctzll(n - 1);
    ll d = (n - 1) >> s;
    for (auto a : {2, 3, 5, 7, 11, 13, 17, 19, 23}) {
        if (a == n) {
            return true;
        }
        ll x = qpow(a, d, n);
        if (x == 1 || x == n - 1) {
            continue;
        }
        int ok = 0;
        for (int i = 0; i < s - 1; ++i) {
            x = mul(x, x, n);
            if (x == n - 1) {
                ok = 1;
                break;
            }
        }
        if (ok == 0) {
            return false;
        }
    }
    return true;
}
vector<ll> factorize(ll n) {
    vector<ll> p;
    auto f = [&](auto&& f, ll n) {
        if (n <= 10000) {
            for (int i = 2; i * i <= n; ++i) {
                for (; n % i == 0; n /= i) {
                    p.push_back(i);
                }
            }
            if (n > 1) {
                p.push_back(n);
            }
            return;
        }
        if (isprime(n)) {
            p.push_back(n);
            return;
        }
        auto g = [&](ll x) {
            return (mul(x, x, n) + 1) % n;
        };
        ll x0 = 2;
        while (true) {
            ll x = x0, y = x0, d = 1;
            ll p = 1, lam = 0;
            ll v = 1;
            while (d == 1) {
                y = g(y);
                ++lam;
                v = mul(v, abs(x - y), n);
                if (lam % 127 == 0) {
                    d = gcd(v, n);
                    v = 1;
                }
                if (p == lam) {
                    x = y;
                    p *= 2;
                    lam = 0;
                    d = gcd(v, n);
                    v = 1;
                }
            }
            if (d != n) {
                f(f, d);
                f(f, n / d);
                return;
            }
            ++x0;
        }
    };
    f(f, n);
    sort(p.begin(), p.end());
    return p;
}
