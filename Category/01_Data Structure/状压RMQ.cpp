// 状压RMQ（RMQ）
//
// 序列的长度 n 满足 log_2 n <= 64

template <class T, class Cmp = std::less<T>>
struct RMQ {
    using u64 = unsigned long long;

    const Cmp cmp = Cmp();
    static constexpr unsigned int B = 64;
    int n;
    vector<vector<T>> a;
    vector<T> pre, suf, ini;
    vector<u64> stk;

    RMQ() {}
    RMQ(const std::vector<T>& v) { init(v); }

    void init(const std::vector<T>& v) {
        this->n = v.size();
        this->pre = this->suf = this->ini = v;
        this->stk.resize(n);
        if (n == 0) {
            return;
        }
        const int M = (n - 1) / B + 1;
        const int lg = std::__lg(M);
        a.assign(lg + 1, std::vector<T>(M));
        for (int i = 0; i < M; i++) {
            a[0][i] = v[i * B];
            for (int j = 1; j < B && i * B + j < n; j++) {
                a[0][i] = std::min(a[0][i], v[i * B + j], cmp);
            }
        }
        for (int i = 1; i < n; i++) {
            if (i % B) {
                this->pre[i] = std::min(this->pre[i], this->pre[i - 1], cmp);
            }
        }
        for (int i = n - 2; i >= 0; i--) {
            if (i % B != B - 1) {
                this->suf[i] = std::min(this->suf[i], this->suf[i + 1], cmp);
            }
        }
        for (int j = 0; j < lg; j++) {
            for (int i = 0; i + (2 << j) <= M; i++) {
                a[j + 1][i] = std::min(a[j][i], a[j][i + (1 << j)], cmp);
            }
        }
        for (int i = 0; i < M; i++) {
            const int l = i * B;
            const int r = std::min(1U * n, l + B);
            u64 s = 0;
            for (int j = l; j < r; j++) {
                while (s && cmp(v[j], v[std::__lg(s) + l])) {
                    s ^= 1ULL << std::__lg(s);
                }
                s |= 1ULL << (j - l);
                this->stk[j] = s;
            }
        }
    }

    T operator()(int l, int r) {
        if (l / B != (r - 1) / B) {
            T ans = std::min(this->suf[l], this->pre[r - 1], cmp);
            l = l / B + 1;
            r = r / B;
            if (l < r) {
                int k = std::__lg(r - l);
                ans = std::min({ans, a[k][l], a[k][r - (1 << k)]}, cmp);
            }
            return ans;
        } else {
            const int x = B * (l / B);
            return this->ini[__builtin_ctzll(this->stk[r - 1] >> (l - x)) + l];
        }
    }
};
