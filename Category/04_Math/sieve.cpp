// 暴力判断一个数是否是质数
bool isPrime(i64 x) {
    for (int i = 2; 1ll * i * i <= x; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return x >= 2;
}


// 记录一个数的所有因数
auto factorize(i64 x) {
    vector<int> fac;
    for (int i = 1; 1ll * i * i <= x; i++) {
        if (x % i == 0) {
            fac.push_back(i);
            if (1ll * i * i != x) {
                fac.push_back(x / i);
            }
        }
    }
    // ranges::sort(fac);
    return fac;
}


// 记录范围内的所有数的因数  1e6
vector<vector<int>> factors;
void sieve(int n) {
    factors.assign(n + 1, {});
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) {
            factors[j].push_back(i);
        }
    }
}


// 找到一个数的所有质因数
auto factorize(i64 x) {
    map<int, int> cnt;
    for (int i = 2; 1ll * i * i <= x; i++) {
        while (x % i == 0) {
            x /= i;
            cnt[i]++;
        }
    }
    if (x > 1) {
        cnt[x]++;
    }
    // return vector(cnt.begin(), cnt.end());
    return cnt;
}


// EXP[x] 为 x 分解质因数后，每个质因数的指数
vector<vector<array<int, 2>>> EXP;
void sieve(int n) {
    EXP.assign(n + 1, {});
    for (int x = 2; x <= n; x++) {
        int a = x;
        for (int i = 2; i * i <= a; i++) {
            int e = 0;
            for (; a % i == 0; a /= i) {
                e++;
            }
            if (e) {
                EXP[x].push_back({i, e});
            }
        }
        if (a > 1) {
            EXP[x].push_back({a, 1});
        }
    }
}


// 按从小到大的顺序生成回文数（1 ~ 1e9 之间）
vector<int> palindrome;
auto init = [] {
    palindrome.push_back(0); // 哨兵
    for (int base = 1; base <= 10000; base *= 10) {
        // 生成奇数长度的回文数
        for (int i = base; i < base * 10; i++) {
            int x = i;
            for (int t = i / 10; t; t /= 10) {
                x = x * 10 + t % 10;
            }
            palindrome.push_back(x);
        }
        // 生成偶数长度的回文数
        if (base <= 1000) {
            for (int i = base; i < base * 10; i++) {
                int x = i;
                for (int t = i; t; t /= 10) {
                    x = x * 10 + t % 10;
                }
                palindrome.push_back(x);
            }
        }
    }
    palindrome.emplace_back(1e9 + 1); // 哨兵
    return 0;
}();


// 埃氏筛  ErlichScreen  1e8
vector<bool> isPrime;
vector<int> primes;
void sieve(int n) {
    isPrime.assign(n + 1, true);
    primes.clear();
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (!isPrime[i]) {
            continue;
        }
        primes.push_back(i);
        for (auto j = 1ll * i * i; j <= n; j += i) {
            isPrime[j] = false;
        }
    }
}


// 线性筛  LinearScreen / EulerScreen  2e8
// ———— 让每个合数只被标记一次
vector<bool> isPrime;
vector<int> primes;
void sieve(int n) {
    isPrime.assign(n + 1, true);
    primes.clear();
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
        for (auto p : primes) {
            if (1ll * i * p > n) {
                break;
            }
            isPrime[i * p] = 0;
            if (i % p == 0) {
                break;
            }
        }
    }
}


// 线性筛 + 最小质因数  1e8
// 最小质因数 * 最大真因数
vector<int> primes;
vector<int> minp;
void sieve(int n) {
    primes.clear();
    minp.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            primes.push_back(i);
        }
        for (auto p : primes) {
            if (1ll * i * p > n) {
                break;
            }
            minp[i * p] = p;
            if (p == minp[i]) {
                break;
            }
        }
    }
}
bool isPrime(int x) {
    return x >= 2 && minp[x] == x;
}


// 欧拉函数（单个数）
/*
· 从 1 到 i 中与 i 互质的数的个数
· pow(a, b) = pow(a, (b%phi[n])) (mod n)
*/
int phi(int x) {
    int res = x;
    for (int i = 2; 1ll * i * i <= x; i++) {
        if (x % i == 0) {
            while (x % i == 0) {
                x /= i;
            }
            res = res / i * (i - 1);
        }
    }
    if (n > 1) {
        res = res / x * (x - 1);
    }
    return res;
}


