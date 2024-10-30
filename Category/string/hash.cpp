using ull = unsigned long long;
static constexpr int base = 13131;
ull Hash(string& s) {
    ull res = 0;
    for (int i = 0; i < s.size(); ++i) {
        res = res * base + s[i];
    }
    return res;
}


// Now you can use this:
string s;
hash<string>{} (s);


/*
    > 自然溢出
    > 底数取质数，尽量使自然溢出后的状态分散
    > 转化时每一位对应的值不为 0
        // 比如： 数字 + 大写 + 小写
        // '0' ~ '9' => 1 ~ 10  'A' ~ 'Z' => 11 ~ 36    'a' ~ 'z' => 37 ~ 62
*/

using ull = unsigned long long;
static constexpr int N = 5e4 + 5;
static constexpr int base = 131;
static constexpr int inf = 0x3f3f3f3f;
vector<ull> p(N), h(N);

void build(string& s) {
    int n = s.size();
    p[0] = 1, h[0] = 0;
    for (int i = 0; i < n; ++i) {
        p[i + 1] = p[i] * base;
        h[i + 1] = h[i] * base + s[i] - 'a' + 1;
    }
}
ull get(string& s) {
    ull val = 0;
    for (char ch : s) {
        val = val * base + (ch - 'a' + 1);
    }
    return val;
}
ull get(int l, int r) { // [l, r)
    return h[r] - h[l] * p[r - l];
}


// class
//
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
bool is_prime(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return n >= 2;
}
int findPrime(int n) {
    while (!is_prime(n)) {
        n++;
    }
    return n;
}
template <int N>
class StringHash {
    static array<int, N> mod;
    static array<int, N> base;
    vector<array<int, N>> p, h;

public:
    StringHash() = default;
    StringHash(const string& s) {
        int n = s.size();
        p.resize(n, 1), h.resize(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < N; ++j) {
                p[i][j] = (i == 0 ? 1 : p[i - 1][j]) * 1ll * base[j] % mod[j];
                h[i][j] = ((i == 0 ? 0 : h[i - 1][j]) * 1ll * base[j] + s[i]) % mod[j];
            }
        }
    }

public:
    array<int, N> query(int l, int r) {
        assert(r >= l - 1);
        array<int, N> ans {};
        if (l > r) {
            return {0, 0};
        }
        for (int i = 0; i < N; ++i) {
            ans[i] = (h[r][i] - (i == 0 ? 0 : h[l - 1][i]) * 1ll * (r - l + 1 == 0 ? 1 : p[r - l][i]) % mod[i] + mod[i]) % mod[i];
        }
        return ans;
    }
};
constexpr int HN = 2;
template <>
array<int, 2> StringHash<HN>::mod {findPrime(rng() % 900000000 + 100000000), findPrime(rng() % 900000000 + 100000000)};
template <>
array<int, 2> StringHash<HN>::base {13331, 131};
using Hashing = StringHash<HN>;
