// 暴力判断一个数是否是质数
bool isPrime(i64 n) {
    for (int i = 2; i * 1ll * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return n >= 2;
}


// 记录范围内的所有数的因数  2e5
vector<vector<int>> factors;
void sieve(int n) {
    factors.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; j += i) {
            factors[j].push_back(i);
        }
    }
}


// 找到一个数的所有质因数
auto factorize(i64 x) {
    map<int, int> res;
    for (int i = 2; i * 1ll * i <= x; ++i) {
        while (x % i == 0) {
            x /= i;
            res[i]++;
        }
    }
    if (x > 1) {
        res[x]++;
    }
    return res;
}


// 按从小到大的顺序生成回文数（1 ~ 1e9 之间）
vector<int> palindrome;
auto init = [] {
    palindrome.emplace_back(0); // 哨兵

    for (int base = 1; base <= 10000; base *= 10) {
        // 生成奇数长度的回文数
        for (int i = base; i < base * 10; ++i) {
            int x = i;
            for (int t = i / 10; t; t /= 10) {
                x = x * 10 + t % 10;
            }
            palindrome.emplace_back(x);
        }
        // 生成偶数长度的回文数
        if (base <= 1000) {
            for (int i = base; i < base * 10; ++i) {
                int x = i;
                for (int t = i; t; t /= 10) {
                    x = x * 10 + t % 10;
                }
                palindrome.emplace_back(x);
            }
        }
    }
    palindrome.emplace_back(1e9 + 1); // 哨兵

    return 0;
}();


// 埃氏筛  ErlichScreen  2e7
vector<int> isPrime;
vector<int> primes;
void sieve(int n) {
    isPrime.assign(n + 1, 1);
    isPrime[0] = isPrime[1] = 0;
    for (int i = 2; i <= n; ++i) {
        if (!isPrime[i]) {
            continue;
        }
        primes.push_back(i);
        for (auto j = i * 1ll * i; j <= n; j += i) {
            isPrime[j] = 0;
        }
    }
}


// 线性筛  （LinearScreen / EulerScreen）  5e7
// ———— 让每个合数只被标记一次
vector<int> isPrime;
vector<int> primes;
void sieve(int n) {
    isPrime.resize(n + 1, 1);
    isPrime[0] = isPrime[1] = 0;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
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
bool isPrime(int n) {
    return n >= 0 && minp[n] == n;
}


// 欧拉函数（单个数）
int phi(int n) {
    int res = n;
    for (int i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            res = res / i * (i - 1);
        }
    }
    if (n > 1) {
        res = res / n * (n - 1);
    }
    return res;
}


// 线性筛 + 欧拉函数（从 1 到 i 中与 i 互质的数的个数）  1e7
vector<int> isPrime;
vector<int> primes;
vector<int> phi;
void sieve(int n) {
    isPrime.assign(n + 1, 1);
    isPrime[0] = isPrime[1] = 0;
    primes.clear();
    phi.resize(n + 1);
    phi[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            isPrime[i * p] = false;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                break;
            }
            phi[i * p] = phi[i] * (p - 1);
        }
    }
}


// 线性筛 + 最小质因数 + 欧拉函数（从 1 到 i 中与 i 互质的数的个数）
vector<int> primes;
vector<int> minp;
vector<int> phi;
void sieve(int n) {
    primes.clear();
    minp.assign(n + 1, 0);
    phi.resize(n + 1);
    phi[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            primes.push_back(i);
            minp[i] = i;
            phi[i] = i - 1;
        }
        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            minp[i * p] = p;
            if (p == minp[i]) {
                phi[i * p] = phi[i] * p;
                break;
            }
            phi[i * p] = phi[i] * (p - 1);
        }
    }
    // for (int i = 2; i <= n; i++) {
    //     phi[i] += phi[i - 1];
    // }
}
bool isPrime(int n) {
    return n >= 0 && minp[n] == n;
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
