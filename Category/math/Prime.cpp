// 暴力判断一个数是否是质数
bool is_prime(ll n) {
    for (int i = 2; i * 1ll * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return n >= 2;
}


// 记录范围内的所有数的因数  1e5
vector<vector<int>> factors;
void sieve(int n) {
    factors.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; j += i) {
            factors[j].emplace_back(i);
        }
    }
}


// 找到一个数的所有质因数
vector<int> prifac;
void sieve(ll x) {
    for (int i = 2; i * 1ll * i <= x; ++i) {
        while (x % i == 0) {
            x /= i;
            prifac.emplace_back(i);
        }
    }
    if (x > 1) {
        prifac.emplace_back(x);
    }
}


// 埃氏筛  ErlichScreen  2e7
vector<int> isPrime;
void sieve(int n) {
    isPrime.assign(n + 1, 1);
    isPrime[0] = isPrime[1] = 0;
    for (int i = 2; i <= n; ++i) {
        if (!isPrime[i]) {
            continue;
        }
        for (auto j = i * 1ll * i; j <= n; j += i) {
            isPrime[j] = 0;
        }
    }
}


// 线性筛  LinearScreen  5e7
// (EulerScreen) ———— 让每个合数只被标记一次
vector<int> primes, isPrime;
void sieve(int n) {
    isPrime.resize(n + 1, 1);
    isPrime[0] = isPrime[1] = 0;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) {
            primes.emplace_back(i);
        }
        for (int& p : primes) {
            if (i * p > n) {
                break;
            }
            isPrime[i * p] = 0;
            if (i % p == 0) {
                break;
            }
        }
    }
}


// 合数 = 最小质因数 * 最大真因数

// 线性筛 + 最小质因数  1e8
vector<int> minp, primes;
void sieve(int n) {
    minp.assign(n + 1, 0);
    primes.clear();

    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            primes.push_back(i);
        }
        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            minp[i * p] = p;
            // 当 minp[i]（即 i 的当前最小质因数）等于当前的质数 p 时，就没有必要继续用更大的质数去更新了
            if (p == minp[i]) {
                break;
            }
        }
    }
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
        for (int& p : primes) {
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
