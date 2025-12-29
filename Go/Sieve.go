// 按从小到大的顺序生成回文数（1 ~ 1e9 之间）
func Sieve() []int {
    palindrome = append(palindrome, 0)
    for base := 1; base <= 10_000; base *= 10 {
        for i := base; i < base*10; i++ {
            x := i
            for j := i / 10; j > 0; j /= 10 {
                x = x*10 + j%10
            }
            palindrome = append(palindrome, x)
        }
        if base <= 1_000 {
            for i := base; i < base*10; i++ {
                x := i
                for j := i; j > 0; j /= 10 {
                    x = x*10 + j%10
                }
                palindrome = append(palindrome, x)
            }
        }
    }
    palindrome = append(palindrome, 1_000_000_001)
    return palindrome
}


// 线性筛 + 最小质因数 + 欧拉函数 + 莫比乌斯函数
var (
    N       = 2_000_000
    isPrime = make([]bool, N+1)
    primes  = make([]int, 0, N+1)
    minp    = make([]int, N+1)
    phi     = make([]int, N+1)
    mu      = make([]int, N+1)
)

func init() {
    for i := 2; i <= N; i++ {
        isPrime[i] = true
    }
    phi[1] = 1
    mu[1] = 1
    for i := 2; i <= N; i++ {
        if isPrime[i] { // minp[i] == 0
            primes = append(primes, i)
            minp[i] = i
            phi[i] = i - 1
            mu[i] = -1
        }
        for _, p := range primes {
            t := p * i
            if t > N {
                break
            }
            isPrime[t] = false
            minp[t] = p
            if i%p == 0 { // minp[i] == p
                phi[t] = phi[i] * p
                mu[t] = 0
                break
            } else {
                phi[t] = phi[i] * (p - 1)
                mu[t] = -mu[i]
            }
        }
    }
}


// 区间筛 [L, R]
func RangeSieve(L, R int) []bool {
    // TODO：预处理 sqrt(R) 以内的素数
    isPrime := make([]bool, R-L+1)
    for i := 0; i <= R-L; i++ {
        isPrime[i] = true
    }
    for _, p := range primes {
        for j := max((L+p-1)/p*p, p*p); j <= R; j += p {
            isPrime[j-L] = false
        }
    }
    if L == 1 {
        isPrime[0] = false
    }
    return isPrime
}
