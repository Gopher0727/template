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
vector<ull> p, h;
auto init = []() {
    p.resize(N);
    h.resize(N);
    return 0;
}();

void build(const string& s) {
    int n = s.size();
    p[0] = 1, h[0] = 0;
    for (int i = 0; i < n; ++i) {
        p[i + 1] = p[i] * base;
        h[i + 1] = h[i] * base + (s[i] - 'a' + 1);
    }
}
ull get(const string& s) {
    ull val = 0;
    for (char ch : s) {
        val = val * base + (ch - 'a' + 1);
    }
    return val;
}
ull get(int l, int r) { // [l, r]
    return h[r + 1] - h[l] * p[r - l + 1];
}
bool IsPre(int l1, int r1, int l2, int r2) {
    int len1 = r1 - l1 + 1;
    int len2 = r2 - l2 + 1;
    if (len1 > len2) {
        return false;
    }
    return get(l1, r1) == get(l2, l2 + len1 - 1);
}


// Double Hash
//
template <typename T, //
          typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, std::vector<int>>::value, bool>::type = true>
class StringHash {
    static const int MOD1 = 1'070'777'777;
    static const int MOD2 = 1'000'000'007;
    vector<int> pow_base1, pow_base2;
    vector<int> pre_hash1, pre_hash2;

public:
    explicit StringHash(const T& s) {
        int n = s.size();
        pow_base1.resize(n + 1);
        pow_base2.resize(n + 1);
        pre_hash1.resize(n + 1);
        pre_hash2.resize(n + 1);

        mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        const int BASE1 = uniform_int_distribution<>(8e8, 9e8)(rng);
        const int BASE2 = uniform_int_distribution<>(8e8, 9e8)(rng);

        pow_base1[0] = pow_base2[0] = 1;
        for (int i = 0; i < n; i++) {
            pow_base1[i + 1] = (long long) pow_base1[i] * BASE1 % MOD1;
            pow_base2[i + 1] = (long long) pow_base2[i] * BASE2 % MOD2;
            pre_hash1[i + 1] = ((long long) pre_hash1[i] * BASE1 + s[i]) % MOD1;
            pre_hash2[i + 1] = ((long long) pre_hash2[i] * BASE2 + s[i]) % MOD2;
        }
    }

    auto get(int l, int r) { // [l, r]
        long long h1 = ((pre_hash1[r + 1] - (long long) pre_hash1[l] * pow_base1[r - l + 1]) % MOD1 + MOD1) % MOD1;
        long long h2 = ((pre_hash2[r + 1] - (long long) pre_hash2[l] * pow_base2[r - l + 1]) % MOD2 + MOD2) % MOD2;
        return h1 << 32 | h2;
    }
    bool IsPre(int l1, int r1, int l2, int r2) {
        int len1 = r1 - l1 + 1;
        int len2 = r2 - l2 + 1;
        if (len1 > len2) {
            return false;
        }
        return get(l1, r1) == get(l2, l2 + len1 - 1);
    }
};
