const int LEN = (1 << 23);
mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct HashMap {
    inline ull F(ull x) {
        static const ull s1 = rnd(), s2 = rnd(), s3 = rnd();
        x += s1;
        x = (x ^ (x >> 33)) * s2;
        x = (x ^ (x >> 30)) * s3;
        return x;
    }
    bitset<LEN + 5> hav;
    ull key[LEN + 5];
    ull value[LEN + 5];
    int gt(const ull& x) {
        int i = F(x) & (LEN - 1);
        while (hav[i] && key[i] != x) i = (i + 1) & (LEN - 1);
        hav[i] = 1, key[i] = x;
        return i;
    }
    ull& operator[](const ull& x) { return value[gt(x)]; }
} f;
