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


