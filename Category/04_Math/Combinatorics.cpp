// 杨辉三角（左对齐）
const int MX = 34;
vector<int> c[MX];
auto init = []() {
    for (int i = 0; i < MX; i++) {
        c[i].resize(i + 1, 1);
        for (int j = 1; j < i; j++) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();


// Lucas:
//
// 由于，C(p, i) = p/i * C(p-1, i-1) = 0 (mod p)  1 <= i <= p-1
// 有，(1+x)^p = 1 + x^p (mod p)
//
// Lucas(n, m) = C(n%p, m%p) * Lucas(n/p, m/p) (mod p)
// 其中，Lucas(x, 0) = 1 (mod p)
//
int Lucas(int n, int k, int p) {
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % p;
        int ki = k % p;
        if (ki > ni) {
            return 0;
        }
        res = res * comb(ni, ki) % p;
        n /= p;
        k /= p;
    }
    return res;
}
