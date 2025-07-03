struct Sieve {
    const int N = 1E7 + 10;

    int n;
    vector<int> _primes;
    vector<int> _lpf;
    vector<int> _phi;
    vector<int> _mobius;

    explicit Sieve() : n(2), _primes {2}, _lpf {0, 1, 2}, _phi {0, 1, 1}, _mobius {0, 1, -1} {}
    explicit Sieve(int n) : Sieve() { init(n); }

    void init(int m) {
        assert(m <= N);
        if (m <= n) {
            return;
        }
        _lpf.resize(m + 1);
        _phi.resize(m + 1);
        _mobius.resize(m + 1);
        for (int i = n + 1; i <= m; i++) {
            if (_lpf[i] == 0) {
                _lpf[i] = i;
                _phi[i] = i - 1;
                _mobius[i] = -1;
                _primes.push_back(i);
            }
            for (auto p : _primes) {
                auto t = 1ll * i * p;
                if (t > m) {
                    break;
                }
                _lpf[t] = p;
                if (i % p == 0) {
                    _phi[t] = _phi[i] * p;
                    _mobius[t] = 0;
                    break;
                }
                _phi[t] = _phi[i] * (p - 1);
                _mobius[t] = -_mobius[i];
            }
        }
        n = m;
    }

    int size() { return _primes.size(); }

    int count(int x) {
        while (_primes.back() <= x) {
            init(n * 2);
        }
        return upper_bound(_primes.begin(), _primes.end(), x) - _primes.begin();
    }

    int prime(int i) {
        if (i > n) {
            init(i * 2);
        }
        return _primes[i];
    }

    int lpf(int x) {
        if (x > n) {
            init(x * 2);
        }
        return _lpf[x];
    }

    int phi(int x) {
        if (x > n) {
            init(x * 2);
        }
        return _phi[x];
    }

    int mobius(int x) {
        if (x > n) {
            init(x * 2);
        }
        return _mobius[x];
    }

    bool isPrime(int x) {
        if (x > n) {
            init(x * 2);
        }
        return x >= 2 && _lpf[x] == x;
    }
} S;

template <class T>
struct Min25 {
    using u32 = unsigned;
    using Func = T (*)(int, int, i64);

    int pcnt;
    i64 n, sqrt;
    vector<i64> nums;
    vector<T> coef;
    vector<vector<T>> sum, ini, val;

    explicit Min25() : n {0}, sqrt {0} {}
    explicit Min25(i64 n, const vector<T>& coef, const vector<Func>& func, u32 lim = -1) { init(n, coef, func, lim); }

    void init(i64 m, const vector<T>& coef, const vector<Func>& func, u32 lim = -1) {
        n = m;
        sqrt = std::sqrt(n);
        pcnt = S.count(sqrt);
        nums.clear();
        for (i64 i = 1; i <= n; i = n / (n / i) + 1) {
            nums.emplace_back(n / i);
        }
        nums.emplace_back(0);
        reverse(nums.begin(), nums.end());
        sum.assign(func.size(), vector<T>(pcnt + 1));
        ini.assign(func.size(), vector<T>(nums.size()));
        for (int i = 0; i < func.size(); i++) {
            auto f = func[i];
            for (int j = 0; j < pcnt; j++) {
                auto b = S.prime(j);
                sum[i][j + 1] = sum[i][j] + f(b, 1, b);
            }
            auto c = f(0, 0, 1);
            for (int j = 1; j < nums.size(); j++) {
                ini[i][j] = f(0, 0, nums[j]) - c;
            }
        }
        for (int i = 0; i < func.size(); i++) {
            auto f = func[i];
            for (int j = 0; j < pcnt; j++) {
                auto b = S.prime(j);
                for (int k = nums.size() - 1; k > 0; k--) {
                    if (1LL * b * b > nums[k]) {
                        break;
                    }
                    ini[i][k] -= f(b, 1, b) * (ini[i][rank(nums[k] / b)] - sum[i][j]);
                }
            }
        }
        val.assign(pcnt, {});
        for (int i = 0; i < pcnt; i++) {
            auto b = S.prime(i);
            val[i].reserve(floor(log(n) / log(b)) + 1);
            val[i].emplace_back(0);
            for (i64 e = 1, p = b, c = 0; e <= lim && p <= n; e++, p *= b, c = 1) {
                val[i].emplace_back(0);
                for (int j = 0; j < coef.size(); j++) {
                    val[i][e] += coef[j] * func[j](b, e, p);
                }
            }
        }
        this->coef = coef;
    }

    int rank(i64 m) {
        if (m <= sqrt) {
            return m;
        } else {
            return nums.size() - n / m;
        }
    }

    T operator[](i64 m) {
        T res = 0;
        for (int i = 0; i < coef.size(); i++) {
            res += coef[i] * ini[i][rank(m)];
        }
        return res;
    }

    T dfs(i64 m, int i) {
        if (S.prime(i) > m) {
            return 0;
        }
        T res = 0;
        for (int j = 0; j < coef.size(); j++) {
            res += coef[j] * (ini[j][rank(m)] - sum[j][i]);
        }
        for (; i < pcnt; i++) {
            auto b = S.prime(i);
            if (1LL * b * b > m) {
                break;
            }
            for (i64 e = 1, p = b, c = 0; e < val[i].size() && p <= m; e++, p *= b, c = 1) {
                res += val[i][e] * (dfs(m / p, i + 1) + c);
            }
        }
        return res;
    }

    T operator()(i64 m) { return dfs(m, 0); }
};
