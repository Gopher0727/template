ll exgcd(ll& a, ll& b, ll& x, ll& y) {
    if (b == 0) {
        return x = 1, y = 0, a;
    }
    ll g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

// ax + b == 0 (mod m)
pair<ll, ll> sol(ll a, ll b, ll m) {
    assert(m > 0);
    b *= -1;
    ll x, y;
    ll g = exgcd(a, m, x, y);
    if (g < 0) {
        g *= -1, x *= -1, y *= -1;
    }
    if (b % g) {
        return {-1, -1};
    }
    x = x * (b / g) % (m / g);
    if (x < 0) {
        x += m / g;
    }
    return {x, m / g};
}

array<ll, 3> exgcd(ll a, ll b) {
    if (b == 0) {
        return {a, 1, 0};
    }
    auto [g, x, y] = exgcd(b, a % b);
    return {g, y, x - a / b * y};
}
