// 因数
#define N 100005
vector<vector<int>> factors(N);
auto init = []() {
    for (int i = 1; i < N; ++i) {
        for (int j = i; j < N; j += i) {
            factors[j].emplace_back(i);
        }
    }
    return 0;
}();

// 质因数
void find(int n) {
    vector<int> ans;

    int x = n;
    for (int i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            x /= i;
            ans.emplace_back(i);
        }
    }

    if (x > 1) {
        ans.emplace_back(x);
    }
}

// 埃氏筛 + 最小质因数
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