// 线性筛 + 欧拉函数  1e8
vector<bool> isPrime;
vector<int> primes;
vector<int> phi;
void sieve(int n) {
    isPrime.assign(n + 1, true);
    primes.clear();
    phi.assign(n + 1, 0);
    isPrime[0] = isPrime[1] = false;
    phi[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (auto p : primes) {
            auto t = 1ll * i * p;
            if (t > n) {
                break;
            }
            isPrime[t] = false;
            if (i % p == 0) {
                phi[t] = phi[i] * p;
                break;
            }
            phi[t] = phi[i] * (p - 1);
        }
    }
}


// 莫比乌斯函数（单个数）
/*
· 如果 n = 1, 则 μ(n) = 1
· 如果 n 是平方因子自由的（即 n 不被任何平方数除尽）, 且 n 有 k 个不同的素因子，则 μ(n) = pow(-1, k)
· 如果 n 不是平方因子自由的（即 n 被某个平方数除尽）, 则 μ(n) = 0
*/
int mobius(int x) {
    if (x == 1) {
        return 1;
    }
    int cnt = 0;
    int a = x;
    for (int i = 2; 1ll * i * i <= a; i++) {
        if (a % i == 0) {
            int e = 0;
            while (a % i == 0) {
                a /= i;
                e++;
            }
            if (e > 1) {
                return 0;
            }
            cnt++;
        }
    }
    if (a > 1) {
        cnt++;
    }
    return (cnt % 2 == 0 ? 1 : -1);
}


// 线性筛 + 莫比乌斯函数  1e8
vector<bool> isPrime;
vector<int> primes;
vector<int> mu;
void sieve(int n) {
    isPrime.assign(n + 1, true);
    primes.clear();
    mu.assign(n + 1, 0);
    isPrime[0] = isPrime[1] = false;
    mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (auto p : primes) {
            auto t = 1ll * i * p;
            if (t > n) {
                break;
            }
            isPrime[t] = false;
            if (i % p == 0) {
                mu[t] = 0;
                break;
            }
            mu[t] = -mu[i];
        }
    }
}


// 线性筛 + 最小质因数 + 欧拉函数 + 莫比乌斯函数  2e7
vector<bool> isPrime;
vector<int> primes;
vector<int> minp;
vector<int> phi;
vector<int> mu;
void sieve(int n) {
    isPrime.assign(n + 1, true);
    primes.clear();
    minp.assign(n + 1, 0);
    phi.assign(n + 1, 0);
    mu.assign(n + 1, 0);
    isPrime[0] = isPrime[1] = false;
    phi[1] = 1;
    mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) { // minp[i] == 0
            primes.push_back(i);
            minp[i] = i;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (auto p : primes) {
            auto t = 1ll * i * p;
            if (t > n) {
                break;
            }
            isPrime[t] = false;
            minp[t] = p;
            if (p == minp[i]) { // i % p == 0
                phi[t] = phi[i] * p;
                mu[t] = 0;
                break;
            }
            phi[t] = phi[i] * (p - 1);
            mu[t] = -mu[i];
        }
    }
    // for (int i = 2; i <= n; i++) {
    //     phi[i] += phi[i - 1];
    // }
}


// 块筛选  ——  计算不大于 n 的质数的总数量  3e8
int count_primes(int n) {
    const int S = 10000;

    vector<int> primes;
    int nsqrt = sqrt(n);
    vector<int> isPrime(nsqrt + 1, 1);
    for (int i = 2; i <= nsqrt; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            for (int j = i * i; j <= nsqrt; j += i) {
                isPrime[j] = 0;
            }
        }
    }

    int result = 0;
    vector<int> block(S);
    for (int k = 0; k * S <= n; k++) {
        fill(block.begin(), block.end(), 1);

        int start = k * S;
        for (auto p : primes) {
            int start_idx = (start + p - 1) / p;
            for (int j = max(start_idx, p) * p - start; j < S; j += p) {
                block[j] = 0;
            }
        }
        if (k == 0) {
            block[0] = block[1] = 0;
        }
        for (int i = 0; i < S && start + i <= n; i++) {
            if (block[i]) {
                result++;
            }
        }
    }
    return result;
}
